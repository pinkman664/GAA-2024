#pragma once
#include "stdafx.h"
#include "IT.h"
#include "LT.h"

#define TYPE_INTEGER	"int"
#define TYPE_STRING		"string"
#define TYPE_SYMBOL		"char"
#define TYPE_BOOL		"bool"
#define SQUERE			"squere"
#define INT_TO_STR		"intToStr"
#define CONCAT			"concat"
#define COPY			"copy"
#define ININT			"inputInt"
#define INSTR			"inputString"
#define INSYM			"inputChar"
#define TYPE_VOID		"void"
#define MAIN			"Main"
#define LENGTH			"Getlength"
#define POW				"pow"
#define STRLEN			"strLen"

#define ISTYPE(str) ( !strcmp(str, TYPE_INTEGER) || !strcmp(str, TYPE_STRING) || !strcmp(str, TYPE_SYMBOL) )

namespace Lex
{
	struct LEX
	{
		LT::LexTable lextable;
		IT::IdTable	idtable;
		LEX() {}
	};
	struct Graph
	{
		char lexema;
		FST::FST graph;
	};
	IT::Entry* getEntry		// формирует и возвращает строку ТИ
	(
		Lex::LEX& tables,						
		char lex,								
		char* id,							
		char* idtype,						
		bool isParam,							
		bool isFunc,	
		Log::LOG log,					
		int line,	
		int position,
		bool& rc_err							
	);

	struct ERROR_S								
	{
		int id;
		char message[ERROR_MAXSIZE_MESSAGE];		
		struct
		{
			short line = -1;						
			short col = -1;						  
		} position;
	};

	bool analyze(LEX& tables, In::IN& in, Log::LOG& log, Parm::PARM& parm);
	int getIndexInLT(LT::LexTable& lextable, int itTableIndex);			
};