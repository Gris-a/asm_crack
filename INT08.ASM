; ===============================================
int08_tpos	FullAddr <08h * (size FullAddr), 0>
int08_old	FullAddr <0, 0>

counter		db 0ffh
prng_state	dw 1h
; ===============================================

; ===============================================
my_int08	proc
; -----------------------------------------------
	inc cs:counter
	cmp cs:counter, 0	; check for counter = 00h
	jz CustomInt08
; -----------------------------------------------
BaseInt08: jmp dword ptr cs:int08_old	; call int 08h
; -----------------------------------------------
CustomInt08:
	push ax bx si

	mov si, offset password	; randomize password
    REPT pass_len
        mov ax, cs:prng_state
        mov bx, ax
        shl bx, 7h
        xor ax, bx

        mov bx, ax
        shr bx, 9h
        xor ax, bx

        mov bx, ax
        shl bx, 8h
        xor ax, bx
        mov cs:prng_state, ax

        shr al, 4h
        add al, 61h
        mov cs:[si], al
    	inc si
    ENDM

	mov al, 20h
	out 20h, al

	pop cx bx ax
	iret
; -----------------------------------------------
my_int08	endp
; ===============================================