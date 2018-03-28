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

#ifndef _IM_ENVIRONMENT_H
#define _IM_ENVIRONMENT_H

#include "../surface/imMaterial.h"

class imEnvironment {
public:
    enum EnvironFlags {
        kHas_2 = (1<<1),
        kHas_4 = (1<<2),
        kHasFogDepth = (1<<3),
        kHasFogColor = (1<<4),
        kHasFogDensity = (1<<5),
        kHasYon = (1<<6),
        kHas_200 = (1<<9),
        kHasClearColor = (1<<11),
        kHasFogControl = (1<<13),
        kHasRenderProcs = (1<<15),
    };

    imEnvironment()
    { }

    ~imEnvironment()
    { }

    bool read(imStream* stream, class imSceneIndex* idx);

private:
    imKeyList m_keys;
    unsigned int m_environFlags;
    float m_fogDensity, m_fogDepth, m_yon;
    imColorRGBA m_fogColor, m_clearColor;
    imRef<imMaterial> m_material;
    std::vector<imKey> m_refs;
};

#endif
