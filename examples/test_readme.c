#include <stdio.h>
#include "../src/harness.h"

void test_modTen (TestResult* result, int seed)
{
	srand(seed);

	int num = rand() % 10;

	report(result, "using number %d", num);
	affirm(result, num >= 0, "modulo 10 must be greater or equal to 0");
	affirm(result, num < 10, "modulo 10 must be less than 10");
}

void test_modNine (TestResult* result, int seed)
{
	srand(seed);

	int num = rand() % 9;

	report(result, "using number %d", num);
	affirm(result, num >= 0, "modulo 9 must be greater or equal to 0");
	affirm(result, num < 9, "modulo 9 must be less than 9");
}

int main (int argc, char** argv)
{
	int seed = 55;
	doTest("Test modulo",
			(TestFunc[]) {
				{ test_modTen, "modTen" },
				{ test_modTen, "modNine" }
			},
			2,
			seed,
			ALL_INFO);

	return 0;
}

