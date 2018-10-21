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

#include "imSoundSource.h"
#include "imCommon.h"

bool imSoundSource::read(imStream* stream)
{
    if (stream->read32() != 0x656d2f93) {
        imLog("Invalid Sound Source Magic");
        return false;
    }
    stream->read32();   // Data size

    m_keys.read(stream);
    imLog("DEBUG: [SDB] [SOUNDSOURCE] '{}' ({})", m_keys.self().name(),
          m_keys.refs().size());

    imLog("DEBUG: [SDB] [SOUNDSOURCE] (+1C) {}", stream->read32());
    m_soundKeys.resize(stream->read32());
    for (size_t i=0; i<m_soundKeys.size(); i++) {
        size_t len = stream->read32();
        m_soundKeys[i] = stream->readString(len);
    }

    imLog("DEBUG: [SDB] [SOUNDSOURCE] (+38) {}", stream->readFloat());
    imLog("DEBUG: [SDB] [SOUNDSOURCE] (?) {}", stream->read32());
    imLog("DEBUG: [SDB] [SOUNDSOURCE] (+70) {}", stream->readFloat());

    size_t len = stream->read32();
    m_sceneObjectKey = stream->readString(len);
    len = stream->read32();
    m_animPathKey = stream->readString(len);
    len = stream->read32();
    m_listenerKey = stream->readString(len);
    len = stream->read32();
    m_observerKey = stream->readString(len);

    imLog("DEBUG: [SDB] [SOUNDSOURCE] (+8C) {}", stream->readFloat());
    size_t count = stream->read32();
    for (size_t i=0; i<count; i++) {
        len = stream->read32();
        imLog("DEBUG: [SDB] [SOUNDSOURCE] (+7C) '{}'", stream->readString(len));
    }

    imLog("DEBUG: [SDB] [SOUNDSOURCE] (+A4) {}", stream->readFloat());
    imLog("DEBUG: [SDB] [SOUNDSOURCE] (+A8) {}", stream->readFloat());
    imLog("DEBUG: [SDB] [SOUNDSOURCE] (+AC) {}", stream->readFloat());
    imLog("DEBUG: [SDB] [SOUNDSOURCE] (+B0) {}", stream->readFloat());
    imLog("DEBUG: [SDB] [SOUNDSOURCE] (+C0) {}", stream->readFloat());
    imLog("DEBUG: [SDB] [SOUNDSOURCE] (+90) {}", stream->readFloat());

    return true;
}
