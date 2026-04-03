#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "testcase.h"

struct testcase {
    test_func func;
    char *id;
    char *desc;
};

testcase testcase_create(test_func func, char *id, char *desc) {
    testcase t = malloc(sizeof(struct testcase));
    if (t == NULL) {
        return NULL;
    }

    t->func = func;
    t->id = strdup(id);
    t->desc = strdup(desc);

    return t;
}

void testcase_destroy(testcase t) {
    if (t == NULL) return;

    free(t->id);
    free(t->desc);
    free(t);
}

test_func testcase_func(testcase t) {
    return (t) ? t->func : NULL;
}

char* testcase_id(testcase t) {
    return (t) ? t->id : NULL;
}

char* testcase_desc(testcase t) {
    return (t) ? t->desc : NULL;
}

