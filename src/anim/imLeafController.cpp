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

#include "imLeafController.h"
#include "imCommon.h"

/* imLeafController */
imLeafController::~imLeafController()
{
    for (size_t i=0; i<m_ease.size(); i++)
        delete m_ease[i];
}

void imLeafController::read(imStream* stream)
{
    imLog("DEBUG: [SDB] [plLeafController] (UruUnknown) {}", stream->read32());

    for (size_t i=0; i<m_ease.size(); i++)
        delete m_ease[i];
    m_ease.resize(stream->read32());
    for (size_t i=0; i<m_ease.size(); i++) {
        m_ease[i] = new imEaseController();
        m_ease[i]->read(stream);
    }

    stream->read32();   // Yep, another zero
}


/* imEaseController */
void imEaseController::read(imStream* stream)
{
    imLeafController::read(stream);

    m_enabled = (stream->read32() != 0);
    if (m_enabled) {
        m_keys.resize(stream->read32());
        for (size_t i=0; i<m_keys.size(); i++)
            m_keys[i].read(stream);
    }
}


/* imScalarController */
void imScalarController::read(imStream* stream)
{
    imLeafController::read(stream);

    m_enabled = (stream->read32() != 0);
    if (m_enabled) {
        m_keys.resize(stream->read32());
        for (size_t i=0; i<m_keys.size(); i++)
            m_keys[i].read(stream);
    }
}


/* imPoint3Controller */
void imPoint3Controller::read(imStream* stream)
{
    imLeafController::read(stream);

    m_enabled = (stream->read32() != 0);
    if (m_enabled) {
        m_keys.resize(stream->read32());
        for (size_t i=0; i<m_keys.size(); i++)
            m_keys[i].read(stream);
    }
}


/* imMatrix44Controller */
void imMatrix44Controller::read(imStream* stream)
{
    imLeafController::read(stream);

    m_keys.resize(stream->read32());
    for (size_t i=0; i<m_keys.size(); i++)
        m_keys[i].read(stream);
}
