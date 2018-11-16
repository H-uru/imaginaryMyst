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

#include "imSceneIndex.h"
#include "imCommon.h"

bool imSceneIndex::read(imStream* stream)
{
    if (stream->read32BE() != 2) {
        imLog("Invalid header version");
        return false;
    }

    size_t len;
    imLog("DEBUG: [SDB] (Header +4) {}", stream->read32());
    stream->read32();   // File size
    len = stream->read32();
    m_name = stream->readString(len);
    len = stream->read32();
    m_page = stream->readString(len);
    imLog("DEBUG: [SDB] Name: '{}', '{}'", m_name, m_page);
    m_sceneRefs.resize(stream->read32());
    for (size_t i=0; i<m_sceneRefs.size(); i++) {
        len = stream->read32();
        m_sceneRefs[i] = stream->readString(len);
        imLog("DEBUG: [SDB] * SceneRef: '{}'", m_sceneRefs[i]);
    }

    if (stream->read32BE() != 0x4edf6994) {
        imLog("Invalid Database Magic");
        return false;
    }
    unsigned int verMaj = stream->read32BE();
    if (verMaj != 2) {
        imLog("Unsupported Database Major Version {}", verMaj);
        return false;
    }
    unsigned int verMin = stream->read32();
    if (verMin != 6) {
        imLog("Unsupported Database Minor Version {}", verMin);
        return false;
    }

    len = stream->read32();
    imLog("DEBUG: [SDB] (+0C) {}", stream->readString(len));
    imLog("DEBUG: [SDB] Key Refs:");
    m_keyRefs.resize(stream->read32());
    for (size_t i=0; i<m_keyRefs.size(); i++) {
        len = stream->read32();
        m_keyRefs[i] = stream->readString(len);
        imLog("DEBUG: [SDB] * '{}'", m_keyRefs[i]);
    }

    m_textures.resize(stream->read32());
    for (size_t i=0; i<m_textures.size(); i++) {
        imLog("DEBUG: [SDB] Texture #{}", i);
        m_textures[i] = std::make_shared<imTexture>();
        if (!m_textures[i]->read(stream))
            return false;
    }

    m_materials.resize(stream->read32());
    for (size_t i=0; i<m_materials.size(); i++) {
        imLog("DEBUG: [SDB] Material #{}", i);
        m_materials[i] = std::make_shared<imMaterial>();
        if (!m_materials[i]->read(stream, this))
            return false;
    }

    if (stream->read32() != 0) {
        imLog("DEBUG: [SDB] Environment");
        if (!m_environment.read(stream, this))
            return false;
    }

    if (stream->read32() != 0) {
        imLog("Error: Vertex Pools not currently supported");
        return false;
    }

    size_t mdbOffset = stream->read32();
    size_t mdbCount = stream->read32();

    m_sounds.resize(stream->read32());
    for (size_t i=0; i<m_sounds.size(); i++) {
        imLog("DEBUG: [SDB] Sound Source #{}", i);
        m_sounds[i] = std::make_shared<imSoundSource>();
        if (!m_sounds[i]->read(stream))
            return false;
    }

    return true;
}
