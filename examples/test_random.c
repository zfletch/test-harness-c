#include <stdio.h>
#include "../src/harness.h"

int multByTwo (int num)
{
	num <<= 1;
	return num;
}

void test_multByTwoWorks (TestResult* result, int seed)
{
	srand(seed);
	int num = rand();
	int num2 = multByTwo(num);

	report(result, "num: %d", num);
	report(result, "num2: %d", num2);
	affirm(result, num2 > num, "the result product should be greater than the factor");
	affirm(result, num2 == num * 2, "multByTwo should multiply by 2");

	onSuccess(result, "multByTwo works!");
}

void test_rand(TestResult* result, int seed)
{

	onSuccess(result, "all rands are pretty big");
	onFailure(result, "some rands are small");

	srand(seed);
	affirm(result, rand() > 3, "first rand is greater than 3");
	affirm(result, rand() > 10, "second rand is greater than 10");
	affirm(result, rand() > 30, "third rand is greater than 30");
	affirm(result, rand() > 600, "fourth rand is greater than 600");
}

int main (int argc, char** argv)
{
	printf("This test runs the same test with several levels of verbosity\n");
	printf("The code works most of the time but not all the time, so depending on the seed it can fail or succeed\n");
	printf("\n");
	TestHarness* harness = (TestHarness*) malloc(sizeof(TestHarness));

	createTestHarness(harness,
			"test mult and rand",
			(TestFunc[]) {
				{ test_multByTwoWorks, "mult test" },
				{ test_rand, "rand test" }
			},
			2);

	printf("Test verbosity ALL_INFO\n");
	runTestHarness(harness, 10, ALL_INFO);
	printf("\n");

	printf("Test verbosity BASIC_INFO\n");
	runTestHarness(harness, 10, BASIC_INFO);
	printf("\n");

	printf("Test verbosity MINIMAL_INFO\n");
	runTestHarness(harness, 10, MINIMAL_INFO);
	printf("\n");

	printf("Test verbosity NO_INFO\n");
	runTestHarness(harness, 10, NO_INFO);

	free(harness);

	return 0;
}
