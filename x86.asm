section .text
bits 64
default rel
global x86

x86:

	XORPS XMM0, XMM0
	
	L1:

		MOVSD XMM1, [RDX]
		MOVSD XMM2, [R8]
		MULSD XMM1, XMM2
		ADDSD XMM0, XMM1

		ADD R8, 8
		ADD RDX, 8
	
	LOOP L1

	RET