#include "../includes/utils.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
	char *s1 = "hello", *s2, s3[] = "abc", word[] = "hello world";
	size_t size;

	/* test strlen */
	size = _strlen(s1);
	printf("strlen: out: [%lu], exp: [5]\n\n", size);

	/* test strcmp */
	printf("strcmp: [hello] vs [hello] = %d\n", _strcmp("hello", "hello"));
	printf("strcmp: [hello] vs [hel] = %d\n", _strcmp("hello", "hel"));
	printf("strcmp: [hel] vs [hello] = %d\n\n", _strcmp("hel", "hello"));

	/* test strcmp */
	printf("strncmp(1): [hi] vs [he] = %d\n", _strncmp("hi", "he", 1));
	printf("strncmp(2): [hi] vs [he] = %d\n\n", _strncmp("hi", "he", 2));

	/* test strdup */
	s2 = _strdup(s1);
	printf("strdup:\n%s\n%s\n\n", s1, s2);
	free(s2);

	/* test strcpy */
	s2 = malloc(4);
	if (!s2) {
		fprintf(stderr, "failed to allocate memory for s2 at strcpy\n");
		return 1;
	}
	if (!_strcpy(s2, s3)) {
		fprintf(stderr, "failed to copy from s3 to s2 at strcpy\n");
		return 1;
	}
	printf("strcpy: [%s] >> [%s]\n\n", s3, s2);

	/* test strchr */
	printf("strchr(e): hello => %s\n", _strchr("hello", 'e'));
	printf("strchr(a): hello => %s\n\n", _strchr("hello", 'a'));

	/* test strspn */
	printf("strspn(space): [hello world] => %lu\n", _strspn("hello world", " "));
	printf("strspn(space): [     hello] => %lu\n\n", _strspn("     hello", " "));
	
	/* test strtok */
	printf("strtok(space): hello world\n");
	s2 = _strtok(word, " ");
	while(s2) {
		printf("[%s]\n", s2);
		s2 = _strtok(NULL, " ");
	}
	putchar(10);

	/* test strrev */
	printf("strrev: [%s] => [", word);
	_strrev(word, 5);
	printf("%s]\n\n", word);

	/* test print family */
	printf("Print Family:\n");
	printo("5 + 5 = ");
	printuo(10);
	printo("\n\n");

	printo("Test custom realloc:\n");
	s1 = malloc(3);
	if (!s1) {
		fprintf(stderr, "failed to allocate memory");
		return 1;
	}
	s1[0] = 'h';
	s1[1] = 'i';
	s1[2] = 0;
	printo(s1);
	printo("\n");

	s1 = _realloc(s1, 3, 4);
	if (!s1) {
		fprintf(stderr, "failed to reallocate memory");
		return 1;
	}
	s1[2] = '!';
	s1[3] = 0;
	printo(s1);
	printo("\n");

	return 0;
}
