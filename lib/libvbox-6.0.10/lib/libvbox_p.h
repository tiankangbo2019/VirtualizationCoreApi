/* This file is part of libvbox
 * Copyright (C) 2019  Michael Hansen
 *
 * libvbox is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * libvbox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with libvbox; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "libvbox.h"

#include <cstring>
#include <type_traits>

#if defined(VBOX_XPCOM)
#   include <nsMemory.h>
#   include <VirtualBox_XPCOM.h>

#   define COM_ERROR_CHECK(rc)                                          \
        do {                                                            \
            if (NS_FAILED(rc))                                          \
                COMError::raise(rc);                                    \
        } while (0)

    typedef PRBool      COM_Bool;
    typedef PRUint8     COM_Byte;
    typedef PRInt16     COM_Short;
    typedef PRUint16    COM_UShort;
    typedef PRInt32     COM_Long;
    typedef PRUint32    COM_ULong;
    typedef PRInt64     COM_Long64;
    typedef PRUint64    COM_ULong64;
    typedef PRUnichar  *COM_Text;

#   define COM_Enum(MSCOM_Type) PRUint32

    static_assert(sizeof(char16_t) == sizeof(PRUnichar), "PRUnichar is not a UTF-16 type");

    inline std::u16string nsToString(const COM_Text text)
    {
        return std::u16string(reinterpret_cast<const char16_t *>(text));
    }

    inline COM_Text nsFromString(const std::u16string &string)
    {
        COM_Text buffer = reinterpret_cast<COM_Text>(nsMemory::Alloc((string.size() + 1) * sizeof(PRUnichar)));
        std::char_traits<char16_t>::copy(reinterpret_cast<char16_t *>(buffer),
                                         string.c_str(), string.size());
        buffer[string.size()] = PRUnichar(0);
        return buffer;
    }

#   define COM_ToString(text)       nsToString(text)
#   define COM_FromString(string)   nsFromString(string)
#   define COM_FreeString(text)     nsMemory::Free(reinterpret_cast<void *>(text))

#   define COM_GetValue(obj, name, result)                              \
        do {                                                            \
            auto rc = obj->Get##name(&result);                          \
            COM_ERROR_CHECK(rc);                                        \
        } while (0)

#   define COM_SetValue(obj, name, value)                               \
        do {                                                            \
            auto rc = obj->Set##name(value);                            \
            COM_ERROR_CHECK(rc);                                        \
        } while (0)

#   define COM_GetValue_Wrap(obj, name, result)                         \
        do {                                                            \
            typedef decltype(result) Ptr;                               \
            Ptr::element_type::COM_Ifc *pValue;                         \
            auto rc = obj->Get##name(&pValue);                          \
            COM_ERROR_CHECK(rc);                                        \
            result = Ptr::wrap(pValue);                                 \
        } while (0)

#   define COM_SetValue_Wrap(obj, name, value)                          \
        do {                                                            \
            auto rc = obj->Set##name(value->get_IFC());                 \
            COM_ERROR_CHECK(rc);                                        \
        } while (0)

#   define COM_GetString(obj, name, result)                             \
        do {                                                            \
            COM_StringProxy proxy;                                      \
            auto rc = obj->Get##name(&proxy.m_text);                    \
            COM_ERROR_CHECK(rc);                                        \
            return proxy.toString();                                    \
        } while (0)

#   define COM_SetString(obj, name, value)                              \
        do {                                                            \
            COM_StringProxy proxy(value);                               \
            auto rc = obj->Set##name(proxy.m_text);                     \
            COM_ERROR_CHECK(rc);                                        \
        } while (0)

#   define COM_GetArray(obj, name, comtype, result)                     \
        do {                                                            \
            COM_ArrayProxy<comtype> proxy;                              \
            auto rc = obj->Get##name(&proxy.m_count, &proxy.m_array);   \
            COM_ERROR_CHECK(rc);                                        \
            proxy.toVector(result);                                     \
        } while (0)

#   define COM_SetArray(obj, name, comtype, value)                      \
        do {                                                            \
            COM_ArrayProxy<comtype> proxy(value);                       \
            auto rc = obj->Set##name(proxy.m_count, proxy.m_array);     \
            COM_ERROR_CHECK(rc);                                        \
        } while (0)

#   define COM_GetArray_Wrap(obj, name, result)                         \
        do {                                                            \
            typedef decltype(result)::value_type Ptr;                   \
            typedef Ptr::element_type::COM_Ifc COM_Ifc;                 \
            COM_ArrayProxy<COM_Ifc *> proxy;                            \
            auto rc = obj->Get##name(&proxy.m_count, &proxy.m_array);   \
            COM_ERROR_CHECK(rc);                                        \
            proxy.toVector(result);                                     \
        } while (0)

#   define COM_SetArray_Wrap(obj, name, value)                          \
        do {                                                            \
            typedef decltype(value)::value_type Ptr;                    \
            typedef Ptr::element_type::COM_Ifc COM_Ifc;                 \
            COM_ArrayProxy<COM_Ifc *> proxy(value);                     \
            auto rc = obj->Set##name(proxy.m_count, proxy.m_array);     \
            COM_ERROR_CHECK(rc);                                        \
        } while (0)

#   define COM_GetStringArray(obj, name, result)                        \
        do {                                                            \
            COM_StringArrayProxy proxy;                                 \
            auto rc = obj->Get##name(&proxy.m_count, &proxy.m_array);   \
            COM_ERROR_CHECK(rc);                                        \
            proxy.toVector(result);                                     \
        } while (0)

#   define COM_SetStringArray(obj, name, value)                         \
        do {                                                            \
            COM_StringArrayProxy proxy(value);                          \
            auto rc = obj->Set##name(proxy.m_count, proxy.constArray()); \
            COM_ERROR_CHECK(rc);                                        \
        } while (0)

#   define COM_ArrayParameter(proxy)    proxy.m_count, proxy.constArray()
#   define COM_ArrayParameterRef(proxy) &proxy.m_count, &proxy.m_array

    // Error code compatibility with MSCOM
#   define E_NOINTERFACE    NS_NOINTERFACE

#elif defined(VBOX_MSCOM)
#   define WIN32_LEAN_AND_MEAN
#   include <VirtualBox.h>

#   define COM_ERROR_CHECK(rc)                                          \
        do {                                                            \
            if (FAILED(rc))                                             \
                COMError::raise(rc);                                    \
        } while (0)

    typedef BOOL        COM_Bool;
    typedef BYTE        COM_Byte;
    typedef SHORT       COM_Short;
    typedef USHORT      COM_UShort;
    typedef LONG        COM_Long;
    typedef ULONG       COM_ULong;
    typedef LONG64      COM_Long64;
    typedef ULONG64     COM_ULong64;
    typedef BSTR        COM_Text;

#   define COM_Enum(MSCOM_Type) MSCOM_Type

    static_assert(sizeof(char16_t) == sizeof(WCHAR), "BSTR is not a UTF-16 type");

    inline std::u16string BSTRToString(BSTR text)
    {
        return std::u16string(reinterpret_cast<const char16_t *>(text),
                              SysStringLen(text));
    }

    inline BSTR BSTRFromString(const std::u16string &string)
    {
        return SysAllocStringLen(reinterpret_cast<const WCHAR *>(string.c_str()),
                                 static_cast<UINT>(string.size()));
    }

#   define COM_ToString(text)       BSTRToString(text)
#   define COM_FromString(string)   BSTRFromString(string)
#   define COM_FreeString(text)     SysFreeString(text)

#   define COM_GetValue(obj, name, result)                              \
        do {                                                            \
            auto rc = obj->get_##name(&result);                         \
            COM_ERROR_CHECK(rc);                                        \
        } while (0)

#   define COM_SetValue(obj, name, value)                               \
        do {                                                            \
            auto rc = obj->put_##name(value);                           \
            COM_ERROR_CHECK(rc);                                        \
        } while (0)

#   define COM_GetValue_Wrap(obj, name, result)                         \
        do {                                                            \
            typedef decltype(result) Ptr;                               \
            Ptr::element_type::COM_Ifc *pValue;                         \
            auto rc = obj->get_##name(&pValue);                         \
            COM_ERROR_CHECK(rc);                                        \
            result = Ptr::wrap(pValue);                                 \
        } while (0)

#   define COM_SetValue_Wrap(obj, name, value)                          \
        do {                                                            \
            auto rc = obj->put_##name(value->get_IFC());                \
            COM_ERROR_CHECK(rc);                                        \
        } while (0)

#   define COM_GetString(obj, name, result)                             \
        do {                                                            \
            COM_StringProxy proxy;                                      \
            auto rc = obj->get_##name(&proxy.m_text);                   \
            COM_ERROR_CHECK(rc);                                        \
            return proxy.toString();                                    \
        } while (0)

#   define COM_SetString(obj, name, value)                              \
        do {                                                            \
            COM_StringProxy proxy(value);                               \
            auto rc = obj->put_##name(proxy.m_text);                    \
            COM_ERROR_CHECK(rc);                                        \
        } while (0)

#   define COM_GetArray(obj, name, comtype, result)                     \
        do {                                                            \
            COM_ArrayProxy<comtype> proxy;                              \
            auto rc = obj->get_##name(&proxy.m_array);                  \
            COM_ERROR_CHECK(rc);                                        \
            proxy.toVector(result);                                     \
        } while (0)

#   define COM_SetArray(obj, name, comtype, value)                      \
        do {                                                            \
            COM_ArrayProxy<comtype> proxy(value);                       \
            auto rc = obj->put_##name(proxy.m_array);                   \
            COM_ERROR_CHECK(rc);                                        \
        } while (0)

#   define COM_GetArray_Wrap(obj, name, result)                         \
        do {                                                            \
            typedef decltype(result)::value_type Ptr;                   \
            typedef Ptr::element_type::COM_Ifc COM_Ifc;                 \
            COM_ArrayProxy<COM_Ifc *> proxy;                            \
            auto rc = obj->get_##name(&proxy.m_array);                  \
            COM_ERROR_CHECK(rc);                                        \
            proxy.toVector(result);                                     \
        } while (0)

#   define COM_SetArray_Wrap(obj, name, value)                          \
        do {                                                            \
            typedef decltype(value)::value_type Ptr;                    \
            typedef Ptr::element_type::COM_Ifc COM_Ifc;                 \
            COM_ArrayProxy<COM_Ifc *> proxy(value);                     \
            auto rc = obj->put_##name(proxy.m_array);                   \
            COM_ERROR_CHECK(rc);                                        \
        } while (0)

#   define COM_GetStringArray(obj, name, result)                        \
        do {                                                            \
            COM_StringArrayProxy proxy;                                 \
            auto rc = obj->get_##name(&proxy.m_array);                  \
            COM_ERROR_CHECK(rc);                                        \
            proxy.toVector(result);                                     \
        } while (0)

#   define COM_SetStringArray(obj, name, value)                         \
        do {                                                            \
            COM_StringArrayProxy proxy(value);                          \
            auto rc = obj->put_##name(proxy.m_array);                   \
            COM_ERROR_CHECK(rc);                                        \
        } while (0)

#   define COM_ArrayParameter(proxy)    proxy.m_array
#   define COM_ArrayParameterRef(proxy) &proxy.m_array

#else
#   error Unsupported COM configuration
#endif

namespace VBox
{
    class COM_StringProxy
    {
    public:
        COM_StringProxy() : m_text() { }

        COM_StringProxy(const std::u16string &value)
        {
            fromString(value);
        }

        ~COM_StringProxy()
        {
            if (m_text)
                COM_FreeString(m_text);
        }

        void fromString(const std::u16string &string)
        {
            m_text = COM_FromString(string);
        }

        std::u16string toString()
        {
            return COM_ToString(m_text);
        }

    public:
        COM_Text m_text;
    };

    template <typename Element>
    class COM_ArrayProxy
    {
    public:
#if defined(VBOX_XPCOM)
        COM_ArrayProxy() : m_count(), m_array() { }
#elif defined(VBOX_MSCOM)
        COM_ArrayProxy() : m_array() { }
#endif

        template <typename Type>
        COM_ArrayProxy(const std::vector<Type> &vector)
            : COM_ArrayProxy()
        {
            fromVector(vector);
        }

        template <typename Wrap>
        COM_ArrayProxy(const std::vector<COMPtr<Wrap>> &vector)
            : COM_ArrayProxy()
        {
            fromVector(vector);
        }

        ~COM_ArrayProxy() { Release(); }

        void Release()
        {
#if defined(VBOX_XPCOM)
            if (m_array)
                nsMemory::Free(reinterpret_cast<void *>(m_array));
#elif defined(VBOX_MSCOM)
            if (m_array)
                SafeArrayDestroy(m_array);
#endif

            m_array = nullptr;
        }

        template <typename Type>
        static typename std::enable_if<sizeof(Type) == sizeof(Element)
                                       && std::is_trivially_copyable<Type>::value, void>::type
        fillVector(std::vector<Type> &result, Element *source)
        {
            std::memcpy(&result[0], source, result.size() * sizeof(Type));
        }

        template <typename Type>
        static typename std::enable_if<sizeof(Type) != sizeof(Element)
                                       || !std::is_trivially_copyable<Type>::value, void>::type
        fillVector(std::vector<Type> &result, Element *source)
        {
            for (size_t i = 0; i < result.size(); ++i)
                result[i] = static_cast<Type>(source[i]);
        }

        template <typename Wrap>
        static void fillVector(std::vector<COMPtr<Wrap>> &result, Element *source)
        {
            for (size_t i = 0; i < result.size(); ++i) {
#if defined(VBOX_MSCOM)
                // This will be Released by SafeArrayDestroy
                source[i]->AddRef();
#endif
                result[i] = COMPtr<Wrap>::wrap(source[i]);
            }
        }

        template <typename Type>
        void toVector(std::vector<Type> &result)
        {
            if (!m_array) {
                result.resize(0);
                return;
            }

#if defined(VBOX_XPCOM)
            result.resize(m_count);
            fillVector(result, m_array);
#elif defined(VBOX_MSCOM)
            Element *pArray = nullptr;
            auto rc = SafeArrayAccessData(m_array, reinterpret_cast<void **>(&pArray));
            COM_ERROR_CHECK(rc);
            result.resize(m_array->rgsabound[0].cElements);
            fillVector(result, pArray);
            SafeArrayUnaccessData(m_array);
#endif
        }

        template <typename Type>
        static typename std::enable_if<sizeof(Type) == sizeof(Element)
                                       && std::is_trivially_copyable<Type>::value, void>::type
        copyVector(Element *result, const std::vector<Type> &source)
        {
            std::memcpy(result, &source[0], source.size() * sizeof(Type));
        }

        template <typename Type>
        static typename std::enable_if<sizeof(Type) != sizeof(Element)
                                       || !std::is_trivially_copyable<Type>::value, void>::type
        copyVector(Element *result, const std::vector<Type> &source)
        {
            for (size_t i = 0; i < source.size(); ++i)
                result[i] = static_cast<Element>(source[i]);
        }

        template <typename Wrap>
        static void copyVector(Element *result, const std::vector<COMPtr<Wrap>> &source)
        {
            for (size_t i = 0; i < source.size(); ++i) {
                result[i] = source[i]->get_IFC();
#if defined(VBOX_MSCOM)
                // This will be Released by SafeArrayDestroy
                result[i]->AddRef();
#endif
            }
        }

        template <typename Type>
        void fromVector(const std::vector<Type> &vector)
        {
            Release();

#if defined(VBOX_XPCOM)
            m_count = vector.size();
            m_array = reinterpret_cast<Element *>(nsMemory::Alloc(m_count * sizeof(Element)));
            copyVector(m_array, vector);
#elif defined(VBOX_MSCOM)
            m_array = SafeArrayCreateVector(VT_UNKNOWN, 0, static_cast<ULONG>(vector.size()));
            Element *pArray = nullptr;
            HRESULT rc = SafeArrayAccessData(m_array, reinterpret_cast<void **>(&pArray));
            COM_ERROR_CHECK(rc);
            copyVector(pArray, vector);
            SafeArrayUnaccessData(m_array);
#endif
        }

#if defined(VBOX_XPCOM)
        Element *constArray() const { return m_array; }
#endif

    public:
#if defined(VBOX_XPCOM)
        PRUint32 m_count;
        Element *m_array;
#elif defined (VBOX_MSCOM)
        SAFEARRAY *m_array;
#endif
    };

    class COM_StringArrayProxy
    {
    public:
#if defined(VBOX_XPCOM)
        COM_StringArrayProxy() : m_count(), m_array() { }
#elif defined(VBOX_MSCOM)
        COM_StringArrayProxy() : m_array() { }
#endif

        COM_StringArrayProxy(const std::vector<std::u16string> &vector)
            : COM_StringArrayProxy()
        {
            fromVector(vector);
        }

        ~COM_StringArrayProxy() { Release(); }

        void Release()
        {
#if defined(VBOX_XPCOM)
            if (m_array) {
                for (PRUint32 i = 0; i < m_count; ++i)
                    nsMemory::Free(reinterpret_cast<void *>(m_array[i]));
                nsMemory::Free(reinterpret_cast<void *>(m_array));
            }
#elif defined(VBOX_MSCOM)
            if (m_array)
                SafeArrayDestroy(m_array);
#endif

            m_array = nullptr;
        }

        void toVector(std::vector<std::u16string> &result)
        {
            if (!m_array) {
                result.resize(0);
                return;
            }

#if defined(VBOX_XPCOM)
            result.resize(m_count);
            for (PRUint32 i = 0; i < m_count; ++i)
                result[i] = nsToString(m_array[i]);
#elif defined(VBOX_MSCOM)
            BSTR *pArray = nullptr;
            auto rc = SafeArrayAccessData(m_array, reinterpret_cast<void **>(&pArray));
            COM_ERROR_CHECK(rc);
            result.resize(m_array->rgsabound[0].cElements);
            for (size_t i = 0; i < result.size(); ++i)
                result[i] = BSTRToString(pArray[i]);
            SafeArrayUnaccessData(m_array);
#endif
        }

        void fromVector(const std::vector<std::u16string> &vector)
        {
            Release();

#if defined(VBOX_XPCOM)
            m_count = vector.size();
            m_array = reinterpret_cast<PRUnichar **>(nsMemory::Alloc(m_count * sizeof(PRUnichar *)));
            for (size_t i = 0; i < vector.size(); ++i)
                m_array[i] = nsFromString(vector[i]);
#elif defined(VBOX_MSCOM)
            m_array = SafeArrayCreateVector(VT_BSTR, 0, static_cast<ULONG>(vector.size()));
            BSTR *pArray = nullptr;
            HRESULT rc = SafeArrayAccessData(m_array, reinterpret_cast<void **>(&pArray));
            COM_ERROR_CHECK(rc);
            for (size_t i = 0; i < vector.size(); ++i)
                pArray[i] = BSTRFromString(vector[i]);
            SafeArrayUnaccessData(m_array);
#endif
        }

#if defined(VBOX_XPCOM)
        const PRUnichar **constArray() const { return const_cast<const PRUnichar **>(m_array); }
#endif

    public:
#if defined(VBOX_XPCOM)
        PRUint32 m_count;
        PRUnichar **m_array;
#elif defined (VBOX_MSCOM)
        SAFEARRAY *m_array;
#endif
    };
}

#define COM_WRAP_IFC(COMType)                                           \
    const void *VBox::COMType::get_IID()                                \
    {                                                                   \
        return reinterpret_cast<const void *>(&IID_##COMType);          \
    }
