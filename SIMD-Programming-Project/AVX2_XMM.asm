section .data
EXCESS_COUNT DQ 0
ADDRESS DQ 0

section .text
bits 64
default rel
global AVX2_XMM

AVX2_XMM:
    ; Store the address of the 1st vector
    MOV [ADDRESS], RDX

    ; Compute for the reduced iterations and the excess
    MOV RAX, RCX
    XOR RDX, RDX
    MOV R10, 2
    DIV R10

    ; Store Excess Count
    MOV [EXCESS_COUNT], RDX

    ; Zero out XMM0
    XORPD XMM0, XMM0

    ; Load Address of Vector 1
    MOV R10, [ADDRESS]

    ; Check if RAX = 0
    CMP RAX, 0
    JE SKIP

    ; Perform Operation using XMM Registers
    MOV RCX, RAX
    L1:
        
        MOVDQU XMM1, [R10]
        MOVDQU XMM2, [R8]
        MULPD XMM1, XMM2
        VADDPD XMM0, XMM1

        ADD R10, 16
        ADD R8, 16

    LOOP L1
    SKIP:

    ; Handle case where EXCESS_COUNT > 0
    CMP QWORD[EXCESS_COUNT], 0
    JE FINISH

        MOVSD XMM1, [R10]
        MOVSD XMM2, [R8]
        MULSD XMM1, XMM2
        ADDSD XMM0, XMM1

    FINISH:

    ; Add the lower and uppper part of the XMM register to get the total value
    HADDPD XMM0, XMM0

    RET