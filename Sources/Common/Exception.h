#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

// Typy wyjatkow
typedef enum {
	ImproperMinpathCallException,
	ImproperMSTCallException,
	WrongCapacityException,
	FileOpenException,
	FileCreateException,
	FileReadException,
	NumberFormatException,
	UnknownVertexException,
	BadSeedException,
	ZeroVerticesException,
	ZeroEdgesException,
	MessageException
} ExceptionCode;

// Implementacja wyjatku
class Exception {
	private:
		ExceptionCode code;  // Kod zależny od rodzaju sytuacji wyjątkowej.
		std::string   param; // Dodatkowy parametr - moze być wykorzystany
		                     // przez metodą obsługi sytuacji wyjątkowej.
	public:
		Exception(ExceptionCode c, const std::string& p);
		virtual ~Exception(void);
		ExceptionCode getCode(void) const;
		std::string   getParam(void) const;
};
	
void HandleException(const Exception&);	
	
#endif // EXCEPTION_H
