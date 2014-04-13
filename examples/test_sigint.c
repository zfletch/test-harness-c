#include <stdio.h>
#include <stdlib.h>
#include "../src/harness.h"

void test_div (TestResult* result, int seed)
{
	while (1);
	affirm(result, 1 / seed >= 0, "Integer rounding when dividing");
}

int main (int argc, char** argv)
{
	printf("This test should loop forever until the user types ^C or sends a SIGINT\n");
	printf("\n");
	doTest("Simple Test",
			(TestFunc[]) { { test_div, NULL }, { test_div, NULL } },
			1,
			0,
			ALL_INFO);
	return 0;
}
