#pragma once
#include "stdafx.h"
#include "Semantic.h"
#define SEPSTREMP  "\n;------------------------------\n"
#define SEPSTR(x)  "\n;----------- " + string(x) + " ------------\n"


#define BEGIN ".586\n"\
".model flat, stdcall\n"\
"includelib libucrt.lib\n"\
"includelib kernel32.lib\n"\
"includelib \"..\\Debug\\Library.lib\"\n"\
"ExitProcess PROTO:DWORD \n"\
".stack 4096\n"

#define END "call system_pause"\
				"\npush 0"\
				"\ncall ExitProcess"\
				"\nSOMETHINGWRONG:"\
				"\npush offset null_division"\
				"\ncall outstrline"\
				"\ncall system_pause"\
				"\npush -1"\
				"\ncall ExitProcess"\
				"\nEXIT_OVERFLOW:"\
				"\npush offset overflow"\
				"\ncall outstrline"\
				"\ncall system_pause"\
				"\npush -2"\
				"\ncall ExitProcess"\
				"\nmain ENDP\nend main"


#define EXTERN "\n outnum PROTO : DWORD\n"\
"\n outstr PROTO : DWORD\n"\
"\n outstrline PROTO : DWORD\n"\
"\n outnumline PROTO : DWORD\n"\
"\n outbool PROTO : DWORD\n"\
"\n outboolline PROTO : DWORD\n"\
"\n inttostr PROTO : DWORD\n"\
"\n system_pause PROTO \n"\
"\n squere PROTO  : DWORD\n"\
"\n strLen PROTO  : DWORD\n"\
"\n inputInt PROTO \n"\
"\n inputString PROTO \n"\
"\n inputChar PROTO \n"\
"\n power PROTO : DWORD, : DWORD\n"\
"\n Getlength PROTO : DWORD\n"\
"\n codetochar PROTO : DWORD\n"


#define ITENTRY(x)  lex.idtable.table[lex.lextable.table[x].idxTI]
#define LEXEMA(x)   lex.lextable.table[x].lexema


namespace Gener
{
	bool findFalseIf(int i, Lex::LEX& lex);
	bool findTrueIf(int i, Lex::LEX& lex);
	bool CodeGeneration(Lex::LEX& tables, Parm::PARM& parm, Log::LOG& log);
	int getIndexType(int i, Lex::LEX& lex);
	int Operation(int i, Lex::LEX& lex, ofstream& ofile, Semantic::Is is, string Nameblock = "undef");
};