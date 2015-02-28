.686
.model flat
includelib c:\masm32\lib\msvcrt.lib
extern __imp__printf: proc
.data
	str1 db "Hello world", 0
	str2 db "%s", 0
	var_str dd 1 dup(0)
.code
_start:
	mov eax, ebp
	mov ebx, 4
	add eax, ebx
	push eax
	push offset str1
	pop ebx
	pop eax
	mov [eax + 0], ebx
	push ebx
	pop eax
	push [ebp + 4]
	push offset str2
	call dword ptr __imp__printf
	add esp, 8
	ret
end _start