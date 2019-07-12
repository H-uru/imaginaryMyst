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

#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>
#include <surface/imMipmap.h>
#include "imPipeline.h"

void imPipeline::initialize()
{
    m_vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_vertexShaderId, 1, &m_vertexShader, NULL);
    glCompileShader(m_vertexShaderId);

    m_fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_fragmentShaderId, 1, &m_fragmentShader, NULL);
    glCompileShader(m_fragmentShaderId);
    m_programId = glCreateProgram();
    glAttachShader(m_programId, m_fragmentShaderId);
    glAttachShader(m_programId, m_vertexShaderId);
    glLinkProgram(m_programId);
    glUseProgram(m_programId);

    glGenVertexArrays(1, &m_vertexArrayId);
    glBindVertexArray(m_vertexArrayId);

    glGenBuffers(1, &m_vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * 3 * sizeof(GLfloat), m_vertices.data(), GL_STATIC_DRAW);
    m_positionLoc = glGetAttribLocation(m_programId, "in_position");
    glVertexAttribPointer(m_positionLoc, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &m_textureBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, m_textureBufferId);
    glBufferData(GL_ARRAY_BUFFER, m_textureCoords.size() * 2 * sizeof(GLfloat), m_textureCoords.data(), GL_STATIC_DRAW);
    m_textureCoordLoc = glGetAttribLocation(m_programId, "in_texCoords");
    glVertexAttribPointer(m_textureCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void imPipeline::render()
{
    glLinkProgram(m_programId);
    glUseProgram(m_programId);
    glEnableVertexAttribArray(m_positionLoc);
    glBindVertexArray(m_vertexArrayId);

    glActiveTexture(GL_TEXTURE0);
    glBindBuffer(GL_ARRAY_BUFFER, m_textureBufferId);
    glEnableVertexAttribArray(m_textureCoordLoc);
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
    glBindVertexArray(0);
}

imPipeline::~imPipeline()
{
    glUseProgram(0);
    glDetachShader(m_programId, m_vertexShaderId);
    glDetachShader(m_programId, m_fragmentShaderId);

    glDeleteShader(m_fragmentShaderId);
    glDeleteShader(m_vertexShaderId);

    glDeleteProgram(m_programId);
}
