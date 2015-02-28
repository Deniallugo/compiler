.686
.model flat
includelib c:\masm32\lib\msvcrt.lib
extern __imp__printf: proc
.data
	str1 db "1", 0
.code
f_main:
	push offset str1
	call dword ptr __imp__printf
	add esp, 4
	ret 0
_start:
	call f_main
	ret
end _start