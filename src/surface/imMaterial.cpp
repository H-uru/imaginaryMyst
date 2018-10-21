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

#include "imMaterial.h"
#include "imCommon.h"
#include "scene/imSceneIndex.h"

imMaterial::~imMaterial()
{
    for (size_t i=0; i<m_layers.size(); i++) {
        if (m_layers[i] != 0)
            delete m_layers[i];
    }
}


bool imMaterial::read(imStream* stream, imSceneIndex* idx)
{
    if (stream->read32() != 0x7440708d) {
        imLog("Invalid Material Magic");
        return false;
    }
    stream->read32();   // Data size

    m_keys.read(stream);
    imLog("DEBUG: [SDB] [MATERIAL] '{}' ({})", m_keys.self().name(),
          m_keys.refs().size());
    m_readFlags = stream->read32();
    imLog("DEBUG: [SDB] [MATERIAL] (Read Flags) {08X}", m_readFlags);

    unsigned int this_2C = stream->read32();
    imLog("DEBUG: [SDB] [MATERIAL] (+2C) {}", this_2C);
    imLog("DEBUG: [SDB] [MATERIAL] (+28) {}", stream->read32());

    m_layers.resize(stream->read32());
    bool bugged = false;
    for (size_t i=0; i<m_layers.size(); i++) {
        imLog("DEBUG: [SDB] Layer #{}", i);
        m_layers[i] = imLayer::MakeLayer(stream->read32());
        if (m_layers[i] == 0)
            bugged = true;
        if (!bugged)
            bugged = !m_layers[i]->read(stream);
    }
    if (bugged)
        return false;

    if ((this_2C & 0x1) != 0) {
        imLog("ERROR: Material flags 0x1 not currently supported");
        return false;
    }

    for (size_t i=0; i<m_layers.size(); i++)
        m_layers[i]->readTextures(stream, idx);

    return true;
}
