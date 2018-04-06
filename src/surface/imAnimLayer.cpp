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

#include "imAnimLayer.h"
#include "imCommon.h"
#include "scene/imSceneIndex.h"

/* imAnimLayer */
bool imAnimLayer::read(imStream* stream)
{
    if (!imLayer::read(stream))
        return false;

    m_frameCount = stream->read32();
    m_frameController.read(stream);
    return true;
}

void imAnimLayer::readTextures(imStream* stream, imSceneIndex* idx)
{
    m_textures.resize(m_frameCount);
    for (size_t i=0; i<m_frameCount; i++)
        m_textures[i] = idx->texture(stream->read32());
}


/* imAVILayer */
bool imAVILayer::read(imStream* stream)
{
    if (!imLayer::read(stream))
        return false;

    m_animFlags = stream->read32();
    imLog("DEBUG: [SDB] [imAVILayer] (AnimFlags) %08X", m_animFlags);
    size_t len = stream->read32();
    m_filename = stream->readString(len);
    imLog("DEBUG: [SDB] [imAVILayer] (Filename) '%s'", m_filename.data());

    if ((m_animFlags & 0x8) != 0) {
        imLog("DEBUG: [SDB] [imAVILayer] (+9C) %d", stream->read32());
        imLog("DEBUG: [SDB] [imAVILayer] (+A0) %d", stream->read32());
    }
    return true;
}


/* imQTLayer */
bool imQTLayer::read(imStream* stream)
{
    if (!imLayer::read(stream))
        return false;

    m_animFlags = stream->read32();
    imLog("DEBUG: [SDB] [imQTLayer] (AnimFlags) %08X", m_animFlags);
    size_t len = stream->read32();
    m_filename = stream->readString(len);
    imLog("DEBUG: [SDB] [imQTLayer] (Filename) '%s'", m_filename.data());
    return true;
}


/* imBinkLayer */
bool imBinkLayer::read(imStream* stream)
{
    if (!imLayer::read(stream))
        return false;

    m_animFlags = stream->read32();
    imLog("DEBUG: [SDB] [imBinkLayer] (AnimFlags) %08X", m_animFlags);
    size_t len = stream->read32();
    m_filename = stream->readString(len);
    imLog("DEBUG: [SDB] [imBinkLayer] (Filename) '%s'", m_filename.data());
    return true;
}


/* imWaterLayer */
bool imWaterLayer::read(imStream* stream)
{
    if (!imLayer::read(stream))
        return false;

    m_animFlags = stream->read32();
    imLog("DEBUG: [SDB] [imWaterLayer] (AnimFlags) %08X", m_animFlags);
    imLog("DEBUG: [SDB] [imWaterLayer] (+EC) %d", stream->read32());
    imLog("DEBUG: [SDB] [imWaterLayer] (+F4) %d", stream->read32());

    size_t count = stream->read32();
    for (size_t i=0; i<count; i++) {
        imLog("DEBUG: [SDB] [WaterEffect] (+0) %d", stream->read32());
        imLog("DEBUG: [SDB] [WaterEffect] (+4) %d", stream->read32());
        imLog("DEBUG: [SDB] [WaterEffect] (+8) %d", stream->read32());
        imLog("DEBUG: [SDB] [WaterEffect] (+0C) %d", stream->read32());
        imLog("DEBUG: [SDB] [WaterEffect] (+10) %d", stream->read32());
        imLog("DEBUG: [SDB] [WaterEffect] (+14) %d", stream->read32());
    }
    return true;
}
