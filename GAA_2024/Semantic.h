#pragma once
#include "stdafx.h"
#include "LT.h"
#include "IT.h"

namespace Semantic
{
	enum Is {
		Fun = 0,
		If,
		Equals,
		Out
	};
	IT::IDDATATYPE SemanticExpressionCheck(int i, Lex::LEX& tables, Log::LOG& log, Is n);
	bool semanticsCheck(Lex::LEX& tables, Log::LOG& log);
};