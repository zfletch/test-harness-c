#include <stdio.h>
#include "../src/harness.h"

int multByTwo (int num)
{
	num >>= 1;
	return num;
}

void test_multByTwoWorks (TestResult* result, int seed)
{
	srand(seed);
	int num = rand();
	int num2 = multByTwo(num);

	report(result, "hello");
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
	int ii;

	TestHarness* harness = (TestHarness*) malloc(sizeof(TestHarness));
	createTestHarness(harness,
			"test mult and rand",
			(TestFunc[]) {
				{ test_multByTwoWorks, "mult test" },
				{ test_rand, "rand test" }
			},
			2);

	runTestHarness(harness, 10, ALL_INFO);
	runTestHarness(harness, 10, BASIC_INFO);
	runTestHarness(harness, 10, MINIMAL_INFO);
	runTestHarness(harness, 10, NO_INFO);

	free(harness);

	return 0;
}
