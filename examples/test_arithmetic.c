#include <stdio.h>
#include "../src/harness.h"

void test_Add (TestResult* result, int seed)
{
	report(result, "Add 1 to seed");

	int new_seed = seed + 1;

	affirm(result, seed == new_seed - 1, "Addition is not working");
}

int main (int argc, char** argv)
{
	doTest("Simple Test",
			(TestFunc[]) { { test_Add, NULL } },
			1,
			0,
			ALL_INFO);
}
