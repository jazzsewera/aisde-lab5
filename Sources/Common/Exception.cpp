#include <iostream>

#include "Exception.h"

Exception::Exception(ExceptionCode c, const std::string& msg) : code(c), param(msg) { }

Exception::~Exception(void) { }

ExceptionCode Exception::getCode(void) const { return code; }
	
std::string Exception::getParam(void) const { return param; }

// Obsluga sytuacji wyjatkowej
void HandleException(const Exception& exception) {
	ExceptionCode code    = exception.getCode();
	std::string parameter = exception.getParam();
	
	switch(code) {
		case ImproperMinpathCallException:
			std::cout << " Prawidłowe wywołanie:\n"; 
			std::cout << "   " << parameter << " plik_z_grafem źrodło ujście\n\n";
			std::cout << " lub (gdy chcesz wyznaczyć odległości 'każdy do każdego'): \n";
			std::cout << "   " << parameter << " plik_z_grafem\n\n"; 
			break;		
		case ImproperMSTCallException:
			std::cout << " Prawidłowe wywołanie:\n"; 
			std::cout << "   " << parameter << " plik_z_grafem\n\n";
			break;
		case FileOpenException: 
			std::cout << "\n Nie mogę otworzyć pliku '" << parameter << "'\n";
			break;
		case FileCreateException: 
			std::cout << "\n Nie mogę stworzyć pliku '" << parameter << "'\n";
			break;
		case FileReadException:
			std::cout << "\n Nieprawidłowy format pliku '" << parameter << "'\n";
			break;
		case NumberFormatException: 
			std::cout << "\n Nieprawidłowy parametr '" << parameter << "'\n";
			break;
		case UnknownVertexException:
			std::cout << "\n W grafie nie ma wierzchołka o numerze " << parameter << "\n";
			break;
		case BadSeedException:
			std::cout << "\n Proszę nadać stałej SEED wartość swojego numeru albumu (plik " << parameter << ")\n";
			break;
		case ZeroVerticesException:
			std::cout << "\n Liczba wierzchołków musi być dodatnia!\n";
			break;
		case ZeroEdgesException:
			std::cout << "\n Liczba krawędzi musi być dodatnia!\n";
			break;
		case WrongCapacityException:			
			std::cout << "\n Pojemność struktury " << parameter << " powinna być dodatnia!\n";
			break;
		case MessageException:
			std::cout << "\n " << parameter << "\n";
			break;			
		default:
			std::cout << "\n Wystąpiła niezidentyfikowana sytuacja wyjątkowa!\n";
	}
	exit (EXIT_FAILURE);
}
