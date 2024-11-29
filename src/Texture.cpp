#include <iostream>
#include <fstream>
#include <string>
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
}

Texture::~Texture(){
    if(textureCreated)
        deleteTexture();
}

void Texture::init(){

    //Set texture to cube of size 1.
    timer.start();
    vitesse =5000.0;

    resolutionBruit = QVector3D(128.0,128.0,128.0);
    freqBruit =QVector4D(2.0,6.0,12.0,24.0);
    facteurBruit =QVector4D(-3.0,0.33,0.33,0.33);

    resolutionBruitCurl = QVector2D(128.0,128.0);
    freqBruitCurl =QVector3D(2.0,6.0,12.0);

    LightEch = 20;
    NuageEch = 50;

    BBmin = QVector3D(-0.5,-0.5,-0.5) ;
    BBmax = QVector3D(0.5,0.5,0.5) ;

//    BBmin = QVector3D(-10.0,-10.0,-10.0) ;
//    BBmax = QVector3D(10.0,10.0,10.0) ;



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


    absorptionNuage = 9.0;
    absorptionLight = 1.0;
    couleurNuage =QVector3D(1.0,1.0,1.0);

    textureCreated = false;


}

void Texture::recompileShaders() {
    init();

    glFunctions->glDetachShader(this->programID, this->vShader);
    glFunctions->glDeleteShader(this->vShader);

    glFunctions->glDetachShader(this->programID, this->fShader);
    glFunctions->glDeleteShader(this->fShader);

    glFunctions->glDetachShader(this->computeID, this->cShader3D);
    glFunctions->glDeleteShader(this->cShader3D);

    glFunctions->glDetachShader(this->computeID_tex2D, this->cShader2D);
    glFunctions->glDeleteShader(this->cShader2D);

    std::string path = "GLSL/shaders/";
    std::string vShaderPath = path + "volume.vert";
    std::string fShaderPath = path + "volume.frag";
    std::string cShader3DPath = path + "tex3D.glsl";
    std::string cShader2DPath = path + "tex2D.glsl";

    glFunctions = glContext->extraFunctions();
    glEnable( GL_DEBUG_OUTPUT );
    glFunctions->glDebugMessageCallback(&MessageCallback, 0 );

    // Create programs and link shaders
    this->computeID = glFunctions->glCreateProgram();
    this->programID = glFunctions->glCreateProgram();

    std::string content = readShaderSource(cShader3DPath);
    if (!content.empty()) {
        this->cShader3D = glFunctions->glCreateShader(GL_COMPUTE_SHADER);
        const char* src = content.c_str();
        glFunctions->glShaderSource(this->cShader3D, 1, &src, NULL);
        glFunctions->glCompileShader(this->cShader3D);
        glFunctions->glAttachShader(this->computeID, this->cShader3D);
        printShaderErrors(glFunctions,this->cShader3D);
    }

    content = readShaderSource(cShader2DPath);
        if (!content.empty()) {
            this->cShader2D = glFunctions->glCreateShader(GL_COMPUTE_SHADER);
            const char* src = content.c_str();
            glFunctions->glShaderSource(this->cShader2D, 1, &src, NULL);
            glFunctions->glCompileShader(this->cShader2D);
            glFunctions->glAttachShader(this->computeID_tex2D, this->cShader2D);
            printShaderErrors(glFunctions,this->cShader2D);
        }

    content = readShaderSource(vShaderPath);
    if (!content.empty()) {
        this->vShader = glFunctions->glCreateShader(GL_VERTEX_SHADER);
        const char* src = content.c_str();
        glFunctions->glShaderSource(this->vShader, 1, &src, NULL);
        glFunctions->glCompileShader(this->vShader);
        glFunctions->glAttachShader(this->programID, this->vShader);
        printShaderErrors(glFunctions,this->vShader);
    }
    content = readShaderSource(fShaderPath);
    if (!content.empty()) {

        this->fShader = glFunctions->glCreateShader(GL_FRAGMENT_SHADER);
        const char* src = content.c_str();
        glFunctions->glShaderSource(this->fShader, 1, &src, NULL);
        glFunctions->glCompileShader(this->fShader);
        glFunctions->glAttachShader(this->programID, this->fShader);
        printShaderErrors(glFunctions,this->fShader);
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
    std::string cShader3DPath = path + "tex3D.glsl";
    std::string cShader2DPath = path + "tex2D.glsl";

    glFunctions = glContext->extraFunctions();
    glEnable( GL_DEBUG_OUTPUT );
    glFunctions->glDebugMessageCallback(&MessageCallback, 0 );


    // Create programs and link shaders
    this->computeID = glFunctions->glCreateProgram();
    this->programID = glFunctions->glCreateProgram();

    std::string content = readShaderSource(cShader3DPath);
    if (!content.empty()) {
        this->cShader3D = glFunctions->glCreateShader(GL_COMPUTE_SHADER);
        const char* src = content.c_str();
        glFunctions->glShaderSource(this->cShader3D, 1, &src, NULL);
        glFunctions->glCompileShader(this->cShader3D);
        glFunctions->glAttachShader(this->computeID, this->cShader3D);
        printShaderErrors(glFunctions,this->cShader3D);
    }

    content = readShaderSource(cShader2DPath);
        if (!content.empty()) {
            this->cShader2D = glFunctions->glCreateShader(GL_COMPUTE_SHADER);
            const char* src = content.c_str();
            glFunctions->glShaderSource(this->cShader2D, 1, &src, NULL);
            glFunctions->glCompileShader(this->cShader2D);
            glFunctions->glAttachShader(this->computeID_tex2D, this->cShader2D);
            printShaderErrors(glFunctions,this->cShader2D);
        }

    content = readShaderSource(vShaderPath);
    if (!content.empty()) {
        this->vShader = glFunctions->glCreateShader(GL_VERTEX_SHADER);
        const char* src = content.c_str();
        glFunctions->glShaderSource(this->vShader, 1, &src, NULL);
        glFunctions->glCompileShader(this->vShader);
        glFunctions->glAttachShader(this->programID, this->vShader);
        printShaderErrors(glFunctions,this->vShader);
    }
    content = readShaderSource(fShaderPath);
    if (!content.empty()) {

        this->fShader = glFunctions->glCreateShader(GL_FRAGMENT_SHADER);
        const char* src = content.c_str();
        glFunctions->glShaderSource(this->fShader, 1, &src, NULL);
        glFunctions->glCompileShader(this->fShader);
        glFunctions->glAttachShader(this->programID, this->fShader);
        printShaderErrors(glFunctions,this->fShader);
    }

    glFunctions->glLinkProgram(this->programID);
    printProgramErrors(glFunctions,programID);

    glFunctions->glLinkProgram(this->computeID);
    printProgramErrors(glFunctions,computeID);

    glFunctions->glLinkProgram(this->computeID_tex2D);
    printProgramErrors(glFunctions,computeID_tex2D);

    checkOpenGLError();
}


void Texture::initTexture(){


    if(textureId != 0 && textureId_2 !=0){
        glDeleteTextures(1, &textureId);
        glDeleteTextures(1, &textureId_2);
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

//    glGenTextures(1, &textureId_2);
//    glFunctions->glBindTexture(GL_TEXTURE_2D, textureId_2);
//    glFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glFunctions->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, resolutionBruitCurl[0], resolutionBruitCurl[1], 0, GL_RGB,
//                 GL_FLOAT, nullptr);

    textureCreated = true;

}

void Texture::updateTextureData(){
    glFunctions->glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, resolutionBruit[0], resolutionBruit[1], resolutionBruit[2], 0, GL_RGBA, GL_FLOAT, nullptr);
}


void Texture::deleteTexture(){
    glDeleteTextures( 1, &textureId);
}

void Texture::computePass() {
    glFunctions->glUseProgram(0);
    glFunctions->glUseProgram(computeID);

    glFunctions->glUniform1f(glFunctions->glGetUniformLocation(computeID, "u_time"), timer.elapsed()/vitesse);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(computeID, "resolution"),1, &resolutionBruit[0]);
    glFunctions->glUniform4fv(glFunctions->glGetUniformLocation(computeID, "frequenceWorley"),1, &freqBruit[0]);

    glFunctions->glBindTexture(GL_TEXTURE_3D, textureId);
    glFunctions->glBindImageTexture (0, textureId, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    QVector3D reso=QVector3D(ceil(resolutionBruit[0]/8),ceil(resolutionBruit[1]/8),ceil(resolutionBruit[2]/8));
    glFunctions->glDispatchCompute(reso[0],reso[1],reso[2]);
    glFunctions->glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    glFunctions->glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
    glFunctions->glBindImageTexture (0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glFunctions->glBindTexture(GL_TEXTURE_3D, 0);

//    glFunctions->glUseProgram(0);
//    glFunctions->glUseProgram(computeID_tex2D);
//    glFunctions->glUniform1f(glFunctions->glGetUniformLocation(computeID_tex2D, "u_time"), timer.elapsed()/vitesse);
//    glFunctions->glUniform2fv(glFunctions->glGetUniformLocation(computeID_tex2D, "resolution"),1, &resolutionBruitCurl[0]);
//    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(computeID_tex2D, "frequenceCurl"),1, &freqBruitCurl[0]);


}

void Texture::draw( QVector3D & LightPos ,  QVector3D & LightCol  , const qglviewer::Camera * camera ){

    if(!textureCreated)
        return;

    computePass();

    glFunctions->glUseProgram(0);
    glFunctions->glUseProgram(programID);



    float pMatrix[16];
    float mvMatrix[16];
    camera->getProjectionMatrix(pMatrix);
    camera->getModelViewMatrix(mvMatrix);

    glFunctions->glUniformMatrix4fv(glFunctions->glGetUniformLocation(programID, "proj_matrix"),
                                    1, GL_FALSE, pMatrix);
    glFunctions->glUniformMatrix4fv(glFunctions->glGetUniformLocation(programID, "mv_matrix"),
                                    1, GL_FALSE, mvMatrix);


    glFunctions->glUniform1f(glFunctions->glGetUniformLocation(programID, "xMax"), resolutionBruit[0]);
    glFunctions->glUniform1f(glFunctions->glGetUniformLocation(programID, "yMax"), resolutionBruit[1]);
    glFunctions->glUniform1f(glFunctions->glGetUniformLocation(programID, "zMax"), resolutionBruit[2]);

    glFunctions->glUniform1i(glFunctions->glGetUniformLocation(programID, "LightSample"), LightEch);
    glFunctions->glUniform1i(glFunctions->glGetUniformLocation(programID, "NuageSample"), NuageEch);

   glFunctions->glUniform1f(glFunctions->glGetUniformLocation(programID, "absorptionNuage"), absorptionNuage);
   glFunctions->glUniform4fv(glFunctions->glGetUniformLocation(programID, "facteurWorley"),1, &facteurBruit[0]);
   glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "couleurNuage"),1, &couleurNuage[0]);

   glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "LightPos"),1, &LightPos[0]);
   glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "LightColor"),1, &LightCol[0]);
   glFunctions->glUniform1f(glFunctions->glGetUniformLocation(programID, "absorptionLight"), absorptionLight);


    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "BBmin"),1,&BBmin[0]);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "BBmax"),1,&BBmax[0]);

    for (int i = 0 ; i < plans.size() ; i++)
    {
        glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, (std::string("plans[") + std::to_string(i) + std::string("].normale")).c_str() ), 1, &plans[i].normale[0]);
        glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, (std::string("plans[") + std::to_string(i) + std::string("].point")).c_str() ), 1, &plans[i].point[0]);
        glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, (std::string("plans[") + std::to_string(i) + std::string("].up_vect")).c_str() ), 1, &plans[i].up_vect[0]);
        glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, (std::string("plans[") + std::to_string(i) + std::string("].right_vect")).c_str() ), 1, &plans[i].right_vect[0]);
    }

    glFunctions->glActiveTexture(GL_TEXTURE0 + textureId);
    glFunctions->glBindTexture(GL_TEXTURE_3D, textureId);
    glFunctions->glUniform1i(glFunctions->glGetUniformLocation(programID, "tex"), textureId);
    //drawPlaneInFrontOfCamera(camera,0.1);
    drawCube();

}

void Texture::drawPlaneInFrontOfCamera(const qglviewer::Camera *camera, float distance) {
    // Désactiver le culling et le Z-buffer pour éviter les problèmes
    glDisable(GL_CULL_FACE);

    // Récupérer la position de la caméra
    qglviewer::Vec cameraPos = camera->position();

    // Direction de la caméra (avant)
    qglviewer::Vec forward = camera->viewDirection();
    forward.normalize();

    // Calculer le centre du plan à une distance donnée
    qglviewer::Vec planeCenter = cameraPos + forward * distance;

    // Obtenir les axes "droite" et "haut" de la caméra
    qglviewer::Vec right = camera->rightVector();
    qglviewer::Vec up = camera->upVector();

    // Taille du plan
    float planeWidth = BBmax[0]-BBmin[0];  // Largeur augmentée
    float planeHeight = BBmax[1]-BBmin[1]; // Hauteur augmentée

    // Calcul des coins
    qglviewer::Vec bottomLeft  = planeCenter - right * (planeWidth / 2.0f) - up * (planeHeight / 2.0f);
    qglviewer::Vec bottomRight = planeCenter + right * (planeWidth / 2.0f) - up * (planeHeight / 2.0f);
    qglviewer::Vec topRight    = planeCenter + right * (planeWidth / 2.0f) + up * (planeHeight / 2.0f);
    qglviewer::Vec topLeft     = planeCenter - right * (planeWidth / 2.0f) + up * (planeHeight / 2.0f);

    // Dessiner le plan
    glBegin(GL_QUADS);
    glVertex3f(bottomLeft.x, bottomLeft.y, bottomLeft.z);
    glVertex3f(bottomRight.x, bottomRight.y, bottomRight.z);
    glVertex3f(topRight.x, topRight.y, topRight.z);
    glVertex3f(topLeft.x, topLeft.y, topLeft.z);
    glEnd();

    // Réactiver le culling et le Z-buffer après le test
    glEnable(GL_CULL_FACE);
}


void Texture::drawCube(){

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
void Texture::setAbsorptionNuageDisplay(float _a){
    absorptionNuage=_a;
}
void Texture::setResolutionBruitX(float _x){
    resolutionBruit[0]=_x;
//    std::cout << resolutionBruit[0] <<std::endl;
    updateTextureData();
}
void Texture::setResolutionBruitY(float _y){
    resolutionBruit[1]=_y;
//    std::cout << resolutionBruit[1] <<std::endl;
    updateTextureData();
}
void Texture::setResolutionBruitZ(float _z){
    resolutionBruit[2]=_z;
//    std::cout << resolutionBruit[2] <<std::endl;
    updateTextureData();
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
void Texture::setAbsorptionLightDisplay(float _a){
    absorptionLight=_a;
}
void Texture::setResolutionBruitCurlX(float _x){
    resolutionBruitCurl[0]=_x;
//    std::cout << resolutionBruit[0] <<std::endl;
    updateTextureData();
}
void Texture::setResolutionBruitCurlY(float _y){
    resolutionBruitCurl[1]=_y;
//    std::cout << resolutionBruit[1] <<std::endl;
    updateTextureData();
}
void Texture::setFreqBruitCurlR(float _r){
    freqBruitCurl[0]=_r;
}
void Texture::setFreqBruitCurlG(float _g){
    freqBruitCurl[1]=_g;
}
void Texture::setFreqBruitCurlB(float _b){
    freqBruitCurl[2]=_b;
}
void Texture::setFacteurBruitR(float _r){
    facteurBruit[0]=_r;
}
void Texture::setFacteurBruitG(float _g){
    facteurBruit[1]=_g;
}
void Texture::setFacteurBruitB(float _b){
    facteurBruit[2]=_b;
}
void Texture::setFacteurBruitA(float _a){
    facteurBruit[3]=_a;
}
void Texture::setVitesse(float _v){
    vitesse=_v;
}

void Texture::clear(){

    if( textureCreated )
        glDeleteTextures(1, &textureId);
        glDeleteTextures(1, &textureId_2);

    init();
}

