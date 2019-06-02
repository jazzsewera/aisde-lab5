#ifndef LIST_H
#define LIST_H

#include "ListElement.h"

// Lista jednokierunkowa obiektów klasy szablonowej ListElement<Type>.
// Umożliwia dodawanie elementów na końcu i pobieranie kolejnych elementów
// (wskazywanych przez 'current').

template <typename Type>
class List {
	private:
		// Wskazuje odpowiednio: element początkowy, ostatni i bieżący
		// (zwracany przez getNext())
		ListElement<Type> *first, *last, *current;
	public:
		List();
		~List();
		void append(ListElement<Type> *newElement);	// Dodaje element na końcu listy.
		void append(Type content); // 'Opakowanie' powyższej funkcji.
		bool hasMoreElements() const;    // Zwraca true, gdy 'current' wskazuje na ListElement.
		Type getNext(); // Kolejne wywołania zwracają kolejne elementy listy (nie usuwając ich).
		Type getCurrent(); // Zwraca zawartośc bieżącego elementu listy. 
		void rewind();  // Powrót do początku listy. Umożliwia ponowne pobieranie kolejnych elementów.
		void print() const;
};

template <typename Type>
List<Type>::List() {
	first   = nullptr;
	current = nullptr;
	last    = nullptr;
}

template <typename Type>
List<Type>::~List() {
	ListElement<Type>* next = first;
	if (first != nullptr) {
		current = first;
		while ((next = current->getNext()) == true) {
			delete current;
			current = next;
		}
		delete current;
	}
}

template <typename Type>
void List<Type>::append(ListElement<Type> *newElement) {
	// Jeśli lista jest pusta...
	if (first == nullptr) {
		first   = newElement;
		current = newElement;
	}
	else
		last->setNext(newElement);
	last = newElement;
}

// Wersja powyższej funkcji, pozwalająca zapomnieć o klasie ListElement<Type>
// przy dodawaniu kolejnych elementów.
template <typename Type>
void List<Type>::append(Type content) { append(new ListElement<Type>(content)); }

template <typename Type>
bool List<Type>::hasMoreElements() const { return current != nullptr; }

template <typename Type>
Type List<Type>::getNext() {
	if (current == nullptr)
		return nullptr;
	ListElement<Type> *currentElement = current;
	current = currentElement->getNext();
	return currentElement->getContent();
}

template <typename Type>
Type List<Type>::getCurrent() {
	return current->getContent();
}

template <typename Type>
void List<Type>::rewind() { current = first; }

template <typename Type>
void List<Type>::print() const {
	ListElement<Type> *currentElement = first;
	while (currentElement != nullptr) {
		currentElement->print();
		currentElement = currentElement->getNext();
	}
}

#endif // LIST_H
