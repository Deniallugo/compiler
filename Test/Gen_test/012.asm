.686
.model flat
includelib c:\masm32\lib\msvcrt.lib
extern __imp__printf: proc
.data
	str1 db "%d", 0dh, 0ah, 0
	str2 db "%d", 0dh, 0ah, 0
	str3 db "%d", 0dh, 0ah, 0
	str4 db "%d", 0dh, 0ah, 0
	var_a dd 1 dup(0)
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
	push [ebp + 0]
	push [ebp + 0]
	pop ebx
	pop eax
	xor edx, edx
	and eax, ebx
	push eax
	push offset str1
	call dword ptr __imp__printf
	add esp, 8
	push [ebp + 0]
	push dword ptr 1
	push dword ptr 3
	pop eax
	pop ebx
	mov ecx, eax
	shl ebx, cl
	push ebx
	pop ebx
	pop eax
	xor edx, edx
	or eax, ebx
	push eax
	push offset str2
	call dword ptr __imp__printf
	add esp, 8
	push [ebp + 0]
	push [ebp + 0]
	pop ebx
	pop eax
	xor edx, edx
	xor eax, ebx
	push eax
	push offset str3
	call dword ptr __imp__printf
	add esp, 8
	push [ebp + 0]
	push offset str4
	call dword ptr __imp__printf
	add esp, 8
	ret 0
_start:
	call f_main
	ret
end _start