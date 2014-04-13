#include <stdio.h>
#include <stdlib.h>
#include "../src/harness.h"

void test_pointerStuff (TestResult* result, int seed)
{
	srand(seed);
	int* block = (int*) rand();

	int ii;
	for (ii = 0; ii < 10; ii += 1) {
		block[ii] = 1;
	}

}

int main (int argc, char** argv)
{
	printf("This test should die due to a segfault (11)\n");
	printf("\n");
	doTest("Simple Test",
			(TestFunc[]) { { test_pointerStuff, NULL } },
			1,
			0,
			ALL_INFO);
}
