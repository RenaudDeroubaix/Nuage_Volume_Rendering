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
    // Récupérer les informations sous forme de chaînes
    const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

    // Afficher les informations correctement
    qDebug() << "OpenGL Version:" << version;
    qDebug() << "Renderer:" << renderer;

    int workGroupCount[3], workGroupSize[3], sharedMemorySize;
    glFunctions = glContext->extraFunctions();
    glFunctions->glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &workGroupCount[0]);
    glFunctions->glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &workGroupCount[1]);
    glFunctions->glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &workGroupCount[2]);

    glFunctions->glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &workGroupSize[0]);
    glFunctions->glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &workGroupSize[1]);
    glFunctions->glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &workGroupSize[2]);

    glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &sharedMemorySize);

    std::cout << "Max work group counts: (" << workGroupCount[0] << ", " << workGroupCount[1] << ", " << workGroupCount[2] << ")\n";
    std::cout << "Max work group sizes: (" << workGroupSize[0] << ", " << workGroupSize[1] << ", " << workGroupSize[2] << ")\n";
    std::cout << "Max shared memory size: " << sharedMemorySize << " bytes\n";
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
    freqBruit =QVector4D(4.0,8.0,16.0,32.0);

    LightEch = 3;
    NuageEch = 10;
    BBmin = QVector3D(-0.5,-0.5,-0.5) ;

    BBmax = QVector3D(0.5,0.5,0.5) ;

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
    printProgramErrors(computeID);
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
    glFunctions->glBindTexture(GL_TEXTURE_3D, textureId);



	//TODO complete texture options
    glFunctions->glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glFunctions->glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFunctions->glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Interpolation : linéaire à l'échantillonnage et filtrage
    glFunctions->glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Minification : interpolation linéaire
    glFunctions->glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Magnification : interpolation linéaire

    // Filtrage par plus proche voisin si besoin :
    // glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Charger les données de la texture dans OpenGL
    glFunctions->glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, resolutionBruit[0], resolutionBruit[1], resolutionBruit[2], 0, GL_RGBA, GL_FLOAT, nullptr);
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
    glFunctions->glUniform4fv(glFunctions->glGetUniformLocation(computeID, "frequenceWorley"),1, &freqBruit[0]);

    glFunctions->glBindTexture(GL_TEXTURE_3D, textureId);
    glFunctions->glBindImageTexture (0, textureId, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    QVector3D reso=QVector3D(ceil(resolutionBruit[0]/8),ceil(resolutionBruit[1]/8),ceil(resolutionBruit[2]/8));
    glFunctions->glDispatchCompute(reso[0],reso[1],reso[2]);
    glFunctions->glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    glFunctions->glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
    glFunctions->glBindImageTexture (0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glFunctions->glBindTexture(GL_TEXTURE_3D, 0);
    glFunctions->glUseProgram(programID);
}

void Texture::draw( const qglviewer::Camera * camera ){
    computePass();


    if(!textureCreated)
        return;

    glDisable(GL_LIGHTING);
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


    glFunctions->glUniform1f(glFunctions->glGetUniformLocation(programID, "xMax"), resolutionBruit[0]);
   glFunctions->glUniform1f(glFunctions->glGetUniformLocation(programID, "yMax"), resolutionBruit[1]);
   glFunctions->glUniform1f(glFunctions->glGetUniformLocation(programID, "zMax"), resolutionBruit[2]);

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


   glFunctions->glActiveTexture(GL_TEXTURE0 + textureId);
   glFunctions->glBindTexture(GL_TEXTURE_3D, textureId); // Bind the 3D texture
   glFunctions->glUniform1i(glFunctions->glGetUniformLocation(programID, "tex"), textureId);

    /***********************************************************************/
    drawBoundingBox(false);

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


           // Face du bas (normale vers -Y)
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
//    std::cout << resolutionBruit[0] <<std::endl;
    initTexture();
}
void Texture::setResolutionBruitY(float _y){
    resolutionBruit[1]=_y;
//    std::cout << resolutionBruit[1] <<std::endl;
    initTexture();
}
void Texture::setResolutionBruitZ(float _z){
    resolutionBruit[2]=_z;
//    std::cout << resolutionBruit[2] <<std::endl;
    initTexture();
}
void Texture::setFreqBruitR(float _r){
    freqBruit[0]=_r;
}
void Texture::setFreqBruitG(float _g){
    freqBruit[1]=_g;
}
void Texture::setFreqBruitB(float _b){
    freqBruit[2]=_b;
}
void Texture::setFreqBruitA(float _a){
    freqBruit[3]=_a;
}

void Texture::clear(){

    if( textureCreated )
        glDeleteTextures(1, &textureId);

    init();

}

