
global cpuid_supported
cpuid_supported: ; store output in eax (cdecl)

    pushfd
    pushfd
    xor dword [esp], 0x00200000 ; bit 21 indicates ability to use 21
    popfd
    pushfd
    pop eax
    xor eax, [esp]
    popfd
    and eax, 0x00200000
    ret

