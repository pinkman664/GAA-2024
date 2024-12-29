#pragma once
#include "stdafx.h"
#include "Semantic.h"

typedef std::vector <LT::Entry> ltvec;
typedef std::vector <int> intvec;



namespace Polish
{
	int PolishNotation(int i, Lex::LEX& lex, Log::LOG log, Semantic::Is n);
	int getPriority(unsigned char e);
	int searchExpression(Lex::LEX lex, Log::LOG log);
};