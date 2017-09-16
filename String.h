/*
 * String.h
 *
 *  Created on: Jun 25, 2015
 *      Author: alexandruserban
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdarg.h>

#define TRUE 1
#define FALSE 0

/*
 *String structure
 */
typedef struct String {

	char * str;

	int (*equals)(struct String*, char*);
	int (*compare)(struct String*, char*);
	int (*length)(struct String *);
	struct String (*clone)(struct String *);

	void (*concat)(struct String*, char*);
	void (*insertAtIndex)(struct String*, int, char*);

	int (*containsSubstring)(struct String*, char*);
	int (*containsIgnoreCase)(struct String *, char *);

	struct String (*substringTo)(struct String *, int);
	struct String (*substringFrom)(struct String *, int);
	struct String (*substringFromTo)(struct String *, int, int);

	char (*charAtIndex)(struct String *, int);

	struct String *(*split)(struct String *, char*, int*);
	struct String (*compose)(int, ...);

	void (*toUpperCase)(struct String *);
	void (*toLowerCase)(struct String *);
	char * (*toCharArray)(struct String *);

	unsigned (*toInt)(struct String *);
	struct String (*toString)(int);

	int (*indexFirstOcc)(struct String *, char *);
	int (*indexLastOcc)(struct String *, char *);

	int (*startsWith)(struct String *, char *);
	int (*endsWith)(struct String *, char *);

	int (*replaceAllOcc)(struct String *, char *, char *);
	int (*replaceFirstOcc)(struct String *, char *, char *);
	int (*replaceLastOcc)(struct String *, char *, char*);
	int (*replaceFromIndex)(struct String *, char *, int);
	int (*replaceToIndex)(struct String *, char *, int);
	int (*replaceFromIndexToIndex)(struct String *, char *, int, int);

	void (*removeCharsInSeq)(struct String *, char *);
	void (*removeCharsNotInSeq)(struct String *, char *);

	void (*reverse)(struct String *);
	void (*trim)(struct String *);

	void (*myfree)(struct String*);
	void (*print)(struct String*);

} String;

/*
 * Necessary function headers.
 */
String new_String(char * str);

/*
 * Returns a new String that ends at given index
 */
String substringToIndex(String * s, int index_to) {

	if (!(index_to >= 0 && index_to < strlen(s->str))) {
		printf("substring : invalid indexes\n");
		return *s;
	}

	char * newstr = strdup(s->str);

	newstr[index_to + 1] = '\0';
	String snew = new_String(newstr);
	free(newstr);
	return snew;

}
/*
 * Returns a new String that starts from a given index
 */
String substringFromIndex(String * s, int index_from) {

	if (!(index_from >= 0 && index_from < strlen(s->str))) {
		printf("substringFrom : invalid indexes\n");
		return *s;
	}

	char * newstr = s->str + index_from;

	return new_String(newstr);

}

/*
 * Returns substring between 2 given indexes
 */
String substringFromTo(String * s, int index_from, int index_to) {

	if (!(index_from < index_to && 0 <= index_from && 0 <= index_to
			&& index_from < strlen(s->str) && index_to < strlen(s->str))) {
		printf("substringFromTo : invalid indexes\n");
		return *s;
	}

	char * new_str = strdup(s->str);
	char * substr = new_str + index_from;
	new_str[index_to + 1] = '\0';
	String new_s = new_String(substr);
	free(new_str);
	return new_s;

}

/*
 * Return char pointed by given index
 */
char charAtIndex(String * s, int index) {
	if (index > -1 && index < strlen(s->str)) {
		return s->str[index];
	}
	printf("charAtIndex : invalid index");
	return 'E';

}

/*
 * Returns an Array of Strings delimited by specific sequence.
 * The Array must be freed after usage.
 * let ar be the array :
 * for (i = 0; i <string_number; ++i) {
 freeStrings(1 , ar[i]);
 }
 free(ar);
 *
 * nr_str is used to store the number of Strings
 */
String * split(String * s, char * seq, int * nr_str) {

	int delims = 0;
	char * curent = s->str;
	while (strstr(curent, seq) != NULL) {
		curent = strstr(curent, seq);
		curent += strlen(seq);
		delims++; //count the number of delimiters
	}

	String * array = calloc(sizeof(String), delims + 1);

	char * l_ind = s->str; //left index
	char* r_ind = s->str; //right index
	int string_nr = 0;

	/* creates the Strings between the delimiters*/
	while (strstr(r_ind, seq) != NULL) { //for each delimiter
		r_ind = strstr(r_ind, seq); //move the right index to its beginning

		char * new_str = strdup(l_ind); //create a new string
		new_str[r_ind - l_ind] = '\0'; //set the end

		if (strlen(new_str) > 0) { //if the new string exists
			array[string_nr++] = new_String(new_str); //create new String
			//and only increment if new String was added;
			free(new_str);

		} else { //free memory
			free(new_str);
		}

		l_ind = r_ind + strlen(seq); //move the left index to the end of the delimiter
		r_ind += strlen(seq); //increment the right index in order to find the next occurrence

		if (strstr(r_ind, seq) == NULL && s->str[l_ind - s->str] != '\0') { //if there is something after last delimiter
			array[string_nr++] = new_String(l_ind); //create new String
		}
	}

	*nr_str = string_nr;
	return array;
}

/*
 * Returns a String composed of given substrings.
 * Takes as arguments the number of substrings and a variable number of char arrays.
 */
struct String compose(int nr_args, ...) {

	va_list valist;

	int i, len = 0;
	char * s;

	va_start(valist, nr_args); //initialize list of Strings

	for (i = 0; i < nr_args; i++) {
		s = va_arg(valist, char *); //next string element
		len += (int) strlen(s);
	}
	len++; //adds 1 for string terminator

	char * news = calloc(sizeof(char), len);

	va_start(valist, nr_args); //reinitialize list of Strings
	for (i = 0; i < nr_args; i++) {
		s = va_arg(valist, char *); //next string element
		strcat(news, s);
	}

	news[len - 1] = '\0';
	String ns = new_String(news);
	free(news);

	va_end(valist);

	return ns;
}

/*
 * Makes the String Lower Case
 */
void toLowerCase(String * s) {

	int i;
	for (i = 0; s->str[i]; i++) {
		if ('A' <= s->str[i] && s->str[i] <= 'Z') {
			s->str[i] += 32; //26 letters + 6 more symbols
		}
	}
}

/*
 * Makes the String Upper Case
 */
void toUpperCase(String * s) {

	int i;
	for (i = 0; s->str[i]; i++) {
		if ('a' <= s->str[i] && s->str[i] <= 'z') {
			s->str[i] -= 32; //26 letters + 6 more symbols
		}
	}

}

/*
 * Return the char array
 */
char* toCharArray(String * s) {
	return s->str;
}

/*
 * Returns an unsigned value if the string is a number
 */
unsigned toInt(struct String * s) {

	if (s->str == NULL) {
		printf("toInt : error invalid character");
		return 0;
	}

	unsigned rez = 0;
	int i;
	for (i = 0; i < strlen(s->str); ++i) {
		if (s->str[i] - '0' < 0 || s->str[i] - '0' > 9) {
			printf("toInt : error invalid digit : %c\n", s->str[i]);
			return 0;
		}
		rez *= 10;
		rez += s->str[i] - '0';
	}
	return rez;
}

/*
 *Returns a String containing the number
 */
String toString(int nr_str) {
	int c = nr_str, cnt = 0;

	/* count number of digits */
	while (c != 0) {
		c /= 10;
		cnt++;
	}

	char * news = calloc(sizeof(char), cnt + 1);

	cnt = 0;
	/* create string by dividing the nubmer */
	while (nr_str) {
		news[cnt++] = '0' + nr_str % 10;
		nr_str /= 10;
	}

	char aux;
	int i, j;
	/* reverses the string */
	for (i = 0, j = cnt - 1; i < cnt / 2; ++i, j--) {
		aux = news[i];
		news[i] = news[j];
		news[j] = aux;
	}

	news[cnt] = '\0';
	String s = new_String(news);
	free(news);
	return s;
}

/*
 * Length of given string.
 */
int length(String * s) {
	return strlen(s->str);
}

/*
 * Clone
 */
String clone(String * s) {
	String news = new_String(s->str);
	return news;
}

/*
 * strcmp
 * Returns 0 if the strings are equal
 * > 0 if the first string is bigger
 * < 0 if the second string is bigger
 */
int compare(String * s, char * str) {
	return strcmp(s->str, str);
}

/*
 * Returns TRUE(1) if Strings are equal , FLASE(0) otherwise
 */
int equals(String * s, char * str) {
	if (strcmp(s->str, str) == 0) {
		return TRUE;
	}
	return FALSE;
}

/*
 * strstr
 * Returns TRUE(1) if the substring is contained by the String
 */
int containsSubstring(String * s, char * substr) {

	if (strstr(s->str, substr) != NULL) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/*
 * Check if String contains substring
 */
int containsIgnoreCase(String * s, char * substr) {

	char * str = strdup(s->str);
	char * csubstr = strdup(substr);

	int i;
	for (i = 0; str[i]; i++) {
		if ('A' <= str[i] && str[i] <= 'Z') {
			str[i] += 32; //26 letters + 6 more symbols
		}
	}

	for (i = 0; csubstr[i]; i++) {
		if ('A' <= csubstr[i] && csubstr[i] <= 'Z') {
			csubstr[i] += 32;
		}
	}

	if (strstr(str, csubstr) != NULL) {
		free(csubstr);
		free(str);
		return TRUE;
	} else {
		free(csubstr);
		free(str);
		return FALSE;
	}
}

/*
 * Receives 2 Strings and concatenates them
 * Returns a new String
 */
void concat(String * s, char * str) {

	char * old = s->str;

	s->str = calloc(sizeof(char), strlen(s->str) + strlen(str) + 1);
	strcpy(s->str, old);
	free(old);

	strcat(s->str, str);

}

/*
 * Inserts substring at given index
 */
void insertAtIndex(String* s, int index, char* str) {

	char * news = calloc(sizeof(char), strlen(s->str) + strlen(str) + 1);
	int i;

	for (i = 0; i < index; i++) {
		news[i] = s->str[i];
	}
	strcat(news, str);
	strcat(news, s->str + index);
	free(s->str);
	s->str = news;

}

void print(String * s) {
	printf("%s", s->str);
}

/**
 * Free memory
 */
void myfree(String * s) {
	if (s->str != NULL) {
		free(s->str);
	}
}

/*
 * Returns the index where the first occurrence if found.
 * Returns -1 if no occurrence is found.
 */
int indexFirstOcc(String* s, char * substr) {
	char * ind = strstr(s->str, substr);

	if (ind != NULL) {
		return ind - s->str;
	} else {
		return -1;
	}
}

/*
 * Returns an index to the last occurrence of the given substring.
 * Returns -1 if no occurrence is found.
 */
int indexLastOcc(String* s, char * substr) {

	char *ind = s->str;

	while (strstr(ind, substr) != NULL) {

		ind = strstr(ind, substr);
		ind += strlen(substr);
	}

	if (ind != NULL) {
		return ind - s->str - strlen(substr);
	} else {
		return -1;
	}
}

/*
 * Returns TRUE(1) if String starts with given substring and FALSE(0) otherwise.
 */
int startsWith(String * s, char* sub) {

	int cnt = strlen(sub), i;
	for (i = 0; i < cnt; ++i) {
		if (sub[i] != s->str[i]) {
			return FALSE;
		}
	}
	return TRUE;
}

/*
 * Returns TRUE(1) if String ends with given substring and FALSE(0) otherwise.
 */
int endsWith(String * s, char* sub) {

	int cnt = strlen(sub);
	int i, j = strlen(sub) - 1, k = strlen(s->str) - 1;

	for (i = 0; i < cnt; ++i, j--, k--) {
		if (sub[j] != s->str[k]) {
			return FALSE;
		}

	}
	return TRUE;
}

/*
 * Replace all occurrences with given substring
 * Returns the number of replacements made.
 */
int replaceAllOcc(String * s, char * oc, char * new_oc) {
	int occurrences = 0;
	char * ind = s->str;

	/*Count occurrences*/
	while ((ind = strstr(ind, oc))) {
		ind += strlen(oc);
		occurrences++;
	}

	char * news = calloc(sizeof(char),
			strlen(s->str) - occurrences * strlen(oc)
					+ occurrences * strlen(new_oc) + 1);

	int start = 0, end = 0, ioc = 0, gap_sum = 0;

	for (end = 0; end < strlen(s->str); ++end) { //traverse initial string

		if (s->str[end] != oc[ioc]) { //if current character is different from one in occurrence
			ioc = 0; //reset occurrence index
			do { //put characters in final string from where we left
				news[start + gap_sum] = s->str[start];
				start++; //increment left index
			} while (start < end);

		} else { //if encounter a character contained in the sequence that has to be replaced

			ioc++;
			if (ioc == strlen(oc)) { //found an occurrence that needs replacement
				ioc = 0; //reset occurrence index
				strcat(news, new_oc); ////concatenate to final string the replacement string
				gap_sum += strlen(new_oc) - strlen(oc); //count the gap between the old occurrence and the new string
				//gap sum is needed in order to put the following character(from current string to final string) at the right idnex
				start = end + 1;			//increment the left index
			}
		}
	}

	free(s->str);
	news[end + gap_sum] = '\0';
	s->str = news;
	return occurrences;				//return number of replacements
}

/*
 * Replace first occurrence with give string.
 * If no occurrence is found then no replacement was done => returns 0
 * If any error occurs returns -1
 */
int replaceFirstOcc(String * s, char * oc, char * new_oc) {

	int end = 0, start = 0, ioc = 0, gap_sum = 0;
	int replaced = 0;
	char * news = calloc(sizeof(char),
			strlen(s->str) - strlen(oc) + strlen(new_oc) + 1);

	for (end = 0; end < strlen(s->str); ++end) { //traverse initial string

		if (s->str[end] != oc[ioc] || replaced == 1) { //if current character is different from one in occurrence or replacement already done
			ioc = 0; //reset occurrence index
			do { //put characters in final string from where we left
				news[start + gap_sum] = s->str[start];
				start++; //increment left index
			} while (start < end);

		} else { //if encounter a character contained in the sequence that has to be replaced

			ioc++;
			if (ioc == strlen(oc)) { //found an occurrence that needs replacement
				ioc = 0; //reset occurrence index
				strcat(news, new_oc); ////concatenate to final string the replacement string
				gap_sum += strlen(new_oc) - strlen(oc); //count the gap between the old occurrence and the new string
				//gap sum is needed in order to put the following character(from current string to final string) at the right idnex
				start = end + 1;			//increment the left index
				replaced = 1;
			}
		}
	}

	free(s->str);
	news[end + gap_sum] = '\0';
	s->str = news;

	if (replaced == 1) {
		return 1;
	} else {
		return 0;
	}

}

/*
 * Replace the last occurrence of given substring.
 * Returns 0 if no replacement was made and 1 otherwise.
 */
int replaceLastOcc(String * s, char * oc, char * new_oc) {
	int occs = 0;
	char * ind = s->str;

	/*Find last occurrence*/
	while (strstr(ind, oc)) {
		ind = strstr(ind, oc);
		ind += strlen(oc);
		occs++;
	}

	char * news = calloc(sizeof(char),
			strlen(s->str) - strlen(oc) + strlen(new_oc) + 1);

	int start = 0;
	while (s->str + start != (ind - strlen(oc))) {//fill new string until index gets to occurrence address
		news[start] = s->str[start];
		start++;
	}

	strcat(news, new_oc);
	strcat(news, ind);
	news[strlen(s->str) - strlen(oc) + strlen(new_oc)] = '\0';
	printf("news %s\n", news);
	free(s->str);
	s->str = news;

	return occs > 0 ? 1 : 0;
}

/*
 * Replace the substring starting from given index (inclusively).
 * Returns 0 if no replacement was made and 1 otherwise.
 */
int replaceFromIndex(String * s, char * substr, int index) {

	/* If index is invalid */
	if (index < 0)
		return 0;

	if (index >= strlen(s->str)) {
		return 0;
	}

	char * new = calloc(sizeof(char) , index + strlen(substr) + 1);
	memmove(new , s->str , index);
	strcat(new,substr);
	free(s->str);
	s->str = new;
	return 1;
}

/*
 * Replace the substring ending at given index (exclusively).
 * Returns 0 if no replacement was made and 1 otherwise.
 */
int replaceToIndex(String * s, char * substr, int index) {

	/*If index is invalid*/
	if (index < 0 || index > strlen(s->str) - 1) {
		return 0;
	}

	char * news = calloc(sizeof(char),
			strlen(substr) + strlen(s->str + index) + 1);

	strcat(news, substr);
	strcat(news, s->str + index);
	free(s->str);
	s->str = news;

	return 1;
}

/*
 * Replace substring delimited by the start index (inclusively) to the end index (exclusively).
 * Returns 0 if no replacement was made.
 */
int replaceFromIndexToIndex(String * s, char * substr, int start, int end) {

	if (start > end || start < 0 || start >= strlen(s->str) || end < 0
			|| end >= strlen(s->str)) {
		printf("replaceFromIndexToIndex : invalid index\n");
		return 0;
	}

	char * new = calloc(sizeof(char), start + strlen(substr) + strlen(s->str + end) +1);
	memmove(new,s->str , start);
	strcat(new , substr );
	strcat(new , s->str + end );

	free(s->str);
	s->str = new;

	return 0;
}

/*
 * Returns 1 if given sequence contains the character and 0 otherwise
 */
int inSeq(char c, char *seq) {
	int i;
	for (i = 0; i < strlen(seq); i++) {
		if (seq[i] == c) {
			return 1;
		}
	}
	return 0;
}

/*
 * Remove chars from given string that are in the sequence
 */
void removeCharsInSeq(String * s, char * seq) {
	char *news = calloc(sizeof(char), strlen(s->str) + 1);
	int i, j;
	for (i = 0, j = 0; i < strlen(s->str); ++i) {
		if (!(inSeq(s->str[i], seq))) {
			news[j] = s->str[i];
			j++;
		}
	}
	free(s->str);
	news[j] = '\0';
	s->str = news;
}

/*
 * Removes chars from given String that are not contained in the sequence
 */
void removeCharsNotInSeq(struct String * s, char * seq) {
	char *news = calloc(sizeof(char), strlen(s->str) + 1);
	int i, j;
	for (i = 0, j = 0; i < strlen(s->str); ++i) {
		if (inSeq(s->str[i], seq)) {
			news[j] = s->str[i];
			j++;
		}
	}
	free(s->str);
	news[j] = '\0';
	s->str = news;
}

/*
 * Reverses give String
 */
void reverse(struct String * s) {
	int i, j;
	char aux;
	for (i = 0, j = strlen(s->str) - 1; i < strlen(s->str) / 2; ++i, j--) {
		aux = s->str[i];
		s->str[i] = s->str[j];
		s->str[j] = aux;
	}
}

/*
 * Removes whitespace from both sides of a string.
 */
void trim(struct String * s) {

	int right = strlen(s->str) - 1;
	int leading = 0, trailing = 0;
	int i, j;

	/* Count leading white spaces */
	while (s->str[leading] == ' ') {
		leading++;
	}

	/* Count trailing white spaces */
	while (s->str[right] == ' ') {
		right--;
		trailing++;
	}

	char * news = calloc(sizeof(char), strlen(s->str) - leading - trailing + 1);

	/* Copy chars to new char array without leading and trailing white spaces */
	for (i = leading, j = 0; i <= right; ++i, ++j) {
		news[j] = s->str[i];
	}

	news[j] = '\0';
	free(s->str);
	s->str = news;

}

/**
 * Constructor function
 */
String new_String(char * str) {

	String s;
	s.str = strdup(str);

	s.equals = &equals;
	s.compare = &compare;
	s.clone = &clone;
	s.concat = &concat;
	s.insertAtIndex = &insertAtIndex;
	s.length = &length;

	s.substringTo = &substringToIndex;
	s.substringFrom = &substringFromIndex;
	s.substringFromTo = &substringFromTo;

	s.charAtIndex = &charAtIndex;
	s.split = &split;
	s.compose = &compose;

	s.containsSubstring = &containsSubstring;
	s.containsIgnoreCase = &containsIgnoreCase;

	s.toLowerCase = &toLowerCase;
	s.toUpperCase = &toUpperCase;
	s.toCharArray = &toCharArray;
	s.toInt = &toInt;
	s.toString = &toString;

	s.indexFirstOcc = &indexFirstOcc;
	s.indexLastOcc = &indexLastOcc;

	s.startsWith = &startsWith;
	s.endsWith = &endsWith;

	s.replaceAllOcc = &replaceAllOcc;
	s.replaceFirstOcc = &replaceFirstOcc;
	s.replaceLastOcc = &replaceLastOcc;
	s.replaceFromIndex = &replaceFromIndex;
	s.replaceToIndex = &replaceToIndex;
	s.replaceFromIndexToIndex = &replaceFromIndexToIndex;

	s.removeCharsInSeq = &removeCharsInSeq;
	s.removeCharsNotInSeq = &removeCharsNotInSeq;
	s.reverse = &reverse;
	s.trim = &trim;

	s.print = &print;
	s.myfree = &myfree;

	return s;
}

/*
 * Function with variable number of arguments to free given number of Strings
 * usage : freeStrings(3 , s1 ,s2 ,s3);
 */
void freeStrings(int nr_arg, ...) {

	va_list valist;

	int i;
	String s;

	va_start(valist, nr_arg); //initialize list of Strings

	for (i = 0; i < nr_arg; i++) {
		s = va_arg(valist, String); //next string element
		s.myfree(&s);
	}

	va_end(valist);
}

