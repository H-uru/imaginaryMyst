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

#ifndef _IM_CONTROLLER_H
#define _IM_CONTROLLER_H

#include "../vfs/imStream.h"

class imController {
public:
    enum Type {
        kNoController, kSimpleController, kCompoundController
    };

    imController()
    { }

    virtual ~imController()
    { }

    virtual void read(imStream* stream) = 0;
};

class imSimplePosController : public imController {
public:
    imSimplePosController() : m_position(0)
    { }

    virtual ~imSimplePosController();

    virtual void read(imStream* stream);

private:
    class imPoint3Controller* m_position;
};

class imCompoundPosController : public imController {
public:
    imCompoundPosController()
    : m_XController(0), m_YController(0), m_ZController(0)
    { }

    virtual ~imCompoundPosController();

    virtual void read(imStream* stream);

private:
    class imScalarController* m_XController;
    class imScalarController* m_YController;
    class imScalarController* m_ZController;
};

#endif
