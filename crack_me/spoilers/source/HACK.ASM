.model tiny
.286

FullAddr	struc
	offst	dw 0
	sgmnt	dw 0
FullAddr	ends

.data
; ===============================================
out_correct     db 'correct  ', 0dh, '$'
out_incorrect	db 'incorrect', 0dh, '$'

password	db 'password', 0dh
pass_len	equ 8
; ===============================================

.code
org 100h
; ===============================================
Start:
	mov ah, 2ch		; set init prng_state on time depended
	int 21h
	add prng_state, dx
; -----------------------------------------------
	mov ax, 3508h		; load int 08h address
	int 21h

	mov int08_old.offst, bx
	mov int08_old.sgmnt, es
; ----------------------------------------------
	les si, int08_tpos	; int 08h interception

	cli
	mov es:[si+0], offset my_int08
	mov es:[si+2], cs
	sti
; ----------------------------------------------
	int 08h			; randomize password
	call verification	; get password verification
	test ax, ax
	je Correct
; -----------------------------------------------
Incorrect:
	mov dx, offset out_incorrect
	jmp Done
; -----------------------------------------------
Correct:
	mov dx, offset out_correct
; -----------------------------------------------
Done:
	mov ah, 09h
	int 21h
; -----------------------------------------------
	les si, int08_tpos	; int 08h recovery

	cli
	mov bx, int08_old.offst
	mov es:[si+0], bx
	mov bx, int08_old.sgmnt
	mov es:[si+2], bx
	sti
; -----------------------------------------------
	mov ax, 4c00h		; exit the programm
	int 21h
; -----------------------------------------------
; ===============================================

; ===============================================
;
; ===============================================
verification	proc
; -----------------------------------------------
	push bp
	mov bp, sp
; -----------------------------------------------
	sub sp, 0bh		; get place in stack for password
	mov bx, sp
	mov byte ptr [bx], 80h	; load password max size
; -----------------------------------------------
	mov si, bx		; load ds:si = input address
	add si, 2

	mov di, ds		; load es:di = password address
	mov es, di
	mov di, offset password
; -----------------------------------------------
	mov dx, bx		; get input
	mov ah, 0ah
	int 21h
; -----------------------------------------------
	cld			; compare input to password
	mov cx, pass_len + 1
	repe cmpsb
	test cx, cx

	mov sp, bp
	pop bp

	je True
; -----------------------------------------------
False:
	mov ax, 1
	ret
; -----------------------------------------------
True:
	mov ax, 0
	ret
; -----------------------------------------------
verification	endp
; ===============================================

; ===============================================
include INT08.ASM
; ===============================================
end	Start
