#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "./harness.h"

static const size_t max_string_size = 256;

static void initTestResult(TestResult* result, Verbosity verbose, const char* name);

// fills out the TestHarness passed in partially, to be passed to runTestHarness
void createTestHarness(
		TestHarness* harness,
		char* name,
		TestFunc* test_funcs,
		size_t test_funcs_length)
{
	harness->name = name;
	harness->test_funcs = test_funcs;
	harness->test_funcs_length = test_funcs_length;
}

// takes a TestHarness returned from createTestHarness, a seed, and a boolean verbose
// runs the test and returns true if it passed and false if it failed
// depending on the Verbosity of verbose, prints information about the test
bool runTestHarness(TestHarness* harness, int seed, Verbosity verbose)
{
	size_t ii;
	bool valid = true;
	char* name_string;

	if (verbose == ALL_INFO || verbose == BASIC_INFO) {
		printf("[start] \"%s\" with seed %d\n", harness->name, seed);
	}

	name_string = (char*) malloc(max_string_size * sizeof(char));

	for (ii = 0; ii < harness->test_funcs_length; ii += 1) {
		TestResult* result = (TestResult*) malloc(sizeof(TestResult));

		const char* name;
		if (harness->test_funcs[ii].name == NULL) {
			snprintf(name_string, max_string_size, "Test %lu", ii);
			name = name_string;
		} else {
			name = harness->test_funcs[ii].name;
		}

		initTestResult(result, verbose, name);

		if (verbose == ALL_INFO) {
			printf("  [start] \"%s\"\n", name);
		}

		harness->test_funcs[ii].func(result, seed);
		if (!result->valid) {
			valid = false;
		}

		if (verbose == BASIC_INFO || verbose == ALL_INFO) {
			if (result->valid) {
				printf("  [success] \"%s\"", result->name);
				if (result->success_text != NULL) {
					printf(" (%s)", result->success_text);
				}
				printf("\n");
			} else {
				printf("  [FAILURE] \"%s\"", result->name);
				if (result->fail_text != NULL) {
					printf(" (%s)", result->fail_text);
				}
				printf("\n");
			}
		}

		if (verbose == ALL_INFO) {
			printf("  [end]\n");
		}

		free(result);
	}

	free(name_string);

	if (verbose == BASIC_INFO || verbose == MINIMAL_INFO || verbose == ALL_INFO) {
		if (valid) {
			printf("[success] \"%s\" with seed %d\n", harness->name, seed);
		} else {
			printf("[FAILURE] \"%s\" with seed %d\n", harness->name, seed);
		}
	}

	if (verbose == BASIC_INFO || verbose == ALL_INFO) {
		printf("[end]\n");
	}

	return valid;
}

bool doTest(char* name,
		TestFunc* test_funcs,
		size_t test_funcs_length,
		int seed,
		Verbosity verbose)
{
	TestHarness* harness = (TestHarness*) malloc(sizeof(TestHarness));

	createTestHarness(harness, name, test_funcs, test_funcs_length);
	bool test_result = runTestHarness(harness, seed, verbose);

	free(harness);
	return test_result;
}


void report(TestResult* result, const char* str)
{
	if (result->verbose == ALL_INFO) {
		printf("    [report] %s\n", str);
	}
}

// assert, need to use a different name because assert exists in <assert.h>
void affirm(TestResult* result, bool valid, const char* fail_text)
{
	if (!result->valid) {
		return;
	}

	if (!valid) {
		result->valid = valid;
		if (result->fail_text == NULL) {
			result->fail_text = fail_text;
		}
	}
}

// if there are no erros, write success_text to the result
void onSuccess(TestResult* result, const char* success_text)
{
	result->success_text = success_text;
}

void onFailure(TestResult* result, const char* fail_text)
{
	result->fail_text = fail_text;
}

// static functions

static void initTestResult(TestResult* result, Verbosity verbose, const char* name)
{
	result->valid = true;
	result->fail_text = NULL;
	result->success_text = NULL;
	result->name = name;
	result->verbose = verbose;
}

