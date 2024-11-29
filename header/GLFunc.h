#ifndef GLFUNC_H
#define GLFUNC_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <QFile>
#include <QTextStream>

#include <QDebug>
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>

static void printspec(QOpenGLContext* glContext)
{
    // Récupérer les informations sous forme de chaînes
    const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

    // Afficher les informations correctement
    qDebug() << "OpenGL Version:" << version;
    qDebug() << "Renderer:" << renderer;

    int workGroupCount[3], workGroupSize[3], sharedMemorySize;
    QOpenGLExtraFunctions* glFunctions = glContext->extraFunctions();
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
}

static void MessageCallback( GLenum source, GLenum type,
                                              GLuint id, GLenum severity,
                                              GLsizei length, const GLchar* message,
                                              const void* userParam )
{
    if (severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM || severity == GL_DEBUG_SEVERITY_LOW) {
        std::string s_severity = (severity == GL_DEBUG_SEVERITY_HIGH ? "High" : severity == GL_DEBUG_SEVERITY_MEDIUM ? "Medium" : "Low");
        std::cout << "Error " << id << " [severity=" << s_severity << "]: " << message << std::endl;
    }
}
static bool checkOpenGLError()
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

static bool printShaderErrors(  QOpenGLExtraFunctions* glFunctions , GLuint & shader)
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
static bool printProgramErrors(QOpenGLExtraFunctions* glFunctions , GLuint & program)
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
static std::string readShaderSource(std::string & filename)
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


#endif // GLFUNC_H
