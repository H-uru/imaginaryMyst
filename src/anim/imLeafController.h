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

#ifndef _IM_LEAFCONTROLLER_H
#define _IM_LEAFCONTROLLER_H

#include "imController.h"
#include "imKeyFrame.h"
#include <vector>

class imLeafController : public imController {
public:
    imLeafController()
    { }

    virtual ~imLeafController();

    virtual void read(imStream* stream);

private:
    std::vector<class imEaseController*> m_ease;
};

class imEaseController : public imLeafController {
public:
    imEaseController()
    { }

    virtual ~imEaseController()
    { }

    virtual void read(imStream* stream);

private:
    bool m_enabled;
    std::vector<imScalarKey> m_keys;
};

class imScalarController : public imLeafController {
public:
    imScalarController()
    { }

    virtual ~imScalarController()
    { }

    virtual void read(imStream* stream);

private:
    bool m_enabled;
    std::vector<imScalarKey> m_keys;
};

class imPoint3Controller : public imLeafController {
public:
    imPoint3Controller()
    { }

    virtual ~imPoint3Controller()
    { }

    virtual void read(imStream* stream);

private:
    bool m_enabled;
    std::vector<imPoint3Key> m_keys;
};

class imMatrix44Controller : public imLeafController {
public:
    imMatrix44Controller()
    { }

    virtual ~imMatrix44Controller()
    { }

    virtual void read(imStream* stream);

private:
    std::vector<imMatrix44Key> m_keys;
};

#endif
