.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib "..\Debug\Library.lib"
ExitProcess PROTO:DWORD 
.stack 4096

 outnum PROTO : DWORD

 outstr PROTO : DWORD

 outstrline PROTO : DWORD

 outnumline PROTO : DWORD

 outbool PROTO : DWORD

 outboolline PROTO : DWORD

 inttostr PROTO : DWORD

 system_pause PROTO 

 squere PROTO  : DWORD

 strLen PROTO  : DWORD

 inputInt PROTO 

 inputString PROTO 

 inputChar PROTO 

 power PROTO : DWORD, : DWORD

 Getlength PROTO : DWORD

 codetochar PROTO : DWORD
.const
 null_division BYTE 'ERROR: DIVISION BY ZERO', 0
 overflow BYTE 'ERROR: VARIABLE OVERFLOW', 0 
	L1 BYTE 'Hello World!', 0
	L2 SDWORD 0
	L3 SDWORD 1
	L4 SDWORD 2
	L5 BYTE 'resultHello = ', 0
	L6 BYTE 'Demonstration of programs!', 0
	L7 BYTE 'by Artsem Garmel!', 0
	L8 BYTE 'X', 0
	L9 BYTE 'symbol = ', 0
	L10 SDWORD 3
	L11 BYTE 'a = ', 0
	L12 SDWORD 21
	L13 BYTE 'b = ', 0
	L14 BYTE 0
	L15 BYTE 'Boolean = ', 0
	L16 BYTE ' ', 0
	L17 SDWORD 17
	L18 SDWORD 5
	L19 SDWORD 23
	L20 SDWORD 9
	L21 BYTE ' & ', 0
	L22 BYTE ' = ', 0
	L23 BYTE '17 | 5 = ', 0
	L24 BYTE '23 ^ 9 = ', 0
	L25 BYTE '~', 0
	L26 SDWORD 4
	L27 BYTE 'pow(2, 2) = ', 0
	L28 BYTE 'squere(4) = ', 0
	L29 BYTE 'fibonacci(4) = ', 0
	L30 BYTE 'Hello', 0
	L31 BYTE 'mess Length = ', 0
	L32 SDWORD 15
	L33 SDWORD 13
	L34 BYTE 'Hello!!!!!!!', 0
	L35 BYTE 'Bye bye', 0
	L36 BYTE 'I have ', 0
	L37 BYTE ' steps left to take!!!', 0
	L38 BYTE 'The End!!!', 0
.data
	mainstr DWORD ?
	mainsymbol DWORD ?
	maina SDWORD 0
	mainb SDWORD 0
	mainBoolean DWORD 0

	mainAND SDWORD 0
	mainOR SDWORD 0
	mainXOR SDWORD 0
	mainNOT SDWORD 0
	mainmess DWORD ?

.code

_Hello PROC 
	push ebx
	push edx
	push ecx
	push offset L1
	pop eax

jmp return_Hello


return_Hello:

	pop ecx
	pop edx
	pop ebx
	ret

SOMETHINGWRONG:
push offset null_division
call outstrline
call system_pause
push -1
call ExitProcess

EXIT_OVERFLOW:
push offset overflow
call outstrline
call system_pause
push -2
call ExitProcess
_Hello ENDP


_Print PROC _Printstr :  DWORD 
	push ebx
	push edx
	push ecx

	push _Printstr
	call outstrline

return_Print:

	pop ecx
	pop edx
	pop ebx
	ret

SOMETHINGWRONG:
push offset null_division
call outstrline
call system_pause
push -1
call ExitProcess

EXIT_OVERFLOW:
push offset overflow
call outstrline
call system_pause
push -2
call ExitProcess
_Print ENDP


_fibonacci PROC _fibonaccin :  SDWORD 
	push ebx
	push edx
	push ecx

if1do:
	push _fibonaccin
	push L2

	pop eax
	pop ebx
	cmp eax, ebx
	je if1Equal1True
	mov eax, 0
	push eax
	jmp if1Equal1End

if1Equal1True:
	mov eax, 1
	push eax

if1Equal1End:
	pop eax
	cmp eax, 0
	jne if1true
	je if1End
if1true:
	push L2
	pop eax

jmp return_fibonacci


	jmp if1End

if1End:

if2do:
	push _fibonaccin
	push L3

	pop eax
	pop ebx
	cmp eax, ebx
	je if2Equal1True
	mov eax, 0
	push eax
	jmp if2Equal1End

if2Equal1True:
	mov eax, 1
	push eax

if2Equal1End:
	push _fibonaccin
	push L4

	pop eax
	pop ebx
	cmp eax, ebx
	je if2Equal2True
	mov eax, 0
	push eax
	jmp if2Equal2End

if2Equal2True:
	mov eax, 1
	push eax

if2Equal2End:

	pop eax
	pop ebx
	cmp eax, 0
	jne if2Or1True
	cmp ebx, 0
	jne if2Or1True
	mov eax, 0
	push eax
	jmp if2Or1End

if2Or1True:
	mov eax, 1
	push eax

if2Or1End:
	pop eax
	cmp eax, 0
	jne if2true
	je if2End
if2true:
	push L3
	pop eax

jmp return_fibonacci


	jmp if2End

if2End:
	push _fibonaccin
	push L3

	pop ebx
	pop eax
	sub eax, ebx
	push eax
		call _fibonacci
	push eax
	push _fibonaccin
	push L4

	pop ebx
	pop eax
	sub eax, ebx
	push eax
		call _fibonacci
	push eax

	pop eax
	pop ebx
	add eax, ebx
	jo EXIT_OVERFLOW
	push eax
	pop eax

jmp return_fibonacci


return_fibonacci:

	pop ecx
	pop edx
	pop ebx
	ret

SOMETHINGWRONG:
push offset null_division
call outstrline
call system_pause
push -1
call ExitProcess

EXIT_OVERFLOW:
push offset overflow
call outstrline
call system_pause
push -2
call ExitProcess
_fibonacci ENDP


main PROC
		call _Hello
	push eax
	pop mainstr


	push offset L5
	call outstr
		call _Hello
	push eax

		call outstrline
	push offset L6
		call _Print
	push eax
	push offset L7
	pop mainstr


	push mainstr
	call outstrline
	push offset L8
	pop mainsymbol


	push offset L9
	call outstr

	push mainsymbol
	call outstrline
	push L10
	pop maina


	push offset L11
	call outstr

	push maina
	call outnumline
	push L12
	pop mainb


	push offset L13
	call outstr

	push mainb
	call outnumline
	xor eax, eax
	mov al, L14
	push eax
	pop mainBoolean


	push offset L15
	call outstr

	push mainBoolean
	call outboolline

	push offset L16
	call outstrline
	push maina
	push mainb

	pop ebx
	pop eax
	and eax, ebx
	push eax
	pop mainAND

	push L17
	push L18

	pop ebx
	pop eax
	or eax, ebx
	push eax
	pop mainOR

	push L19
	push L20

	pop ebx
	pop eax
	xor eax, ebx
	push eax
	pop mainXOR

	push maina

	pop eax
	not eax
	push eax
	pop mainNOT


	push maina
	call outnum

	push offset L21
	call outstr

	push mainb
	call outnum

	push offset L22
	call outstr

	push mainAND
	call outnumline

	push offset L23
	call outstr

	push mainOR
	call outnumline

	push offset L24
	call outstr

	push mainXOR
	call outnumline

	push offset L25
	call outstr

	push maina
	call outnum

	push offset L22
	call outstr

	push mainNOT
	call outnumline

	push offset L16
	call outstrline
	push L26
		call squere
	push eax
	pop mainb


	push offset L27
	call outstr
	push L4
	push L4
	push L4
	push L4
		call power
	push eax

		call outnumline

	push offset L28
	call outstr

	push mainb
	call outnumline

	push offset L29
	call outstr
	push L26
	push L26
		call _fibonacci
	push eax

		call outnumline
	push offset L30
	pop mainmess


	push offset L31
	call outstr
	push mainmess
	push mainmess
		call Getlength
	push eax

		call outnumline

	push offset L16
	call outstrline
	push L2
	push L10

	pop eax
	pop ebx
	add eax, ebx
	jo EXIT_OVERFLOW
	push eax
	pop maina

	push L32
	pop mainb


if3do:
	push mainb
	push L33

	pop eax
	pop ebx
	cmp eax, ebx
	je if3Equal1True
	mov eax, 0
	push eax
	jmp if3Equal1End

if3Equal1True:
	mov eax, 1
	push eax

if3Equal1End:
	pop eax
	cmp eax, 0
	jne if3true
	je if3false
if3true:

	push offset L34
	call outstrline

	jmp if3End

if3false:

	push offset L35
	call outstrline

	jmp if3End

if3End:

while1do:
	push maina
	push mainb

	pop eax
	pop ebx
	cmp eax, ebx
	jne while1NotEqual1True
	mov eax, 0
	push eax
	jmp while1NotEqual1End

while1NotEqual1True:
	mov eax, 1
	push eax

while1NotEqual1End:
	pop eax
	cmp eax, 0
	je while1End

	push offset L36
	call outstr

	push mainb
	call outnum

	push offset L37
	call outstrline
	push mainb
	push L3

	pop ebx
	pop eax
	sub eax, ebx
	push eax
	pop mainb

	jmp while1do

while1End:

	push offset L38
	call outstrline
call system_pause
push 0
call ExitProcess
SOMETHINGWRONG:
push offset null_division
call outstrline
call system_pause
push -1
call ExitProcess
EXIT_OVERFLOW:
push offset overflow
call outstrline
call system_pause
push -2
call ExitProcess
main ENDP
end main