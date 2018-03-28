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

/* imSimplePosController */
imSimplePosController::~imSimplePosController()
{
    if (m_position != 0)
        delete m_position;
}

void imSimplePosController::read(imStream* stream)
{
    if (m_position != 0)
        delete m_position;

    if (stream->read32() != 0) {
        m_position = new imPoint3Controller();
        m_position->read(stream);
    } else {
        m_position = 0;
    }
}


/* imCompoundPosController */
imCompoundPosController::~imCompoundPosController()
{
    if (m_XController != 0)
        delete m_XController;
    if (m_YController != 0)
        delete m_XController;
    if (m_ZController != 0)
        delete m_XController;
}

void imCompoundPosController::read(imStream* stream)
{
    if (m_XController != 0)
        delete m_XController;
    if (m_YController != 0)
        delete m_XController;
    if (m_ZController != 0)
        delete m_XController;

    if (stream->read32() != 0) {
        m_XController = new imScalarController();
        m_XController->read(stream);
    } else {
        m_XController = 0;
    }

    if (stream->read32() != 0) {
        m_YController = new imScalarController();
        m_YController->read(stream);
    } else {
        m_YController = 0;
    }

    if (stream->read32() != 0) {
        m_ZController = new imScalarController();
        m_ZController->read(stream);
    } else {
        m_ZController = 0;
    }
}
