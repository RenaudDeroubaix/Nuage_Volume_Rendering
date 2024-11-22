#include <QApplication>
#include <QOpenGLContext>

#include <QDebug>
#include "../header/Window.h"

int main(int argc, char *argv[])
{
    qDebug() << "OpenGL version:" << QSurfaceFormat::defaultFormat().majorVersion()
             << "." << QSurfaceFormat::defaultFormat().minorVersion();


    QOpenGLContext* context = QOpenGLContext::currentContext();
    if (context) {
        qDebug() << "OpenGL version:" << context->format().majorVersion()
                 << "." << context->format().minorVersion();
    } else {
        qWarning() << "No current OpenGL context!";
    }

    QApplication a(argc, argv);


    Window w;
    w.show();
    return a.exec();
}
