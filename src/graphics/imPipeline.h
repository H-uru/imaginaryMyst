/* This file is part of imaginaryMyst.
 *
 * imaginaryMyst is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * imaginaryMyst is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with imaginaryMyst.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef IMAGINARYMYST_IMPIPELINE_H
#define IMAGINARYMYST_IMPIPELINE_H


#include <GL/gl.h>
#include <vector>
#include <glm/glm.hpp>

#include "surface/imMipmap.h"

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
