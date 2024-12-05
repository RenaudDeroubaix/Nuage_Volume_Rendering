#include "../header/TextureViewer.h"
#include <cfloat>
#include <QFileDialog>
#include <QGLViewer/manipulatedCameraFrame.h>

using namespace std;
using namespace qglviewer;

TextureViewer::TextureViewer(QWidget *parent):QGLViewer(parent){
    //init();
}

void TextureViewer::init() {
    // Initialisation des objets
    texture = new Texture(QOpenGLContext::currentContext(),camera());
    light = new Light(QOpenGLContext::currentContext());
    skybox = new SkyBox(QOpenGLContext::currentContext());

    // Initialisation de la scène
    setManipulatedFrame(new ManipulatedFrame());

    // Désactiver l'éclairage fixe OpenGL (non utilisé ici)
    glDisable(GL_LIGHTING);

    // Configurer le test de profondeur pour un rendu 3D correct
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Mode de remplissage des polygones (face avant uniquement)
    glPolygonMode(GL_FRONT, GL_FILL);

    // Configurer l'ordre des faces pour le culling
    glFrontFace(GL_CCW);

    // Couleur de fond (bleu ciel)
    //glClearColor(0.529f, 0.808f, 0.922f, 1.0f); // RGB: (135, 206, 235)

    // Activer le face culling (par défaut, désactivé pour la lumière dans draw())

    glEnable(GL_CULL_FACE); // Activer le culling pour la texture
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    camera()->setZNearCoefficient(0.000001);
    camera()->setZClippingCoefficient(1000.0);


}

void TextureViewer::draw() {
    // Effacer le tampon de couleur et de profondeur
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




    glCullFace(GL_BACK);
    skybox->draw(light->getpos(), light->getcol(), light->getdir() , camera());
    // Configurer la caméra
    //

    glDisable(GL_DEPTH_TEST); // Désactiver le test de profondeur pour la lumière

    glCullFace(GL_FRONT);
    light->draw(camera());



    // 1. Dessiner la texture 3D (opaque)
    glEnable(GL_DEPTH_TEST);
    //glDepthMask(GL_TRUE); // Autoriser l'écriture dans le tampon de profondeur
    glCullFace(GL_FRONT);  // Dessiner les faces avant uniquement
    texture->draw(light->getpos(), light->getcol(), camera());


    // Mettre à jour la scène
    camera()->setSceneCenter(camera()->position());
    update();
}



void TextureViewer::drawMesh() {
    // Exemple de dessin d'un mesh (ajouté si nécessaire)
    glBegin(GL_TRIANGLES);
    for (const auto& triangle : triangles) {
        for (size_t i = 0; i < 3; ++i) {
            const qglviewer::Vec& vertex = vertices[triangle[i]];
            glVertex3f(vertex.x, vertex.y, vertex.z);
        }
    }
    glEnd();
}


void TextureViewer::clear(){
    texture->clear(camera());
}


void TextureViewer::updateCamera(const qglviewer::Vec & center, float radius){
    camera()->setSceneCenter(center);
    camera()->setSceneRadius(radius);
    
    camera()->showEntireScene();
}

void TextureViewer::openOffMesh(const QString &fileName) {
    std::cout << "Opening " << fileName.toStdString() << std::endl;

    // open the file
    std::ifstream myfile;
    myfile.open(fileName.toStdString());
    if (!myfile.is_open())
    {
        std::cout << fileName.toStdString() << " cannot be opened" << std::endl;
        return;
    }

    std::string magic_s;

    myfile >> magic_s;

    // check if it's OFF
    if( magic_s != "OFF" )
    {
        std::cout << magic_s << " != OFF :   We handle ONLY *.off files." << std::endl;
        myfile.close();
        exit(1);
    }

    int n_vertices , n_faces , dummy_int;
    myfile >> n_vertices >> n_faces >> dummy_int;

    // Clear any verticies
    vertices.clear();

    // Read the verticies
    for( int v = 0 ; v < n_vertices ; ++v )
    {
        float x , y , z;
        myfile >> x >> y >> z ;
        vertices.push_back( Vec( x , y , z ) );
    }

    // Clear any triangles
    triangles.clear();

    // Read the triangles
    for( int f = 0 ; f < n_faces ; ++f )
    {
        int n_vertices_on_face;
        myfile >> n_vertices_on_face;
        if( n_vertices_on_face == 3 )
        {
            unsigned int _v1 , _v2 , _v3;
            myfile >> _v1 >> _v2 >> _v3;
            triangles.push_back( {_v1, _v2, _v3} );
        }
        else if( n_vertices_on_face == 4 )
        {
            unsigned int _v1 , _v2 , _v3 , _v4;

            myfile >> _v1 >> _v2 >> _v3 >> _v4;
            triangles.push_back({_v1, _v2, _v3});
            triangles.push_back({_v1, _v3, _v4});
        }
        else
        {
            std::cout << "We handle ONLY *.off files with 3 or 4 vertices per face" << std::endl;
            myfile.close();
            exit(1);
        }
    }

}


std::istream & operator>>(std::istream & stream, qglviewer::Vec & v)
{
    stream >>
            v.x >>
            v.y >>
            v.z;
    
    return stream;
}

void TextureViewer::onNuageSliderChanged(int value){
    texture->setNuageEch(value);
    update();
}
void TextureViewer::onNuageSpinBoxChanged(int value){
    texture->setNuageEch(value);
    update();
}
void TextureViewer::onLightSliderChanged(int value){
    texture->setLightEch(value);
    update();
}
void TextureViewer::onLightSpinBoxChanged(int value){
    texture->setLightEch(value);
    update();
}

void TextureViewer::setRedNuage(float _r){
    texture->setRedNuageDisplay(_r);
    update();
}
void TextureViewer::setGreenNuage(float _g){
    texture->setGreenNuageDisplay(_g);
    update();
}
void TextureViewer::setBlueNuage(float _b){
    texture->setBlueNuageDisplay(_b);
    update();
}
void TextureViewer::setXlightpos(float _x){
    light->setXlightposDisplay(_x);
    update();
}
void TextureViewer::setYlightpos(float _y){
    light->setYlightposDisplay(_y);
    update();
}
void TextureViewer::setZlightpos(float _z){
    light->setZlightposDisplay(_z);
    update();
}
void TextureViewer::setRlightcol(float _r){
    light->setRlightcolDisplay(_r);
    update();
}
void TextureViewer::setGlightcol(float _g){
    light->setGlightcolDisplay(_g);
    update();
}
void TextureViewer::setBlightcol(float _b){
    light->setBlightcolDisplay(_b);
    update();
}
void TextureViewer::setAbsorptionNuage(float _a){
    texture->setAbsorptionNuageDisplay(_a);
    update();
}

void TextureViewer::recompileShaders() {
    texture->recompileShaders();
    update();
}
void TextureViewer::setResolutionBruitX(float _x){
    texture->setResolutionBruitX(pow(2,_x));
    update();
}
void TextureViewer::setResolutionBruitY(float _y){
    texture->setResolutionBruitY(pow(2,_y));
    update();
}
void TextureViewer::setResolutionBruitZ(float _z){
    texture->setResolutionBruitZ(pow(2,_z));
    update();
}
void TextureViewer::setFreqBruitR(float _r){
    texture->setFreqBruitR(_r);
    update();
}
void TextureViewer::setFreqBruitG(float _g){
    texture->setFreqBruitG(_g);
    update();
}
void TextureViewer::setFreqBruitB(float _b){
    texture->setFreqBruitB(_b);
    update();
}
void TextureViewer::setFreqBruitA(float _a){
    texture->setFreqBruitA(_a);
    update();
}
void TextureViewer::setRayonSoleil(float _rayonValue){
    light->setRayon(_rayonValue);
    update();
}
void TextureViewer::setAbsorptionLight(float _a){
    texture->setAbsorptionLightDisplay(_a);
    update();
}
void TextureViewer::setResolutionBruitCurlX(float _x){
    texture->setResolutionBruitCurlX(pow(2,_x));
    update();
}
void TextureViewer::setResolutionBruitCurlY(float _y){
    texture->setResolutionBruitCurlY(pow(2,_y));
    update();
}
void TextureViewer::setFreqBruitCurlR(float _r){
    texture->setFreqBruitCurlR(_r);
    update();
}
void TextureViewer::setFreqBruitCurlG(float _g){
    texture->setFreqBruitCurlG(_g);
    update();
}
void TextureViewer::setFreqBruitCurlB(float _b){
    texture->setFreqBruitCurlB(_b);
    update();
}
void TextureViewer::setFacteurBruitR(float _r){
    texture->setFacteurBruitR(_r);
    update();
}
void TextureViewer::setFacteurBruitG(float _g){
    texture->setFacteurBruitG(_g);
    update();
}
void TextureViewer::setFacteurBruitB(float _b){
    texture->setFacteurBruitB(_b);
    update();
}
void TextureViewer::setFacteurBruitA(float _a){
    texture->setFacteurBruitA(_a);
    update();
}
void TextureViewer::setVitesse(float _v){
    texture->setVitesse(_v);
    update();
}


void TextureViewer::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_R :update(); break;
    default : QGLViewer::keyPressEvent(e);
}
}

QString TextureViewer::helpString() const
{
    QString text("<h2>S i m p l e V i e w e r</h2>");
    text += "Use the mouse to move the camera around the object. ";
    text += "You can respectively revolve around, zoom and translate with the three mouse buttons. ";
    text += "Left and middle buttons pressed together rotate around the camera view direction axis<br><br>";
    text += "Pressing <b>Alt</b> and one of the function keys (<b>F1</b>..<b>F12</b>) defines a camera keyFrame. ";
    text += "Simply press the function key again to restore it. Several keyFrames define a ";
    text += "camera path. Paths are saved when you quit the application and restored at next start.<br><br>";
    text += "Press <b>F</b> to display the frame rate, <b>A</b> for the world axis, ";
    text += "<b>Alt+Return</b> for full screen mode and <b>Control+S</b> to save a snapshot. ";
    text += "See the <b>Keyboard</b> tab in this window for a complete shortcut list.<br><br>";
    text += "Double clicks automates single click actions: A left button double click aligns the closer axis with the camera (if close enough). ";
    text += "A middle button double click fits the zoom of the camera and the right button re-centers the scene.<br><br>";
    text += "A left button double click while holding right button pressed defines the camera <i>Revolve Around Point</i>. ";
    text += "See the <b>Mouse</b> tab and the documentation web pages for details.<br><br>";
    text += "Press <b>Escape</b> to exit the TextureViewer.";
    return text;
}
