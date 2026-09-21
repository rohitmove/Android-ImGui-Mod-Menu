#pragma once
#include "../Include/KittyMemory/MemoryPatch.h"

inline void RunBypass() {
    text_section_t textSec = KittyMemory::getSectionInfo(OBFUSCATE("libil2cpp.so"));
    if (textSec.address) {
        MemoryPatch::protectRegion(textSec.address, textSec.size, _PROT_READ | _PROT_WRITE | _PROT_EXEC);
    }
}
