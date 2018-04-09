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

#include "imAnimBehavior.h"
#include "imLeafController.h"
#include "imCommon.h"

/* imAnimController */
void imAnimController::read(imStream* stream)
{
    imLog("DEBUG: [SDB] [hsDynamicValue] (+0C) %d", stream->read32());
    imLog("DEBUG: [SDB] [hsDynamicValue] (+10) %f", stream->readFloat());

    allocController(stream->read32());
    if (m_controller != 0)
        m_controller->read(stream);
}

void imAnimController::allocController(unsigned int ctype)
{
    if (m_controller != 0)
        delete m_controller;
    if (ctype == imController::kNoController) {
        m_controller = 0;
        return;
    }

    switch (m_type) {
    case kTypeScalar:
        m_controller = new imScalarController();
        break;
    case kTypeColor:
        if (ctype == imController::kCompoundController)
            m_controller = new imCompoundPosController();
        else
            m_controller = new imSimplePosController();
        break;
    case kTypeMatrix44:
        m_controller = new imMatrix44Controller();
        break;
    default:
        m_controller = 0;
    }
}


/* imAnimEvent */
void imAnimEvent::read(imStream* stream)
{
    // Zeroes...  What fun!
    stream->read32();
    stream->read32();
    stream->read32();
    stream->read32();

    imLog("DEBUG: [SDB] [hsFrameEvent] (+0) %d", stream->read32());
    imLog("DEBUG: [SDB] [hsFrameEvent] (+8) %d", stream->read32());
    imLog("DEBUG: [SDB] [hsFrameEvent] (+0C) %d", stream->read32());
    imLog("DEBUG: [SDB] [hsFrameEvent] (+4) %d", stream->read32());

    size_t len = stream->read32();
    imString str = stream->readString(len);
    imLog("DEBUG: [SDB] [hsFrameEvent] (+10) '%s'", str.data());
}


/* imAnimBehavior */
bool imAnimBehavior::read(imStream* stream)
{
    imLog("DEBUG: [SDB] [hsDynamicBehavior]");

    // Zeroes...  How worthless!
    stream->read32();
    stream->read32();
    stream->read32();
    stream->read32();

    if (stream->read32BE() != 0x1831b) {
        imLog("Invalid AnimBehavior magic!");
        return false;
    }
    if (stream->read32BE() != 3) {
        imLog("Unsupported AnimBehavior Major Version");
        return false;
    }
    if (stream->read32BE() != 0) {
        imLog("Unsupported AnimBehavior Minor Version");
        return false;
    }

    m_saveFlags = stream->read32();
    m_animTime = stream->readFloat();
    m_frameCount = stream->read32();
    m_frameRate = stream->readFloat();
    m_firstFrame = stream->read32();
    m_lastFrame = stream->read32();

    m_events.resize(stream->read32());
    for (size_t i=0; i<m_events.size(); i++)
        m_events[i].read(stream);

    if ((m_saveFlags & 0x102) != 0) {
        imLog("ERROR: AnimBehavior flags 0x%X not currently supported",
              m_saveFlags & 0x102);
        return false;
    }
    return true;
}
