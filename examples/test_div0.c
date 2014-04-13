#include <stdio.h>
#include <stdlib.h>
#include "../src/harness.h"

void test_div (TestResult* result, int seed)
{
	affirm(result, 1 / seed >= 0, "Integer rounding when dividing");
}

int main (int argc, char** argv)
{
	printf("This test should die when seed is 0 and succeed otherwise\n");
	printf("\n");
	doTest("Test division",
			(TestFunc[]) { { test_div, NULL } },
			1,
			0,
			ALL_INFO);
	doTest("Test division",
			(TestFunc[]) { { test_div, NULL } },
			1,
			3,
			ALL_INFO);
	return 0;
}
