#include "stdafx.h"
#include "Generator.h"
#include "Semantic.h"
#include <string>

using namespace std;
namespace Gener
{
	int countUNDEF = 0;
	enum Logic {
		And = 0,
		Or,
		Less,
		Great,
		Equal,
		NotEqual,
		EqLess,
		EqGreat,
		Not
	};
	stack<string> loopstackIf;
	stack<string> loopstackWhile;
	vector<vector<bool>> inIf(1, vector<bool>(3, false));
	bool CodeGeneration(Lex::LEX& lex, Parm::PARM& parm, Log::LOG& log)
	{
		stack<IT::Entry> temp;
		bool gen_ok;

		ofstream ofile(parm.out);

		ofile << BEGIN;
		ofile << EXTERN;
		ofile << ".const\n null_division BYTE 'ERROR: DIVISION BY ZERO', 0\n overflow BYTE 'ERROR: VARIABLE OVERFLOW', 0 \n";

		int whileNum = 0, ifNum = 0;
		int count = 0;
		for (int i = 0; i < lex.idtable.size; i++)
		{
			if (lex.idtable.table[i].idtype == IT::L)
			{
				ofile << "\t" << lex.idtable.table[i].id;
				if (lex.idtable.table[i].iddatatype == IT::SYM)
				{
					ofile << " BYTE \'" << lex.idtable.table[i].value.symbol << "\', 0\n";
				}
				if (lex.idtable.table[i].iddatatype == IT::STR)
				{
					ofile << " BYTE \'" << lex.idtable.table[i].value.vstr.str << "\', 0\n";
				}
				if (lex.idtable.table[i].iddatatype == IT::INT)
				{
					ofile << " SDWORD " << lex.idtable.table[i].value.vint << endl;
				}
				if (lex.idtable.table[i].iddatatype == IT::BOOL)
				{
					ofile << " BYTE " << lex.idtable.table[i].value.vbool << endl;
				}
			}
		}
		ofile << ".data\n";
		for (int i = 0; i < lex.idtable.size; i++)
		{
			if (lex.idtable.table[i].idtype == IT::IDTYPE::V)
			{
				ofile << "\t" << lex.idtable.table[i].id;
				if (lex.idtable.table[i].iddatatype == IT::SYM)
				{
					ofile << " DWORD ?\n";
				}
				if (lex.idtable.table[i].iddatatype == IT::STR)
				{
					ofile << " DWORD ?\n";
				}
				if (lex.idtable.table[i].iddatatype == IT::INT)
				{
					ofile << " SDWORD 0\n";
				}
				if (lex.idtable.table[i].iddatatype == IT::BOOL)
				{
					ofile << " DWORD 0\n" << endl;
				}
			}
		}


		string func_name = "";

		bool flag_main = false,
			flag_return = false;


		ofile << "\n.code\n\n";
		for (int i = 0; i < lex.lextable.size; i++)
		{
			switch (lex.lextable.table[i].lexema)
			{
			case LEX_FUNCTION:
			{
				i++;
				flag_return = false;
				ofile << (func_name = ITENTRY(i).id) << " PROC ";
				int j = i + 2;
				while (LEXEMA(j) != LEX_RIGHTTHESIS)
				{
					if (lex.lextable.table[j].lexema == LEX_ID)
					{
						ofile << lex.idtable.table[lex.lextable.table[j].idxTI].id << " : ";
						if (ITENTRY(j).iddatatype == IT::IDDATATYPE::INT)
						{
							ofile << " SDWORD ";
						}
						else if (ITENTRY(j).iddatatype == IT::IDDATATYPE::BOOL){
							ofile << " DWORD ";
						}
						else {
							ofile << " DWORD ";
						}
					}
					if (LEXEMA(j) == LEX_COMMA)
					{
						ofile << ", ";
					}
					j++;
				}
				ofile << endl;
				ofile << "\tpush ebx" << endl;
				ofile << "\tpush edx" << endl;
				ofile << "\tpush ecx" << endl;
				break;
			}
			case LEX_MAIN:
			{
				flag_main = true;
				ofile << "main PROC\n";
				break;
			}
			case LEX_RETURN:
			{
				if (flag_main)
				{
					Log::WriteError(log.stream, Error::GetError(320, lex.lextable.table[i].sn, 0));
					ofile.close();
					return false;
				}
				if (LEXEMA(i + 1) != LEX_SEPARATOR)
				{
					i = Operation(i + 1, lex, ofile, Semantic::Equals);
					ofile << "\tpop eax\n";
					ofile << "\njmp return" << func_name << "\n\n";
				}
				break;
			}
			case LEX_BRACELET:
			{
				if (!inIf.back()[0] && !flag_main)
				{
					ofile << "\nreturn" << func_name << ":\n";
					ofile << "\n\tpop ecx" << endl;
					ofile << "\tpop edx" << endl;
					ofile << "\tpop ebx" << endl;
					ofile << "\tret\n";
					ofile << "\nSOMETHINGWRONG:"\
						"\npush offset null_division"\
						"\ncall outstrline\n"\
						"call system_pause"\
						"\npush -1"\
						"\ncall ExitProcess\n"\
						"\nEXIT_OVERFLOW:"\
						"\npush offset overflow"\
						"\ncall outstrline\n"\
						"call system_pause"\
						"\npush -2"\
						"\ncall ExitProcess\n";
					ofile << func_name + " ENDP\n\n\n";
				}
				if (inIf.back()[0] && inIf.back()[1])
				{
					ofile << "\tjmp " << loopstackIf.top() << "do\n\n";
					ofile << loopstackIf.top() << "End:\n"; 
					loopstackIf.pop();
					inIf.back().erase(inIf.back().begin(), inIf.back().end());
					inIf.pop_back();
				}
				if (inIf.back()[0] && !inIf.back()[1])
				{
					ofile << "\n\tjmp " << loopstackIf.top() << "End\n\n";
				}
				if (LEXEMA(i + 1) != LEX_ISTRUE && LEXEMA(i + 1) != LEX_ISFALSE && (!inIf.back()[1] && inIf.size() != 1))
				{
					ofile << loopstackIf.top() << "End:\n";
					loopstackIf.pop();
					inIf.back().erase(inIf.back().begin(), inIf.back().end());
					inIf.pop_back();
				}
				break;
			}
			case LEX_EQUAL:
			{
				int result_position = i - 1;
				string name = ITENTRY(result_position).id + to_string(++count);
				i = Operation(i + 1, lex, ofile, Semantic::Equals, name);
				ofile << "\tpop " << ITENTRY(result_position).id << endl << endl;

				break;
			}
			case LEX_STDFUNC:
			case LEX_ID:
			{
				if (ITENTRY(i).idtype == IT::IDTYPE::F || ITENTRY(i).idtype == IT::IDTYPE::S)
				{
					i = Operation(i + 1, lex, ofile, Semantic::Fun)-1;
					ofile << "\t\tcall " << ITENTRY(i).id << endl;
					ofile << "\tpush eax\n";
					i++;
					break;
				}
				break;
			}
			case LEX_NEWLINE:
			{
				i++;
				while (LEXEMA(i + 1) != LEX_SEPARATOR)
				{
					if (lex.lextable.table[i].idxTI != TI_NULLIDX)
					{
						if (ITENTRY(i).idtype == IT::IDTYPE::F || ITENTRY(i).idtype == IT::IDTYPE::S)
						{
							int positionsfun = i;
							i = Operation(i + 1, lex, ofile, Semantic::Fun) - 1;
							if (LEXEMA(i + 2) == LEX_SEPARATOR)
							{
								i = positionsfun;
								break;
							}
							ofile << "\t\tcall " << ITENTRY(i).id << endl;
							ofile << "\tpush eax\n";
							i++;
							switch (ITENTRY(positionsfun).iddatatype)
							{
							case IT::IDDATATYPE::INT:
								ofile << "\n\t\tcall outnum\n";
								break;
							case IT::IDDATATYPE::BOOL:
								ofile << "\n\t\tcall outbool\n";
								break;
							case IT::IDDATATYPE::SYM:
							case IT::IDDATATYPE::STR:
								ofile << "\n\t\tcall outstr\n";
								break;
							}
						}
						else
						{
							switch (ITENTRY(i).iddatatype)
							{
							case IT::IDDATATYPE::INT:
								ofile << "\n\tpush " << ITENTRY(i).id << "\n\tcall outnum\n";
								break;
							case IT::IDDATATYPE::SYM:
							case IT::IDDATATYPE::STR:
								if (ITENTRY(i).idtype == IT::IDTYPE::L)
									ofile << "\n\tpush offset " << ITENTRY(i).id << "\n\tcall outstr\n";
								else
									ofile << "\n\tpush " << ITENTRY(i).id << "\n\tcall outstr\n";
								break;
							case IT::IDDATATYPE::BOOL:
								if (ITENTRY(i).idtype == IT::IDTYPE::L)
								{
									ofile << "\txor eax, 0FFFFFFFFh\n";
									ofile << "\tmov al, " << ITENTRY(i).id << endl;
									ofile << "\tpush eax\n\tcall outbool\n";
								}
								else
									ofile << "\n\tpush " << ITENTRY(i).id << "\n\tcall outbool\n";
								break;
							}
						}
					}
					i++;
				}
				if (ITENTRY(i).idtype == IT::IDTYPE::F || ITENTRY(i).idtype == IT::IDTYPE::S)
				{
					int positionsfun = i;
					i = Operation(i + 1, lex, ofile, Semantic::Fun) - 1;
					ofile << "\t\tcall " << ITENTRY(i).id << endl;
					ofile << "\tpush eax\n";
					i++;
					switch (ITENTRY(positionsfun).iddatatype)
					{
					case IT::IDDATATYPE::INT:
						ofile << "\n\t\tcall outnumline\n";
						break;
					case IT::IDDATATYPE::BOOL:
						ofile << "\n\t\tcall outboolline\n";
						break;
					case IT::IDDATATYPE::SYM:
					case IT::IDDATATYPE::STR:
						ofile << "\n\t\tcall outstrline\n";
						break;
					}
				}
				else
				{
					switch (ITENTRY(i).iddatatype)
					{
					case IT::IDDATATYPE::INT:
						ofile << "\n\tpush " << ITENTRY(i).id << "\n\tcall outnumline\n";
						break;
					case IT::IDDATATYPE::SYM:
					case IT::IDDATATYPE::STR:
						if (ITENTRY(i).idtype == IT::IDTYPE::L)
							ofile << "\n\tpush offset " << ITENTRY(i).id << "\n\tcall outstrline\n";
						else
							ofile << "\n\tpush " << ITENTRY(i).id << "\n\tcall outstrline\n";
						break;
					case IT::IDDATATYPE::BOOL:
						if (ITENTRY(i).idtype == IT::IDTYPE::L)
						{
							ofile << "\txor eax, eax\n";
							ofile << "\tmov al, " << ITENTRY(i).id << endl;
							ofile << "\tpush eax\n\tcall outboolline\n";
						}
						else
							ofile << "\n\tpush " << ITENTRY(i).id << "\n\tcall outboolline\n";
						break;
					}
				}
				i++;
				break;
			}
			case LEX_WRITE:
			{
				i++;
				while (LEXEMA(i) != LEX_SEPARATOR)
				{
					if (lex.lextable.table[i].idxTI != TI_NULLIDX)
					{
						if (ITENTRY(i).idtype == IT::IDTYPE::F || ITENTRY(i).idtype == IT::IDTYPE::S)
						{
							int positionsfun = i;
							i = Operation(i + 1, lex, ofile, Semantic::Fun) - 1;
							ofile << "\t\tcall " << ITENTRY(i).id << endl;
							ofile << "\tpush eax\n";
							i++;
							switch (ITENTRY(positionsfun).iddatatype)
							{
							case IT::IDDATATYPE::INT:
								ofile << "\n\t\tcall outnum\n";
								break;
							case IT::IDDATATYPE::BOOL:
								ofile << "\n\t\tcall outbool\n";
								break;
							case IT::IDDATATYPE::SYM:
							case IT::IDDATATYPE::STR:
								ofile << "\n\t\tcall outstr\n";
								break;
							}
						}
						else
						{
							switch (ITENTRY(i).iddatatype)
							{
							case IT::IDDATATYPE::INT:
								ofile << "\n\tpush " << ITENTRY(i).id << "\n\tcall outnum\n";
								break;
							case IT::IDDATATYPE::SYM:
							case IT::IDDATATYPE::STR:
								if (ITENTRY(i).idtype == IT::IDTYPE::L)
									ofile << "\n\tpush offset " << ITENTRY(i).id << "\n\tcall outstr\n";
								else
									ofile << "\n\tpush " << ITENTRY(i).id << "\n\tcall outstr\n";
								break;
							case IT::IDDATATYPE::BOOL:
								if (ITENTRY(i).idtype == IT::IDTYPE::L)
								{
									ofile << "\txor eax, eax\n";
									ofile << "\tmov al, " << ITENTRY(i).id << endl;
									ofile << "\tpush eax\n\tcall outbool\n";
								}
								else
									ofile << "\n\tpush " << ITENTRY(i).id << "\n\tcall outbool\n";
								break;
							}
						}
					}
					i++;
				}
				break;
			}
			case LEX_WHILE:
			{
				if (!inIf.back()[2])
				{
					inIf.emplace_back();
					inIf.back().resize(3, true);
					inIf.back()[2] = false;
					whileNum++;
					loopstackIf.push("while" + to_string(whileNum));
					ofile << "\n" << loopstackIf.top() << "do:\n";
				}
				i += 2;
				if (LEXEMA(i) == LEX_LITERAL)
				{
					if (ITENTRY(i).iddatatype == IT::IDDATATYPE::STR || ITENTRY(i).iddatatype == IT::IDDATATYPE::SYM)
					{
						ofile << "\tpush offset " << ITENTRY(i).id << endl;
					}
					else if (ITENTRY(i).iddatatype == IT::IDDATATYPE::BOOL)
					{
						ofile << "\txor eax, eax\n";
						ofile << "\tmov al, " << ITENTRY(i).id << endl;
						ofile << "\tpush eax\n";
					}
					else
					{
						ofile << "\tpush " << ITENTRY(i).id << endl;
					}
				}
				else
				{
					if (ITENTRY(i).idtype != IT::IDTYPE::F)
						ofile << "\tpush " << ITENTRY(i).id << endl;
				}
				if (LEXEMA(i + 1) != LEX_RIGHTTHESIS)
				{
					i = Operation(i + 1, lex, ofile, Semantic::If, loopstackIf.top());
				}
				ofile << "\tpop eax" << endl;
				ofile << "\tcmp eax, 0" << endl;

				if (!inIf.back()[2])
				{
					ofile << "\tje " << loopstackIf.top() << "End" << "\n";
				}
				else
				{
					ofile << "\tjne " << loopstackIf.top() << "do" << "\n";
				}
				break;
			}
			case LEX_DO:
			{
				inIf.emplace_back();
				inIf.back().resize(3, true);
				whileNum++;
				loopstackIf.push("while" + to_string(whileNum));
				ofile << "\n" << loopstackIf.top() << "do:\n";
				break;
			}
			case LEX_IF:
			{
				inIf.emplace_back();
				inIf.back().resize(3, false);
				ifNum++;
				loopstackIf.push("if" + to_string(ifNum));
				ofile << "\n" << loopstackIf.top() << "do:\n";
				i += 2;
				if (LEXEMA(i) == LEX_LITERAL)
				{
					if (ITENTRY(i).iddatatype == IT::IDDATATYPE::STR || ITENTRY(i).iddatatype == IT::IDDATATYPE::SYM)
					{
						ofile << "\tpush offset " << ITENTRY(i).id << endl;
					}
					else if (ITENTRY(i).iddatatype == IT::IDDATATYPE::BOOL)
					{
						ofile << "\txor eax, eax\n";
						ofile << "\tmov al, " << ITENTRY(i).id << endl;
						ofile << "\tpush eax\n";
					}
					else
					{
						ofile << "\tpush " << ITENTRY(i).id << endl;
					}
				}
				else
				{
					if (ITENTRY(i).idtype != IT::IDTYPE::F)
						ofile << "\tpush " << ITENTRY(i).id << endl;
				}
				if (LEXEMA(i + 1) != LEX_RIGHTTHESIS)
				{
					i = Operation(i + 1, lex, ofile, Semantic::If, loopstackIf.top());
				}
				ofile << "\tpop eax" << endl;
				ofile << "\tcmp eax, 0" << endl;
				if (findTrueIf(i + 1, lex))
					ofile << "\tjne " << loopstackIf.top() << "true" << endl;
				else
					ofile << "\tjne " << loopstackIf.top() << "End" << endl;
				if (findFalseIf(i + 1, lex))
					ofile << "\tje " << loopstackIf.top() << "false" << endl;
				else
					ofile << "\tje " << loopstackIf.top() << "End" << endl;
				break;
			}
			case LEX_ISFALSE:
			{
				ofile << loopstackIf.top() << "false:\n";
				inIf.back()[0] = true;
				break;
			}
			case LEX_ISTRUE:
			{
				ofile << loopstackIf.top() << "true:\n";
				inIf.back()[0] = true;
				break;
			}
			}
		}
		ofile << END;
		ofile.close();
		return true;
	};

	bool findFalseIf(int i, Lex::LEX& lex) {
		bool isTrue = false;
		while (LEXEMA(i) != LEX_ISFALSE)
		{
			if (LEXEMA(i) == LEX_ISTRUE)
			{
				isTrue = true;
			}
			if (LEXEMA(i) == LEX_ISFALSE)
			{
				break;
			}
			if ((LEXEMA(i) == LEX_BRACELET && isTrue && LEXEMA(i + 1) != LEX_ISFALSE))
			{
				return false;
			}
			i++;
		}
		return true;
	}
	bool findTrueIf(int i, Lex::LEX& lex) {
		bool isFalse = false;
		while (LEXEMA(i) != LEX_ISTRUE)
		{
			if (LEXEMA(i) == LEX_ISFALSE)
			{
				isFalse = true;
			}
			if (LEXEMA(i) == LEX_ISTRUE)
			{
				break;
			}
			if ((LEXEMA(i) == LEX_BRACELET && isFalse && LEXEMA(i + 1) != LEX_ISTRUE))
			{
				return false;
			}
			i++;
		}
		return true;
	}

	int Operation(int i, Lex::LEX& lex, ofstream& ofile, Semantic::Is is, string Nameblock) {
		if (Nameblock == "undef")
		{
			Nameblock = "undef" + to_string(++countUNDEF);
		}
		int NumLogic[] = { 0,0,0,0,0,0,0,0,0 }, NumCompare = 0;
		int infun = 0;
		while ((LEXEMA(i) != LEX_SEPARATOR && Semantic::Is::Equals == is)
			|| ((LEXEMA(i-1) != LEX_SUBST || infun != 0) && Semantic::Is::Fun == is)
			|| (LEXEMA(i) != LEX_RIGHTTHESIS && Semantic::Is::If == is))
		{
			switch (LEXEMA(i))
			{
			case LEX_SUBST:
			{
				if (infun != 0)
				{
					infun--;
					ofile << "\t\tcall " << ITENTRY(i).id << endl;
					ofile << "\tpush eax\n";
					i++;
				}
				break;
			}
			case LEX_LITERAL:
			{
				if (ITENTRY(i).iddatatype == IT::IDDATATYPE::STR || ITENTRY(i).iddatatype == IT::IDDATATYPE::SYM )
				{
					ofile << "\tpush offset " << ITENTRY(i).id << endl;
				}
				else if(ITENTRY(i).iddatatype == IT::IDDATATYPE::BOOL)
				{
					ofile << "\txor eax, eax\n";
					ofile << "\tmov al, " << ITENTRY(i).id << endl;
					ofile << "\tpush eax\n";
				}
				else
				{
					ofile << "\tpush " << ITENTRY(i).id << endl;
				}
				break;
			}

			case LEX_STDFUNC:
			case LEX_ID:
			{
				if (ITENTRY(i).idtype == IT::IDTYPE::F || ITENTRY(i).idtype == IT::IDTYPE::S)
				{
					infun++;
					break;
				}
				ofile << "\tpush " << ITENTRY(i).id << endl;
				break;
			}
			case LEX_STAR:
			{
				ofile << "\n\tpop eax\n\tpop ebx\n";
				ofile << "\timul ebx\n\tjo EXIT_OVERFLOW\n\tpush eax\n";

				break;
			}
			case LEX_PLUS:
			{
				ofile << "\n\tpop eax\n\tpop ebx\n";
				ofile << "\tadd eax, ebx\n\tjo EXIT_OVERFLOW\n\tpush eax\n";

				break;
			}
			case LEX_MINUS:
			{
				ofile << "\n\tpop ebx\n\tpop eax\n";
				ofile << "\tsub eax, ebx\n\tpush eax\n";

				break;
			}
			case LEX_DIRSLASH:
			{
				ofile << "\n\tpop ebx\n\tpop eax\n";
				ofile << "\tcmp ebx,0\n"\
					"\tje SOMETHINGWRONG\n";
				ofile << "\tcdq\n";
				ofile << "\tidiv ebx\n\tpush eax\n";
				break;
			}
			case LEX_PROCENT:
			{

				ofile << "\n\tpop ebx\n\tpop eax\n";
				ofile << "\tcmp ebx,0\n"\
					"\tje SOMETHINGWRONG\n";
				ofile << "\tcdq\n";
				ofile << "\tidiv ebx\n\tpush edx\n";
				break;
			}
			case LEX_INVERSION_SIGN:
			{
				ofile << "\n\tpop eax\n";
				ofile << "\tneg eax\n";
				ofile << "\tpush eax\n";
				break;
			}
			case LEX_AND:
			{
				ofile << "\n\tpop ebx\n\tpop eax\n";
				ofile << "\tand eax, ebx\n";
				ofile << "\tpush eax\n";
				break;
			}
			case LEX_OR:
			{
				ofile << "\n\tpop ebx\n\tpop eax\n";
				ofile << "\tor eax, ebx\n";
				ofile << "\tpush eax\n";
				break;
			}
			case LEX_XOR:
			{
				ofile << "\n\tpop ebx\n\tpop eax\n";
				ofile << "\txor eax, ebx\n";
				ofile << "\tpush eax\n";
				break;
			}
			case LEX_NOT:
			{
				ofile << "\n\tpop eax\n";
				ofile << "\tnot eax\n";
				ofile << "\tpush eax\n";
				break;
			}
			case LEX_LOGIC_AND:
			{
				NumLogic[And]++;
				ofile << "\n\tpop eax\n\tpop ebx\n";
				ofile << "\tcmp eax, 0\n";
				ofile << "\tje " << Nameblock << "And" << NumLogic[And] << "False\n";
				ofile << "\tcmp ebx, 0\n";
				ofile << "\tje " << Nameblock << "And" << NumLogic[And] << "False\n";
				ofile << "\tmov eax, 1\n";
				ofile << "\tpush eax\n";
				ofile << "\tjmp " << Nameblock << "And" << NumLogic[And] << "End\n\n";
				ofile << Nameblock << "And" << NumLogic[And] << "False:\n";
				ofile << "\tmov eax, 0\n";
				ofile << "\tpush eax\n\n";
				ofile << Nameblock << "And" << NumLogic[And] << "End:\n";
				break;
			}
			case LEX_LOGIC_OR:
			{
				NumLogic[Or]++;
				ofile << "\n\tpop eax\n\tpop ebx\n";
				ofile << "\tcmp eax, 0\n";
				ofile << "\tjne " << Nameblock << "Or" << NumLogic[Or] << "True\n";
				ofile << "\tcmp ebx, 0\n";
				ofile << "\tjne " << Nameblock << "Or" << NumLogic[Or] << "True\n";
				ofile << "\tmov eax, 0\n";
				ofile << "\tpush eax\n";
				ofile << "\tjmp " << Nameblock << "Or" << NumLogic[Or] << "End\n\n";
				ofile << Nameblock << "Or" << NumLogic[Or] << "True:\n";
				ofile << "\tmov eax, 1\n";
				ofile << "\tpush eax\n\n";
				ofile << Nameblock << "Or" << NumLogic[Or] << "End:\n";
				break;
			}
			case LEX_LOGIC_EQUALS:
			{
				NumLogic[Equal]++;
				ofile << "\n\tpop eax\n\tpop ebx\n";
				int j, p;
				if (lex.lextable.table[i - (j = getIndexType(i - 1, lex))].idxTI != TI_NULLIDX
					&& lex.lextable.table[i - (p = getIndexType(i - (j + 1), lex)) - j].idxTI != TI_NULLIDX)
				{
					if (ITENTRY(i - j).iddatatype == IT::IDDATATYPE::STR)
					{
						NumCompare++;
						if (LEXEMA(i - j) == LEX_LITERAL)
						{
							ofile << "\n\tmov ecx, " << ITENTRY(i - j).value.vstr.len << endl;
						}
						else
						{
							ofile << "\n\tpush eax\n";
							ofile << "\tmov edi, eax\n";
							ofile << "\tmov ecx, 0FFFFFFFFh\n";
							ofile << "\tmov al, 0\n";
							ofile << "\trepne scasb\n";
							ofile << "\tnot ecx\n";
							ofile << "\tdec ecx\n";
							ofile << "\tpop eax\n";
						}
						if (LEXEMA(i - p - j) == LEX_LITERAL)
						{
							ofile << "\tcmp ecx, " << ITENTRY(i - p - j).value.vstr.len << endl;
						}
						else
						{
							ofile << "\n\tpush eax\n";
							ofile << "\tpush ebx\n";
							ofile << "\tmov edi, ebx\n";
							ofile << "\tmov ebx, ecx\n";
							ofile << "\tmov ecx, 0FFFFFFFFh\n";
							ofile << "\tmov al, 0\n";
							ofile << "\trepne scasb\n";
							ofile << "\tnot ecx\n";
							ofile << "\tdec ecx\n";
							ofile << "\tcmp ecx, ebx\n";
							ofile << "\tpop ebx\n";
							ofile << "\tpop eax\n\n";
						}
						ofile << "\tje " << Nameblock << "Compare" << NumCompare << "\n";
						ofile << "\tmov eax, 0\n";
						ofile << "\tpush eax\n";
						ofile << "\tjmp " << Nameblock << "Equal" << NumLogic[Equal] << "End\n\n";
						ofile << Nameblock << "Compare" << NumCompare << ":\n";
						ofile << "\tmov esi, eax\n";
						ofile << "\tmov edi, ebx\n";
						ofile << "\trepe cmpsb\n";
						ofile << "\tjz " << Nameblock << "Equal" << NumLogic[Equal] << "True\n";

					}
					else if (ITENTRY(i - j).iddatatype == IT::IDDATATYPE::SYM)
					{

						ofile << "\tmov esi, eax\n";
						ofile << "\tmov edi, ebx\n";
						ofile << "\tmov al, BYTE PTR [esi]" << "\n";
						ofile << "\tmov bl, BYTE PTR [edi]" << "\n";
						ofile << "\tcmp al, bl\n";
						ofile << "\tje " << Nameblock << "Equal" << NumLogic[Equal] << "True\n";

					}
					else
					{
						ofile << "\tcmp eax, ebx\n";
						ofile << "\tje " << Nameblock << "Equal" << NumLogic[Equal] << "True\n";
					}
				}
				else
				{
					ofile << "\tcmp eax, ebx\n";
					ofile << "\tje " << Nameblock << "Equal" << NumLogic[Equal] << "True\n";

				}
				ofile << "\tmov eax, 0\n";
				ofile << "\tpush eax\n";
				ofile << "\tjmp " << Nameblock << "Equal" << NumLogic[Equal] << "End\n\n";
				ofile << Nameblock << "Equal" << NumLogic[Equal] << "True:\n";
				ofile << "\tmov eax, 1\n";
				ofile << "\tpush eax\n\n";
				ofile << Nameblock << "Equal" << NumLogic[Equal] << "End:\n";
				break;
			}
			case LEX_LOGIC_NOT_EQUALS:
			{
				NumLogic[NotEqual]++;
				ofile << "\n\tpop eax\n\tpop ebx\n";
				int j, p;
				if (lex.lextable.table[i - (j = getIndexType(i - 1, lex))].idxTI != TI_NULLIDX
					&& lex.lextable.table[i - (p = getIndexType(j - 1, lex)) - j].idxTI != TI_NULLIDX)
				{
					if (ITENTRY(i - j).iddatatype == IT::IDDATATYPE::STR)
					{
						NumCompare++;
						if (LEXEMA(i - j) == LEX_LITERAL)
						{
							ofile << "\n\tmov ecx, " << ITENTRY(i - j).value.vstr.len << endl;
						}
						else
						{
							ofile << "\n\tpush eax\n";
							ofile << "\tmov edi, eax\n";
							ofile << "\tmov ecx, 0FFFFFFFFh\n";
							ofile << "\tmov al, 0\n";
							ofile << "\trepne scasb\n";
							ofile << "\tnot ecx\n";
							ofile << "\tdec ecx\n";
							ofile << "\tpop eax\n\n";
						}
						if (LEXEMA(i - p - j) == LEX_LITERAL)
						{
							ofile << "\tcmp ecx, " << ITENTRY(i - p - j).value.vstr.len << endl;
						}
						else
						{
							ofile << "\tpush eax\n";
							ofile << "\tpush ebx\n";
							ofile << "\tmov edi, ebx\n";
							ofile << "\tmov ebx, ecx\n";
							ofile << "\tmov ecx, 0FFFFFFFFh\n";
							ofile << "\tmov al, 0\n";
							ofile << "\trepne scasb\n";
							ofile << "\tnot ecx\n";
							ofile << "\tdec ecx\n";
							ofile << "\tcmp ecx, ebx\n";
							ofile << "\tpop ebx\n";
							ofile << "\tpop eax\n";
						}
						ofile << "\tje " << Nameblock << "Compare" << NumCompare << "\n";
						ofile << "\tjmp " << Nameblock << "NotEqual" << NumLogic[NotEqual] << "True\n\n";
						ofile << Nameblock << "Compare" << NumCompare << ":\n";
						ofile << "\tmov esi, eax\n";
						ofile << "\tmov edi, ebx\n";
						ofile << "\trepe cmpsb\n";
						ofile << "\tjnz " << Nameblock << "NotEqual" << NumLogic[NotEqual] << "True\n";

					}
					else if (ITENTRY(i - j).iddatatype == IT::IDDATATYPE::SYM)
					{

						ofile << "\tmov esi, eax\n";
						ofile << "\tmov edi, ebx\n";
						ofile << "\tmov al, BYTE PTR [esi]" << "\n";
						ofile << "\tmov bl, BYTE PTR [edi]" << "\n";
						ofile << "\tcmp al, bl\n";
						ofile << "\tjne " << Nameblock << "NotEqual" << NumLogic[NotEqual] << "True\n";

					}
					else
					{
						ofile << "\tcmp eax, ebx\n";
						ofile << "\tjne " << Nameblock << "NotEqual" << NumLogic[NotEqual] << "True\n";
					}
				}
				else
				{
					ofile << "\tcmp eax, ebx\n";
					ofile << "\tjne " << Nameblock << "NotEqual" << NumLogic[NotEqual] << "True\n";

				}
				ofile << "\tmov eax, 0\n";
				ofile << "\tpush eax\n";
				ofile << "\tjmp " << Nameblock << "NotEqual" << NumLogic[NotEqual] << "End\n\n";
				ofile << Nameblock << "NotEqual" << NumLogic[NotEqual] << "True:\n";
				ofile << "\tmov eax, 1\n";
				ofile << "\tpush eax\n\n";
				ofile << Nameblock << "NotEqual" << NumLogic[NotEqual] << "End:\n";
				break;
			}
			case LEX_LOGIC_LESS:
			{
				NumLogic[Less]++;
				ofile << "\n\tpop eax\n\tpop ebx\n";
				ofile << "\tcmp eax, ebx\n";
				ofile << "\tjl " << Nameblock << "Less" << NumLogic[Less] << "True\n";
				ofile << "\tmov eax, 0\n";
				ofile << "\tpush eax\n";
				ofile << "\tjmp " << Nameblock << "Less" << NumLogic[Less] << "End\n\n";
				ofile << Nameblock << "Less" << NumLogic[Less] << "True:\n";
				ofile << "\tmov eax, 1\n";
				ofile << "\tpush eax\n\n";
				ofile << Nameblock << "Less" << NumLogic[Less] << "End:\n";
				break;
			}
			case LEX_LOGIC_MORE:
			{
				NumLogic[Great]++;
				ofile << "\n\tpop eax\n\tpop ebx\n";
				ofile << "\tcmp eax, ebx\n";
				ofile << "\tjg " << Nameblock << "Great" << NumLogic[Great] << "True\n";
				ofile << "\tmov eax, 0\n";
				ofile << "\tpush eax\n";
				ofile << "\tjmp " << Nameblock << "Great" << NumLogic[Great] << "End\n\n";
				ofile << Nameblock << "Great" << NumLogic[Great] << "True:\n";
				ofile << "\tmov eax, 1\n";
				ofile << "\tpush eax\n\n";
				ofile << Nameblock << "Great" << NumLogic[Great] << "End:\n";
				break;
			}
			case LEX_LOGIC_LESS_EQ:
			{
				NumLogic[EqLess]++;
				ofile << "\n\tpop eax\n\tpop ebx\n";
				ofile << "\tcmp eax, ebx\n";
				ofile << "\tjle " << Nameblock << "EqLess" << NumLogic[EqLess] << "True\n";
				ofile << "\tmov eax, 0\n";
				ofile << "\tpush eax\n";
				ofile << "\tjmp " << Nameblock << "EqLess" << NumLogic[EqLess] << "End\n\n";
				ofile << Nameblock << "EqLess" << NumLogic[EqLess] << "True:\n";
				ofile << "\tmov eax, 1\n";
				ofile << "\tpush eax\n\n";
				ofile << Nameblock << "EqLess" << NumLogic[EqLess] << "End:\n";
				break;
			}
			case LEX_LOGIC_MORE_EQ:
			{
				NumLogic[EqGreat]++;
				ofile << "\n\tpop eax\n\tpop ebx\n";
				ofile << "\tcmp eax, ebx\n";
				ofile << "\tjge " << Nameblock << "EqGreat" << NumLogic[EqGreat] << "True\n";
				ofile << "\tmov eax, 0\n";
				ofile << "\tpush eax\n";
				ofile << "\tjmp " << Nameblock << "EqGreat" << NumLogic[EqGreat] << "End\n\n";
				ofile << Nameblock << "EqGreat" << NumLogic[EqGreat] << "True:\n";
				ofile << "\tmov eax, 1\n";
				ofile << "\tpush eax\n\n";
				ofile << Nameblock << "EqGreat" << NumLogic[EqGreat] << "End:\n";
				break;
			}

			case LEX_LOGIC_NOT:
			{
				NumLogic[Not]++;
				ofile << "\n\tpop eax\n";
				ofile << "\tcmp eax, 0\n";
				ofile << "\tje " << Nameblock << "Not" << NumLogic[Not] << "True\n";
				ofile << "\tmov eax, 0\n";
				ofile << "\tpush eax\n";
				ofile << "\tjmp " << Nameblock << "Not" << NumLogic[Not] << "End\n\n";
				ofile << Nameblock << "Not" << NumLogic[Not] << "True:\n";
				ofile << "\tmov eax, 1\n";
				ofile << "\tpush eax\n\n";
				ofile << Nameblock << "Not" << NumLogic[Not] << "End:\n";
				break;
			}
			}

			i++;
		}
		return i;
	}
	int getIndexType(int i, Lex::LEX& lex)
	{
		int inThesis = 0, position = 1;
		if (LEXEMA(i) == LEX_RIGHTTHESIS)
		{
			while (LEXEMA(i) != LEX_LEFTHESIS || inThesis != 0)
			{
				if (LEXEMA(i) == LEX_RIGHTTHESIS)
				{
					inThesis++;
				}
				if (LEXEMA(i) == LEX_LEFTHESIS)
				{
					inThesis--;
				}
				i--;
				position++;
			}
			position++;
		}
		return position;
	}
}