Test Harness
==============

Unit testing framework for small C projects.

I was working on a C project and I wanted to write a few unit tests.
When I looked for C unit testing frameworks, the frameworks that I found
looked excellent but were all much more complicated than what I needed.

I wrote this test harness to provide a very simple unit test framework.
It's based on four functions that can be used in unit testing functions:

- void affirm(TestResult* result, bool valid, const char* fail\_text):
	if _valid_ is false then the test fails with the message fail\_text
- void onSuccess(TestResult* result, const char* success\_text):
	if the test succeeds then the test uses success\_text in the output
- void onFailure(TestResult* result, const char* fail\_text):
	if the test fails then the test uses fail\_text in the output
- void report(TestResult* result, const char* str, ...):
	if verbose is set then string passed in is printed

The unit testing functions are then used by the test harness functions.
The following three harness functions use function pointers to
the unit test functions to run the unit tests and print the output:

- bool doTest(char* name, TestFunc* test\_funcs, size\_t test\_funcs\_length, int seed, Verbosity verbose):
	runs each of the tests passed in (_test\_funcs_) and returns true if they
	all pass and false otherwise; the argument _verbose_ determines what
	information is printed, if any
- void createTestHarness(TestHarness* harness, char* name, TestFunc* test\_funcs, size\_t test\_funcs\_length):
	given a pointer to a TestHarness (_harness_) and a list of tests (_test\_funcs_), modifies the TestHarness so that it can be passed into runTestHarness
- bool runTestHarness(TestHarness* harness, int seed, Verbosity verbose):
	basically the same as doTest except it also takes in a TestHarness, which
	allows the same test to be run multiple times in a loop with a different
	seed

Finally there are four levels of Verbosity that can be passed into doTest
and runTestHarness:

- NO\_INFO:
	prints nothing
- MINIMAL\_INFO:
	prints whether the TestHarness with the particular seed that is
	passed in succeeds, fails, or dies
- BASIC\_INFO:
	prints whether each unit test within the TestHarness succeeds
	or fails along with when the TestHarness starts and ends
- ALL\_INFO:
	prints everything that BASIC\_INFO prints in addition to
	report() calls and when each unit test starts and ends


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

Which produces the output:

    [start] "Test modulo" with seed 55
      [start] "modTen"
        [report] using number 5
      [success] "modTen"
      [end]
      [start] "modNine"
        [report] using number 5
      [success] "modNine"
      [end]
    [success] "Test modulo" with seed 55
    [end]


See the _examples/_ directory for some example programs. All of the functions
mentioned above and all of the different levels of Verbosity should be
represented.

The examples can be run with _examples/run\_all.sh_ or they can be
run individually.

Notes
==============

Tested on OSX version 10.9.2 with Apple LLVM version 5.1 (clang-503.0.40) (based on LLVM 3.4svn) and gcc-mp-4.8 (MacPorts gcc48 4.8.2_0) 4.8.2

