.model tiny
.286

.data
pass	db '<GDROC'
N = 0


.code
org 100h

Start:
	mov bl, 0f0h
	mov cl, 6
	REPT 6
	sub bl, cl
	dec cl
	sub byte ptr pass + N, bl
	N = N + 1
	ENDM
	
	mov ax, 4c00h
	int 21h

end	Start