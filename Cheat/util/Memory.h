#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <tlhelp32.h>

class Module
{
public:
    __int64 m_pStart = 0, m_pEnd = 0;
public:
    Module() = default;
    Module(const char* dllName)
    {
        this->m_pStart = reinterpret_cast<uintptr_t>(GetModuleHandle(dllName));
        if (this->m_pStart == 0) return;
        auto DosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(this->m_pStart);
        auto NTHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(this->m_pStart + DosHeader->e_lfanew);
        this->m_pEnd = this->m_pStart + NTHeaders->OptionalHeader.SizeOfImage;
    }
};

namespace mem {
    // Call the virtual function at the given index
    template <typename T, int nIndex, class CBaseClass, typename... Args_t>
    __forceinline T CallVFunc(CBaseClass* thisptr, Args_t... argList) noexcept
    {
        using VirtualFn_t = T(__thiscall*)(const void*, decltype(argList)...);
        return (*reinterpret_cast<VirtualFn_t* const*>(reinterpret_cast<std::uintptr_t>(thisptr)))[nIndex](thisptr, argList...);
    }

    // Get the virtual function at the given index
    template <typename T = void*>
    __forceinline T GetVFunc(const void* thisptr, std::size_t nIndex) noexcept
    {
        return (*static_cast<T* const*>(thisptr))[nIndex];
    }

    // Resolve a relative address
    __forceinline void* GetAbsoluteAddress(uintptr_t pRelativeAddress, int nPreOffset = 0x0, int nPostOffset = 0x0)
    {
        pRelativeAddress += nPreOffset;
        pRelativeAddress += sizeof(std::int32_t) + *reinterpret_cast<std::int32_t*>(pRelativeAddress);
        pRelativeAddress += nPostOffset;
        return (void*)pRelativeAddress;
    }
}