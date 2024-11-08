# -------------------------------------------------
# Project created by QtCreator 2010-01-27T15:21:45
# -------------------------------------------------
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
LIBS = -lQGLViewer-qt5 \
    -lglut \
    -lGLU
