#pragma once
#include "stdafx.h"
#include "IT.h"

#define LEX_SEPARATORS				'S'
#define	LEX_ID_TYPE    				't'	
#define	LEX_STDFUNC    				'p'
#define LEX_MASSIV					'm'
#define	LEX_ID						'i'	
#define	LEX_LITERAL					'l'	
#define	LEX_FUNCTION				'f'	
#define	LEX_MAIN					'm'	
#define	LEX_SEPARATOR				';'	
#define	LEX_COMMA					','	 		
#define	LEX_LEFTBRACE				'{'			
#define	LEX_BRACELET				'}'			
#define	LEX_SQ_LBRACELET			'['			
#define	LEX_SQ_RBRACELET			']'			
#define	LEX_LEFTHESIS				'('			
#define	LEX_RIGHTTHESIS				')'			
#define	LEX_PLUS					'+'	
#define	LEX_MINUS					'-'	
#define	LEX_STAR					'*'	
#define LEX_DIRSLASH				'/'	
#define LEX_PROCENT					'%'
#define LEX_AND						'&'
#define LEX_OR						'|'
#define LEX_NOT						'~'
#define LEX_XOR						'^'
#define	LEX_EQUAL					'='					
#define LEX_IF						'?'	
#define LEX_WHILE					'h'
#define LEX_ISFALSE					'r'	
#define LEX_FALSE					'a'	
#define LEX_ISTRUE					'w'	
#define LEX_TRUE					'u'	
#define LEX_DO						'd'	
#define LEX_WRITE					'o'	
#define LEX_NEWLINE					'b'	
#define LEX_RETURN					'e'	
#define LEX_VOID					'g'	
#define LEX_LOGIC_EQUALS			'0'	
#define LEX_LOGIC_NOT_EQUALS		'1'	
#define LEX_LOGIC_MORE				'2'	
#define LEX_LOGIC_LESS				'3'	
#define LEX_LOGIC_MORE_EQ			'4'	
#define LEX_LOGIC_LESS_EQ			'5'
#define LEX_LOGIC_AND				'6'
#define LEX_LOGIC_OR				'7'
#define LEX_LOGIC_NOT				'!'	
#define LEX_INVERSION_SIGN			'`'
#define LEX_SUBST					'@'
#define	LEXEMA_FIXSIZE				 1	
#define	LT_MAXSIZE					 4096
#define	NULLDX_TI					 0xffffffff	

namespace LT {
	struct Entry	
	{
		unsigned char lexema;
		int sn;
		int pn;
		int idxTI;						

		Entry();
		Entry(unsigned char lexema, int snn, int pnn, int idxti = NULLDX_TI);

	};

	struct LexTable						
	{
		int maxsize;					
		int size;						
		Entry* table;					
	};

	LexTable Create(		
		int size			
	);

	void Add(				
		LexTable& lextable,	
		Entry entry			
	);

	Entry GetEntry(			
		LexTable& lextable,	
		int n				
	);

	void Delete(LexTable& lextable);

	Entry writeEntry(				
		Entry& entry,
		unsigned char lexema,
		int indx,
		int line
	);

	void makeOutWithLTFILE(std::ostream* stream, LT::LexTable& table);
	void makeOutWithLT(LT::LexTable& table);
}