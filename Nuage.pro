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
# Check if libQGLViewer-qt5.so exists in the system path
QGLVIEWER_FOUND = $$system(test -f /usr/lib/x86_64-linux-gnu/libQGLViewer-qt5.so && echo "yes" || echo "no")

# Conditional check based on whether the library is found
contains(QGLVIEWER_FOUND, "yes") {
    # If QGLViewer-qt5 is found, link with it
    LIBS += -lQGLViewer-qt5
} else {
    # If QGLViewer-qt5 is not found, use the custom QGLViewer library
    INCLUDEPATH += $${EXT_DIR}/libQGLViewer-2.6.1
    LIBS += -L$${EXT_DIR}/libQGLViewer-2.6.1/QGLViewer -lQGLViewer
}

LIBS += -lglut \
    -lGLU
LIBS += -lgsl \
    -lgomp
LIBS += -lblas \
    -lgomp
release:QMAKE_CXXFLAGS_RELEASE += -O3 \
    -fopenmp
release:QMAKE_CFLAGS_RELEASE += -O3 \
    -fopenmp
