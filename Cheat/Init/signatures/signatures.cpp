#include "signatures.h"

#ifdef _DEBUG
#include "../../util/Console.h" // console::print
#endif

// Initialize Signatures
bool signatures::init() {
    for (int i = 0; i < m_vSignatures.size(); i++) {
        if (!m_vSignatures[i].find()) {
            // Print to console
#ifdef _DEBUG
            console::print(m_vSignatures[i].name.c_str(), m_vSignatures[i].address);
#endif
            ::Sleep(1000);
            return false;
        }

        // Print to console
#ifdef _DEBUG
        console::print(m_vSignatures[i].name.c_str(), m_vSignatures[i].address);
#endif
    }
    return true;
}

// Find Signatures address by name
void* signatures::find(std::string name) {
    for (int i = 0; i < m_vSignatures.size(); i++) {
        if (m_vSignatures[i].name == name) {
            return m_vSignatures[i].address;
        }
    }
    return nullptr;
}


// Signature class
bool Signature::find() {
    // Get Module
    uintptr_t Start = reinterpret_cast<uintptr_t>(GetModuleHandle(dll.c_str()));
    auto DosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(Start);
    auto NTHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(Start + DosHeader->e_lfanew);
    uintptr_t End = Start + NTHeaders->OptionalHeader.SizeOfImage;

    // Convert string to bytes
    std::vector<int16_t> PatternBytes;
    size_t len = signature.length();
    for (size_t i = 0; i < len; i += 3) {
        const std::string ByteString = signature.substr(i, 2);
        if (ByteString == "??" || ByteString == "?? ")
            PatternBytes.push_back(-1);
        else if (ByteString == "? ") {
            PatternBytes.push_back(-1);
            i--;
        }
        else
        {
            char* End;
            long byte = std::strtol(ByteString.c_str(), &End, 16);

            if (*End != 0)
                return FALSE;

            PatternBytes.push_back((int16_t)static_cast<uint8_t>(byte));
        }
    }

    // Find Pattern
    uintptr_t rangeEnd = End - PatternBytes.size();
    for (uintptr_t Address = Start; Address <= rangeEnd; ++Address) {
        bool Match = true;
        for (size_t i = 0; i < PatternBytes.size(); ++i) {
            if (PatternBytes[i] == -1) continue;
            if (*(uint8_t*)(Address + i) != PatternBytes[i]) {
                Match = false;
                break;
            }
        }

        if (Match) {
            // Relative Address
            if (nPreOffset != 0) {
                Address += nPreOffset;
                Address += sizeof(std::int32_t) + *reinterpret_cast<std::int32_t*>(Address);
            }
            Address += nPostOffset;

            // Save
            if (save)
                *save = (void*)Address;
            address = (void*)Address;
            return true;
        }
    }
    return false;
}