.686
.model flat
includelib c:\masm32\lib\msvcrt.lib
extern __imp__printf: proc
.data
	str1 db "%d %d %d", 0
.code
f_main:
	mov eax, ebp
	mov ebx, 0
	add eax, ebx
	push eax
	push dword ptr 3
	pop ebx
	pop eax
	mov [eax + 0], ebx
	push ebx
	pop eax
	mov eax, ebp
	mov ebx, 0
	add eax, ebx
	push eax
	mov eax, ebp
	mov ebx, 0
	add eax, ebx
	push eax
	pop ebx
	pop eax
	mov [eax + 0], ebx
	push ebx
	pop eax
	mov eax, ebp
	mov ebx, 0
	add eax, ebx
	push eax
	mov eax, ebp
	mov ebx, 0
	add eax, ebx
	push eax
	pop ebx
	pop eax
	mov [eax + 0], ebx
	push ebx
	pop eax
	push [ebp + 0]
	mov eax, ebp
	mov ebx, 0
	add eax, ebx
	push eax
	pop eax
	mov ebx, [eax + 0]
	push ebx
	push [ebp + 0]
	push offset str1
	call dword ptr __imp__printf
	add esp, 16
	ret 0
_start:
	call f_main
	ret
end _start