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
    src/TextureDockWidget.cpp
HEADERS += header/Window.h \
    header/TextureViewer.h \
    header/Texture.h \
    header/TextureDockWidget.h \
    header/Vec3D.h

RESOURCES += \
    GLSL/shaders/volume.frag \
    GLSL/shaders/volume.vert \
    GLSL/shaders/volume.comp \

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
