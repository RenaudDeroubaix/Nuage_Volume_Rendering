#include "../header/TextureViewer.h"
#include <cfloat>
#include <QFileDialog>
#include <QGLViewer/manipulatedCameraFrame.h>

#include "../header/Vec3D.h"

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
    plan = initPlan();
    openOBJMesh(":/Ressources/mountain/Mountain.obj", plan);

    plan->initializeGL();
    plan->initTexture(":/Ressources/mountain/textures/aerial_grass_rock_diff_4k.jpg");

    // Initialisation de la scène
    setManipulatedFrame(new ManipulatedFrame());

    // Désactiver l'éclairage fixe OpenGL (non utilisé ici)
    glDisable(GL_LIGHTING);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Mode de remplissage des polygones (face avant uniquement)
    glPolygonMode(GL_FRONT, GL_FILL);

    // Configurer l'ordre des faces pour le culling
    glFrontFace(GL_CCW);

    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    camera()->setZNearCoefficient(0.00001);
    camera()->setZClippingCoefficient(1000.0);

    camera()->setPosition(qglviewer::Vec(-2 , 1 , -3));
    camera()->lookAt(qglviewer::Vec(-1 , 1 , 0));


}

void TextureViewer::draw() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_BACK);
    glDepthMask(GL_TRUE);
    if(isOnlyCloud)
    {
        skybox->draw(light->getpos(), light->getcol(), light->getdir() , camera());
        texture->draw(light->getpos(), light->getcol(), camera());
        glCullFace(GL_FRONT);
        light->draw(camera() , isLightUtime);
    }
    else
    {
        updateCamera();
        skybox->draw(light->getpos(), light->getcol(), light->getdir() , camera());
        if(plan != nullptr)
            plan->draw(light->getpos(), light->getcol(),camera());

        texture->draw(light->getpos(), light->getcol(), camera());
        glCullFace(GL_FRONT);
        light->draw(camera() , isLightUtime);

    }
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

Mesh* TextureViewer::initPlan(){
    Mesh* plan= new Mesh(QOpenGLContext::currentContext());
    plan->initGLSL();
    return plan;
}

void TextureViewer::updateCamera(){
    camera()->setSceneCenter(camera()->position());

    Vec3D<float> right = Vec3D<float>::crossProduct(Vec3D<float>(camera()->viewDirection()[0] , camera()->viewDirection()[1] , camera()->viewDirection()[2]),
                                                    Vec3D<float>(0. , 1. ,0.));
    right.normalize();

    Vec3D<float> newup = Vec3D<float>::crossProduct(right ,Vec3D<float>(camera()->viewDirection()[0] , camera()->viewDirection()[1] , camera()->viewDirection()[2]));

    camera()->setUpVector(qglviewer::Vec(newup[0] , newup[1] , newup[2]) , true);

}

void TextureViewer::openOBJMesh(const QString &fileName, Mesh* m) {
    std::cout << "Opening " << fileName.toStdString() << std::endl;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file:" << fileName;
        return;
    }

    QTextStream in(&file);
    QVector3D minVertex(FLT_MAX, FLT_MAX, FLT_MAX);  // Initialiser à des valeurs très grandes
    QVector3D maxVertex(-FLT_MAX, -FLT_MAX, -FLT_MAX); // Initialiser à des valeurs très petites

    // Chargement des données
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        // Skip comments and empty lines
        if (line.startsWith("#") || line.isEmpty())
            continue;

        // Split line into parts
        QStringList parts = line.split(" ", Qt::SkipEmptyParts);

        if (parts[0] == "v") {
            // Vertex position
            if (parts.size() < 4) continue;
            QVector3D vertex(parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat());
            m->vertices.append(vertex);

            // Mettre à jour les coordonnées min et max
            minVertex = QVector3D(qMin(minVertex.x(), vertex.x()), qMin(minVertex.y(), vertex.y()), qMin(minVertex.z(), vertex.z()));
            maxVertex = QVector3D(qMax(maxVertex.x(), vertex.x()), qMax(maxVertex.y(), vertex.y()), qMax(maxVertex.z(), vertex.z()));
        } else if (parts[0] == "vt") {
            // Texture coordinates
            if (parts.size() < 3) continue;
            m->textureCoords.append(QVector2D(parts[1].toFloat(), parts[2].toFloat()));
        } else if (parts[0] == "vn") {
            // Vertex normal
            if (parts.size() < 4) continue;
            m->normals.append(QVector3D(parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat()));
        } else if (parts[0] == "f") {
            // Face
            Face face;
            for (int i = 1; i < parts.size(); ++i) {
                QStringList indices = parts[i].split("/", Qt::KeepEmptyParts);
                if (indices.size() > 0 && !indices[0].isEmpty())
                    face.vertexIndices.append(indices[0].toInt() - 1);
                if (indices.size() > 1 && !indices[1].isEmpty())
                    face.textureIndices.append(indices[1].toInt() - 1);
                if (indices.size() > 2 && !indices[2].isEmpty())
                    face.normalIndices.append(indices[2].toInt() - 1);
            }
            m->faces.append(face);
        }
    }

    file.close();

    // Normalisation des vertices
    QVector3D range = maxVertex - minVertex;
    QVector3D center = minVertex + range / 2.0f;

//    for (int i = 0; i < m->vertices.size(); ++i) {
//        QVector3D& vertex = m->vertices[i];

//        // Déplacer le vertex vers l'origine et le normaliser dans l'intervalle [0, 1]
//        vertex -= center;  // Centrer le mesh
//        vertex /= range;   // Redimensionner le mesh dans l'intervalle [0, 1]
//    }

    std::cout << "OBJ LOAD, nbVertices: " << m->vertices.size() << std::endl;
    std::cout << "          nbUVcoords: " << m->textureCoords.size() << std::endl;
    std::cout << "          nbNormales: " << m->normals.size() << std::endl;
    std::cout << "          nbFaces: " << m->faces.size() << std::endl;

    return;
}


//void TextureViewer::openOffMesh(const QString &fileName) {
//    std::cout << "Opening " << fileName.toStdString() << std::endl;

//    // open the file
//    std::ifstream myfile;
//    myfile.open(fileName.toStdString());
//    if (!myfile.is_open())
//    {
//        std::cout << fileName.toStdString() << " cannot be opened" << std::endl;
//        return;
//    }

//    std::string magic_s;

//    myfile >> magic_s;

//    // check if it's OFF
//    if( magic_s != "OFF" )
//    {
//        std::cout << magic_s << " != OFF :   We handle ONLY *.off files." << std::endl;
//        myfile.close();
//        exit(1);
//    }

//    int n_vertices , n_faces , dummy_int;
//    myfile >> n_vertices >> n_faces >> dummy_int;

//    // Clear any verticies
//    vertices.clear();

//    // Read the verticies
//    for( int v = 0 ; v < n_vertices ; ++v )
//    {
//        float x , y , z;
//        myfile >> x >> y >> z ;
//        vertices.push_back( Vec( x , y , z ) );
//    }

//    // Clear any triangles
//    triangles.clear();

//    // Read the triangles
//    for( int f = 0 ; f < n_faces ; ++f )
//    {
//        int n_vertices_on_face;
//        myfile >> n_vertices_on_face;
//        if( n_vertices_on_face == 3 )
//        {
//            unsigned int _v1 , _v2 , _v3;
//            myfile >> _v1 >> _v2 >> _v3;
//            triangles.push_back( {_v1, _v2, _v3} );
//        }
//        else if( n_vertices_on_face == 4 )
//        {
//            unsigned int _v1 , _v2 , _v3 , _v4;

//            myfile >> _v1 >> _v2 >> _v3 >> _v4;
//            triangles.push_back({_v1, _v2, _v3});
//            triangles.push_back({_v1, _v3, _v4});
//        }
//        else
//        {
//            std::cout << "We handle ONLY *.off files with 3 or 4 vertices per face" << std::endl;
//            myfile.close();
//            exit(1);
//        }
//    }

//}


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
void TextureViewer::setxBBmin( float _x){
    texture->setxBBmin(_x);
    update();
}
void TextureViewer::setyBBmin( float _y){
    texture->setyBBmin(_y);
    update();
}
void TextureViewer::setzBBmin( float _z){
    texture->setzBBmin(_z);
    update();
}
void TextureViewer::setxBBmax( float _x){
    texture->setxBBmax(_x);
    update();
}
void TextureViewer::setyBBmax( float _y){
    texture->setyBBmax(_y);
    update();
}
void TextureViewer::setzBBmax( float _z){
    texture->setzBBmax(_z);
    update();
}
void TextureViewer::setcamerapos(QVector3D pos , QVector3D target){
    camera()->setPosition(qglviewer::Vec(pos.x() , pos.y() , pos.z()));
    camera()->lookAt(qglviewer::Vec(target.x() , target.y() , target.z()));
    update();
}
void TextureViewer::setIsLightUTime(bool b){
    isLightUtime = b;
    update();
}
void  TextureViewer::setboolOnlyCloud(bool b){
    isOnlyCloud = b;
    update();
}
void TextureViewer::setFormeNuage(int id){
    switch (id) {
    case 0:
        qDebug() << "Forme sélectionnée : Cube";
        break;
    case 1:
        qDebug() << "Forme sélectionnée : Sphère";
        break;
    case 2:
        qDebug() << "Forme sélectionnée : Torus";
        break;
    case 3:
        qDebug() << "Forme sélectionnée : Croix";
        break;
    case 4:
        qDebug() << "Forme sélectionnée : Capsule";
        break;
    default:
        qDebug() << "Forme inconnue";
        break;
    }
    texture->setForme(id);
    update();
}
void TextureViewer::setAttenuationBord(float a){
    texture->setAttenuationBord(a);
    update();
}
void TextureViewer::setRayonSphere(float r){
    texture->setRayonSphere(r);
    update();
}
void TextureViewer::setRayonInternTorus(float r){
    texture->setRayonInternTorus(r);
    update();
}
void TextureViewer::setRayonExternTorus(float r){
    texture->setRayonExternTorus(r);
    update();
}
void TextureViewer::setLongueurCapsule(float l){
    texture->setLongueurCapsule(l);
    update();
}
void TextureViewer::setRayonCapsule(float r){
    texture->setRayonCapsule(r);
    update();
}
void TextureViewer::setTailleBarre(float t){
    texture->setTailleBarre(t);
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
