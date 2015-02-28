.686
.model flat
includelib c:\masm32\lib\msvcrt.lib
extern __imp__printf: proc
.data
	str1 db "%d", 0
.code
f_main:
	mov eax, ebp
	mov ebx, -1
	add eax, ebx
	push eax
	push 'a'
	pop ebx
	pop eax
	mov [eax + 0], bl
	xor ebx, ebx
	mov bl, [eax + 0]
	push eax
	push ebx
	pop eax
	mov eax, ebp
	mov ebx, -8
	add eax, ebx
	push eax
	push [ebp + -1]
	push [ebp + -1]
	pop ebx
	pop eax
	xor edx, edx
	sub eax, ebx
	push eax
	pop ebx
	pop eax
	mov [eax + 0], ebx
	push ebx
	pop eax
	push [ebp + -8]
	push offset str1
	call dword ptr __imp__printf
	add esp, 8
	ret 0
_start:
	call f_main
	ret
end _start