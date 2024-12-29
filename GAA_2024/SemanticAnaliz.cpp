#include "stdafx.h"
#include "Semantic.h"

namespace Semantic
{
	bool Semantic::semanticsCheck(Lex::LEX& tables, Log::LOG& log)
	{
		bool sem_ok = true, inMain = false;
		for (int i = 0; i < tables.lextable.size; i++)
		{
			switch (tables.lextable.table[i].lexema)
			{
			case LEX_DIRSLASH:
			case LEX_PROCENT:
			{
				int k = i-1;
				if (tables.lextable.table[k].lexema == LEX_LITERAL)
				{
					if (tables.idtable.table[tables.lextable.table[k].idxTI].value.vint == 0)
					{
						sem_ok = false;
						Log::WriteError(log.stream, Error::GetError(320, tables.lextable.table[k].sn, tables.lextable.table[k].pn));
						Log::WriteError(NULL, Error::GetError(320, tables.lextable.table[k].sn, tables.lextable.table[k].pn));
					}
				}
				break;
			}
			case LEX_MAIN:
			{
				inMain = true;
				break;
			}
			case LEX_RETURN:
			{
				if (inMain)
				{
					Log::WriteError(log.stream, Error::GetError(325, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
					Log::WriteError(NULL, Error::GetError(325, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
				}
				break;
			}
			case LEX_EQUAL:
			{
				if (i > 0 && tables.lextable.table[i - 1].idxTI != TI_NULLIDX)
				{
					IT::IDDATATYPE lefttype = tables.idtable.table[tables.lextable.table[i - 1].idxTI].iddatatype;
					if (lefttype == IT::IDDATATYPE::STR || lefttype == IT::IDDATATYPE::SYM)
					{
						if (tables.idtable.table[tables.lextable.table[i + 1].idxTI].iddatatype != lefttype)
						{
							Log::WriteError(log.stream, Error::GetError(314, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
							Log::WriteError(NULL, Error::GetError(314, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
							sem_ok = false;
						}
						int countFun = 0, j = i + 1;
						if ((tables.idtable.table[tables.lextable.table[j].idxTI].idtype == IT::IDTYPE::F || tables.idtable.table[tables.lextable.table[j].idxTI].idtype == IT::IDTYPE::S))
						{
							j++;
							while (tables.lextable.table[j - 1].lexema != LEX_SUBST || countFun != 0)
							{
								if (tables.lextable.table[j].lexema == LEX_ID &&
									(tables.idtable.table[tables.lextable.table[j].idxTI].idtype == IT::IDTYPE::F || tables.idtable.table[tables.lextable.table[j].idxTI].idtype == IT::IDTYPE::S))
								{
									countFun++;
								}
								if (tables.lextable.table[j].lexema == LEX_SUBST && countFun != 0)
								{
									countFun--;
								}
								j++;
							}
						}
						if (tables.lextable.table[j + 1].lexema != LEX_SEPARATOR) {

							Log::WriteError(log.stream, Error::GetError(316, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
							Log::WriteError(NULL, Error::GetError(316, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
							sem_ok = false;
						}
					}
					else
					{
						IT::IDDATATYPE righttype = SemanticExpressionCheck(i, tables, log, Equals);
						if (righttype == IT::IDDATATYPE::UNDEF)
						{
							sem_ok = false;
						}
						else if(righttype != lefttype)
						{
							Log::WriteError(log.stream, Error::GetError(314, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
							Log::WriteError(NULL, Error::GetError(314, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
							sem_ok = false;
						}
					}
				}
				break;
			}
			case LEX_WHILE:
			case LEX_IF:
			{
				IT::IDDATATYPE rifhttype = SemanticExpressionCheck(i+2, tables, log, If);
				if (rifhttype != IT::IDDATATYPE::INT && rifhttype != IT::IDDATATYPE::BOOL)
				{
					Log::WriteError(log.stream, Error::GetError(322, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
					Log::WriteError(NULL, Error::GetError(322, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
					sem_ok = false;
				}
				break;
			}
			case LEX_STDFUNC:
			case LEX_ID:
			{
				IT::Entry e = tables.idtable.table[tables.lextable.table[i].idxTI];
				if (i > 0 && tables.lextable.table[i - 1].lexema == LEX_FUNCTION)
				{
					if (e.idtype == IT::IDTYPE::F)
					{
						int numIf = 0;
						vector<int> countIf(255, -1); countIf[0] = 0;
						vector<int> vetv(255, -1); vetv[0] = 0;
						bool isDo = false; bool recursia = false;
						vector<vector<bool>> validreturn(1, vector<bool>(1, false));
						for (int k = i + 1; tables.lextable.table[k].lexema != LEX_BRACELET || numIf != 0; k++)
						{
							char l = tables.lextable.table[k].lexema;
							switch (l)
							{

							case LEX_ID:
							{
								if (tables.idtable.table[tables.lextable.table[k].idxTI].idtype == IT::IDTYPE::F 
								|| tables.idtable.table[tables.lextable.table[k].idxTI].idtype == IT::IDTYPE::S)
								{
									if (!strcmp(e.id, tables.idtable.table[tables.lextable.table[k].idxTI].id))
									{
										recursia = true;
									}
								}
								break;
							}
							case LEX_DO:
							{
								numIf++;
								vetv[numIf]++;
								validreturn.emplace_back();
								validreturn[numIf].resize(2, false);
								isDo = true;
								break;
							}
							case LEX_IF:
							{
								numIf++;
								validreturn.emplace_back();
								validreturn[numIf].resize(2, false);
								break;
							}

							case LEX_WHILE:
							{
								if (isDo)
								{
									isDo = false;
									break;
								}
								numIf++;
								vetv[numIf]++;
								validreturn.emplace_back();
								validreturn[numIf].resize(2, false);
								break;
							}
							case LEX_ISFALSE:
								vetv[numIf]++;
								break;
							case LEX_ISTRUE:
								vetv[numIf]++;
								break;
							case LEX_BRACELET:
							{
								if (tables.lextable.table[k+1].lexema == LEX_ISFALSE || tables.lextable.table[k + 1].lexema == LEX_ISTRUE)
								{
									break;
								}
								if (numIf != 0)
								{
									if (validreturn[numIf][1] == true || validreturn[numIf][0] == true)
									{
										if (validreturn[numIf][1] == true && validreturn[numIf][0] == true)
										{
											validreturn[numIf - 1][vetv[numIf - 1]] = true;
										}
									}
								}
								validreturn[numIf].erase(validreturn[numIf].begin(), validreturn[numIf].end());
								validreturn.pop_back();
								vetv[numIf] = -1;
								numIf--;
								break;
							}
							}
							if (l == LEX_RETURN)
							{
								int next = tables.lextable.table[k + 1].idxTI;
								if (numIf == 0)
									validreturn[0][0] = true;
								else
									validreturn[numIf][vetv[numIf]] = true;
								if (next != TI_NULLIDX)
								{
									if (e.iddatatype == IT::IDDATATYPE::STR || e.iddatatype == IT::IDDATATYPE::SYM)
									{
										if (tables.idtable.table[next].iddatatype != e.iddatatype)
										{
											Log::WriteError(log.stream, Error::GetError(314, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
											Log::WriteError(NULL, Error::GetError(314, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
											sem_ok = false;
										}
										int countFun = 0, j = k+1;
										if ((tables.idtable.table[tables.lextable.table[j].idxTI].idtype == IT::IDTYPE::F || tables.idtable.table[tables.lextable.table[j].idxTI].idtype == IT::IDTYPE::S))
										{
											j++;
											while (tables.lextable.table[j-1].lexema != LEX_SUBST || countFun != 0)
											{
												if (tables.lextable.table[j].lexema == LEX_ID &&
													(tables.idtable.table[tables.lextable.table[j].idxTI].idtype == IT::IDTYPE::F || tables.idtable.table[tables.lextable.table[j].idxTI].idtype == IT::IDTYPE::S))
												{
													countFun++;
												}
												if (tables.lextable.table[j].lexema == LEX_SUBST && countFun != 0)
												{
													countFun--;
												}
												j++;
											}
										}
										if (tables.lextable.table[j + 1].lexema != LEX_SEPARATOR) {
											
											Log::WriteError(log.stream, Error::GetError(316, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
											Log::WriteError(NULL, Error::GetError(316, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
											sem_ok = false;
										}
									}
									else
									{
										IT::IDDATATYPE righttype = SemanticExpressionCheck(k, tables, log, Equals);
										if (righttype == IT::IDDATATYPE::UNDEF)
										{
											sem_ok = false;
										}
										else if (righttype != e.iddatatype)
										{
											Log::WriteError(log.stream, Error::GetError(315, tables.lextable.table[k].sn, tables.lextable.table[k].pn));
											Log::WriteError(NULL, Error::GetError(315, tables.lextable.table[k].sn, tables.lextable.table[k].pn));
											sem_ok = false;
											break;
										}

									}
								}
								else if (e.iddatatype != IT::IDDATATYPE::PROC)
								{
									Log::WriteError(log.stream, Error::GetError(315, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
									Log::WriteError(NULL, Error::GetError(315, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
									sem_ok = false;
									break;
								}
							}
							if (k == tables.lextable.size)
								break;
						}
						if (!validreturn[0][0] && recursia && sem_ok)
						{
							Log::WriteError(log.stream, Error::GetError(324, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
							Log::WriteError(NULL, Error::GetError(324, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
							sem_ok = false;
						}
						else if (!validreturn[0][0] && e.iddatatype != IT::IDDATATYPE::PROC && sem_ok)
						{
							Log::WriteError(log.stream, Error::GetError(323, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
							Log::WriteError(NULL, Error::GetError(323, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
							sem_ok = false;
						}
					}
				}
				if (tables.lextable.table[i - 1].lexema != LEX_FUNCTION)
				{
					if (e.idtype == IT::IDTYPE::F || e.idtype == IT::IDTYPE::S)
					{
						int paramscount = NULL;
						if (e.value.params.count != NULL)
							for (int j = i + 1; tables.lextable.table[j].lexema != LEX_SUBST; j++)
							{
								if (tables.lextable.table[j].lexema == LEX_ID || tables.lextable.table[j].lexema == LEX_LITERAL)
								{
									paramscount++;
									IT::IDDATATYPE ctype = SemanticExpressionCheck(j, tables, log, Fun);
									if (ctype == IT::IDDATATYPE::UNDEF)
									{
										sem_ok = false;
										break;
									}
									else if (ctype != e.value.params.types[paramscount - 1])
									{
										Log::WriteError(log.stream, Error::GetError(309, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
										Log::WriteError(NULL, Error::GetError(309, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
										sem_ok = false;
										break;
									}
									int countFun = 0;
									while (tables.lextable.table[j].lexema != LEX_SUBST && tables.lextable.table[j].lexema != LEX_COMMA)
									{
										if (tables.lextable.table[j].lexema == LEX_ID &&
											(tables.idtable.table[tables.lextable.table[j].idxTI].idtype == IT::IDTYPE::F || tables.idtable.table[tables.lextable.table[j].idxTI].idtype == IT::IDTYPE::S))
										{
											countFun++;
										}
										if (tables.lextable.table[j].lexema == LEX_SUBST && countFun != 0)
										{
											countFun--;
										}
										j++;
									}
									j--;
								}
								if (j == tables.lextable.size)
									break;
							}
						if (paramscount != e.value.params.count)
						{
							Log::WriteError(log.stream, Error::GetError(308, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
							Log::WriteError(NULL, Error::GetError(308, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
							sem_ok = false;
						}
						if (paramscount > 10)
						{
							Log::WriteError(log.stream, Error::GetError(307, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
							Log::WriteError(NULL, Error::GetError(307, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
							sem_ok = false;
						}
					}
				}
				break;
			}
			}
		}
		return sem_ok;
	}
	IT::IDDATATYPE SemanticExpressionCheck(int i, Lex::LEX& tables, Log::LOG& log, Is n) {
		stack<IT::IDDATATYPE> typeOperand; typeOperand.push(IT::IDDATATYPE::PROC);
		int countFun = 0;
		for (;
			((tables.lextable.table[i].lexema != LEX_COMMA && tables.lextable.table[i].lexema != LEX_SUBST) && Semantic::Fun == n)
			|| (tables.lextable.table[i].lexema != LEX_SEPARATOR && Semantic::Equals == n)
			|| (tables.lextable.table[i].lexema != LEX_RIGHTTHESIS && Semantic::If == n);
			i++)
		{
			switch (tables.lextable.table[i].lexema)
			{
			case LEX_STDFUNC:
			case LEX_ID:
			case LEX_LITERAL:
			{
				typeOperand.push(tables.idtable.table[tables.lextable.table[i].idxTI].iddatatype);
				if (tables.idtable.table[tables.lextable.table[i].idxTI].idtype == IT::IDTYPE::F || tables.idtable.table[tables.lextable.table[i].idxTI].idtype == IT::IDTYPE::S)
				{
					while (tables.lextable.table[i-1].lexema != LEX_SUBST || countFun != 0)
					{
						if (tables.lextable.table[i].lexema == LEX_ID &&
							(tables.idtable.table[tables.lextable.table[i].idxTI].idtype == IT::IDTYPE::F || tables.idtable.table[tables.lextable.table[i].idxTI].idtype == IT::IDTYPE::S))
						{
							countFun++;
						}
						if (tables.lextable.table[i].lexema == LEX_SUBST && countFun != 0)
						{
							countFun--;
						}
						i++;
					}
				}
				break;
			}
			case LEX_PLUS: case LEX_MINUS: case LEX_STAR: case LEX_DIRSLASH: case LEX_PROCENT: case LEX_AND: case LEX_OR: case LEX_XOR:
			case LEX_LOGIC_OR: case LEX_LOGIC_AND: case LEX_LOGIC_EQUALS: case LEX_LOGIC_NOT_EQUALS: case LEX_LOGIC_LESS:
			case LEX_LOGIC_LESS_EQ: case LEX_LOGIC_MORE: case LEX_LOGIC_MORE_EQ:
			{
				IT::IDDATATYPE leftTypeOperand = typeOperand.top(); typeOperand.pop();
				IT::IDDATATYPE rightTypeOperand = typeOperand.top(); typeOperand.pop();
				if (leftTypeOperand == rightTypeOperand)
				{
					if (leftTypeOperand == IT::IDDATATYPE::STR || leftTypeOperand == IT::IDDATATYPE::SYM)
					{
						if (tables.lextable.table[i].lexema != LEX_LOGIC_EQUALS && tables.lextable.table[i].lexema != LEX_LOGIC_NOT_EQUALS)
						{
							Log::WriteError(log.stream, Error::GetError(318, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
							Log::WriteError(NULL, Error::GetError(318, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
							typeOperand.push(IT::IDDATATYPE::UNDEF);
							break;
						}
						typeOperand.push(IT::IDDATATYPE::BOOL);
						break;
					}
					switch (tables.lextable.table[i].lexema)
					{
					case LEX_LOGIC_OR: case LEX_LOGIC_AND: case LEX_LOGIC_EQUALS: case LEX_LOGIC_NOT_EQUALS: case LEX_LOGIC_LESS:
					case LEX_LOGIC_LESS_EQ: case LEX_LOGIC_MORE: case LEX_LOGIC_MORE_EQ:
					{
						typeOperand.push(IT::IDDATATYPE::BOOL);
						break;
					}
					default:
					{
						typeOperand.push(IT::IDDATATYPE::INT);
						break;
					}
					}
				}
				else
				{
					if ((leftTypeOperand == IT::IDDATATYPE::BOOL && rightTypeOperand == IT::IDDATATYPE::INT) || (rightTypeOperand == IT::IDDATATYPE::BOOL && leftTypeOperand == IT::IDDATATYPE::INT))
					{
						break;
					}
					Log::WriteError(log.stream, Error::GetError(319, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
					Log::WriteError(NULL, Error::GetError(319, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
					typeOperand.push(IT::IDDATATYPE::UNDEF);
				}
				break;
			}
			case LEX_NOT: case LEX_LOGIC_NOT:
			{
				IT::IDDATATYPE TypeOperand = typeOperand.top(); 
				if (TypeOperand != IT::IDDATATYPE::INT && TypeOperand != IT::IDDATATYPE::BOOL)
				{
					Log::WriteError(log.stream, Error::GetError(321, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
					Log::WriteError(NULL, Error::GetError(321, tables.lextable.table[i].sn, tables.lextable.table[i].pn));
					typeOperand.push(IT::IDDATATYPE::UNDEF);
				}
				break;
			}
			}
			if (typeOperand.top() == IT::IDDATATYPE::UNDEF)
			{
				break;
			}
		}
		return typeOperand.top();
	}
}