#include "../header/skybox.h"


SkyBox::SkyBox(QOpenGLContext* context)
{
   glContext = context;
   init();
   initShader();
}
void SkyBox::init(){
    maxy = 55.0;

    BBmin = QVector3D(-500.0,-500.0,-500.0) ;
    BBmax = QVector3D(500.0,500.0,500.0) ;
}
void SkyBox::initShader(){
    std::string path = "GLSL/shaders/";
    std::string fShaderPath = path + "skybox.frag";
    std::string vShaderPath = path + "skybox.vert";


    glFunctions = glContext->extraFunctions();
    glEnable( GL_DEBUG_OUTPUT );
    glFunctions->glDebugMessageCallback(&MessageCallback, 0 );


    // Create programs and link shaders
    this->ProgramID = glFunctions->glCreateProgram();

    std::string content = readShaderSource(vShaderPath);
    if (!content.empty()) {
        this->vShader = glFunctions->glCreateShader(GL_VERTEX_SHADER);
        const char* src = content.c_str();
        glFunctions->glShaderSource(this->vShader, 1, &src, NULL);
        glFunctions->glCompileShader(this->vShader);
        glFunctions->glAttachShader(this->ProgramID, this->vShader);
        printShaderErrors(glFunctions ,this->vShader);
    }


    content = readShaderSource(fShaderPath);
    if (!content.empty()) {
        this->fShader = glFunctions->glCreateShader(GL_FRAGMENT_SHADER);
        const char* src = content.c_str();
        glFunctions->glShaderSource(this->fShader, 1, &src, NULL);
        glFunctions->glCompileShader(this->fShader);
        glFunctions->glAttachShader(this->ProgramID, this->fShader);
        printShaderErrors(glFunctions ,this->fShader);
    }
    std::cout<< "Skybox Program : " << ProgramID << std::endl;
    glFunctions->glLinkProgram(this->ProgramID);
    printProgramErrors(glFunctions ,ProgramID);
    checkOpenGLError();



}
void SkyBox::draw(QVector3D & LightPos ,  QVector3D & LightCol  , QVector3D & LightDir ,  const qglviewer::Camera * camera )
{
    glFunctions->glUseProgram(0);
    glFunctions->glUseProgram(ProgramID);

    float pMatrix[16];
    float mvMatrix[16];
    camera->getProjectionMatrix(pMatrix);
    camera->getModelViewMatrix(mvMatrix);

    glFunctions->glUniformMatrix4fv(glFunctions->glGetUniformLocation(ProgramID, "proj_matrix"),
                                    1, GL_FALSE, pMatrix);
    glFunctions->glUniformMatrix4fv(glFunctions->glGetUniformLocation(ProgramID, "mv_matrix"),
                                    1, GL_FALSE, mvMatrix);
    glFunctions->glUniform1f(glFunctions->glGetUniformLocation(ProgramID, "maxy"), maxy);


    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(ProgramID, "LightPos"),1, &LightPos[0]);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(ProgramID, "LightDir"),1, &LightDir[0]);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(ProgramID, "LightColor"),1, &LightCol[0]);



    drawCube();

}
void SkyBox::drawCube(){

    glBegin(GL_QUADS);

    // Face arrière (normale vers -Z)
    glVertex3f(BBmin.x(), BBmin.y(), BBmin.z()); // Bottom Left
    glVertex3f(BBmax.x(), BBmin.y(), BBmin.z()); // Bottom Right
    glVertex3f(BBmax.x(), BBmax.y(), BBmin.z()); // Top Right
    glVertex3f(BBmin.x(), BBmax.y(), BBmin.z()); // Top Left

    // Face avant (normale vers +Z)
    glVertex3f(BBmin.x(), BBmin.y(), BBmax.z()); // Bottom Left
    glVertex3f(BBmin.x(), BBmax.y(), BBmax.z()); // Top Left
    glVertex3f(BBmax.x(), BBmax.y(), BBmax.z()); // Top Right
    glVertex3f(BBmax.x(), BBmin.y(), BBmax.z()); // Bottom Right

    // Face gauche (normale vers -X)
    glVertex3f(BBmin.x(), BBmin.y(), BBmax.z()); // Bottom Right
    glVertex3f(BBmin.x(), BBmin.y(), BBmin.z()); // Bottom Left
    glVertex3f(BBmin.x(), BBmax.y(), BBmin.z()); // Top Left
    glVertex3f(BBmin.x(), BBmax.y(), BBmax.z()); // Top Right

    // Face droite (normale vers +X)
    glVertex3f(BBmax.x(), BBmax.y(), BBmin.z()); // Top Left
    glVertex3f(BBmax.x(), BBmin.y(), BBmin.z()); // Bottom Left
    glVertex3f(BBmax.x(), BBmin.y(), BBmax.z()); // Bottom Right
    glVertex3f(BBmax.x(), BBmax.y(), BBmax.z()); // Top Right


    // Face du bas (normale vers -Y)
    glVertex3f(BBmax.x(), BBmin.y(), BBmin.z()); // Bottom Right
    glVertex3f(BBmin.x(), BBmin.y(), BBmin.z()); // Bottom Left
    glVertex3f(BBmin.x(), BBmin.y(), BBmax.z()); // Top Left
    glVertex3f(BBmax.x(), BBmin.y(), BBmax.z()); // Top Right


    // Face du haut (normale vers +Y)
    glVertex3f(BBmin.x(), BBmax.y(), BBmax.z()); // Bottom Right
    glVertex3f(BBmin.x(), BBmax.y(), BBmin.z()); // Bottom Left
    glVertex3f(BBmax.x(), BBmax.y(), BBmin.z()); // Top Left
    glVertex3f(BBmax.x(), BBmax.y(), BBmax.z()); // Top Right


    glEnd();
}

