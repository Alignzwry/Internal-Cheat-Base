#pragma once
#include <Windows.h>
#include <stdint.h>
#include <string>
#include <vector>

class Signature {
public:
    Signature(std::string name, std::string dll, std::string signature, void** address = 0, int nPreOffset = 0, int nPostOffset = 0) : name(name), dll(dll), signature(signature), save(address), nPreOffset(nPreOffset), nPostOffset(nPostOffset) {}

    bool find();

public:
    // Name, not neccessarily needed if not using signatures::find
    std::string name;

    // Address that holds the result and will also be saved into "save"
    void* address = 0;

private:
    // Module to search in
    std::string dll;

    // the signature to search for
    std::string signature;

    // Address to write the found value to
    void** save = 0;

    // Offset to resolve relative address
    int nPreOffset = 0;

    // Offset
    int nPostOffset = 0;
};

// Includ all files that you need to save your signatures to
#include "../render/render.h"
namespace signatures {
    bool init();
    void* find(std::string name);

    // Your Signatures
    inline std::vector<Signature> m_vSignatures = {
        Signature("Present", "dxgi.dll", "90 00 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 37 41 8B F0 45 8B C8 44 8B", (void**)&Render::oPresent, 0, -16),
        Signature("ResizeBuffers", "dxgi.dll", "48 08 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 68 D8 48 81 EC 28 01 00 00 41 8B F9 41", (void**)&Render::oResizeBuffers, 0, -16),
        Signature("CreateSwapChain", "dxgi.dll", "FF 48 81 EC 98 01 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 88 00 00 00 4D", (void**)&Render::oCreateSwapChain, 0, -16),
        Signature("TranslateMessages", "user32.dll", "89 50 10 57 41 54 41 55 41 56 41 57 48 81 EC ?? ?? ?? ?? 45 8B F1 4D", (void**)&Render::oTranslateMessages, 0, -16)
    };
}