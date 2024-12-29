#include "stdafx.h"
#include "PolishNotation.h"
using namespace std;
int countExpression = 0;

namespace Polish
{

	int getPriority(unsigned char e)
	{
		switch (e)
		{
		case LEX_LEFTHESIS: case LEX_RIGHTTHESIS: return 0;
		case LEX_COMMA: return 1;
		case LEX_LOGIC_OR: return 2;
		case LEX_LOGIC_AND: return 3;
		case LEX_OR: return 4;
		case LEX_XOR: return 5;
		case LEX_AND: return 6;
		case LEX_LOGIC_EQUALS: case LEX_LOGIC_NOT_EQUALS: return 7;
		case LEX_LOGIC_LESS: case LEX_LOGIC_LESS_EQ: case LEX_LOGIC_MORE: case LEX_LOGIC_MORE_EQ: return 8;
		case LEX_PLUS: case LEX_MINUS: return 9;
		case LEX_STAR: case LEX_DIRSLASH: case LEX_PROCENT: return 10;
		case LEX_NOT: case LEX_LOGIC_NOT: return 11;
		default: return -1;
		}
	}
	int searchExpression(Lex::LEX lex, Log::LOG log) {
		bool fl = false, inEqual = false, inIf = false, inFun = false;
		int inThesis = 0;
		Lex::LEX result;
		result.lextable = LT::Create(lex.lextable.size);
		result.idtable = IT::Create(lex.idtable.size);
		*log.stream << "\nРезультат преобразований лекскем по польской нотации:\n";
		for (int i = 0, k = 0; i < lex.lextable.size; i++)
		{
			if (inEqual)
			{
				if (lex.lextable.table[i].lexema == LEX_SEPARATOR)
				{
					inEqual = false;
				}
				continue;
			}
			if (inFun)
			{
				if (lex.lextable.table[i].lexema == LEX_SEPARATOR)
				{
					inFun = false;
				}
				continue;
			}
			if (inIf)
			{
				if (lex.lextable.table[i].lexema == LEX_RIGHTTHESIS)
				{
					if (inThesis == 0)
					{
						inIf = false;
						continue;
					}
					inThesis--;
				}
				if (lex.lextable.table[i].lexema == LEX_LEFTHESIS)
				{
					inThesis++;
				}
				continue;
			}
			if (lex.lextable.table[i].lexema == LEX_EQUAL || lex.lextable.table[i].lexema == LEX_RETURN) {
				lex.lextable.size = PolishNotation(++i, lex, log, Semantic::Equals);
				inEqual = true;
			}
			else if((lex.lextable.table[i].lexema == LEX_ID || lex.lextable.table[i].lexema == LEX_STDFUNC)
				&& lex.lextable.table[i-1].lexema != LEX_FUNCTION
				&&	(lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::IDTYPE::F
					|| lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::IDTYPE::S)) {
				lex.lextable.size = PolishNotation(i, lex,log, Semantic::Equals);
				inFun = true;
			}
			else if (lex.lextable.table[i].lexema == LEX_IF || lex.lextable.table[i].lexema == LEX_WHILE) {
				lex.lextable.size = PolishNotation((i+=2), lex, log, Semantic::If);
				inIf = true;
			}
			
		}
		*log.stream << "\n---------------------------------------------\n";
		return lex.lextable.size;
	}
	int PolishNotation(int lextable_pos, Lex::LEX& lex, Log::LOG log, Semantic::Is n)
	{
		stack<LT::Entry> stack;													
		queue<LT::Entry> queue;														
		LT::Entry temp;		temp.idxTI = -1;	temp.lexema = '#';	temp.sn = -1;
		LT::Entry tilda;	tilda.idxTI = -1;	tilda.lexema = '`';	tilda.sn = -1;
		LT::Entry endFun;	endFun.idxTI = -1;	endFun.lexema = '|';	endFun.sn = -1;
		LT::Entry func;		func.idxTI = -1;	func.lexema = '@';	func.sn = -1;
		int countLex = 0;															
		int countParm = 0;		
		int inThesis = 0;
		int posLex = lextable_pos;																
		bool findFunc = false;															
		bool flagthethis = false;
		char* buf = new char[2];													
		for (int i = lextable_pos; 
			((lex.lextable.table[i].lexema != LEX_RIGHTTHESIS && lex.lextable.table[i].lexema != LEX_COMMA) && Semantic::Fun == n)
			|| (lex.lextable.table[i].lexema != LEX_SEPARATOR && Semantic::Equals == n)
			|| ((lex.lextable.table[i].lexema != LEX_RIGHTTHESIS || inThesis != 0) && Semantic::If == n);
			i++, countLex++)
		{
			switch (lex.lextable.table[i].lexema)
			{
			case LEX_STDFUNC:
			case LEX_ID:	
			{
				if(lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::IDTYPE::F || lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::IDTYPE::S)
				{
					int function = i;
					queue.push(lex.lextable.table[i++]);
					i++;
					stack.push(endFun);
					while(lex.lextable.table[i].lexema != LEX_RIGHTTHESIS)
					{
						if (lex.lextable.table[i + 1].lexema != LEX_RIGHTTHESIS && lex.lextable.table[i + 1].lexema != LEX_COMMA)
						{
							lex.lextable.size = PolishNotation(i, lex, log, Semantic::Fun);
							int start = i;
							int countFun = 0;
							while ((lex.lextable.table[i].lexema != LEX_COMMA && lex.lextable.table[i].lexema != LEX_RIGHTTHESIS) || countFun != 0)
							{
								if (lex.lextable.table[i].lexema == LEX_ID &&
									(lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::IDTYPE::F || lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::IDTYPE::S))
								{
									countFun++;
								}
								if (lex.lextable.table[i].lexema == LEX_SUBST)
								{
									countFun--;
								}
								i++;
							}
							int end = i;
							for (int j = end-1; j >= start; j--)
							{
								countLex++;
								stack.push(lex.lextable.table[j]);
							}
						}
						else
						{
							stack.push(lex.lextable.table[i++]);
							countLex++;
						}
						countParm++;
						if (lex.lextable.table[i].lexema == LEX_COMMA)
						{
							countLex++;
							stack.push(lex.lextable.table[i]);
						}
						if (lex.lextable.table[i].lexema == LEX_RIGHTTHESIS)
						{
							break;
						}
						i++;
					}
					while (stack.top().lexema != '|')
					{
						queue.push(stack.top());
						stack.pop();
					}
					func.sn = lex.lextable.table[i].sn;
					func.idxTI = lex.lextable.table[function].idxTI;
					lex.lextable.table[i] = func;
					queue.push(lex.lextable.table[i]);
					_itoa_s(countParm, buf, 2, 11);
					stack.top().lexema = buf[0];
					stack.top().idxTI = -1; stack.top().sn = lex.lextable.table[i].sn;
					queue.push(stack.top());
					stack.pop();
					countParm = 0;
					countLex+=2;
					break;
				}
				else
					queue.push(lex.lextable.table[i]);
				continue;
			}
			case LEX_LITERAL:
			{
				queue.push(lex.lextable.table[i]);
				continue;
			}
			case LEX_LEFTHESIS:													
			{
				if (lex.lextable.table[i + 1].lexema == LEX_MINUS && lex.lextable.table[i + 2].lexema == LEX_ID && !findFunc)
				{
					flagthethis = true;
					continue;
				}
				if (Semantic::If == n)
					inThesis++;
				stack.push(lex.lextable.table[i]);
				continue;
			}
			case LEX_RIGHTTHESIS:												
			{
				while (stack.top().lexema != LEX_LEFTHESIS)
				{
					queue.push(stack.top());
					stack.pop();
				}
				if (stack.empty())
					return 0;
				if (Semantic::If == n)
					inThesis--;
				stack.pop();
				continue;
			}
			case LEX_PLUS: case LEX_MINUS: case LEX_STAR: case LEX_DIRSLASH: case LEX_PROCENT: case LEX_AND: case LEX_OR: case LEX_NOT: case LEX_XOR:
			case LEX_LOGIC_OR: case LEX_LOGIC_AND: case LEX_LOGIC_NOT: case LEX_LOGIC_EQUALS: case LEX_LOGIC_NOT_EQUALS: case LEX_LOGIC_LESS: 
			case LEX_LOGIC_LESS_EQ: case LEX_LOGIC_MORE: case LEX_LOGIC_MORE_EQ:
			{
				if (flagthethis)
				{
					tilda.sn = lex.lextable.table[i].sn;
					lex.lextable.table[i] = tilda;
					queue.push(lex.lextable.table[i + 1]);								
					queue.push(lex.lextable.table[i]);									
					flagthethis = false;
					i += 2;
					countLex += 2;
					continue;
				}
				while (!stack.empty() && getPriority(lex.lextable.table[i].lexema) <= getPriority(stack.top().lexema))	
				{
					queue.push(stack.top());											
					stack.pop();
				}
				stack.push(lex.lextable.table[i]);
				continue;
			}
			}
		}
		while (!stack.empty())														
		{
			if (stack.top().lexema == LEX_LEFTHESIS || stack.top().lexema == LEX_RIGHTTHESIS)
				return 0;
			queue.push(stack.top());													
			stack.pop();
		}
		*log.stream << ++countExpression << ". - ";
		while (countLex != 0)															
		{
			if (!queue.empty()) {
				lex.lextable.table[posLex++] = queue.front();
				if(lex.lextable.table[posLex - 1].lexema != '#' && lex.lextable.table[posLex - 1].lexema != ',')
					*log.stream << lex.lextable.table[posLex - 1].lexema << " ";
				queue.pop();
				
			}
			else
			{
				lex.lextable.table[posLex++] = temp;
			}
			countLex--;
		}
		*log.stream << "\n";
		for (int i = 0; i < posLex; i++)												
		{
			if (lex.lextable.table[i].lexema == LEX_LITERAL)
				lex.idtable.table[lex.lextable.table[i].idxTI].idxfirstLE = i;
		}
		return lex.lextable.size;
	}
}

