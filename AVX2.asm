section .data
EXCESS_COUNT DQ 0
ADDRESS DQ 0

section .text
bits 64
default rel
global AVX2

AVX2: 
    ; Store the address of the 1st vector
    MOV [ADDRESS], RDX

    ; Compute for the reduced iterations and the excess
    MOV RAX, RCX
    XOR RDX, RDX
    MOV R10, 4
    DIV R10

    ; Store Excess Count
    MOV [EXCESS_COUNT], RDX

    ; Zero out YMM0
    VXORPD YMM0, YMM0

    ; Zero out XMM0 and XMM1
    XORPD XMM0, XMM0
    XORPD XMM1, XMM1

    ; Load Address of Vector 1
    MOV R10, [ADDRESS]

    ; Check if RAX = 0
    CMP RAX, 0
    JE SKIP

    ; Perform Operation using YMM Registers
    MOV RCX, RAX
    L1:

        VMOVDQU YMM1, [R10]
        VMOVDQU YMM2, [R8]
        VMULPD YMM1, YMM2
        VADDPD YMM0, YMM1

        ADD R10, 32
        ADD R8, 32

    LOOP L1
    SKIP:

    ; Extract the values from YMM0 and place them in XMM0 and XMM1
    VEXTRACTF128 XMM1, YMM0, 1
    VEXTRACTF128 XMM0, YMM0, 0

    ; Add the contents of both XMM registers such that XMM0 will contain the total
    HADDPD XMM0, XMM1
    HADDPD XMM0, XMM0

    ; Handle case where EXCESS_COUNT > 0
    CMP QWORD[EXCESS_COUNT], 0
    JE FINISH


        MOV RCX, QWORD[EXCESS_COUNT]
        L2:

            MOVSD XMM1, [R10]
            MOVSD XMM2, [R8]
            MULSD XMM1, XMM2
            ADDSD XMM0, XMM1

            ADD R10, 8
            ADD R8, 8


        LOOP L2


    FINISH:

    RET