#pragma once
#include "stdafx.h"
#include "IT.h"
#include "LT.h"

#define ID_MAXSIZE			32					
#define SCOPED_ID_MAXSIZE   (ID_MAXSIZE*2-1)		
#define MAXSIZE_TI			4096					
#define TI_INT_DEFAULT		0x00000000				
#define TI_STR_DEFAULT		0x00				
#define TI_ARRAY_DEFAULT	0x0000				
#define TI_SYM_DEFAULT		0x00					
#define TI_NULLIDX			0xffffffff				
#define STR_MAXSIZE			255						
#define TI_INT_MAXSIZE		2147483647			
#define TI_INT_MINSIZE		-2147483647

#define MAX_PARAMS_COUNT	9			   			
#define POW_PARAMS_CNT		2	
#define LENGTH_PARAMS_CNT	1
#define SQUERE_PARAMS_CNT	1						
#define STRLEN_PARAMS_CNT	1						
#define INTTOSTR_PARAMS_CNT	1						
#define CONCAT_PARAMS_CNT	2						
#define COPY_PARAMS_CNT		1						
#define INPUT_PARAMS_CNT	0				

#define SQUERE_TYPE			IT::IDDATATYPE::INT
#define POW_TYPE			IT::IDDATATYPE::INT
#define LENGTH_TYPE			IT::IDDATATYPE::INT
#define STRLEN_TYPE			IT::IDDATATYPE::INT
#define INT_TO_STR_TYPE		IT::IDDATATYPE::STR
#define CONCAT_TYPE			IT::IDDATATYPE::STR
#define COPY_TYPE			IT::IDDATATYPE::STR

#define ININT_TYPE			IT::IDDATATYPE::INT
#define INSTR_TYPE			IT::IDDATATYPE::STR
#define INSYM_TYPE			IT::IDDATATYPE::SYM

namespace IT {

	enum IDDATATYPE { INT = 1, STR = 2, SYM = 3, BOOL = 4, PROC = 5, UNDEF }; 
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, S = 5, Z = 6 };			
	enum STDFNC {
		F_POW, F_POWER, F_SQUERE, F_STRLEN, F_NOT_STD,									
		F_INT_TO_STR, F_ININT, F_INSTR, F_INSYM, F_LENGTH
	}; // Стандартная функция

	static const IDDATATYPE POW_PARAMS[] = { IT::IDDATATYPE::INT, IT::IDDATATYPE::INT };	
	static const IDDATATYPE LENGTH_PARAMS[] = { IT::IDDATATYPE::STR };
	static const IDDATATYPE SQUERE_PARAMS[] = { IT::IDDATATYPE::INT };								
	static const IDDATATYPE STRLEN_PARAMS[] = { IT::IDDATATYPE::STR };									
	static const IDDATATYPE INT_TO_STR_PARAMS[] = { IT::IDDATATYPE::INT };								
	static const IDDATATYPE CONCAT_PARAMS[] = { IT::IDDATATYPE::STR, IT::IDDATATYPE::STR };			
	static const IDDATATYPE COPY_PARAMS[] = { IT::IDDATATYPE::STR };									

	struct Entry	
	{
		int			idxfirstLE;			
		char	id[SCOPED_ID_MAXSIZE];	// Имя индентификатора
		IDDATATYPE	iddatatype;			// Тип значении индентификатора
		IDTYPE		idtype;				// Тип индентификатора

		union
		{
			int vint;			
			struct
			{
				int len;							
				char str[STR_MAXSIZE - 1];
			} vstr;									
			char symbol;					
			struct
			{
				int count;					
				IDDATATYPE* types;		
			} params;
			struct
			{
				int count;
				int* idxTI;
			} varr;
			bool vbool;
		} value;	

		Entry()							
		{
			this->value.vint = TI_INT_DEFAULT;
			this->value.vstr.len = NULL;
			this->value.params.count = NULL;
			this->value.symbol = NULL;
		};
		Entry(char* id, int idxLT, IDDATATYPE datatype, IDTYPE idtype) 
		{
			strncpy_s(this->id, id, SCOPED_ID_MAXSIZE - 1);
			this->idxfirstLE = idxLT;
			this->iddatatype = datatype;
			this->idtype = idtype;
		};
	};

	struct IdTable				
	{
		int maxsize;		
		int size;				
		Entry* table;			
	};

	IdTable Create(int size);	

	void Add(				
		IdTable& idtable,
		Entry entry		
	);
	int isId(		
		IdTable& idtable,	
		char id[SCOPED_ID_MAXSIZE]	
	);
	bool SetValue(IT::Entry* entry, char* value);	
	bool SetValue(IT::IdTable& idtable, int index, char* value);
	void makeOutWithIT(IT::IdTable& idtable);
	void makeOutWithFILEIT(std::ostream* stream, IT::IdTable& idtable);
}