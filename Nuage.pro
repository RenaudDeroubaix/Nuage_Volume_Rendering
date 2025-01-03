QT += xml
QT += opengl
TARGET = nuage
TEMPLATE = app
MOC_DIR = ./moc
OBJECTS_DIR = ./obj
DEPENDPATH += ./GLSL
INCLUDEPATH += ./GLSL
SOURCES += src/Main.cpp \
    src/Window.cpp \
    src/TextureViewer.cpp \
    src/Texture.cpp \
    src/TextureDockWidget.cpp \
    src/light.cpp \
    src/mesh.cpp \
    src/skybox.cpp
HEADERS += header/Window.h \
    header/GLFunc.h \
    header/PowerOfTwoSpinBox.h \
    header/TextureViewer.h \
    header/Texture.h \
    header/TextureDockWidget.h \
    header/Vec3D.h \
    header/light.h \
    header/mesh.h \
    header/WidgetSetup.h \
    header/skybox.h

RESOURCES += \
    GLSL/shaders/tex3D.glsl \
    GLSL/shaders/tex2D.glsl \
    GLSL/shaders/volume.frag \
    GLSL/shaders/volume.vert \
    GLSL/shaders/light.frag \
    GLSL/shaders/light.vert \
    GLSL/shaders/plan.frag \
    GLSL/shaders/plan.vert \
    GLSL/shaders/skybox.frag \
    GLSL/shaders/skybox.vert \
    Ressources/mountain/Mountain.obj \
    Ressources/mountain/textures/aerial_grass_rock_diff_4k.jpg\

INCLUDEPATH = ./GLSL
EXT_DIR = external

# {
#  INCLUDEPATH += $${EXT_DIR}/libQGLViewer-2.6.1
#  LIBS += -L$${EXT_DIR}/libQGLViewer-2.6.1/QGLViewer -lQGLViewer
# }

message("Checking for QGLViewer availability...")

# Test with a shell command
QGLVIEWER_AVAILABLE = $$system("ld -lQGLViewer-qt5 -o /dev/null 2>/dev/null && echo yes || echo no")
equals(QGLVIEWER_AVAILABLE, yes) {
    message("Found QGLViewer-qt5. Linking with it.")
    LIBS += -lQGLViewer-qt5
} else {
    message("System QGLViewer-qt5 not found. Falling back to local library.")
    INCLUDEPATH += $${EXT_DIR}/libQGLViewer-2.6.1
    LIBS += -L$${EXT_DIR}/libQGLViewer-2.6.1/QGLViewer -lQGLViewer
}


LIBS += -lglut \
    -lGLU
LIBS += -lgsl \
    -lgomp
#LIBS += -lblas \
#    -lgomp
release:QMAKE_CXXFLAGS_RELEASE += -O3 \
    -fopenmp
release:QMAKE_CFLAGS_RELEASE += -O3 \
    -fopenmp
