.686
.model flat
includelib c:\masm32\lib\msvcrt.lib
extern __imp__printf: proc
.data
	str1 db "%d %d", 0
.code
_start:
	mov eax, ebp
	mov ebx, 4
	add eax, ebx
	push eax
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	push eax
	pop ebx
	pop eax
	mov [eax + 0], ebx
	push ebx
	pop eax
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	push eax
	push dword ptr 1
	pop ebx
	pop eax
	mov [eax + 0], ebx
	push ebx
	pop eax
	push [ebp + 8]
	push [ebp + 4]
	push offset str1
	call dword ptr __imp__printf
	add esp, 12
	ret
end _start