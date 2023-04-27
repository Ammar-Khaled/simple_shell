#include <stdio.h>
#include "../includes/string.h"

int main(void) {
	char s1[] = "  hello   world    ", *s2;

	printf("strtrim:\n[%s]\n", s1);
	s2 = _strtrim(s1);
	printf("[%s]\n", s2);
	return 0;
}
