#pragma once
#define __STDC_WANT_LIB_EXT1__ 1
#include "LexAnaliz.h"
#include "stdafx.h"
#include "Graphs.h"

#include <string.h>
#include <cstring>
#include <stdio.h>

#include "IT.h"
#include "LT.h"
#include "Parm.h"
namespace Lex
{
	int numCostr[] = { 0, 0, 0, 0 };
	Graph graphs[N_GRAPHS] =
	{
		{ LEX_SEPARATORS, FST::FST(GRAPH_SEPARATORS) },
		{ LEX_LOGIC_EQUALS, FST::FST(GRAPH_LOGIC_EQ) },
		{ LEX_LOGIC_NOT_EQUALS, FST::FST(GRAPH_LOGIC_NOT_EQ) },	
		{ LEX_LOGIC_NOT, FST::FST(GRAPH_LOGIC_NOT) },	
		{ LEX_LOGIC_AND, FST::FST(GRAPH_LOGIC_AND) },
		{ LEX_LOGIC_OR, FST::FST(GRAPH_LOGIC_OR) },
		{ LEX_LOGIC_MORE, FST::FST(GRAPH_LOGIC_MORE) },
		{ LEX_LOGIC_LESS, FST::FST(GRAPH_LOGIC_LESS) },
		{ LEX_LOGIC_MORE_EQ, FST::FST(GRAPH_LOGIC_EQ_MORE) },
		{ LEX_LOGIC_LESS_EQ, FST::FST(GRAPH_LOGIC_EQ_LESS) },
		{ LEX_ID_TYPE, FST::FST(GRAPH_INTEGER) },
		{ LEX_ID_TYPE, FST::FST(GRAPH_BOOLEAN) },
		{ LEX_VOID, FST::FST(GRAPH_VOID) },
		{ LEX_STDFUNC, FST::FST(GRAPH_POW) },
		{ LEX_STDFUNC, FST::FST(GRAPH_LENGTH) },
		{ LEX_STDFUNC, FST::FST(GRAPH_INT_TO_CHAR) },
		{ LEX_STDFUNC, FST::FST(GRAPH_CONCAT) },
		{ LEX_STDFUNC, FST::FST(GRAPH_SQUERE) },
		{ LEX_STDFUNC, FST::FST(GRAPH_STRLEN) },
		{ LEX_STDFUNC, FST::FST(GRAPH_COPY) },
		{ LEX_STDFUNC, FST::FST(GRAPH_ININT) },
		{ LEX_STDFUNC, FST::FST(GRAPH_INSYM) },
		{ LEX_STDFUNC, FST::FST(GRAPH_INSTR) },
		{ LEX_LITERAL, FST::FST(GRAPH_INT_LITERAL) },
		{ LEX_LITERAL, FST::FST(GRAPH_STRING_LITERAL) },
		{ LEX_LITERAL, FST::FST(GRAPH_SYMBOL_LITERAL) },
		{ LEX_LITERAL, FST::FST(GRAPH_FALSE) },
		{ LEX_LITERAL, FST::FST(GRAPH_TRUE) },
		{ LEX_MAIN, FST::FST(GRAPH_MAIN) },
		{ LEX_ID_TYPE, FST::FST(GRAPH_STRING) },
		{ LEX_FUNCTION, FST::FST(GRAPH_FUNCTION) },
		{ LEX_ID_TYPE, FST::FST(GRAPH_SYMBOL) },
		{ LEX_RETURN, FST::FST(GRAPH_RETURN) },
		{ LEX_WRITE, FST::FST(GRAPH_WRITE) },
		{ LEX_NEWLINE, FST::FST(GRAPH_WRITELINE) },
		{ LEX_IF, FST::FST(GRAPH_IF) },
		{ LEX_WHILE, FST::FST(GRAPH_WHILE) },
		{ LEX_DO, FST::FST(GRAPH_DO) },
		{ LEX_ISFALSE, FST::FST(GRAPH_ISFALSE) },
		{ LEX_ISTRUE, FST::FST(GRAPH_ISTRUE) },
		{ LEX_ID, FST::FST(GRAPH_ID) }
	};

	char* getScopeName(LEX& tables, char* prevword)
	{
		char* main = new char[5];
		main = (char*)"main\0";
		if (strcmp(prevword, MAIN) == 0)
			return main;
		char* a = new char[20];
		for (int i = tables.lextable.size - 1; i >= 0 && tables.lextable.table[i + 2].lexema != LEX_LEFTHESIS; i--)
		{
			switch (tables.lextable.table[i].lexema)
			{
			case LEX_ID: {
				if (tables.idtable.table[tables.lextable.table[i].idxTI].idtype == IT::IDTYPE::F
					|| tables.idtable.table[tables.lextable.table[i].idxTI].idtype == IT::IDTYPE::S)
					return tables.idtable.table[tables.lextable.table[i].idxTI].id;
				break;
			}
			case LEX_DO: {
				a[0] = '\0';
				char* number = new char[15];
				strncat(a, "while", 20);
				_itoa_s(++numCostr[0], number, 15, 10);
				strncat(a, number, 20);
				return a;
			}
			case LEX_ISFALSE: {
				a[0] = '\0';
				char* number = new char[13];
				strncat(a, "iffalse", 20);
				_itoa_s(++numCostr[1], number, 13, 10);
				strncat(a, number, 20);
				return a;
			}
			case LEX_ISTRUE: {
				a[0] = '\0';
				char* number = new char[14];
				strncat(a, "iftrue", 20);
				_itoa_s(++numCostr[2], number, 14, 10);
				strncat(a, number, 20);
				return a;
			}
			case LEX_WHILE: {
				a[0] = '\0';
				char* number = new char[15];
				strncat(a, "while", 20);
				_itoa_s(++numCostr[0], number, 15, 10);
				strncat(a, number, 20);
				return a;
			}
			default:
				break;
			}
		}
		return nullptr;
	}

	bool isLiteral(char* id)
	{
		if (isdigit(*id) || *id == IN_CODE_QUOTE || *id == LEX_MINUS || *id == IN_CODE_NOT_DOUBLE_QUOTE 
			|| *id == NULL || !strcmp(id, "true") || !strcmp(id, "false"))
			return true;
		return false;
	}
	
	IT::STDFNC getStandFunction(char* id)
	{
		if (!strcmp(SQUERE, id))
			return IT::STDFNC::F_SQUERE;
		else if (!strcmp(POW, id))
			return IT::STDFNC::F_POW;
		else if (!strcmp(LENGTH, id))
			return IT::STDFNC::F_LENGTH;
		else if (!strcmp(STRLEN, id))
			return IT::STDFNC::F_STRLEN;
		else if (!strcmp(INT_TO_STR, id))
			return IT::STDFNC::F_INT_TO_STR;
		
		else if (!strcmp(ININT, id))
			return IT::STDFNC::F_ININT;
		else if (!strcmp(INSYM, id))
			return IT::STDFNC::F_INSYM;
		else if (!strcmp(INSTR, id))
			return IT::STDFNC::F_INSTR;

		return IT::STDFNC::F_NOT_STD;
	}
	bool getBool(char* value) {
		if (!strcmp(value, "true"))
			return true;
		return false;
	}

	int getLiteralIndex(IT::IdTable ittable, char* value, IT::IDDATATYPE type) 
	{
		for (int i = 0; i < ittable.size; i++)
		{
			if (ittable.table[i].idtype == IT::IDTYPE::L && ittable.table[i].iddatatype == type)
			{
				switch (type)
				{
				case IT::IDDATATYPE::INT: {
					int val;
					if ((value[0] == '-' && (value[1] == '0' && value[2] == 'x')) || (value[0] == '0' && value[1] == 'x'))
						val = strtol(value, NULL, 16);
					else
						val = strtol(value, NULL, 10);
					if (ittable.table[i].value.vint == val)
						return i;
					break;
				}
				case IT::IDDATATYPE::STR:
					char buf[STR_MAXSIZE];
					for (unsigned j = 1; j < strlen(value) - 1; j++) 
						buf[j - 1] = value[j];
					buf[strlen(value) - 2] = IN_CODE_NULL;
					if (strcmp((char*)ittable.table[i].value.vstr.str, buf) == 0)
						return i;
					break;
				case IT::IDDATATYPE::SYM:
					if (ittable.table[i].value.symbol == value[1])
						return i;
					break;
				case IT::IDDATATYPE::BOOL:
					if (ittable.table[i].value.vbool == getBool(value))
						return i;
				}
			}
		}
		return TI_NULLIDX;
	}

	IT::IDDATATYPE getType(char* curword, char* idtype)
	{
		if (idtype == nullptr)
			return IT::IDDATATYPE::UNDEF;
		if (!strcmp(TYPE_SYMBOL, idtype))
			return IT::IDDATATYPE::SYM; 
		if (!strcmp(TYPE_VOID, idtype))
			return IT::IDDATATYPE::PROC; 
		if (!strcmp(TYPE_STRING, idtype))
			return IT::IDDATATYPE::STR;  
		if (!strcmp(TYPE_INTEGER, idtype))
			return IT::IDDATATYPE::INT;
		if (!strcmp(TYPE_BOOL, idtype))
			return IT::IDDATATYPE::BOOL;
		
		if (isdigit(*curword) || *curword == LEX_MINUS)
			return IT::IDDATATYPE::INT;		
		else if (*curword == IN_CODE_QUOTE)
			return IT::IDDATATYPE::STR;	
		else if (*curword == IN_CODE_NOT_DOUBLE_QUOTE)
			return IT::IDDATATYPE::SYM;	
		else if (!strcmp(curword, "true") || !strcmp(curword, "false"))
			return IT::IDDATATYPE::BOOL;
		

		return IT::IDDATATYPE::UNDEF;	
	}

	char* getNextLiteralName()		
	{
		static int literalNumber = 1;
		char* buf = new char[SCOPED_ID_MAXSIZE];
		char num[3];
		strcpy_s(buf, ID_MAXSIZE, "L");
		_itoa_s(literalNumber++, num, 10);
		strcat(buf, num);

		return buf;
	}

	int getIndexInLT(LT::LexTable& lextable, int itTableIndex)					
	{
		if (itTableIndex == TI_NULLIDX)	
			return lextable.size;
		for (int i = 0; i < lextable.size; i++)
			if (itTableIndex == lextable.table[i].idxTI)
				return i;
		return TI_NULLIDX;
	}

	IT::Entry* getEntry(		
		Lex::LEX& tables,						
		char lex,								
		char* id,							
		char* idtype,							
		bool isParam,							
		bool isFunc,
		Log::LOG log,							
		int line,	
		int position,
		bool& lex_ok)						
	{
		IT::IDDATATYPE type = getType(id, idtype);
		int index = IT::isId(tables.idtable, id);	
		if (lex == LEX_LITERAL)
			index = getLiteralIndex(tables.idtable, id, type);
		if (index != TI_NULLIDX)
			return nullptr;					
		IT::Entry* itentry = new IT::Entry;
		itentry->iddatatype = type; 

		itentry->idxfirstLE = getIndexInLT(tables.lextable, index); 

		if (lex == LEX_LITERAL) 
		{
			bool int_ok = IT::SetValue(itentry, id);
			if (int_ok && itentry->iddatatype == IT::IDDATATYPE::INT)
			{
				char p[11];
				_itoa(itentry->value.vint, p, 10);
				index = getLiteralIndex(tables.idtable, p, type);
				if (index != TI_NULLIDX)
					return nullptr;
			}
			if (!int_ok)
			{
				Log::WriteError(log.stream, Error::GetError(313, line, position));
				Log::WriteError(NULL, Error::GetError(313, line, position));
				lex_ok = false;
			}
			if (itentry->iddatatype == IT::IDDATATYPE::STR && itentry->value.vstr.len == 0)
			{
				
				Log::WriteError(log.stream, Error::GetError(310, line, position));
				Log::WriteError(NULL, Error::GetError(310, line, position));
				lex_ok = false;
			}
			strcpy_s(itentry->id, getNextLiteralName());
			itentry->idtype = IT::IDTYPE::L;
		}
		else 
		{
			switch (type)
			{
			
			case IT::IDDATATYPE::STR:
				strcpy_s(itentry->value.vstr.str, "");
				itentry->value.vstr.len = TI_STR_DEFAULT;
				break;
			case IT::IDDATATYPE::INT:
				itentry->value.vint = TI_INT_DEFAULT;
				break;
			case IT::IDDATATYPE::SYM:
				itentry->value.symbol = TI_SYM_DEFAULT;
				break;
			case IT::IDDATATYPE::BOOL:
				itentry->value.vbool = false;
				break;
			}

			if (isFunc) 
			{
				switch (getStandFunction(id))
				{
				case IT::STDFNC::F_SQUERE:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = SQUERE_TYPE;
					itentry->value.params.count = SQUERE_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[SQUERE_PARAMS_CNT];
					for (int k = 0; k < SQUERE_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::SQUERE_PARAMS[k];
					break;
				}
				case IT::STDFNC::F_POW:
				{
					strncat(id, "er", ID_MAXSIZE);
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = POW_TYPE;
					itentry->value.params.count = POW_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[POW_PARAMS_CNT];
					for (int k = 0; k < POW_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::POW_PARAMS[k];
					break;
				}
				case IT::STDFNC::F_LENGTH:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = LENGTH_TYPE;
					itentry->value.params.count = LENGTH_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[LENGTH_PARAMS_CNT];
					for (int k = 0; k < LENGTH_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::LENGTH_PARAMS[k];
					break;
				}
				case IT::STDFNC::F_STRLEN:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = STRLEN_TYPE;
					itentry->value.params.count = STRLEN_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[STRLEN_PARAMS_CNT];
					for (int k = 0; k < STRLEN_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::STRLEN_PARAMS[k];
					break;
				}
				case IT::STDFNC::F_INT_TO_STR:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = INT_TO_STR_TYPE;
					itentry->value.params.count = INTTOSTR_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[INTTOSTR_PARAMS_CNT];
					for (int k = 0; k < INTTOSTR_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::INT_TO_STR_PARAMS[k];
					break;
				}
				
				case IT::STDFNC::F_ININT:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = ININT_TYPE;
					itentry->value.params.count = INPUT_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[INPUT_PARAMS_CNT];
					for (int k = 0; k < INPUT_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::COPY_PARAMS[k];
					break;
				}
				case IT::STDFNC::F_INSYM:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = INSYM_TYPE;
					itentry->value.params.count = INPUT_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[INPUT_PARAMS_CNT];
					for (int k = 0; k < INPUT_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::COPY_PARAMS[k];
					break;
				}
				case IT::STDFNC::F_INSTR:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = INSTR_TYPE;
					itentry->value.params.count = INPUT_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[INPUT_PARAMS_CNT];
					for (int k = 0; k < INPUT_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::COPY_PARAMS[k];
					break;
				}
				case IT::STDFNC::F_NOT_STD:
					itentry->idtype = IT::IDTYPE::F;
					break;

				}

			}
			else if (isParam)
				itentry->idtype = IT::IDTYPE::P;
			else
				itentry->idtype = IT::IDTYPE::V;

			if (!isFunc)
			{
				memset(itentry->id, '\0', SCOPED_ID_MAXSIZE);
				strncat(itentry->id, id, SCOPED_ID_MAXSIZE);
			}
			else
			{
				char temp[SCOPED_ID_MAXSIZE] = "";
				strncat(temp, id, ID_MAXSIZE);
				memset(itentry->id, '\0', SCOPED_ID_MAXSIZE);
				strncat(itentry->id, id, SCOPED_ID_MAXSIZE);
			}
		}

		// -------------------------------------------------------
		int i = tables.lextable.size; // индекс в ТЛ текущего ИД

		if (i > 1 && itentry->idtype == IT::IDTYPE::F && tables.lextable.table[i - 1].lexema != LEX_FUNCTION)
		{
			// в объявлении не указан тип функции
			Log::WriteError(log.stream, Error::GetError(303, line, position));
			Log::WriteError(NULL, Error::GetError(303, line, position));
			lex_ok = false;
		}
		if (itentry->iddatatype == IT::IDDATATYPE::UNDEF)
		{
			// невозможно определелить тип
			Log::WriteError(log.stream, Error::GetError(300, line, position));
			Log::WriteError(NULL, Error::GetError(300, line, position));
			lex_ok = false;
		}
		return itentry;
	}

	bool analyze(LEX& tables, In::IN& in, Log::LOG& log, Parm::PARM& parm) {

		static bool lex_ok = true;
		tables.lextable = LT::Create(LT_MAXSIZE);
		tables.idtable = IT::Create(MAXSIZE_TI);

		bool isParam = false, isFunc = false, isMain = false, isIf = false, isDo = false;
		int enterPoint = NULL;
		char curword[STR_MAXSIZE], nextword[STR_MAXSIZE];
		int curline;
		int position;
		int nesting = 0;
		std::vector <char*> scopes;		
		for (int i = 0; i < in.words->size; i++)
		{
			strcpy_s(curword, in.words[i].word);

			if (i < in.words->size - 1)
				strcpy_s(nextword, in.words[i + 1].word);

			curline = in.words[i].line;
			position = in.words[i].positions;
			isFunc = false;
			int idxTI = TI_NULLIDX;

			for (int j = 0; j < N_GRAPHS; j++) {
				FST::FST fst((unsigned char*)curword, graphs[j].graph);

				if (FST::execute(fst)) {
					char lexema = graphs[j].lexema;

					switch (lexema)
					{
					case LEX_MAIN:
						enterPoint++;
						break;
					case LEX_SEPARATORS:
					{
						switch (*curword) {
						case LEX_LEFTHESIS:		
						{
							if (tables.lextable.table[tables.lextable.size - 2].lexema == LEX_FUNCTION)
							{
								isParam = true;
								char* functionname = new char[ID_MAXSIZE];					
								char* scopename = getScopeName(tables, in.words[i - 1].word);
								if (scopename == nullptr)
									break;
								memset(functionname, '\0', ID_MAXSIZE);
								strncat(functionname, scopename, ID_MAXSIZE);
								scopes.push_back(functionname);
							}
							break;
						}
						case LEX_RIGHTTHESIS:
						{
							isParam = false;
							if ((*in.words[i - 1].word == LEX_LEFTHESIS && *in.words[i + 1].word == LEX_LEFTBRACE) 
								|| (i > 2 && (tables.lextable.table[tables.lextable.size - 2].lexema == LEX_ID_TYPE))
								&& !scopes.empty())
								scopes.pop_back();
							break;
						}
						case LEX_LEFTBRACE:		
						{
							if (isIf)
							{
								nesting++;
							}
							else
								isMain = true;
							char* blockname = new char[ID_MAXSIZE];
							char* scopename = getScopeName(tables, in.words[i - 1].word);
							if (scopename == nullptr)
								break;
							memset(blockname, '\0', ID_MAXSIZE);
							strncat(blockname, scopename, ID_MAXSIZE);
							scopes.push_back(blockname);
							break;
						}
						case LEX_BRACELET:		
						{
							if (isIf)
							{
								nesting--;
								if (nesting == 0)
								{
									isIf = false;
								}
							}
							else
								isMain = false;
							if (!scopes.empty())
								scopes.pop_back();
							break;
						}

						}
						lexema = *curword;
						break;
					}
					case LEX_DO:
					{
						isDo = true;
					}
					case LEX_ISTRUE:
					case LEX_ISFALSE:
					{
						isIf = true;
						break;
					}
					case LEX_WHILE:
					{
						if (isDo)
						{
							isDo = false;
							break;
						}
						isIf = true;
						break;
					}
					case LEX_ID:
					case LEX_STDFUNC:
					case LEX_LITERAL:
					{
						char id[STR_MAXSIZE] = "";
						idxTI = NULLDX_TI;								
						if (*nextword == LEX_LEFTHESIS || IT::isId(tables.idtable, curword) != TI_NULLIDX)
						{
							isFunc = true;
							if (getStandFunction(curword) == IT::STDFNC::F_NOT_STD)
								strncat(id, "_", ID_MAXSIZE);
						}
						char* idtype = (isFunc && i > 1) ?						
							in.words[i - 2].word : in.words[i - 1].word;	
						if (i == 0)
							idtype = nullptr;
						if (isIf && !isFunc)
						{
							if (tables.lextable.table[tables.lextable.size - 1].lexema == LEX_ID_TYPE)
							{
								strncpy_s(id, scopes.back(), ID_MAXSIZE);
							}
							else {
								int i = scopes.size() - 1;
								for (i; i >= 0; i--)
								{
									char temp[STR_MAXSIZE] = "";
									strncpy_s(temp, scopes[i], ID_MAXSIZE);
									strncat(temp, curword, ID_MAXSIZE);
									int index = IT::isId(tables.idtable, temp);
									if (index != TI_NULLIDX)
									{
										strncpy_s(id, scopes[i], ID_MAXSIZE);
										break;
									}
								}
								if (i == -1)
								{
									strncpy_s(id, scopes.back(), ID_MAXSIZE);
								}
							}
						}
						else if (!isFunc && !scopes.empty())
							strncpy_s(id, scopes.back(), ID_MAXSIZE);
						strncat(id, curword, ID_MAXSIZE);
						if (isLiteral(curword))
							strcpy_s(id, curword); // литерал заменяется своим значением
						IT::Entry* itentry = getEntry(tables, lexema, id, idtype, isParam, isFunc, log, curline, position, lex_ok);

						if (itentry != nullptr) 
						{
							if (isFunc && getStandFunction(curword) == IT::STDFNC::F_NOT_STD)
							{
								itentry->value.params.count = NULL;
								itentry->value.params.types = new IT::IDDATATYPE[MAX_PARAMS_COUNT];
								for (int k = i; in.words[k].word[0] != LEX_RIGHTTHESIS; k++)
								{
									if (k == in.words->size || in.words[k].word[0] == LEX_SEPARATOR)
										break;
									if (ISTYPE(in.words[k].word))
									{
										if (itentry->value.params.count >= MAX_PARAMS_COUNT)
										{
											Log::WriteError(log.stream, Error::GetError(306, curline, position));
											Log::WriteError(NULL, Error::GetError(306, curline, position));
											lex_ok = false;
											break;
										}
										itentry->value.params.types[itentry->value.params.count++] = getType(NULL, in.words[k].word);
									}
								}
							}
							IT::Add(tables.idtable, *itentry);
							idxTI = tables.idtable.size - 1;
						}
						else 
						{
							int i = tables.lextable.size - 1; 
							if (i > 0 && tables.lextable.table[i - 1].lexema == LEX_FUNCTION || tables.lextable.table[i].lexema == LEX_FUNCTION
								|| tables.lextable.table[i - 1].lexema == LEX_ID_TYPE || tables.lextable.table[i].lexema == LEX_ID_TYPE
								|| tables.lextable.table[i - 1].lexema == LEX_VOID || tables.lextable.table[i].lexema == LEX_VOID)
							{
								Log::WriteError(log.stream, Error::GetError(305, curline, position));
								Log::WriteError(NULL, Error::GetError(305, curline, position));
								lex_ok = false;
							}
							idxTI = IT::isId(tables.idtable, id);	
							if (lexema == LEX_LITERAL)
							{
								idxTI = getLiteralIndex(tables.idtable, curword, getType(id, in.words[i - 1].word)); 
								if (idxTI == -1)
								{
									int temp;
									if ((getType(id, in.words[i - 1].word)) == IT::IDDATATYPE::INT)
									{
										if ((curword[0] == '-' && (curword[1] == '0' && curword[2] == 'x')) || (curword[0] == '0' && curword[1] == 'x'))
											temp = strtol(curword, NULL, 16);
									}
									char p[10];
									_itoa(temp, p, 10);
									idxTI = getLiteralIndex(tables.idtable, p, getType(id, in.words[i - 1].word));
								}
							}
						}
					}
					break;
					}
					LT::Entry* ltentry = new LT::Entry(lexema, curline, position, idxTI);
					LT::Add(tables.lextable, *ltentry);
					break;
				}
				else if (j == N_GRAPHS - 1) 
				{
					Log::WriteError(log.stream, Error::GetError(201, curline, position));
					Log::WriteError(NULL, Error::GetError(201, curline, position));
					lex_ok = false;
				}
			}
		}
		if (enterPoint == NULL) 
		{
			Log::WriteError(log.stream, Error::GetError(301));
			Log::WriteError(NULL, Error::GetError(301));
			lex_ok = false;
		}
		if (enterPoint > 1) 
		{
			Log::WriteError(log.stream, Error::GetError(302));
			Log::WriteError(NULL, Error::GetError(302));
			lex_ok = false;
		}
		if (isMain)
		{
			Log::WriteError(log.stream, Error::GetError(317));
			Log::WriteError(NULL, Error::GetError(317));
			lex_ok = false;
		}
		return lex_ok;
	}
}