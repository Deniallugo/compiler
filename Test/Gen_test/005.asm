.686
.model flat
includelib c:\masm32\lib\msvcrt.lib
extern __imp__printf: proc
.data
	str1 db "%d", 0
	var_a dd 1 dup(0)
	var_b dd 1 dup(0)
.code
f_main:
	mov eax, ebp
	mov ebx, 0
	add eax, ebx
	push eax
	push dword ptr 1
	pop ebx
	pop eax
	mov [eax + 0], ebx
	push ebx
	pop eax
	mov eax, ebp
	mov ebx, 4
	add eax, ebx
	push eax
	push dword ptr 1
	pop ebx
	pop eax
	mov [eax + 0], ebx
	push ebx
	pop eax
	mov eax, ebp
	mov ebx, 0
	add eax, ebx
	push eax
	push [ebp + 4]
	push dword ptr 4
	pop ebx
	pop eax
	xor edx, edx
	imul ebx
	push eax
	pop ebx
	pop eax
	mov [eax + 0], ebx
	push ebx
	pop eax
	push [ebp + 0]
	push offset str1
	call dword ptr __imp__printf
	add esp, 8
	ret 0
_start:
	call f_main
	ret
end _start