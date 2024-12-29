#pragma once
#include "In.h"
#include "Error.h"
#include "stdafx.h"

namespace Log {
	struct LOG {
		wchar_t logfile[PARM_MAX_SIZE]; 
		std::ofstream* stream; 
	};

	static const LOG INITLOG = { L"",NULL };	
	LOG getlog(wchar_t logfile[]);					
	void WriteLine(LOG log, const char* c, ...);
	void WriteLine(LOG log, const wchar_t* c, ...);	
	void WriteLog(LOG log);					
	void WriteParm(LOG log, Parm::PARM parm);
	void WriteIn(LOG log, In::IN in);			
	void WriteError(std::ostream* stream, Error::ERROR error);	
	void Close(LOG log);							
	void WriteLineConsole(char* c, ...);
	void WriteTable(LOG log, In::InWord* table);
};