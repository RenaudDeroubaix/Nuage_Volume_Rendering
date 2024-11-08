#include <iostream>
#include <fstream>
#include <algorithm>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "../header/Texture.h"

#include <complex>


Texture::Texture(QOpenGLContext* context)
{

    glContext = context;
    init();
    initGLSL();
    initTexture();
    computePass();
}

Texture::~Texture(){
    if(textureCreated)
        deleteTexture();
}

void Texture::init(){

    //Set texture to cube of size 1.
    xMax = 128;
    yMax = 128;
    zMax = 128;

    textureCreated = false;


}

void Texture::recompileShaders() {
    std::string path = "GLSL/shaders/";
    std::string vShaderPath = path + "volume.vert";
    std::string fShaderPath = path + "volume.frag";
    std::string cShaderPath = path + "volume.comp";

    glFunctions = glContext->extraFunctions();
    glEnable( GL_DEBUG_OUTPUT );
    glFunctions->glDebugMessageCallback(&Texture::MessageCallback, 0 );

    // Create programs and link shaders
    this->programID = glFunctions->glCreateProgram();
    std::string content = readShaderSource(cShaderPath);
    if (!content.empty()) {
        this->cShader = glFunctions->glCreateShader(GL_COMPUTE_SHADER);
        const char* src = content.c_str();
        glFunctions->glShaderSource(this->cShader, 1, &src, NULL);
        glFunctions->glCompileShader(this->cShader);
        glFunctions->glAttachShader(this->programID, this->cShader);
        printShaderErrors(this->cShader);
    }
    content = readShaderSource(vShaderPath);    if (!content.empty()) {
        this->vShader = glFunctions->glCreateShader(GL_VERTEX_SHADER);
        const char* src = content.c_str();
        glFunctions->glShaderSource(this->vShader, 1, &src, NULL);
        glFunctions->glCompileShader(this->vShader);
        glFunctions->glAttachShader(this->programID, this->vShader);
        printShaderErrors(this->vShader);
    }
    content = readShaderSource(fShaderPath);
    if (!content.empty()) {
        this->fShader = glFunctions->glCreateShader(GL_FRAGMENT_SHADER);
        const char* src = content.c_str();
        glFunctions->glShaderSource(this->fShader, 1, &src, NULL);
        glFunctions->glCompileShader(this->fShader);
        glFunctions->glAttachShader(this->programID, this->fShader);
        printShaderErrors(this->fShader);
    }

    glFunctions->glLinkProgram(this->programID);
    glFunctions->glUseProgram(programID);
    printProgramErrors(programID);
    checkOpenGLError();

    initTexture();
}

void Texture::initGLSL(){
    std::string path = "GLSL/shaders/";
    std::string vShaderPath = path + "volume.vert";
    std::string fShaderPath = path + "volume.frag";
    std::string cShaderPath = path + "volume.comp";

    glFunctions = glContext->extraFunctions();
    glEnable( GL_DEBUG_OUTPUT );
    glFunctions->glDebugMessageCallback(&Texture::MessageCallback, 0 );

    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    // Create programs and link shaders
    this->computeID = glFunctions->glCreateProgram();
    this->programID = glFunctions->glCreateProgram();

    std::string content = readShaderSource(cShaderPath);
    if (!content.empty()) {
        this->cShader = glFunctions->glCreateShader(GL_COMPUTE_SHADER);
        const char* src = content.c_str();
        glFunctions->glShaderSource(this->cShader, 1, &src, NULL);
        glFunctions->glCompileShader(this->cShader);
        glFunctions->glAttachShader(this->computeID, this->cShader);
        printShaderErrors(this->cShader);
    }

    content = readShaderSource(vShaderPath);
    if (!content.empty()) {
        this->vShader = glFunctions->glCreateShader(GL_VERTEX_SHADER);
        const char* src = content.c_str();
        glFunctions->glShaderSource(this->vShader, 1, &src, NULL);
        glFunctions->glCompileShader(this->vShader);
        glFunctions->glAttachShader(this->programID, this->vShader);
        printShaderErrors(this->vShader);
    }
    content = readShaderSource(fShaderPath);
    if (!content.empty()) {

        this->fShader = glFunctions->glCreateShader(GL_FRAGMENT_SHADER);
        const char* src = content.c_str();
        glFunctions->glShaderSource(this->fShader, 1, &src, NULL);
        glFunctions->glCompileShader(this->fShader);
        glFunctions->glAttachShader(this->programID, this->fShader);
        printShaderErrors(this->fShader);
    }


    printProgramErrors(programID);
    checkOpenGLError();
}

void /*GLAPIENTRY */Texture::MessageCallback( GLenum source, GLenum type,
                                              GLuint id, GLenum severity,
                                              GLsizei length, const GLchar* message,
                                              const void* userParam )
{
    if (severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM || severity == GL_DEBUG_SEVERITY_LOW) {
        std::string s_severity = (severity == GL_DEBUG_SEVERITY_HIGH ? "High" : severity == GL_DEBUG_SEVERITY_MEDIUM ? "Medium" : "Low");
        std::cout << "Error " << id << " [severity=" << s_severity << "]: " << message << std::endl;
    }
}
bool Texture::checkOpenGLError()
{
    bool error = false;
    int glErr = glGetError();
    while(glErr != GL_NO_ERROR)
    {
        std::cout << "[OpenGL] Error: " << glErr << std::endl;
        error = true;
        glErr = glGetError();
    }
    return !error;
}

bool Texture::printShaderErrors(GLuint shader)
{
    int state = 0;
    glFunctions->glGetShaderiv(shader, GL_COMPILE_STATUS, &state);
    if (state == 1)
        return true;
    int len = 0;
    int chWritten = 0;
    char* log;
    glFunctions->glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0)
    {
        log = (char*)malloc(len);
        glFunctions->glGetShaderInfoLog(shader, len, &chWritten, log);
        std::cout << "[OpenGL] Shader error: " << log << std::endl;
        free(log);
    }
    return false;
}
bool Texture::printProgramErrors(int program)
{
    int state = 0;
    glFunctions->glGetProgramiv(program, GL_LINK_STATUS, &state);
    if (state == 1)
        return true;
    int len = 0;
    int chWritten = 0;
    char* log;
    glFunctions->glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    if (len > 0)
    {
        log = (char*)malloc(len);
        glFunctions->glGetProgramInfoLog(program, len, &chWritten, log);
        std::cout << "[OpenGL] Program error: " << log << std::endl;
        free(log);
    }
    return false;
}

std::string Texture::readShaderSource(std::string filename)
{
    std::string content = "";
    QString qFilename = QString::fromStdString(filename);
    if (!QFile::exists(qFilename))
        qFilename = ":" + qFilename;
    if (!QFile::exists(qFilename)) {
        std::cerr << "The shader " << filename << " doesn't exist!" << std::endl;
        return "";
    }
    QFile file(qFilename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    std::string line;
    QTextStream in(&file);
    while (!in.atEnd()) {
        line = in.readLine().toStdString();
        content += line + " \n";
    }
    file.close();
    return content;
}


void Texture::initTexture(){


    if(textureId != 0){
        glDeleteTextures(1, &textureId);
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_3D, textureId);



	//TODO complete texture options
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Interpolation : linéaire à l'échantillonnage et filtrage
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Minification : interpolation linéaire
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Magnification : interpolation linéaire

    // Filtrage par plus proche voisin si besoin :
    // glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Charger les données de la texture dans OpenGL
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, xMax, yMax, zMax, 0, GL_RGBA, GL_FLOAT, nullptr);
    textureCreated = true;

}


void Texture::deleteTexture(){
    glDeleteTextures( 1, &textureId);
}

void Texture::computePass() {
    glFunctions->glLinkProgram(this->computeID);
    glFunctions->glUseProgram(computeID);
    glFunctions->glBindTexture(GL_TEXTURE_3D, textureId);
    glFunctions->glBindImageTexture (0, textureId, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glFunctions->glDispatchCompute(16,16,16);
    glFunctions->glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    glFunctions->glBindImageTexture (0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glFunctions->glBindTexture(GL_TEXTURE_3D, 0);
    glFunctions->glLinkProgram(this->programID);
}

void Texture::draw( const qglviewer::Camera * camera ){
    glFunctions->glUseProgram(programID);

    if(!textureCreated)
        return;

    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_3D);

    glPolygonMode( GL_FRONT_AND_BACK , GL_FILL );

    // GPU start
    // Récuperation des matrices de projection / vue-modèle

    float pMatrix[16];
    float mvMatrix[16];
    camera->getProjectionMatrix(pMatrix);
    camera->getModelViewMatrix(mvMatrix);
    glFunctions->glUniformMatrix4fv(glFunctions->glGetUniformLocation(programID, "proj_matrix"),
                                    1, GL_FALSE, pMatrix);
    glFunctions->glUniformMatrix4fv(glFunctions->glGetUniformLocation(programID, "mv_matrix"),
                                    1, GL_FALSE, mvMatrix);


    ///***********************************************************************/
    ////Parameters to given to the shader // TODO complete
    /***********************************************************************/


   glFunctions->glUniform1f(glFunctions->glGetUniformLocation(programID, "xMax"), xMax);
   glFunctions->glUniform1f(glFunctions->glGetUniformLocation(programID, "yMax"), yMax);
   glFunctions->glUniform1f(glFunctions->glGetUniformLocation(programID, "zMax"), zMax);

//   glFunctions->glUniform1f(glFunctions->glGetUniformLocation(programID, "xCutPosition"), xCutPosition);
//   glFunctions->glUniform1f(glFunctions->glGetUniformLocation(programID, "yCutPosition"), yCutPosition);
//   glFunctions->glUniform1f(glFunctions->glGetUniformLocation(programID, "zCutPosition"), zCutPosition);

//   glFunctions->glUniform1i(glFunctions->glGetUniformLocation(programID, "xCutDirection"), xCutDirection);
//   glFunctions->glUniform1i(glFunctions->glGetUniformLocation(programID, "yCutDirection"), yCutDirection);
//   glFunctions->glUniform1i(glFunctions->glGetUniformLocation(programID, "zCutDirection"), zCutDirection);


   glActiveTexture(GL_TEXTURE0 + textureId);
   glBindTexture(GL_TEXTURE_3D, textureId); // Bind the 3D texture
   glFunctions->glUniform1i(glFunctions->glGetUniformLocation(programID, "tex"), textureId);

    /***********************************************************************/

    drawBoundingBox(false);

}

void Texture::drawCube(){
    glBegin(GL_QUADS);

    glVertex3f(0.0f, 0.0f, 0.0f);	// Bottom Right Of The Texture and Quad
    glVertex3f(0.0f, yMax, 0.0f);	// Top Right Of The Texture and Quad
    glVertex3f(xMax, yMax, 0.0f);	// Top Left Of The Texture and Quad
    glVertex3f(xMax, 0.0f, 0.0f);	// Bottom Left Of The Texture and Quad
    // Bottom Face
    glVertex3f(0.0f, 0.0f, 0.0f);	// Top Right Of The Texture and Quad
    glVertex3f(xMax, 0.0f, 0.0f);	// Top Left Of The Texture and Quad
    glVertex3f(xMax, 0.0f, zMax);	// Bottom Left Of The Texture and Quad
    glVertex3f(0.0f, 0.0f, zMax);	// Bottom Right Of The Texture and Quad
    // Left Face
    glVertex3f(0.0f, 0.0f, 0.0f);	// Bottom Left Of The Texture and Quad
    glVertex3f(0.0f, 0.0f, zMax);	// Bottom Right Of The Texture and Quad
    glVertex3f(0.0f, yMax, zMax);	// Top Right Of The Texture and Quad
    glVertex3f(0.0f, yMax, 0.0f);	// Top Left Of The Texture and Quad
    // Right face
    glVertex3f(xMax, 0.0f, 0.0f);	// Bottom Right Of The Texture and Quad
    glVertex3f(xMax, yMax, 0.0f);	// Top Right Of The Texture and Quad
    glVertex3f(xMax, yMax, zMax);	// Top Left Of The Texture and Quad
    glVertex3f(xMax, 0.0f,  zMax);	// Bottom Left Of The Texture and Quad

    // Front Face
    glVertex3f(0.0f, 0.0f, zMax);	// Bottom Left Of The Texture and Quad
    glVertex3f(xMax, 0.0f, zMax);	// Bottom Right Of The Texture and Quad
    glVertex3f(xMax, yMax, zMax);	// Top Right Of The Texture and Quad
    glVertex3f(0.0f,  yMax,  zMax);	// Top Left Of The Texture and Quad

    // Top Face
    glVertex3f(0.0f,  yMax, 0.0f);	// Top Left Of The Texture and Quad
    glVertex3f(0.0f, yMax,  zMax);	// Bottom Left Of The Texture and Quad
    glVertex3f(xMax, yMax, zMax);	// Bottom Right Of The Texture and Quad
    glVertex3f(xMax, yMax, 0.0f);	// Top Right Of The Texture and Quad
    glEnd();
}


//void Texture::drawCutPlanes(){

//    double x = xCutPosition + xCutDirection*.001;
//    double y = yCutPosition + yCutDirection*.001;
//    double z = zCutPosition + zCutDirection*.001;

//    glColor4f(1.0,0.,0.,0.25);
//    glBegin(GL_QUADS);

//    if(xCutDisplay){
//        // Right face
//        glVertex3f( x, 0.0f, 0.0f);	// Bottom Right Of The Texture and Quad
//        glVertex3f( x, yMax, 0.0f);	// Top Right Of The Texture and Quad
//        glVertex3f( x, yMax, zMax);	// Top Left Of The Texture and Quad
//        glVertex3f( x, 0.0f, zMax);	// Bottom Left Of The Texture and Quad
//    }

//    if(zCutDisplay){
//        // Front Face
//        glVertex3f(0.0f, 0.0f, z);	// Bottom Left Of The Texture and Quad
//        glVertex3f(xMax, 0.0f, z);	// Bottom Right Of The Texture and Quad
//        glVertex3f(xMax, yMax, z);	// Top Right Of The Texture and Quad
//        glVertex3f(0.0f, yMax, z);	// Top Left Of The Texture and Quad
//    }

//    if(yCutDisplay){
//        // Top Face
//        glVertex3f(0.0f, y, 0.0f);	// Top Left Of The Texture and Quad
//        glVertex3f(0.0f, y, zMax);	// Bottom Left Of The Texture and Quad
//        glVertex3f(xMax, y, zMax);	// Bottom Right Of The Texture and Quad
//        glVertex3f(xMax, y, 0.0f);	// Top Right Of The Texture and Quad
//    }
//    glEnd();


//}

void Texture::drawBoundingBox(bool fill){

    glPolygonMode (GL_FRONT_AND_BACK, fill ? GL_FILL : GL_FILL);
    glColor3f(1.f,0.f,0.f);
    drawCube();
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

}

void Texture::build(const std::vector<unsigned char> & data, const std::vector<unsigned char> & labels,
                    unsigned int & nx , unsigned int & ny , unsigned int & nz,
                    float & dx , float & dy , float & dz,
                    std::map<unsigned char, QColor> & labelsToColor){

    if(textureCreated)
        deleteTexture();




    //qDebug() << "Data.size():" << data.size() << " rgbTexture size:" << n[0]*n[1]*n[2];











    initTexture();
    computePass();
}


//void Texture::setXCut(int _xCut){
//    xCut = 1.-double(_xCut)/n[0];
//    xCutPosition = xMax*xCut;
//}

//void Texture::setYCut(int _yCut){
//    yCut = 1.- double(_yCut)/n[1];
//    yCutPosition = yMax*yCut;
//}

//void Texture::setZCut(int _zCut){
//    zCut = 1.0-double(_zCut)/n[2];
//    zCutPosition = zMax*zCut;
//}

void Texture::clear(){

    if( textureCreated )
        glDeleteTextures(1, &textureId);

    init();

}

