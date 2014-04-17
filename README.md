Test Harness
==============

Unit testing framework for small C projects.

I was working on a C project and I wanted to write a few unit tests.
When I looked for C unit testing frameworks, the frameworks that I found
looked excellent but were all much more complicated than what I needed.

I wrote this test harness to provide a very simple unit testing framework.


Unit Test Functions
--------------

Unit tests are based on four functions:

- `void affirm(TestResult* result, bool valid, const char* fail_text)`:
	if `valid` is false then the test fails with the message `fail_text`
- `void onSuccess(TestResult* result, const char* success_text)`:
	if the test succeeds then the test uses `success_text` in the output
- `void onFailure(TestResult* result, const char* fail_text)`:
	if the test fails then the test uses `fail_text` in the output
- `void report(TestResult* result, const char* str, ...)`:
	if verbose is set then the string passed in is printed


Test Harness Functions
--------------

Test harnesses are built on the following three functions:

- `bool doTest(char* name, TestFunc* test_funcs, size_t test_funcs_length, int seed, Verbosity verbose)`:
	runs each of the tests passed in (`test_funcs`) and returns true if they
	all pass and false otherwise; the argument `verbose` determines what
	information is printed, if any
- `void createTestHarness(TestHarness* harness, char* name, TestFunc* test_funcs, size_t test_funcs_length)`:
	given a pointer to a `TestHarness` (`harness`) and a list of tests (`test_funcs`), modifies the TestHarness so that it can be passed into `runTestHarness`
- `bool runTestHarness(TestHarness* harness, int seed, Verbosity verbose)`:
	basically the same as `doTest` except it also takes in a `TestHarness`, which
	allows the same test to be run multiple times in a loop with a different
	seed


Levels of Verbosity
--------------

Finally there are four levels of `Verbosity` that can be passed into `doTest`
and `runTestHarness`:

- `NO_INFO`:
	prints nothing
- `MINIMAL_INFO`:
	prints whether the `TestHarness` with the particular seed that is
	passed in succeeds, fails, or dies
- `BASIC_INFO`:
	prints whether each unit test within the `TestHarness` succeeds
	or fails along with when the `TestHarness` starts and ends
- `ALL_INFO`:
	prints everything that `BASIC_INFO` prints in addition to
	`report` calls and data about when each unit test starts and ends


Simple Example
==============

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

Which produces the output:

	[start] "Test some functions" with seed 55
	  [start] "modTen"
		[report] using number 5
	  [success] "modTen"
	  [end]
	  [start] "minusTen"
	  [success] "minusTen" (yes!)
	  [end]
	  [start] "plusTen"
	  [FAILURE] "plusTen" (adding ten to a number makes it smaller)
	  [end]
	[FAILURE] "Test some functions" with seed 55
	[end]

See the `examples/` directory for some example programs. All of the functions
mentioned above and all of the different levels of `Verbosity` should be
represented.

The examples can be compiled with `examples/compile_all.sh`.
They can be run with `examples/run_all.sh` or they can be run individually.

Notes
==============

Tested on OSX version 10.9.2 with Apple LLVM version 5.1 (clang-503.0.40) (based on LLVM 3.4svn) and gcc-mp-4.8 (MacPorts gcc48 4.8.2\_0) 4.8.2

