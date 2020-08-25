// Generic code injecting utils
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  LINK/2012 <dma2012@hotmail.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Types.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdint>
#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>
#include <forward_list>

namespace Util
{

union MemPtr
{
protected:
    void* p;
    uintptr_t a;

public:
    // Constructors
    MemPtr() : p(0) {}
    MemPtr(std::nullptr_t) : p(nullptr) {}
    MemPtr(const MemPtr& x) : p(x.p) {}
    MemPtr(void* x) : p(x) {}
    MemPtr(uintptr_t x) : a(x) {}

    template <class T>
    MemPtr(T* x) : p((void *)x) {}

    bool is_null() const { return this->p != nullptr; }
    uintptr_t as_int() const { return this->a; }

    explicit operator bool() const { return is_null(); }

    MemPtr get() const { return *this; }
    template <class T> T* get() const { return get(); }
    template <class T> T* get_raw() { return get(); }

    template <class T>
    operator T*() const { return reinterpret_cast<T*>(p); }

    // Comparison
    bool operator ==(const MemPtr& rhs) const
    {
        return this->a == rhs.a;
    }

    bool operator !=(const MemPtr& rhs) const
    {
        return this->a != rhs.a;
    }

    bool operator <(const MemPtr& rhs) const
    {
        return this->a < rhs.a;
    }

    bool operator <=(const MemPtr& rhs) const
    {
        return this->a <= rhs.a;
    }

    bool operator >(const MemPtr& rhs) const
    {
        return this->a > rhs.a;
    }

    bool operator >=(const MemPtr& rhs) const
    {
        return this->a >= rhs.a;
    }

    // Operators
    MemPtr operator +(const MemPtr& rhs) const
    {
        return MemPtr(this->a + rhs.a);
    }

    MemPtr operator -(const MemPtr& rhs) const
    {
        return MemPtr(this->a - rhs.a);
    }

    MemPtr operator *(const MemPtr& rhs) const
    {
        return MemPtr(this->a * rhs.a);
    }

    MemPtr operator /(const MemPtr& rhs) const
    {
        return MemPtr(this->a / rhs.a);
    }

    // ..=
    MemPtr operator +=(const MemPtr& rhs) const
    {
        return MemPtr(this->a + rhs.a);
    }

    MemPtr operator -=(const MemPtr& rhs) const
    {
        return MemPtr(this->a - rhs.a);
    }

    MemPtr operator *=(const MemPtr& rhs) const
    {
        return MemPtr(this->a * rhs.a);
    }

    MemPtr operator /=(const MemPtr& rhs) const
    {
        return MemPtr(this->a / rhs.a);
    }
};

template <uintptr_t addr>
struct LazyPointer
{
public:
    // Returns the final raw pointer
    static MemPtr get()
    {
        return xget().get();
    }

    template <class T>
    static T* get()
    {
        return get().get<T>();
    }

private:
    // Returns the final pointer
    static MemPtr xget()
    {
        static void* ptr = nullptr;
        if (!ptr) ptr = MemPtr(addr).get();
        return MemPtr(ptr);
    }
};

template <uintptr_t addr>
inline MemPtr LazyPtr()
{
    return LazyPointer<addr>::get();
}

// Memory protection

inline bool ProtectMemory(MemPtr addr, size_t size, DWORD protection)
{
    return VirtualProtect(addr.get(), size, protection, &protection) != 0;
}

inline bool UnprotectMemory(MemPtr addr, size_t size, DWORD& out_oldprotect)
{
    return VirtualProtect(addr.get(), size, PAGE_EXECUTE_READWRITE, &out_oldprotect) != 0;
}

struct ScopedUnprotect
{
    MemPtr addr;
    size_t size;
    DWORD dwOldProtect;
    bool bUnprotected;

    ScopedUnprotect(MemPtr addr, size_t size)
    {
        if (size == 0) bUnprotected = false;
        else bUnprotected = UnprotectMemory(this->addr = addr.get<void>(), this->size = size, dwOldProtect);
    }

    ~ScopedUnprotect()
    {
        if (bUnprotected) ProtectMemory(this->addr.get(), this->size, this->dwOldProtect);
    }
};

class SectionUnprotect
{
public:
    SectionUnprotect(HINSTANCE hInstance, const char* name)
    {
        IMAGE_NT_HEADERS* ntHeader = (IMAGE_NT_HEADERS*)((BYTE*)hInstance + ((IMAGE_DOS_HEADER*)hInstance)->e_lfanew);
        IMAGE_SECTION_HEADER* pSection = IMAGE_FIRST_SECTION(ntHeader);

        DWORD VirtualAddress = MAXDWORD;
        SIZE_T VirtualSize = MAXDWORD;
        for (SIZE_T i = 0, j = ntHeader->FileHeader.NumberOfSections; i < j; ++i, ++pSection)
        {
            if (strncmp((const char*)pSection->Name, name, IMAGE_SIZEOF_SHORT_NAME) == 0)
            {
                VirtualAddress = (DWORD)hInstance + pSection->VirtualAddress;
                VirtualSize = pSection->Misc.VirtualSize;
                break;
            }
        }

        if (VirtualAddress == MAXDWORD)
            return;

        size_t QueriedSize = 0;
        while (QueriedSize < VirtualSize)
        {
            MEMORY_BASIC_INFORMATION MemoryInf;
            DWORD dwOldProtect;

            VirtualQuery((LPCVOID)VirtualAddress, &MemoryInf, sizeof(MemoryInf));
            VirtualProtect(MemoryInf.BaseAddress, MemoryInf.RegionSize, PAGE_EXECUTE_READWRITE, &dwOldProtect);
            m_queriedProtects.emplace_front(MemoryInf.BaseAddress, MemoryInf.RegionSize, MemoryInf.Protect);
            QueriedSize += MemoryInf.RegionSize;
        }
    };

    ~SectionUnprotect()
    {
        for (auto& it : m_queriedProtects)
        {
            DWORD dwOldProtect;
            VirtualProtect(std::get<0>(it), std::get<1>(it), std::get<2>(it), &dwOldProtect);
        }
    }

private:
    std::forward_list<std::tuple<LPVOID, SIZE_T, DWORD>> m_queriedProtects;
};

// Methods for reading/writing memory */

// Gets contents from a memory address
template <class T>
inline T MemRead(MemPtr addr)
{
    //return *(T *)addr.as_int();
    return *addr.get<T>();
}

template <class T = void> // "= void" (not needed?!) who cares..
inline T& ReadOffset(MemPtr ptr, size_t offset)
{
    return *(ptr + offset).get<T>();
}

// memset with unprotection
inline void MemSet(MemPtr addr, s32 value, size_t size)
{
    ScopedUnprotect xprotect(addr, size);
    memset(addr.get<void>(), value, size);
}

// memcpy with unprotection
inline void MemCpy(MemPtr addr, void const* src, size_t size)
{
    ScopedUnprotect xprotect(addr, size);
    memcpy(addr.get<void>(), src, size);
}

// MemWrite
template <class T>
inline void MemWrite(MemPtr addr, T value)
{
    ScopedUnprotect xprotect(addr, sizeof(T));

    if (MemRead<T>(addr) != value)
    {
        MemCpy(addr, &value, sizeof(T));
    }
}

// MemPatch
template <class T>
inline void MemPatch(MemPtr addr, T value)
{
    ScopedUnprotect xprotect(addr, sizeof(T));

    if (MemRead<T>(addr) != value)
    {
        //*(T *)addr.as_int() = value;
        *addr.get<T>() = value;
    }
}

//
// AdjustPointer
//  searches in the range [@addr, @addr + @max_search] for a pointer in the range [@default_base, @default_end] and replaces
//  it with the proper offset in the pointer @replacement_base.
//  does memory unprotection if @vp is true.
inline MemPtr AdjustPointer(
    MemPtr addr, MemPtr replacement_base, MemPtr default_base, MemPtr default_end, size_t max_search = 8)
{
    ScopedUnprotect xprotect(addr, max_search + sizeof(void*));
    for (size_t i = 0; i < max_search; ++i)
    {
        MemPtr ptr = MemRead<void*>(addr + i);
        if (ptr >= default_base.get() && ptr <= default_end.get())
        {
            auto result = replacement_base + (ptr - default_base.get());
            MemPatch<void*>(addr + i, result.get());
            return result;
        }
    }
    return nullptr;
}

// And some utilities :)
inline void MemFill(MemPtr addr, u8 value, size_t size = 1)
{
    //ScopedUnprotect xprotect(addr, size);
    MemSet(addr, value, size);
}

// string copy
inline void CopyStr(MemPtr addr, char const* value)
{
    ScopedUnprotect xprotect(addr, 1);
    strcpy(addr, value);
}

inline void CopyStrEx(MemPtr addr, char const* value, size_t count)
{
    ScopedUnprotect xprotect(addr, count);
    strncpy(addr, value, count);
}

inline void ZeroMem(MemPtr at, size_t count = 1)
{
    MemFill(at, 0, count);
}

inline void MakeNOP(MemPtr at, size_t count = 1)
{
    MemFill(at, 0x90, count);
}

inline void MakeRangedNOP(MemPtr at, MemPtr until)
{
    return MakeNOP(at, size_t(until.get_raw<char>() - at.get_raw<char>()));
}

// C3 RET
inline void MakeRET(MemPtr at)
{
    MemWrite<u8>(at, 0xC3);
}

// C2 RET (2Bytes)
inline void MakeRET(MemPtr at, u16 pop)
{
    MemWrite<u8>(at, 0xC2);
    MemWrite<u16>(at + 1, pop);
}

inline void MakeRETEx(MemPtr at, u8 ret = 1)
{
    MemPatch<u8>(at, 0xB0); // mov al, @ret
    MemPatch<u8>(0x5DF8F0 + 1, ret);
    MakeRET(0x5DF8F0 + 2, 4);
}

// for making functions return 0
inline void MakeRET0(MemPtr at)
{
    MemWrite<u8>(at, 0x33); // xor eax, eax
    MemWrite<u8>(at + 1, 0xC0);
    MakeRET(at + 2);
}

// Uses al instead of eax
inline void MakeRET0Ex(MemPtr at)
{
    MemWrite<u8>(at, 0x32); // xor al, al
    MemWrite<u8>(at + 1, 0xC0);
    MakeRET(at + 2);
}

// Jump Short
inline void MakeJMP(MemPtr at)
{
    MemWrite<u8>(at, 0xEB);
}

inline MemPtr GetAbsoluteOffset(int rel_value, MemPtr end_of_instruction)
{
    return end_of_instruction.get<char>() + rel_value;
}

inline int GetRelativeOffset(MemPtr abs_value, MemPtr end_of_instruction)
{
    return uintptr_t(abs_value.get<char>() - end_of_instruction.get<char>());
}

inline MemPtr ReadRelativeOffset(MemPtr at, size_t sizeof_addr = 4)
{
    switch (sizeof_addr)
    {
        case 1:
            return (GetAbsoluteOffset(MemRead<s8>(at), at + sizeof_addr));
        case 2:
            return (GetAbsoluteOffset(MemRead<s16>(at), at + sizeof_addr));
        case 4:
            return (GetAbsoluteOffset(MemRead<s32>(at), at + sizeof_addr));
    }
    return nullptr;
}

inline void MakeRelativeOffset(MemPtr at, MemPtr dest, size_t sizeof_addr = 4)
{
    switch (sizeof_addr)
    {
        case 1:
            MemWrite<s8>(at, static_cast<s8>(GetRelativeOffset(dest, at + sizeof_addr)));
        case 2:
            MemWrite<s16>(at, static_cast<s16>(GetRelativeOffset(dest, at + sizeof_addr)));
        case 4:
            MemWrite<s32>(at, static_cast<s32>(GetRelativeOffset(dest, at + sizeof_addr)));
    }
}

inline MemPtr GetBranchDestination(MemPtr at)
{
    switch (MemRead<u8>(at))
    {
        // We need to handle other instructions (and prefixes) later...
        case 0xE8: // call rel
        case 0xE9: // jmp rel
            return ReadRelativeOffset(at + 1, 4);

        case 0xFF:
        {
            switch (MemRead<u8>(at + 1))
            {
                case 0x15: // call dword ptr [addr]
                case 0x25: // jmp dword ptr [addr]
                    return *(MemRead<uintptr_t*>(at + 2));
            }
            break;
        }
    }
    return nullptr;
}

// Jump Near
inline MemPtr MakeJMP(MemPtr at, MemPtr dest)
{
    auto p = GetBranchDestination(at);
    MemWrite<u8>(at, 0xE9);
    MakeRelativeOffset(at + 1, dest, 4);
    return p;
}

inline MemPtr MakeCALL(MemPtr at, MemPtr dest)
{
    auto p = GetBranchDestination(at);
    MemWrite<u8>(at, 0xE8);
    MakeRelativeOffset(at + 1, dest, 4);
    return p;
}

inline void MakeJA(MemPtr at, MemPtr dest)
{
    MemWrite<u16>(at, 0x870F);
    MakeRelativeOffset(at + 2, dest, 4);
}

inline void MakeJE(MemPtr at, MemPtr dest)
{
    MemWrite<u16>(at, 0x840F);
    MakeRelativeOffset(at + 2, dest, 4);
}

inline void MakeJS(MemPtr at, MemPtr dest)
{
    MemWrite<u16>(at, 0x880F);
    MakeRelativeOffset(at + 2, dest, 4);
}

inline void MakeJNE(MemPtr at, MemPtr dest) // jnz too
{
    MemWrite<u16>(at, 0x850F);
    MakeRelativeOffset(at + 2, dest, 4);
}

// MakeJMP with NOPing @count
inline MemPtr MakeJMP(MemPtr at, MemPtr dest, size_t count)
{
    auto p = GetBranchDestination(at);
    MakeNOP(at, count);
    MakeJMP(at, dest);
    return p;
}

// Gets the virtual method table from the object @self
inline void** GetVMT(const void* self)
{
    return *(void ***)(self);
}

inline void** GetVFT(const void* self) { return GetVMT(self); } // Virtual Function Table
inline void** GetVCT(const void* self) { return GetVMT(self); } // Virtual Call Table
inline void** GetVTable(const void* self) { return GetVMT(self); } // vtable
inline void** GetVFTable(const void* self) { return GetVMT(self); } // vftable

inline MemPtr GetVF(MemPtr self, size_t index)
{
    return GetVMT(self.get<void>())[index];
}

// Calling Functions of GTASA exe
// Call function at @p returning @Ret with args @Args
// compiler's default calling convention
template <class Ret, class... Args>
inline Ret Call(MemPtr p, Args... a)
{
    auto fn = (Ret(*)(Args...)) p.get<void>();
    return fn(std::forward<Args>(a)...);
}

template <class Ret, class... Args>
inline Ret Cdecl(MemPtr p, Args... a)
{
    auto fn = (Ret(__cdecl*)(Args...))p.get<void>();
    return fn(std::forward<Args>(a)...);
}

template <class Ret, class ...Args>
inline Ret StdCall(MemPtr p, Args... a)
{
    auto fn = (Ret(__stdcall*)(Args...))p.get<void>();
    return fn(std::forward<Args>(a)...);
}

template <class Ret, class ...Args>
inline Ret ThisCall(MemPtr p, Args... a)
{
    auto fn = (Ret(__thiscall*)(Args...))p.get<void>();
    return fn(std::forward<Args>(a)...);
}

template <size_t index>
struct Vtbl
{
    template <class Ret, class... Args>
    static Ret Call(Args... a)
    {
		int i;
        auto obj = MemPtr(std::get<0>(std::forward_as_tuple(a...)));
        auto p = MemPtr((*obj.template get<void**>())[i]);
        return ThisCall<Ret>(p, std::forward<Args>(a)...);
    }
};

template <class Ret, class... Args>
inline Ret FastCall(MemPtr p, Args... a)
{
    auto fn = (Ret(__fastcall*)(Args...))p.get<void>();
    return fn(std::forward<Args>(a)...);
}

// A Patch where you install and uninstall it later
// Saves some memory contents for changes, after done, you can restore this later..
struct RestorablePatch
{
    // Saves @size bytes from @at
    RestorablePatch(MemPtr at, size_t size = 1) : addr_(at), numBytes_(size) { MemCpy(bytes_, at.get<void>(), size); }

    // Restore what was there on destruction
    ~RestorablePatch() { MemCpy(addr_, bytes_, numBytes_); }

    void Write(size_t offset, u8 value) { MemWrite<u8>(addr_ + offset, value); }

    // without destructing it
    void Restore() { MemCpy(addr_, bytes_, numBytes_); }

protected:
    u8* bytes_;
    size_t numBytes_;
    MemPtr addr_;
};

}
