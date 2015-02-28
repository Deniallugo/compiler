.686
.model flat
includelib c:\masm32\lib\msvcrt.lib
extern __imp__printf: proc
.data
	str1 db "%d %d %d", 0
	var_a dd 1 dup(0)
	var_b dd 10 dup(0)
.code
_start:
	mov eax, ebp
	mov ebx, -76
	add eax, ebx
	push eax
	push dword ptr 0
	pop eax
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	push eax
	push dword ptr 1
	pop ebx
	pop eax
	mov [eax + 0], ebx
	push ebx
	pop eax
	mov eax, ebp
	mov ebx, -76
	add eax, ebx
	push eax
	push dword ptr 1
	pop eax
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	push eax
	push dword ptr 2
	pop ebx
	pop eax
	mov [eax + 0], ebx
	push ebx
	pop eax
	mov eax, ebp
	mov ebx, -76
	add eax, ebx
	push eax
	push dword ptr 2
	pop eax
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	push eax
	push dword ptr 3
	pop ebx
	pop eax
	mov [eax + 0], ebx
	push ebx
	pop eax
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	push eax
	push dword ptr 0
	pop eax
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	push eax
	mov eax, ebp
	mov ebx, -76
	add eax, ebx
	push eax
	push dword ptr 0
	pop eax
	mov ebx, 4
	imul eax, ebx
	pop ebx
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
	push dword ptr 0
	pop eax
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	push eax
	push dword ptr 2
	pop eax
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	push eax
	pop eax
	push [eax + 0]
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	push eax
	push dword ptr 0
	pop eax
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	push eax
	push dword ptr 0
	pop eax
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	push [eax + 0]
	pop eax
	push [eax + 0]
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	push eax
	push dword ptr 0
	pop eax
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	push eax
	push dword ptr 1
	pop eax
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	push eax
	pop eax
	push [eax + 0]
	push offset str1
	call dword ptr __imp__printf
	add esp, 16
	ret
end _start