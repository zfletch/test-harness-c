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

void test_minusTen (TestResult* result, int seed)
{
	srand(seed);

	int num = rand();;
	int num_minus_ten = num - 10;

	onSuccess(result, "yes!");
	onFailure(result, "no!");

	affirm(result, num > num_minus_ten, "subtracting ten from a number makes it smaller");
}

void test_plusTen (TestResult* result, int seed)
{
	srand(seed);

	int num = rand();;
	int num_plus_ten = num + 10;

	affirm(result, num > num_plus_ten, "adding ten to a number makes it smaller");
}

int main (int argc, char** argv)
{
	printf("This test contains three unit tests\n");
	printf("The first two should succeed and the final one should fail\n");
	printf("\n");

	int seed = 55;
	doTest("Test some functions",
			(TestFunc[]) {
				{ test_modTen, "modTen" },
				{ test_minusTen, "minusTen" },
				{ test_plusTen, "plusTen" },
			},
			3,
			seed,
			ALL_INFO);

	return 0;
}

