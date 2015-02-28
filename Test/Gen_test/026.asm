.686
.model flat
includelib c:\masm32\lib\msvcrt.lib
extern __imp__printf: proc
.data
	str1 db "%d %d", 0
.code
f_main:
	mov eax, ebp
	mov ebx, 0
	add eax, ebx
	push eax
	push dword ptr 5
	pop ebx
	pop eax
	mov [eax + 0], ebx
	push ebx
	pop eax
	mov eax, ebp
	mov ebx, -396
	add eax, ebx
	push eax
	push [ebp + 0]
	pop eax
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	push eax
	push dword ptr 15
	pop eax
	neg eax
	push eax
	pop ebx
	pop eax
	mov [eax + 0], ebx
	push ebx
	pop eax
	mov eax, ebp
	mov ebx, -396
	add eax, ebx
	push eax
	push [ebp + 0]
	pop eax
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	push eax
	push [ebp + 0]
	push [ebp + 0]
	pop ebx
	pop eax
	xor edx, edx
	imul ebx
	push eax
	push [ebp + 0]
	pop ebx
	pop eax
	xor edx, edx
	imul ebx
	push eax
	push dword ptr 5
	pop ebx
	pop eax
	xor edx, edx
	idiv ebx
	push eax
	push dword ptr 3
	pop ebx
	pop eax
	xor edx, edx
	idiv ebx
	push edx
	push dword ptr 1
	pop ebx
	pop eax
	xor edx, edx
	idiv ebx
	push eax
	push dword ptr 1
	pop ebx
	pop eax
	xor edx, edx
	sub eax, ebx
	push eax
	pop ebx
	pop ecx
	mov eax, [ecx + 0]
	xor edx, edx
	imul ebx
	mov [ecx + 0], eax
	push eax
	pop eax
	mov eax, ebp
	mov ebx, -396
	add eax, ebx
	push eax
	push dword ptr 99
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
	mov ebx, -396
	add eax, ebx
	push eax
	push [ebp + 0]
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
	ret 0
_start:
	call f_main
	ret
end _start