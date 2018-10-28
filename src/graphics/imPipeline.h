#ifndef IMAGINARYMYST_IMPIPELINE_H
#define IMAGINARYMYST_IMPIPELINE_H


#include <imString.h>
#include <GL/gl.h>
#include <vector>
#include <glm/glm.hpp>

#include "../surface/imMipmap.h"

class imPipeline {

private:
    const char* m_vertexShader = "#version 330 core\n"
                                    "in vec3 in_position;\n"
                                    "in vec2 in_texCoords;\n"
                                    "out vec2 UVcoords;\n"
                                    "void main()\n"
                                    "{\n"
                                    "gl_Position =  vec4(in_position, 1);\n"
                                    "UVcoords = in_texCoords;\n"
                                    "}";
    const char* m_fragmentShader = "#version 330 core\n"
                                      "out vec4 color;\n"
                                      "in vec2 UVcoords;\n"
                                      "uniform sampler2D texSampler;\n"
                                      "void main()\n"
                                      "{\n"
                                      "color = texture(texSampler, UVcoords).rgba;\n"
                                      "}";
    GLuint m_programId;
    GLuint m_positionLoc;
    GLuint m_vertexShaderId;
    GLuint m_fragmentShaderId;

    GLuint m_vertexArrayId;
    GLuint m_vertexBufferId;
    GLuint m_textureBufferId;
    GLuint m_textureCoordLoc;

    const std::vector<glm::vec3> m_vertices = {glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(1.0f, -1.0f, 0.0f),
                                               glm::vec3(1.0f, 1.0f, 0.0f),glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f) };
    const std::vector<glm::vec2> m_textureCoords = {glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f)};

public:
    void initialize();
    void render();
    ~imPipeline();
};


#endif //IMAGINARYMYST_IMPIPELINE_H
