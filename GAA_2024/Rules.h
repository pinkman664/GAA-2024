#pragma once
#include "GRB.h"

namespace GRB
{
	Greibach greibach(NS('S'), TS('$'), 14,

/*1*/	Rule(NS('S'), GRB_ERROR_SERIES, 3,	// Объявление функции
			// S → tfiPTS | gfiPTS | m{K}
			Rule::Chain(6, TS('t'), TS('f'), TS('i'), NS('P'), NS('T'), NS('S')),
			Rule::Chain(6, TS('g'), TS('f'), TS('i'), NS('P'), NS('T'), NS('S')),
			Rule::Chain(4, TS('m'), TS('{'), NS('K'), TS('}'))
		),

/*2*/	Rule(NS('T'), GRB_ERROR_SERIES + 1, 5,	// Тело функции	
			// T → {eW;} | {KeW;}
			Rule::Chain(6, TS('{'), NS('K'), TS('e'), NS('W'), TS(';'), TS('}')),
			Rule::Chain(5, TS('{'), NS('K'), TS('e'), TS(';'), TS('}')),
			Rule::Chain(3, TS('{'), NS('K'), TS('}')),
			Rule::Chain(5, TS('{'), TS('e'), NS('W'), TS(';'), TS('}')),
			Rule::Chain(4, TS('{'), TS('e'), TS(';'), TS('}'))
		), 

/*3*/	Rule(NS('P'), GRB_ERROR_SERIES + 3, 2, // Список параметров функции при его объявлений
			// P → () | (E)
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(2, TS('('), TS(')'))
		),

/*4*/	Rule(NS('E'), GRB_ERROR_SERIES + 4, 2, // Параметры функции при его объявлений
			// E → ti,E | ti
 			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('E')),
			Rule::Chain(2, TS('t'), TS('i'))
		),

/*5*/	Rule(NS('F'), GRB_ERROR_SERIES + 5, 2, // список фактических параметров функции
			// F → (N) | ()
			Rule::Chain(3, TS('('), NS('N'), TS(')')),
			Rule::Chain(2, TS('('), TS(')'))
		),

/*6*/	Rule(NS('N'), GRB_ERROR_SERIES + 6, 32, // Параметры функции (некоторые параметры могут быть выражением)
			// N → i | l | (W) | (-i) | iF | pF | ~W | !W 
			// N → iOW | lOW | (W)OW | (-i)OW | iFOW | pFOW | ~WOW | !WOW 
			// N → i,N | l,N | (W),N | (-i),N | iF,N | pF,N | ~W,N | !W,N 
			// N → iOW,N | lOW,N | (W)OW,N | (-i)OW,N | iFOW,N | pFOW,N | ~WOW,N | !WOW,N 
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('('), NS('W'), TS(')')),
			Rule::Chain(4, TS('('), TS('-'), TS('i'), TS(')')),
			Rule::Chain(2, TS('i'), NS('F')),
			Rule::Chain(2, TS('p'), NS('F')),
			Rule::Chain(2, TS('~'), NS('W')),
			Rule::Chain(2, TS('!'), NS('W')),

			Rule::Chain(3, TS('i'), NS('O'), NS('W')),
			Rule::Chain(3, TS('l'), NS('O'), NS('W')),
			Rule::Chain(5, TS('('), NS('W'), TS(')'), NS('O'), NS('W')),
			Rule::Chain(6, TS('('), TS('-'), TS('i'), TS(')'), NS('O'), NS('W')),
			Rule::Chain(4, TS('i'), NS('F'), NS('O'), NS('W')),
			Rule::Chain(4, TS('p'), NS('F'), NS('O'), NS('W')),
			Rule::Chain(4, TS('~'), NS('W'), NS('O'), NS('W')),
			Rule::Chain(4, TS('!'), NS('W'), NS('O'), NS('W')), 

			Rule::Chain(3, TS('i'), TS(','), NS('N')),
			Rule::Chain(3, TS('l'), TS(','), NS('N')),
			Rule::Chain(5, TS('('), NS('W'), TS(')'), TS(','), NS('N')),
			Rule::Chain(6, TS('('), TS('-'), TS('i'), TS(')'), TS(','), NS('N')),
			Rule::Chain(4, TS('i'), NS('F'), TS(','), NS('N')),
			Rule::Chain(4, TS('p'), NS('F'), TS(','), NS('N')),
			Rule::Chain(4, TS('~'), NS('W'), TS(','), NS('N')),
			Rule::Chain(4, TS('!'), NS('W'), TS(','), NS('N')),

			Rule::Chain(5, TS('i'), NS('O'), NS('W'), TS(','), NS('N')),
			Rule::Chain(5, TS('l'), NS('O'), NS('W'), TS(','), NS('N')),
			Rule::Chain(7, TS('('), NS('W'), TS(')'), NS('O'), NS('W'), TS(','), NS('N')),
			Rule::Chain(8, TS('('), TS('-'), TS('i'), TS(')'), NS('O'), NS('W'), TS(','), NS('N')),
			Rule::Chain(6, TS('i'), NS('F'), NS('O'), NS('W'), TS(','), NS('N')),
			Rule::Chain(6, TS('p'), NS('F'), NS('O'), NS('W'), TS(','), NS('N')),
			Rule::Chain(6, TS('~'), NS('W'), NS('O'), NS('W'), TS(','), NS('N')),
			Rule::Chain(6, TS('!'), NS('W'), NS('O'), NS('W'), TS(','), NS('N'))
		),

/*7*/	Rule(NS('R'), GRB_ERROR_SERIES + 7, 4, // Условная конструкция 
			// R → rY | wY | rYwY | wYrY
			Rule::Chain(2, TS('r'), NS('Y')),
			Rule::Chain(2, TS('w'), NS('Y')),
			Rule::Chain(4, TS('r'), NS('Y'), TS('w'), NS('Y')),
			Rule::Chain(4, TS('w'), NS('Y'), TS('r'), NS('Y'))
		),

/*8*/	Rule(NS('O'), GRB_ERROR_SERIES + 8, 16, // Операторы
			// O → 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 // логические
			// O → + | - | * | / | % | & | | | ^ // арифметические и побитовые
			Rule::Chain(1, TS('0')),
			Rule::Chain(1, TS('1')),
			Rule::Chain(1, TS('2')),
			Rule::Chain(1, TS('3')),
			Rule::Chain(1, TS('4')),
			Rule::Chain(1, TS('5')),
			Rule::Chain(1, TS('6')),
			Rule::Chain(1, TS('7')),
			Rule::Chain(1, TS('+')),
			Rule::Chain(1, TS('-')),
			Rule::Chain(1, TS('*')),
			Rule::Chain(1, TS('%')),
			Rule::Chain(1, TS('/')),
			Rule::Chain(1, TS('&')),
			Rule::Chain(1, TS('|')),
			Rule::Chain(1, TS('^'))
		),

/*9*/	Rule(NS('W'), GRB_ERROR_SERIES + 9, 16, // Выражение
			// W → i | l | (W) | (-i) | iF | pF | ~W | !W |
			// W → iOW | lOW | (W)OW | (-i)OW | iFOW | pFOW | ~WOW | !WOW |
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('('), NS('W'), TS(')')),
			Rule::Chain(4, TS('('), TS('-'), TS('i'), TS(')')),
			Rule::Chain(2, TS('i'), NS('F')),
			Rule::Chain(2, TS('p'), NS('F')),
			Rule::Chain(2, TS('~'), NS('W')),
			Rule::Chain(2, TS('!'), NS('W')),

			Rule::Chain(3, TS('i'), NS('O'), NS('W')),
			Rule::Chain(3, TS('l'), NS('O'), NS('W')),
			Rule::Chain(6, TS('('), TS('-'), TS('i'), TS(')'), NS('O'), NS('W')),
			Rule::Chain(5, TS('('), NS('W'), TS(')'), NS('O'), NS('W')),
			Rule::Chain(4, TS('i'), NS('F'), NS('O'), NS('W')),
			Rule::Chain(4, TS('p'), NS('F'), NS('O'), NS('W')),
			Rule::Chain(4, TS('~'), NS('W'), NS('O'), NS('W')),
			Rule::Chain(4, TS('!'), NS('W'), NS('O'), NS('W'))

		),

/*10*/	Rule(NS('K'), GRB_ERROR_SERIES + 10, 20, //Операторы программы
			// Z → 	ti=W; | i=W; | ti; | iF; | pF; | oB; | ^B; | ?(W)R | h(W)Y | dYh(W);						
			// Z →	ti=W;K | i=W;K | ti;K | iF;K | pF;K | oB;K | ^B;K | ?(W)RK | h(W)YK | dYh(W);K
			Rule::Chain(5, TS('t'), TS('i'), TS('='), NS('W'), TS(';')),			
			Rule::Chain(4, TS('i'), TS('='), NS('W'), TS(';')),		
			Rule::Chain(3, TS('t'), TS('i'), TS(';')),
			Rule::Chain(3, TS('i'), NS('F'), TS(';')),
			Rule::Chain(3, TS('p'), NS('F'), TS(';')),
			Rule::Chain(3, TS('o'), NS('B'), TS(';')),				
			Rule::Chain(3, TS('b'), NS('B'), TS(';')),	
			Rule::Chain(5, TS('?'), TS('('), NS('W'), TS(')'), NS('R')),
			Rule::Chain(5, TS('h'), TS('('), NS('W'), TS(')'), NS('Y')),
			Rule::Chain(7, TS('d'), NS('Y'), TS('h'), TS('('), NS('W'), TS(')'), TS(';')),

			Rule::Chain(6, TS('t'), TS('i'), TS('='), NS('W'), TS(';'), NS('K')),
			Rule::Chain(5, TS('i'), TS('='), NS('W'), TS(';'), NS('K')),	
			Rule::Chain(4, TS('t'), TS('i'), TS(';'), NS('K')),	
			Rule::Chain(4, TS('i'), NS('F'), TS(';'), NS('K')),
			Rule::Chain(4, TS('p'), NS('F'), TS(';'), NS('K')),
			Rule::Chain(4, TS('o'), NS('B'), TS(';'), NS('K')),				
			Rule::Chain(4, TS('b'), NS('B'), TS(';'), NS('K')),	
			Rule::Chain(6, TS('?'), TS('('), NS('W'), TS(')'), NS('R'), NS('K')),
			Rule::Chain(6, TS('h'), TS('('), NS('W'), TS(')'), NS('Y'), NS('K')),
			Rule::Chain(8, TS('d'), NS('Y'), TS('h'), TS('('), NS('W'), TS(')'), TS(';'), NS('K'))
		),

/*11*/		Rule(NS('B'), GRB_ERROR_SERIES + 11, 8, //Выводы;
			Rule::Chain(1, TS('i')),			
			Rule::Chain(1, TS('l')),
			Rule::Chain(2, TS('i'), NS('F')),
			Rule::Chain(2, TS('p'), NS('F')),

			Rule::Chain(3, TS('i'), TS('+'), NS('B')),
			Rule::Chain(3, TS('l'), TS('+'), NS('B')),	
			Rule::Chain(4, TS('p'), NS('F'), TS('+'), NS('B')),
			Rule::Chain(4, TS('p'), NS('F'), TS('+'), NS('B'))
		),

/*12*/	Rule(NS('Y'), GRB_ERROR_SERIES + 12, 6, // Тело конструкции
			Rule::Chain(6, TS('{'), NS('K'), TS('e'), NS('W'), TS(';'), TS('}')),
			Rule::Chain(5, TS('{'), NS('K'), TS('e'), TS(';'), TS('}')),
			Rule::Chain(3, TS('{'), NS('K'), TS('}')),
			Rule::Chain(5, TS('{'), TS('e'), NS('W'), TS(';'), TS('}')),
			Rule::Chain(4, TS('{'), TS('e'), TS(';'), TS('}')),
			Rule::Chain(2, TS('{'), TS('}'))
		),
/*13*/	Rule(NS('V'), GRB_ERROR_SERIES, 2, // Переменные	
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l'))
		),

/*14*/	Rule(NS('U'), GRB_ERROR_SERIES, 2, // Функция			
			Rule::Chain(2, TS('i'), NS('F')),
			Rule::Chain(2, TS('p'), NS('F'))
		)
	);
}