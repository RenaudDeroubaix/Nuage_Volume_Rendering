#include "../header/mesh.h"

struct Vertex {
    QVector3D position;  // Vertex position (x, y, z)
    QVector3D normal;    // Vertex normal (nx, ny, nz)
    QVector2D texCoord;  // Texture coordinate (u, v)
};

Mesh::Mesh(QOpenGLContext* context)
{
    glContext = context;
}


void Mesh::initializeGL() {
    std::vector<Vertex> vertexData;
    for (const auto& face : faces) {
        for (int i = 0; i < face.vertexIndices.size(); ++i) {
            Vertex vertex;
            vertex.position = vertices[face.vertexIndices[i]];
            vertex.normal = normals[face.normalIndices[i]];
            vertex.texCoord = textureCoords[face.textureIndices[i]];
            vertexData.push_back(vertex);
        }
    }

    // Generate VAO
    glFunctions->glGenVertexArrays(1, &vao);
    glFunctions->glBindVertexArray(vao);

    // Generate VBO and fill with vertex data
    glFunctions->glGenBuffers(1, &vbo);
    glFunctions->glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glFunctions->glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex), vertexData.data(), GL_STATIC_DRAW);

    // Define vertex attribute pointers
    glFunctions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glFunctions->glEnableVertexAttribArray(0);

    glFunctions->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glFunctions->glEnableVertexAttribArray(1);

    glFunctions->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glFunctions->glEnableVertexAttribArray(2);

    // Unbind VAO
    glFunctions->glBindVertexArray(0);
}


void Mesh::draw(QVector3D &LightPos, QVector3D &LightCol, const qglviewer::Camera* camera) {
    // Activate shader program
    glFunctions->glUseProgram(programID);

    // Set camera matrices
    float pMatrix[16];
    float mvMatrix[16];
    camera->getProjectionMatrix(pMatrix);
    camera->getModelViewMatrix(mvMatrix);


    glFunctions->glUniformMatrix4fv(glFunctions->glGetUniformLocation(programID, "proj_matrix"), 1, GL_FALSE, pMatrix);
    glFunctions->glUniformMatrix4fv(glFunctions->glGetUniformLocation(programID, "mv_matrix"), 1, GL_FALSE, mvMatrix);

    // Set lighting uniforms
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "LightPos"), 1, &LightPos[0]);
    glFunctions->glUniform3fv(glFunctions->glGetUniformLocation(programID, "LightColor"), 1, &LightCol[0]);

    // Bind texture if applicable
    if (textureId) {
            // Activate texture unit 0
            glFunctions->glActiveTexture(GL_TEXTURE0);
            // Bind the texture
            glFunctions->glBindTexture(GL_TEXTURE_2D, textureId);
            // Inform the shader that "textureSampler" uses texture unit 0
            glFunctions->glUniform1i(glFunctions->glGetUniformLocation(programID, "textureSampler"), 0);
        }
    // Bind VAO and draw
    glFunctions->glBindVertexArray(vao);
    glFunctions->glDrawArrays(GL_TRIANGLES, 0, faces.size() * 3);
    glFunctions->glBindVertexArray(0);

    // Unbind texture
    if (textureId) {
        glFunctions->glBindTexture(GL_TEXTURE_2D, 0);
    }

}




void Mesh::initGLSL(){
    std::string path = "GLSL/shaders/";
    std::string vShaderPath = path + "plan.vert";
    std::string fShaderPath = path + "plan.frag";

    glFunctions = glContext->extraFunctions();
    glEnable( GL_DEBUG_OUTPUT );
    glFunctions->glDebugMessageCallback(&MessageCallback, 0 );


    // Create programs and link shaders
    this->programID = glFunctions->glCreateProgram();

    std::string content = readShaderSource(vShaderPath);

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
    std::cout<< "mesh Program : " <<  programID << std::endl;
    glFunctions->glLinkProgram(this->programID);
    std::cout << "ERROR SHADER Plan programID" << std::endl;
    printProgramErrors(glFunctions,programID);
    std::cout << "Fin error init GLSL Plan" << std::endl;


    checkOpenGLError();
}


void Mesh::initTexture(const QString &fileName){


    if(textureId != 0 ){
        glDeleteTextures(1, &textureId);
    }
    QImage textureImage(fileName);
    if (textureImage.isNull()) {
        qWarning("Failed to load texture");
        return;
    }

    textureImage = textureImage.convertToFormat(QImage::Format_RGBA8888);

    glFunctions->glGenTextures(1, &textureId);
    glFunctions->glBindTexture(GL_TEXTURE_2D, textureId);

    glFunctions->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage.width(), textureImage.height(),
                              0, GL_RGBA, GL_UNSIGNED_BYTE, textureImage.bits());
    glFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFunctions->glBindTexture(GL_TEXTURE_2D, 0);


}

void Mesh::deleteTexture(){
    glDeleteTextures( 1, &textureId);
}
