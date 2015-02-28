.686
.model flat
includelib c:\masm32\lib\msvcrt.lib
extern __imp__printf: proc
.data
	str1 db "%d %d %d %d", 0
	var_a dd 50 dup(0)
.code
_start:
	mov eax, ebp
	mov ebx, -180
	add eax, ebx
	push eax
	push dword ptr 5
	pop eax
	mov ebx, 20
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
	push dword ptr 1
	pop eax
	neg eax
	push eax
	pop ebx
	pop eax
	mov [eax + 0], ebx
	push ebx
	pop eax
	mov eax, ebp
	mov ebx, -180
	add eax, ebx
	push eax
	push dword ptr 0
	pop eax
	mov ebx, 20
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
	push eax
	mov eax, ebp
	mov ebx, -180
	add eax, ebx
	push eax
	push dword ptr 5
	pop eax
	mov ebx, 20
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
	push [eax + 0]
	pop eax
	pop ecx
	push eax
	pop ebx
	pop eax
	mov [eax + 0], ebx
	push ebx
	pop eax
	mov eax, ebp
	mov ebx, -180
	add eax, ebx
	push eax
	push dword ptr 5
	pop eax
	mov ebx, 20
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
	push eax
	push [eax + 0]
	pop eax
	pop ecx
	push eax
	mov eax, ebp
	mov ebx, -180
	add eax, ebx
	push eax
	push dword ptr 0
	pop eax
	mov ebx, 20
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
	push [eax + 0]
	pop eax
	pop ecx
	push eax
	mov eax, ebp
	mov ebx, -180
	add eax, ebx
	push eax
	push dword ptr 0
	pop eax
	mov ebx, 20
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
	push eax
	push [eax + 0]
	pop eax
	pop ecx
	push eax
	mov eax, ebp
	mov ebx, -180
	add eax, ebx
	push eax
	push dword ptr 5
	pop eax
	mov ebx, 20
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
	push [eax + 0]
	pop eax
	pop ecx
	push eax
	push offset str1
	call dword ptr __imp__printf
	add esp, 20
	ret
end _start