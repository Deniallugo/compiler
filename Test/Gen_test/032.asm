.686
.model flat
includelib c:\masm32\lib\msvcrt.lib
extern __imp__printf: proc
.data
	str1 db "%d", 0
.code
_start:
	mov eax, ebp
	mov ebx, 0
	add eax, ebx
	push eax
	push dword ptr 0
	pop eax
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	push [eax + 0]
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	push eax
	pop ebx
	pop eax
	mov [eax + 0], ebx
	push ebx
	pop eax
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	push eax
	push dword ptr 5
	pop ebx
	pop eax
	mov [eax + 0], ebx
	push ebx
	pop eax
	push [ebp + -4]
	mov eax, ebp
	mov ebx, 0
	add eax, ebx
	push eax
	push dword ptr 0
	pop eax
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	push [eax + 0]
	push [eax + 0]
	pop eax
	pop ecx
	push eax
	push dword ptr 0
	pop eax
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	push eax
	push [eax + 0]
	pop eax
	pop ecx
	push eax
	push offset str1
	call dword ptr __imp__printf
	add esp, 12
	ret
end _start