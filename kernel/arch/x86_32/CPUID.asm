
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

global cpuid_vendor_code
cpuid_vendor_code:
    push ebp
    mov ebp, esp
    push ebx ; for some reason ebx is callee-saved

    xor eax, eax
    cpuid

    mov eax, [ebp+0xC] ; get the argument
    mov [eax+0x0], ebx
    mov [eax+0x4], edx
    mov [eax+0x8], ecx

    pop ebx
    mov esp, ebp
    pop ebp
    ret

global cpuid_get_features ; extern "C" void cpuid_get_features(uint32_t* ecx, uint32_t* edx);
cpuid_get_features:
    push ebp
    mov ebp, esp
    push ebx

    mov eax, 0x1
    cpuid

    mov ebx, [ebp+0xC]
    mov [ebx], edx
    mov ebx, [ebp+0x8]
    mov [ebx], ecx

    pop ebx
    mov esp, ebp
    pop ebp
    ret

