#include <iostream>
#include <conio.h>
#include <string>
#include <strstream>
#include <ctime>
#include <cstring>
extern "C"
{
	int __stdcall Getlength(char* str) {
		if (str == nullptr) {	
			return 0; 
		}
		return strlen(str); 
	}


	int __stdcall power(int a, int b)
	{
		return pow(a, b);
	}

	int __stdcall squere(int a)
	{
		
		a = sqrt(a);

	
		return a;
	}

	

	char* __stdcall inttostr(int a) {
		std::strstream stream;

		stream << a << '\0';

		return stream.str();
	}

	char* __stdcall codetochar(int a) {
		char* c = new char[1];
		*c = (char)a;
		return c;
	}



	int __stdcall inputInt() {
		int result;

		std::cin >> result;

		return result;
	}
	char* __stdcall inputString() {
		char* buf = (char*)calloc(1024, 1);
		char* str;
		int len;

		std::cin >> buf;

		len = strlen(buf);

		str = (char*)calloc(len + 1, 1);

		for (short i = 0; i < len; i++)
			str[i] = buf[i];

		str[len] = '\0';

		return str;
	}
	char* __stdcall inputChar() {
		char symbol = _getch();
		char* result = (char*)calloc(2, 1);

		std::cout << symbol << std::endl;

		result[0] = symbol;
		result[1] = '\0';

		return result;
	}

	int __stdcall outnum(int value)
	{
		std::cout << value;

		return 0;
	}
	int __stdcall outstr(char* ptr)
	{
		if (ptr == nullptr)
			std::cout << std::endl;

		for (int i = 0; ptr[i] != '\0'; i++)
			std::cout << ptr[i];

		return 0;
	}

	int __stdcall outbool(int value)
	{
		if (value)
			std::cout << "true";
		else
			std::cout << "false";
		return 0;
	}
	int __stdcall outboolline(int value)
	{
		if (value)
			std::cout << "true" << std::endl;
		else
			std::cout << "false" << std::endl;
		return 0;
	}

	int __stdcall outnumline(int value)
	{
		std::cout << value << std::endl;

		return 0;
	}
	int __stdcall outstrline(char* ptr)
	{
		if (ptr == nullptr)
			std::cout << std::endl;

		for (int i = 0; ptr[i] != '\0'; i++)
			std::cout << ptr[i];

		std::cout << std::endl;

		return 0;
	}

	int __stdcall system_pause()
	{
		system("pause");
		return 0;
	}
}