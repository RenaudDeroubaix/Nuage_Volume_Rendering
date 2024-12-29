
#include "../header/light.h"


Light::Light(QOpenGLContext* context)
{
   glContext = context;
   init();
   initLightShader();
}

void Light::init(){
    time = 30000.0;
    timer.start();

    LightPos = QVector3D(0.0, 5.0 , 0.0);
    LightColor =  QVector3D(1.0,1.0,1.0);
    LightDir = -LightPos.normalized();
    rayon = 1.f;

}

QVector3D Light::lightposition() {
    // Temps écoulé normalisé pour éviter de dépasser 2π
    float t = (timer.elapsed() / time) * 2 * M_PI;
    if (t > 2 * M_PI) {
        timer.start();
        t = 0.0f;
    }

    // Rayon de la sphère à partir de la position initiale
    float SR = LightPos.length();

    // Nouvelle position de la lumière sur le cercle
    QVector3D newLightPosition = QVector3D(
        -SR * cos(t),  // Coordonnée x (inversée pour rotation d'est en ouest)
        SR * sin(t),   // Coordonnée y
        LightPos.z()           // Coordonnée z (à l'origine)
    );

    return newLightPosition;
}
void Light::initLightShader(){
    std::string path = "GLSL/shaders/";
    std::string fShaderPath = path + "light.frag";
    std::string vShaderPath = path + "light.vert";


    glFunctions = glContext->extraFunctions();
    glEnable( GL_DEBUG_OUTPUT );
    glFunctions->glDebugMessageCallback(&MessageCallback, 0 );


    // Create programs and link shaders
    this->LightID = glFunctions->glCreateProgram();

    std::string content = readShaderSource(vShaderPath);
    if (!content.empty()) {
        this->vShader = glFunctions->glCreateShader(GL_VERTEX_SHADER);
        const char* src = content.c_str();
        glFunctions->glShaderSource(this->vShader, 1, &src, NULL);
        glFunctions->glCompileShader(this->vShader);
        glFunctions->glAttachShader(this->LightID, this->vShader);
        printShaderErrors(glFunctions ,this->vShader);
    }


    content = readShaderSource(fShaderPath);
    if (!content.empty()) {
        this->fShader = glFunctions->glCreateShader(GL_FRAGMENT_SHADER);
        const char* src = content.c_str();
        glFunctions->glShaderSource(this->fShader, 1, &src, NULL);
        glFunctions->glCompileShader(this->fShader);
        glFunctions->glAttachShader(this->LightID, this->fShader);
        printShaderErrors(glFunctions ,this->fShader);
    }
    std::cout<< "Light Program : " <<  LightID << std::endl;
    glFunctions->glLinkProgram(this->LightID);
    printProgramErrors(glFunctions ,LightID);
    checkOpenGLError();

}
void Light::draw(const qglviewer::Camera * camera , bool islightUtime)
{
    if (islightUtime)lp = lightposition();
    else lp = LightPos;

    LightDir = lp.normalized();
    glFunctions->glUseProgram(0);
    glFunctions->glUseProgram(LightID);


    float pMatrix[16];
    float mvMatrix[16];
    camera->getProjectionMatrix(pMatrix);
    camera->getModelViewMatrix(mvMatrix);

    glFunctions->glUniformMatrix4fv(glFunctions->glGetUniformLocation(LightID, "proj_matrix"),
                                    1, GL_FALSE, pMatrix);
    glFunctions->glUniformMatrix4fv(glFunctions->glGetUniformLocation(LightID, "mv_matrix"),
                                    1, GL_FALSE, mvMatrix);


    glFunctions->glUniform1f(glFunctions->glGetUniformLocation(LightID, "rayon"), rayon);

    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(LightID, "LightPos"),1, &lp[0]);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(LightID, "LightColor"),1, &LightColor[0]);


    drawSquareForLight();

}
void Light::drawSquareForLight()
{

    float xMin = lp.x() - rayon, xMax = lp.x() + rayon;
    float yMin = lp.y() - rayon, yMax = lp.y() + rayon;

    glBegin(GL_QUADS);
    glVertex3f(xMin, yMin, lp.z());
    glVertex3f(xMin, yMax, lp.z());
    glVertex3f(xMax, yMax, lp.z());
    glVertex3f(xMax, yMin, lp.z());
    glEnd();

}
void Light::setXlightposDisplay(float _x){
    LightPos[0]=_x;
}
void Light::setYlightposDisplay(float _y){
    LightPos[1]=_y;
}
void Light::setZlightposDisplay(float _z){
    LightPos[2]=_z;
}
void Light::setRlightcolDisplay(float _r){
    LightColor[0]=_r;
}
void Light::setGlightcolDisplay(float _g){
    LightColor[1]=_g;
}
void Light::setBlightcolDisplay(float _b){
    LightColor[2]=_b;
}
void Light::setRayon(float _r){
    rayon = _r;
}
