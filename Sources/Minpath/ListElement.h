#ifndef LIST_ELEMENT_H
#define LIST_ELEMENT_H

#include <iostream>

// Element listy jednokierunkowej (np. klasy szablonowej List<>).
// Sklada się z zawartoęci dowolnego typu 'Type' i wskaźnika
// na kolejny element listy. Jeśli mamy do czynienia z ostatnim elementem
// listy, wskaźnik przyjmuje wartość 'nullptr'.

template <typename Type>
class ListElement {

	private:
		Type content;
		ListElement<Type> *nextElement;

	public:
		ListElement(Type c) : content(c), nextElement(nullptr) { } ;
		Type getContent() const { return content; }
		ListElement *getNext() const { return nextElement; }
		void setNext(ListElement* element) { nextElement = element; }
		void print() const { std::cout << content << std::endl; }
};

#endif // LIST_ELEMENT_H
