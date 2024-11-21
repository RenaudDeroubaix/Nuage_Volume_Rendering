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
    timer.start();

    resolutionBruit = QVector3D(128.0,128.0,128.0);

    xMax = 128;
    yMax = 128;
    zMax = 128;
    LightEch = 10;
    NuageEch = 10;
    BBmin = QVector3D(-0.5,-0.5,-0.5);

    BBmax = QVector3D(0.5,0.5,0.5);

    LightPos =  QVector3D(0.0,1.0,0.0);
    LightColor =  QVector3D(1.0,1.0,1.0);

    plans.push_back(Plan(
        QVector3D(BBmin.x(), BBmin.y(), BBmin.z()),  // Bottom-left corner of the back face
        QVector3D(-1.0, 0.0, 0.0),                  // Normal points towards -X
        QVector3D(0.0, BBmax.y() - BBmin.y(), 0.0), // Right vector spans in Y
        QVector3D(0.0, 0.0, BBmax.z() - BBmin.z())  // Up vector spans in Z
        ));
    plans.push_back(Plan(
        QVector3D(BBmin.x(), BBmin.y(), BBmin.z()),  // Bottom-left corner of the bottom face
        QVector3D(0.0, -1.0, 0.0),                  // Normal points towards -Y
        QVector3D(BBmax.x() - BBmin.x(), 0.0, 0.0), // Right vector spans in X
        QVector3D(0.0, 0.0, BBmax.z() - BBmin.z())  // Up vector spans in Z
        ));
    plans.push_back(Plan(
        QVector3D(BBmin.x(), BBmin.y(), BBmin.z()),  // Bottom-left corner of the left face
        QVector3D(0.0, 0.0, -1.0),                  // Normal points towards -Z
        QVector3D(BBmax.x() - BBmin.x(), 0.0, 0.0), // Right vector spans in X
        QVector3D(0.0, BBmax.y() - BBmin.y(), 0.0)  // Up vector spans in Y
        ));
    plans.push_back(Plan(
        QVector3D(BBmax.x(), BBmin.y(), BBmin.z()),  // Bottom-left corner of the front face
        QVector3D(1.0, 0.0, 0.0),                   // Normal points towards +X
        QVector3D(0.0, BBmax.y() - BBmin.y(), 0.0), // Right vector spans in Y
        QVector3D(0.0, 0.0, BBmax.z() - BBmin.z())  // Up vector spans in Z
        ));
    plans.push_back(Plan(
        QVector3D(BBmin.x(), BBmax.y(), BBmin.z()),  // Bottom-left corner of the top face
        QVector3D(0.0, 1.0, 0.0),                   // Normal points towards +Y
        QVector3D(BBmax.x() - BBmin.x(), 0.0, 0.0), // Right vector spans in X
        QVector3D(0.0, 0.0, BBmax.z() - BBmin.z())  // Up vector spans in Z
        ));
    plans.push_back(Plan(
        QVector3D(BBmin.x(), BBmin.y(), BBmax.z()),  // Bottom-left corner of the right face
        QVector3D(0.0, 0.0, 1.0),                   // Normal points towards +Z
        QVector3D(BBmax.x() - BBmin.x(), 0.0, 0.0), // Right vector spans in X
        QVector3D(0.0, BBmax.y() - BBmin.y(), 0.0)  // Up vector spans in Y
        ));


    absorptionNuage = 0.9;
    couleurNuage =QVector3D(1.0,1.0,1.0);

    textureCreated = false;


}

void Texture::recompileShaders() {
    init();
    glFunctions->glDetachShader(this->programID, this->vShader);
    glFunctions->glDeleteShader(this->vShader);

    glFunctions->glDetachShader(this->programID, this->fShader);
    glFunctions->glDeleteShader(this->fShader);

    glFunctions->glDetachShader(this->computeID, this->cShader);
    glFunctions->glDeleteShader(this->cShader);

    std::string path = "GLSL/shaders/";
    std::string vShaderPath = path + "volume.vert";
    std::string fShaderPath = path + "volume.frag";
    std::string cShaderPath = path + "volume.comp";

    glFunctions = glContext->extraFunctions();
    glEnable( GL_DEBUG_OUTPUT );
    glFunctions->glDebugMessageCallback(&Texture::MessageCallback, 0 );

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
     glFunctions->glLinkProgram(this->programID);
     glFunctions->glLinkProgram(this->computeID);
     initTexture();
     computePass();



}

void Texture::initLightShader() {
    std::string path = "GLSL/shaders/";
    std::string vShaderPath = path + "light.vert";
    std::string fShaderPath = path + "light.frag";

    // Créer un programme spécifique pour la lumière
    this->lightID = glFunctions->glCreateProgram();

    // Charger et compiler le Vertex Shader
    std::string content = readShaderSource(vShaderPath);
    if (!content.empty()) {
        GLuint vShader = glFunctions->glCreateShader(GL_VERTEX_SHADER);
        const char* src = content.c_str();
        glFunctions->glShaderSource(vShader, 1, &src, NULL);
        glFunctions->glCompileShader(vShader);
        glFunctions->glAttachShader(this->lightID, vShader);
        printShaderErrors(vShader);
        glFunctions->glDeleteShader(vShader); // Shader inutile après l'attachement
    }

    // Charger et compiler le Fragment Shader
    content = readShaderSource(fShaderPath);
    if (!content.empty()) {
        GLuint fShader = glFunctions->glCreateShader(GL_FRAGMENT_SHADER);
        const char* src = content.c_str();
        glFunctions->glShaderSource(fShader, 1, &src, NULL);
        glFunctions->glCompileShader(fShader);
        glFunctions->glAttachShader(this->lightID, fShader);
        printShaderErrors(fShader);
        glFunctions->glDeleteShader(fShader); // Shader inutile après l'attachement
    }

    // Lier le programme de shaders
    glFunctions->glLinkProgram(this->lightID);
    printProgramErrors(this->lightID);
}


void Texture::initGLSL(){
    std::string path = "GLSL/shaders/";
    std::string vShaderPath = path + "volume.vert";
    std::string fShaderPath = path + "volume.frag";
    std::string cShaderPath = path + "volume.comp";

    glFunctions = glContext->extraFunctions();
    glEnable( GL_DEBUG_OUTPUT );
    glFunctions->glDebugMessageCallback(&Texture::MessageCallback, 0 );


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

    glFunctions->glLinkProgram(this->programID);
    glFunctions->glLinkProgram(this->computeID);
    printProgramErrors(programID);
    checkOpenGLError();
    initLightShader();
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
    glFunctions->glUseProgram(0);
    glFunctions->glUseProgram(computeID);

    glFunctions->glUniform1f(glFunctions->glGetUniformLocation(computeID, "u_time"), timer.elapsed()/5000.0);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(computeID, "resolution"),1, &resolutionBruit[0]);

    glFunctions->glBindTexture(GL_TEXTURE_3D, textureId);
    glFunctions->glBindImageTexture (0, textureId, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glFunctions->glDispatchCompute(16,16,16);
    glFunctions->glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    glFunctions->glBindImageTexture (0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glFunctions->glBindTexture(GL_TEXTURE_3D, 0);
    glFunctions->glUseProgram(programID);
}

void Texture::draw( const qglviewer::Camera * camera ){
    computePass();


    if(!textureCreated)
        return;

    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_3D);
    //glPolygonMode( GL_FRONT , GL_FILL );

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

   glFunctions->glUniform1i(glFunctions->glGetUniformLocation(programID, "LightSample"), LightEch);
   glFunctions->glUniform1i(glFunctions->glGetUniformLocation(programID, "NuageSample"), NuageEch);

   glFunctions->glUniform1f(glFunctions->glGetUniformLocation(programID, "absorptionNuage"), absorptionNuage);
   glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "couleurNuage"),1, &couleurNuage[0]);

   glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "LightPos"),1, &LightPos[0]);
   glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "LightColor"),1, &LightColor[0]);


   glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "BBmin"),1,&BBmin[0]);
   glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "BBmax"),1,&BBmax[0]);

    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "plans[0].normale"),1, &plans[0].normale[0]);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "plans[0].point"),1, &plans[0].point[0]);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "plans[0].up_vect"),1, &plans[0].up_vect[0]);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "plans[0].right_vect"),1, &plans[0].right_vect[0]);


    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "plans[1].normale"),1, &plans[1].normale[0]);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "plans[1].point"),1, &plans[1].point[0]);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "plans[1].up_vect"),1, &plans[1].up_vect[0]);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "plans[1].right_vect"),1, &plans[1].right_vect[0]);

    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "plans[2].normale"),1, &plans[2].normale[0]);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "plans[2].point"),1, &plans[2].point[0]);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "plans[2].up_vect"),1, &plans[2].up_vect[0]);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "plans[2].right_vect"),1, &plans[2].right_vect[0]);

    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "plans[3].normale"),1, &plans[3].normale[0]);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "plans[3].point"),1, &plans[3].point[0]);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "plans[3].up_vect"),1, &plans[3].up_vect[0]);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "plans[3].right_vect"),1, &plans[3].right_vect[0]);

    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "plans[4].normale"),1, &plans[4].normale[0]);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "plans[4].point"),1, &plans[4].point[0]);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "plans[4].up_vect"),1, &plans[4].up_vect[0]);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "plans[4].right_vect"),1, &plans[4].right_vect[0]);

    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "plans[5].normale"),1, &plans[5].normale[0]);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "plans[5].point"),1, &plans[5].point[0]);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "plans[5].up_vect"),1, &plans[5].up_vect[0]);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "plans[5].right_vect"),1, &plans[5].right_vect[0]);



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
    drawLight(camera);

}

void Texture::drawCube(){
//    Définition des limites du cube entre -0.5 et 0.5 pour x, y et z
//    float xMinCube = -0.5, xMaxCube = 0.5;
//    float yMinCube = -0.5, yMaxCube = 0.5;
//    float zMinCube = -0.5, zMaxCube = 0.5;

//    float xMinCube =0, xMaxCube = 128;
//    float yMinCube = 0, yMaxCube = 128;
//    float zMinCube = 0, zMaxCube = 128;

    float xMinCube = BBmin.x(), xMaxCube = BBmax.x();
    float yMinCube = BBmin.y(), yMaxCube = BBmax.y();
    float zMinCube = BBmin.z(), zMaxCube = BBmax.z();

    glBegin(GL_QUADS);

        // Face arrière (normale vers -Z)
        glVertex3f(xMinCube, yMinCube, zMinCube); // Bottom Left
        glVertex3f(xMaxCube, yMinCube, zMinCube); // Bottom Right
        glVertex3f(xMaxCube, yMaxCube, zMinCube); // Top Right
        glVertex3f(xMinCube, yMaxCube, zMinCube); // Top Left

        // Face avant (normale vers +Z)
        glVertex3f(xMinCube, yMinCube, zMaxCube); // Bottom Left
        glVertex3f(xMinCube, yMaxCube, zMaxCube); // Top Left
        glVertex3f(xMaxCube, yMaxCube, zMaxCube); // Top Right
        glVertex3f(xMaxCube, yMinCube, zMaxCube); // Bottom Right

        // Face gauche (normale vers -X)
        glVertex3f(xMinCube, yMinCube, zMaxCube); // Bottom Right
        glVertex3f(xMinCube, yMinCube, zMinCube); // Bottom Left
        glVertex3f(xMinCube, yMaxCube, zMinCube); // Top Left
        glVertex3f(xMinCube, yMaxCube, zMaxCube); // Top Right

        // Face droite (normale vers +X)
        glVertex3f(xMaxCube, yMaxCube, zMinCube); // Top Left
        glVertex3f(xMaxCube, yMinCube, zMinCube); // Bottom Left
        glVertex3f(xMaxCube, yMinCube, zMaxCube); // Bottom Right
        glVertex3f(xMaxCube, yMaxCube, zMaxCube); // Top Right


        // Face du bas (normale vers -Y
        glVertex3f(xMaxCube, yMinCube, zMinCube); // Bottom Right
        glVertex3f(xMinCube, yMinCube, zMinCube); // Bottom Left
        glVertex3f(xMinCube, yMinCube, zMaxCube); // Top Left
        glVertex3f(xMaxCube, yMinCube, zMaxCube); // Top Right


        // Face du haut (normale vers +Y)
        glVertex3f(xMinCube, yMaxCube, zMaxCube); // Bottom Right
        glVertex3f(xMinCube, yMaxCube, zMinCube); // Bottom Left
        glVertex3f(xMaxCube, yMaxCube, zMinCube); // Top Left
        glVertex3f(xMaxCube, yMaxCube, zMaxCube); // Top Right


        glEnd();
}


void Texture::drawLight(const qglviewer::Camera *camera) {
    glFunctions->glUseProgram(0);
    glFunctions->glUseProgram(lightID);

    // Matrices pour la caméra
    float pMatrix[16];
    float mvMatrix[16];
    camera->getProjectionMatrix(pMatrix);
    camera->getModelViewMatrix(mvMatrix);
    glFunctions->glUniformMatrix4fv(glFunctions->glGetUniformLocation(lightID, "proj_matrix"),
                                    1, GL_FALSE, pMatrix);
    glFunctions->glUniformMatrix4fv(glFunctions->glGetUniformLocation(lightID, "mv_matrix"),
                                    1, GL_FALSE, mvMatrix);

    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(lightID, "lightColor"), 1, &LightColor[0]);

    // Rayon et subdivisions
    float radius = 0.1f;
    int latSegments = 20; // Segments de latitude
    int lonSegments = 20; // Segments de longitude

    // Dessiner la sphère
    for (int i = 0; i < latSegments; ++i) {
        float lat0 = M_PI * (-0.5f + (float)(i) / latSegments);       // Angle latitude début
        float lat1 = M_PI * (-0.5f + (float)(i + 1) / latSegments);   // Angle latitude fin
        float sinLat0 = sin(lat0), cosLat0 = cos(lat0);
        float sinLat1 = sin(lat1), cosLat1 = cos(lat1);

        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= lonSegments; ++j) {
            float lon = 2 * M_PI * (float)(j) / lonSegments; // Angle longitude
            float sinLon = sin(lon), cosLon = cos(lon);

            // Point 1
            float x1 = cosLon * cosLat0;
            float y1 = sinLon * cosLat0;
            float z1 = sinLat0;
            glVertex3f(LightPos.x() + radius * x1, LightPos.y() + radius * y1, LightPos.z() + radius * z1);

            // Point 2
            float x2 = cosLon * cosLat1;
            float y2 = sinLon * cosLat1;
            float z2 = sinLat1;
            glVertex3f(LightPos.x() + radius * x2, LightPos.y() + radius * y2, LightPos.z() + radius * z2);
        }
        glEnd();
    }

    glFunctions->glUseProgram(0);
    glFunctions->glUseProgram(programID);
}




void Texture::drawBoundingBox(bool fill){

    //glPolygonMode (GL_FRONT_AND_BACK, fill ? GL_FILL : GL_FILL);
    //glColor3f(1.f,0.f,0.f);
    drawCube();
    //glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

}



void Texture::setLightEch(int value)
{
    LightEch = value;
}
void Texture::setNuageEch(int value)
{
    NuageEch = value;
}
void Texture::setRedNuageDisplay(float _r){
    couleurNuage[0] = _r;
}
void Texture::setGreenNuageDisplay(float _g){
    couleurNuage[1]=_g;
}
void Texture::setBlueNuageDisplay(float _b){
    couleurNuage[2]=_b;
}
void Texture::setXlightposDisplay(float _x){
    LightPos[0]=_x;
}
void Texture::setYlightposDisplay(float _y){
    LightPos[1]=_y;
}
void Texture::setZlightposDisplay(float _z){
    LightPos[2]=_z;
}
void Texture::setRlightcolDisplay(float _r){
    LightColor[0]=_r;
}
void Texture::setGlightcolDisplay(float _g){
    LightColor[1]=_g;
}
void Texture::setBlightcolDisplay(float _b){
    LightColor[2]=_b;
}
void Texture::setAbsorptionNuageDisplay(float _a){
    absorptionNuage=_a;
}
void Texture::setResolutionBruitX(float _x){
    resolutionBruit[0]=_x;
}
void Texture::setResolutionBruitY(float _y){
    resolutionBruit[1]=_y;
}
void Texture::setResolutionBruitZ(float _z){
    resolutionBruit[2]=_z;
}

void Texture::clear(){

    if( textureCreated )
        glDeleteTextures(1, &textureId);

    init();

}

