#include "MFST.h"
#include "stdafx.h"
#include "LexAnaliz.h"
#include "In.h"
#include "GRB.h"
#include <iostream>
#include "PolishNotation.h"
#include "Semantic.h"
#include "Generator.h"

using namespace std;

int _tmain(int argc, wchar_t* argv[])
{
	setlocale(0, "");
	char LEXERROR[] = "Лексический анализ завершен с ошибками";
	char SYNTERROR[] = "Синтаксический анализ завершен с ошибками";
	char SEMERROR[] = "Обнаружены семантические ошибки";
	char GENERROR[] = "Генерация кодо завершена с ошибками";
	char POLISHERROR[] = "Ошибка при попытке преобразования выражения";
	char LEXGOOD[] = "Лексический анализ завершен без ошибок";
	char SYNTGOOD[] = "Синтаксический анализ завершен без ошибок";
	char SEMGOOD[] = "Семантический анализ завершен без ошибок";
	char GENGOOD[] = "Генерация кодов завершена без ошибок";
	char POLISHGOOD[] = "Преобразование выражений завершено без ошибок";
	char MESSAGE[] = "--------------------КОНЕЧНЫЕ ТАБЛИЦЫ ЛЕКСЕМ И ИДЕНТИФИКАТОРОВ-------------------";
	char STOP[] = "\nВыполнение программы остановлено";
	char ALLGOOD[] = "Программа успешно завершена!";

	Log::LOG log = Log::INITLOG;
	bool logInit = false;

	try {
		Parm::PARM parm = Parm::getparm(argc, argv);

		logInit = true;

		log = Log::getlog(parm.log);

		Log::WriteLine(log, "Тест:", " без ошибок ", "");
		Log::WriteLog(log);
		Log::WriteParm(log, parm);

		In::IN in = In::getin(parm.in, log.stream);
		Log::WriteIn(log, in);

		in.words = In::getWordsTable(log.stream, in.text, in.code, in.size);
		Log::WriteTable(log, in.words);

		Lex::LEX tables;
		bool lex_ok = Lex::analyze(tables, in, log, parm);

		LT::makeOutWithLTFILE(log.stream, tables.lextable);
		IT::makeOutWithFILEIT(log.stream, tables.idtable);

		if (!lex_ok)
		{
			Log::WriteLine(log, LEXERROR, "");
			Log::WriteLineConsole(LEXERROR, STOP, "");

			return 0;
		}
		else
			Log::WriteLineConsole(LEXGOOD, "");


		MFST_TRACE_START(log.stream);

		MFST::Mfst mfst(tables, GRB::getGreibach());

		bool synt_ok = mfst.start(log);

		mfst.savededucation();
		mfst.printrules(log);

		if (!synt_ok)
		{
			Log::WriteLine(log, SYNTERROR, "");
			Log::WriteLineConsole(SYNTERROR, STOP, "");

			return 0;
		}
		else
			Log::WriteLineConsole(SYNTGOOD, "");

		tables.lextable.size = Polish::searchExpression(tables, log);
		if (tables.lextable.size == 0) {
			Log::WriteLine(log, POLISHERROR, "");
			Log::WriteLineConsole(POLISHERROR, STOP, "");

			return 0;
		}
		else
			Log::WriteLineConsole(POLISHGOOD, "");

		LT::makeOutWithLTFILE(log.stream, tables.lextable);

		bool sem_ok = Semantic::semanticsCheck(tables, log);

		if (!sem_ok)
		{
			Log::WriteLine(log, SEMERROR, "");
			Log::WriteLineConsole(SEMERROR, STOP, "");

			return 0;
		}
		else
			Log::WriteLineConsole(SEMGOOD, "");


		bool gen_ok = Gener::CodeGeneration(tables, parm, log);

		if (!gen_ok) {
			Log::WriteLine(log, GENERROR, "");
			Log::WriteLineConsole(GENERROR, STOP, "");

			return 0;
		}
		else
			Log::WriteLineConsole(GENGOOD, "");

		Log::WriteLine(log, ALLGOOD, "");
		Log::WriteLineConsole(ALLGOOD, "");

		Log::Close(log);
	}
	catch (Error::ERROR e)
	{
		if (logInit) {
			Log::WriteLine(log, "Работа заврешена с ошибками!", "");
			Log::WriteError(log.stream, e);
		}

		cout << "Работа заврешена с ошибками!" << endl;
		cout << e.message << " Строка " << e.position.line << ", позиция " << e.position.cells << endl;
	}
	catch (...)
	{
		std::cout << "Ошибка: Системный сбой" << std::endl;
	}
}