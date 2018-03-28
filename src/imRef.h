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

#ifndef _IM_REF_H
#define _IM_REF_H

class imDataRef {
public:
    imDataRef() : m_refs(1)
    { }

    virtual ~imDataRef()
    { }

    void addRef()
    { ++m_refs; }

    void delRef()
    {
        if (--m_refs == 0)
            delete this;
    }

private:
    unsigned long m_refs;
};

template<class _R>
class imRef {
public:
    imRef(_R* ref = 0) : m_ref(ref)
    { }

    imRef(const imRef<_R>& ref) : m_ref(ref.m_ref)
    {
        if (m_ref != 0)
            m_ref->addRef();
    }

    ~imRef<_R>()
    {
        if (m_ref != 0)
            m_ref->delRef();
    }

    imRef& operator=(_R* ref)
    {
        if (m_ref != 0)
            m_ref->delRef();
        m_ref = ref;
        return *this;
    }

    imRef& operator=(const imRef<_R>& ref)
    {
        if (ref.m_ref != 0)
            ref.m_ref->addRef();
        if (m_ref != 0)
            m_ref->delRef();
        m_ref = ref.m_ref;
        return *this;
    }

    bool operator==(_R* ref) const
    { return m_ref == ref; }

    bool operator==(const imRef<_R>& ref) const
    { return m_ref == (_R*)ref; }

    operator _R*() const
    { return m_ref; }

    _R& operator*() const
    { return *m_ref; }

    _R* operator->() const
    { return m_ref; }

    template<class _T>
    _T* cast() const
    { return (_T*)m_ref; }

    template<class _T>
    imRef<_T> convert() const
    {
        imRef<_T> cref((_T*)m_ref);
        if (m_ref != 0)
            m_ref->addRef();
        return cref;
    }

private:
    _R* m_ref;
};

#endif
