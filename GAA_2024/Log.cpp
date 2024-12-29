#include "stdafx.h"

namespace Log {
	LOG getlog(wchar_t logfile[]) {
		LOG log;

		log.stream = new std::ofstream;
		log.stream->open(logfile);
		if (!log.stream->is_open()) { throw ERROR_THROW(112); }
		wcscpy_s(log.logfile, logfile);

		return log;
	}

	void WriteLine(LOG log, const char* c, ...) {
		const char** pc = &c;
		while (*pc != "") {
			*log.stream << *pc;
			pc++;
		}
		*log.stream << std::endl;
	}
	void WriteLine(LOG log, const wchar_t* c, ...) {

		const wchar_t** pc = &c;
		while (*pc != L"") {
			const int SIZE = 10;
			char buff[SIZE];
			size_t tsize = 0;
			wcstombs_s(&tsize, buff, *pc, SIZE);
			*log.stream << buff;
			pc++;
		}
		*log.stream << std::endl;
	}
	void WriteLog(LOG log) {

		char date[100];
		tm local;
		time_t currentTime;
		currentTime = time(NULL);
		localtime_s(&local, &currentTime);
		strftime(date, 100, "%d.%m.%Y %H:%M:%S", &local);
		*log.stream << "--- Протокол --- Дата: " << date << std::endl;

	}
	void WriteParm(LOG log, Parm::PARM parm) {

		char buff[PARM_MAX_SIZE];
		size_t tsize = 0;

		*log.stream << "--- Параметры --- " << std::endl;
		wcstombs_s(&tsize, buff, parm.in, PARM_MAX_SIZE);
		*log.stream << "-in : " << buff << std::endl;
		wcstombs_s(&tsize, buff, parm.out, PARM_MAX_SIZE);
		*log.stream << "-out: " << buff << std::endl;
		wcstombs_s(&tsize, buff, parm.log, PARM_MAX_SIZE);
		*log.stream << "-log: " << buff << std::endl;
	}
	void WriteIn(LOG log, In::IN in) {

		*log.stream << "--- Исходные данные --- " << std::endl;
		*log.stream << "Количество символов : " << in.size << std::endl;
		*log.stream << "Количество строк    : " << in.lines << std::endl;
		*log.stream << "Проигнорировано     : " << in.ignor << std::endl;
	}
	void WriteTable(LOG log, In::InWord* table)
	{
		*log.stream << "\n\n ------------------ Таблица слов ------------------" << std::endl;
		*log.stream << std::setw(2) << "№" << std::setw(2) << "Строка: " << std::endl;
		for (int i = 0; i < table->size; i++)
			*log.stream << std::setw(2) << i << std::setw(3) << table[i].line << " |  " << table[i].word << std::endl;
		*log.stream << table->size;
	}

	void WriteError(std::ostream* stream, Error::ERROR e)
	{
		char* str;
		if (e.id >= 200 && e.id<= 299)
		{
			str = new char[12];
			str = (char*)"Лексическая\0";
		}
		else if (e.id >= 300 && e.id <= 399)
		{
			str = new char[14];
			str = (char*)"Семантическая\0";
		}
		else
		{
			str = new char[1];
			str = (char*)"\0";
		}
		if (stream == NULL)
		{
			if (e.position.cells == -1 || e.position.line == -1)
				std::cout << std::endl << str <<  " ошибка " << e.id << ": " << e.message << std::endl;
			else if (e.position.cells == NULL)
				std::cout << std::endl << str << " ошибка " << e.id << ": " << e.message
				<< " Строка: " << e.position.line << std::endl;
			else
				std::cout << std::endl << str << " ошибка " << e.id << ": " << e.message
				<< " Cтрока: " << e.position.line
				<< ", позиция: " << e.position.cells << std::endl;
		}
		else
		{
			if (e.position.cells == -1 || e.position.line == -1)
				*stream << std::endl << str << " ошибка " << ": " << e.message;
			else if (e.position.cells == NULL)
				*stream << std::endl << str << " ошибка " << ": " << e.message
				<< " Строка: " << e.position.line;
			else
				*stream << std::endl << str << " ошибка " << ": " << e.message
				<< " Cтрока: " << e.position.line
				<< ", позиция: " << e.position.cells;
			*stream << std::endl;
		}
	}

	void Close(LOG log) {
		log.stream->close();
		delete log.stream;
	}

	void WriteLineConsole(char* c, ...)
	{
		char** ptr = &c;			
		char* result;				
		result = new char[15];
		int size = 0;
		while (strcmp(*ptr, "") != 0)
		{
			size_t slen = strlen(*ptr);
			result = (char*)realloc(result, size + slen);
			result[size] = '\0';
			size += slen; 
			strcat_s(result, size + 1, *ptr);
			ptr++;
		}
		std::cout << result << std::endl;
	}

}