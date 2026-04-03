#ifndef TEST_CASE_H
#define TEST_CASE_H

typedef struct testcase *testcase;
typedef int (*test_func)(char*);



testcase testcase_create(test_func, char *id, char *desc);
void testcase_destroy(testcase t);
test_func testcase_func(testcase t);
char* testcase_id(testcase t);
char* testcase_desc(testcase t);

#endif