/*
 * StringTests.c
 *
 *  Created on: Jun 25, 2015
 *      Author: alexandruserban
 */

#include "String.h"

int main() {

	String s1 = new_String("abcdefg"), s2 = new_String("a1bc11aaa"), ss1, ss2,
			ss3, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s16;

	printf("-------split---------\n");
	String * ar;
	int string_number, i;
	ar = s2.split(&s2, "1", &string_number);

	for (i = 0; i < string_number; ++i) {
		ar[i].print(&ar[i]);
		printf("\n");
	}
	for (i = 0; i < string_number; ++i) {
		freeStrings(1, ar[i]);
	}
	free(ar);

	printf("-------substring from---------\n");
	ss1 = s1.substringFrom(&s1, 2);
	ss1.print(&ss1);
	printf("\n");

	printf("-------substring to---------\n");
	ss2 = s2.substringTo(&s2, 3);
	ss2.print(&ss2);
	printf("\n");

	printf("-------substring interval---------\n");
	ss3 = s2.substringFromTo(&s2, 2, 4);
	ss3.print(&ss3);
	printf("\n");

	printf("-------index first occ---------\n");
	printf("%s\n", s2.str + (s2.indexFirstOcc(&s2, "c1")));
	printf("%s\n", s2.str + (s2.indexLastOcc(&s2, "a")));

	printf("-------starts with ends with---------\n");
	printf("%d\n", s2.startsWith(&s2, "1abc"));
	printf("%d\n", s2.endsWith(&s2, "1v"));

	printf("-------replace all---------\n");
	s3 = new_String("abc1asdf1");
	printf("%d\n", s3.replaceAllOcc(&s3, "1", "3"));
	printf("\ns3: %s\n", s3.str);
	printf("s3: %d\n", (int) strlen(s3.str));

	printf("-------replace First---------\n");
	s4 = new_String("0123456189");
	s4.replaceFirstOcc(&s4, "1", "3");
	printf("\ns4: %s\n", s4.str);
	printf("s4: %d\n", (int) strlen(s4.str));

	printf("-------replace Last---------\n");
	s5 = new_String("0101010");
	s5.replaceLastOcc(&s5, "1", "3");
	printf("\ns5: %s\n", s5.str);
	printf("s5: %d\n", (int) strlen(s5.str));

	printf("-------replace FromIndex---------\n");
	s6 = new_String("01234567");
	s6.replaceFromIndex(&s6, "AAA", 3);
	printf("\ns6: %s\n", s6.str);

	printf("-------replace ToIndex---------\n");
	s7 = new_String("01234567");
	s7.replaceToIndex(&s7, "AAA", 3);
	printf("\ns7: %s\n", s7.str);

	printf("-------replace INTERVAL--------\n");
	s8 = new_String("01234567");
	s8.replaceFromIndexToIndex(&s8, "AAA", 2, 6);
	printf("\ns8: %s\n", s8.str);

	//TODO to test valgrind
	printf("-------trim--------\n");
	s9 = new_String("  aa ");
	s9.trim(&s9);
	printf("lun %d \n", s9.length(&s9));
	printf("\ns9:%s\n", s9.str);

	printf("-------reverse--------\n");
	s10 = new_String("12345");
	s10.reverse(&s10);
	printf("\ns10:%s\n", s10.str);

	printf("-------remove inSeq--------\n");
	s11 = new_String("12345");
	s11.removeCharsInSeq(&s11, "3489");
	printf("\ns11:%s\n", s11.str);

	printf("-------remove NotinSeq--------\n");
	s12 = new_String("12345");
	s12.removeCharsNotInSeq(&s12, "3489");
	printf("\ns12:%s\n", s12.str);

	printf("--------insert at index------\n");
	s13 = new_String("012345678");
	s13.insertAtIndex(&s13, 3, "AA");
	printf("\ns13:%s\n", s13.str);

	printf("--------string to int--------\n");
	s14 = new_String("");
	printf("%d\n", s14.toInt(&s14));

	printf("--------int to string --------\n");
	s15 = s14.toString(1234);
	printf("%s\n", s15.str);

	printf("--------compose --------\n");
	s16 = s14.compose(3, "a", "b", "csfdf");
	printf("%s\n", s16.str);

	freeStrings(19, ss3, ss1, ss2, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11,
			s12, s13, s14, s15, s16);
	return 0;
}

