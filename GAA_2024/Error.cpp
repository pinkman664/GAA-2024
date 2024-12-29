#include "stdafx.h"

namespace Error
{
	ERROR errors[ERROR_MAX_ENTRY] =
	{
		//системные ошибки
		ERROR_ENTRY(0, "SYSTEM Недопустимый код ошибки."),
		ERROR_ENTRY(1, "SYSTEM Системный сбой."),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40),
		ERROR_ENTRY_NODEF10(50), ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),



		ERROR_ENTRY(100, "SYSTEM Параметр -in должен быть задан."),
		ERROR_ENTRY(101, "SYSTEM Превышена длина входного параметра."),
		ERROR_ENTRY(102, "SYSTEM Ошибка при открытии файла с исходным кодом(-in)."),
		ERROR_ENTRY(103, "SYSTEM Ошибка при создании файла протокола(-log)."),
		ERROR_ENTRY_NODEF(104), ERROR_ENTRY_NODEF(105), ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107), ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY_NODEF10(110), ERROR_ENTRY_NODEF10(120), ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140),
		ERROR_ENTRY_NODEF10(150), ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),



		//лексические ошибки
		ERROR_ENTRY(200, "Недопустимый символ в исходном файле(-in)."),
		ERROR_ENTRY(201, "Неизвестная последовательность символов."),
		ERROR_ENTRY(202, "Превышен размер таблицы лексем."),
		ERROR_ENTRY(203, "Превышен размер таблицы идентификаторов."),
		ERROR_ENTRY_NODEF(204), ERROR_ENTRY_NODEF(205), ERROR_ENTRY_NODEF(206), ERROR_ENTRY_NODEF(207), ERROR_ENTRY_NODEF(208), ERROR_ENTRY_NODEF(209),
		ERROR_ENTRY_NODEF10(210), ERROR_ENTRY_NODEF10(220), ERROR_ENTRY_NODEF10(230), ERROR_ENTRY_NODEF10(240),
		ERROR_ENTRY_NODEF10(250), ERROR_ENTRY_NODEF10(260), ERROR_ENTRY_NODEF10(270), ERROR_ENTRY_NODEF10(280), ERROR_ENTRY_NODEF10(290),


		//семантические ошибки
		ERROR_ENTRY(300, "Идентификатор не объявлен."),
		ERROR_ENTRY(301, "Отсутствует точка входа main."),
		ERROR_ENTRY(302, "Несколько точек входа main."),
		ERROR_ENTRY(303, "Не указан тип идентификатора при объявлении."),
		ERROR_ENTRY(304, "Отсутствует ключевое слово при объявлении."),
		ERROR_ENTRY(305, "Попытка переопределения идентификатора."),
		ERROR_ENTRY(306, "Превышено максимальное количество параметров функции"),
		ERROR_ENTRY(307, "Слишком много параметров в вызове функции."),
		ERROR_ENTRY(308, "Кол-во ожидаемых функцией и передаваемых параметров не совпадают."),
		ERROR_ENTRY(309, "Несовпадение типов передаваемых параметров."),
		ERROR_ENTRY(310, "Использование пустого строкового литерала недопустимо."),
		ERROR_ENTRY(311, "Обнаружен символ \'\"\'. Возможно, не закрыт строковый литерал."),
		ERROR_ENTRY(312, "Превышен размер строкового литерала."),
		ERROR_ENTRY(313, "Недопустимый целочисленный литерал."),
		ERROR_ENTRY(314, "Типы данных в выражении не совпадают с типом переменной."),
		ERROR_ENTRY(315, "Тип функции и возвращаемого значения не совпадают."),
		ERROR_ENTRY(316, "Недопустимое строковое выражение после =."),
		ERROR_ENTRY(317, "Не закрыт блок main."),
		ERROR_ENTRY(318, "Логическое сравнение со строками может быть только == или !=."),
		ERROR_ENTRY(319, "В выражений возник конфликт типов. Невозможно приведение типов."),
		ERROR_ENTRY(320, "Деление на ноль недопустимо."),
		ERROR_ENTRY(321, "Операция ! и ~ допустимо только для операнда целочисленного и логического типа."),
		ERROR_ENTRY(322, "В условном выражении возникла ошибка."),
		ERROR_ENTRY(323, "Не во всех случаях в функции имеет return."),
		ERROR_ENTRY(324, "Нельзя использовать рекурсию, если не во всех случах в функции имеет return."),
		ERROR_ENTRY(325, "В Main нельзя использовать return."),
		ERROR_ENTRY_NODEF(326), ERROR_ENTRY_NODEF(327), ERROR_ENTRY_NODEF(328), ERROR_ENTRY_NODEF(329),
		ERROR_ENTRY_NODEF10(330),ERROR_ENTRY_NODEF10(340),ERROR_ENTRY_NODEF10(350),
		ERROR_ENTRY_NODEF10(360),ERROR_ENTRY_NODEF10(370),ERROR_ENTRY_NODEF10(380),ERROR_ENTRY_NODEF10(390),
		ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),

		//синтаксические ошибки
		ERROR_ENTRY(600, ". Неверная структура программы."),
		ERROR_ENTRY(601, ". Неверная структура тела функции."),
		ERROR_ENTRY(602, ". Неверная структура тела процедуры."),
		ERROR_ENTRY(603, ". Неверная структура списка параметров функции."),
		ERROR_ENTRY(604, "в списке параметров функции."),
		ERROR_ENTRY(605, ". Неверная структура списка фактических параметров функции."),
		ERROR_ENTRY(606, "в списке фактических параметров функции."),
		ERROR_ENTRY(607, ". Неверная условная конструкция."),
		ERROR_ENTRY(608, " в выражении. Такой оператор не существует."),
		ERROR_ENTRY(609, " в выражении."),
		ERROR_ENTRY(610, "в программе. Неверная структура операторы программы."),
		ERROR_ENTRY(611, "в операторе Write/Writeline. Неверно передано параметров оператора."),
		ERROR_ENTRY(612, ". Неверная структура тела конструкции."),
		ERROR_ENTRY(613, "в индентификаторе/литерале."),
		ERROR_ENTRY_NODEF(614), ERROR_ENTRY_NODEF(615),
		ERROR_ENTRY_NODEF(616),ERROR_ENTRY_NODEF(617), ERROR_ENTRY_NODEF(618), ERROR_ENTRY_NODEF(619),
		ERROR_ENTRY_NODEF10(620), ERROR_ENTRY_NODEF10(630), ERROR_ENTRY_NODEF10(640), ERROR_ENTRY_NODEF10(650),
		ERROR_ENTRY_NODEF10(660), ERROR_ENTRY_NODEF10(670), ERROR_ENTRY_NODEF10(680), ERROR_ENTRY_NODEF10(690),
		ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
	};

	ERROR GetError(int id)
	{
		if (id < 0 || id > ERROR_MAX_ENTRY)
			return errors[0];
		else
			return errors[id];
	}

	ERROR GetError(int id, int line, int cells)
	{
		ERROR err = GetError(id);
		err.position.cells = cells;
		err.position.line = line;
		return err;
	}
};