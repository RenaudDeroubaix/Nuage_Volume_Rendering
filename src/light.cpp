
#include "../header/light.h"


Light::Light(QOpenGLContext* context)
{
   glContext = context;
   init();
   initLightShader();
}
void Light::init(){

    LightPos =  QVector3D(0.0,1.0,0.0);
    LightColor =  QVector3D(1.0,1.0,1.0);
    rayon = 1.f;


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

    glFunctions->glLinkProgram(this->LightID);
    printProgramErrors(glFunctions ,LightID);
    checkOpenGLError();

}
void Light::draw(const qglviewer::Camera * camera )
{
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

    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(LightID, "LightPos"),1, &LightPos[0]);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(LightID, "LightColor"),1, &LightColor[0]);


    drawSquareForLight();

}
void Light::drawSquareForLight()
{

    float xMin = LightPos.x() - rayon, xMax = LightPos.x() + rayon;
    float yMin = LightPos.y() - rayon, yMax = LightPos.y() + rayon;

    glBegin(GL_QUADS);
    glVertex3f(xMin, yMin, LightPos.z()); // Utilisez LightPos.z au lieu de 0
    glVertex3f(xMin, yMax, LightPos.z());
    glVertex3f(xMax, yMax, LightPos.z());
    glVertex3f(xMax, yMin, LightPos.z());


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
    rayon=_r;
}
