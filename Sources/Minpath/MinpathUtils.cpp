/**************************************************************/
/*                          AISDE - lab. 5                    */
/**************************************************************/

#include <cstring>
#include <cctype>
#include <cstdlib>

#include "Exception.h"
#include "MinpathUtils.h"

// Dodaje 2 liczby. A wszystko dlatego,
// ze nieskonczoność + coś to wciąż nieskonczoność.
ulong Add(ulong x, ulong y) {
	if (x == INF || y == INF)
		return INF;
	else
		return x + y;
}

// Konwertuje łańcuch znaków na liczbę calkowitą (int).
int String2Int(const char *str) {
	for (unsigned int i = 0; i < strlen(str); i++) {
	 	if (isdigit(str[i]) == 0) {
			throw Exception(NumberFormatException, str);
			break;
		}
	}
	return atoi(str);
}

time_point CurrentTime(void) { return std::chrono::steady_clock::now(); }

long TimeDiffMicroseconds(time_point from_time, time_point to_time) {
	return std::chrono::duration_cast<std::chrono::microseconds>(to_time - from_time).count();
}
