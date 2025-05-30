;"
.model flat,c

.code

;-----------------------------------
PGMImageInvert proc
	push ebp
	mov ebp, esp

	push edi
	push esi

	mov edi, [ebp + 8]	;edi = ResultBuffer
	mov esi, [ebp + 12]	;esi = Buffer
	mov edx, [ebp + 16] ;edx = Size

	vpxor ymm1, ymm1, ymm1	
	vpcmpeqd ymm1, ymm1, ymm1	;ymm1 = 1's
	xor ecx, ecx

@@:
	vmovdqa ymm0, ymmword ptr[esi + ecx]

	vpsubusb ymm2, ymm1, ymm0	;255 - PixelValue

	vmovdqa ymmword ptr[edi + ecx], ymm2
	add ecx, 32
	cmp ecx, edx
	jl @B

	pop esi
	pop edi
	pop ebp
    vzeroall
	ret

PGMImageInvert endp

;-----------------------------------
PGMImageBright proc

push ebp
	mov ebp, esp

	push edi
	push esi
	push ebx

	mov edi, [ebp + 8]	;edi = ResultBuffer
	mov esi, [ebp + 12]	;esi = Buffer
	mov edx, [ebp + 16] ;edx = Size
	mov ebx, [ebp + 20] ;ebx = BrightFactor

	vpbroadcastb ymm1, ebx	
	xor ecx, ecx

@@:
	vmovdqa ymm0, ymmword ptr[esi + ecx]

	vpaddusb ymm2, ymm1, ymm0	

	vmovdqa ymmword ptr[edi + ecx], ymm2
	add ecx, 32
	cmp ecx, edx
	jl @B

	pop ebx
	pop esi
	pop edi
	pop ebp
    vzeroall
	ret

PGMImageBright endp

;-----------------------------------
PGMImageThreshold proc

push ebp
	mov ebp, esp

	push edi
	push esi
	push ebx

	mov edi, [ebp + 8]	;edi = ResultBuffer
	mov esi, [ebp + 12]	;esi = Buffer
	mov edx, [ebp + 16] ;edx = Size

	vpxor ymm1, ymm1, ymm1
	vpcmpeqd ymm1, ymm1, ymm1

	vpxor ymm4, ymm4, ymm4

	xor ecx, ecx

@@:
	vmovdqa ymm0, ymmword ptr[esi + ecx]

	vpcmpgtb ymm3, ymm1, ymm0	

	vpblendvb ymm2, ymm4, ymm1, ymm3

	vmovdqa ymmword ptr[edi + ecx], ymm2
	add ecx, 32
	cmp ecx, edx
	jl @B

	pop ebx
	pop esi
	pop edi
	pop ebp
    vzeroall
	ret

PGMImageThreshold endp
end