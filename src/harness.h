#ifndef HARNESS_H
#define HARNESS_H

#include <stdbool.h>
#include <stdlib.h>

typedef enum Verbosity {
	NO_INFO,
	MINIMAL_INFO,
	BASIC_INFO,
	ALL_INFO,
} Verbosity;

// what is passed into and set by test functions
typedef struct TestResult {
	bool valid; // true if the test passed, false otherwie
	const char* fail_text;
	const char* success_text;
	const char* name;
	Verbosity verbose;
} TestResult;

typedef struct TestFunc {
	void (*func)(TestResult* result, int seed);
	const char* name;
} TestFunc;

// wrapper that takes test settings and uses them to run a test
typedef struct TestHarness {
	const char* name;
	TestFunc* test_funcs;
	size_t test_funcs_length;
} TestHarness;

// runs the test and returns true if it passed and false if it failed
// depending on the Verbosity of verbose, prints information about the test
bool doTest(
		char* name,
		TestFunc* test_funcs,
		size_t test_funcs_length,
		int seed,
		Verbosity verbose);

// fills out the TestHarness passed in partially, to be passed to runTestHarness
void createTestHarness(
		TestHarness* harness,
		char* name,
		TestFunc* test_funcs,
		size_t test_funcs_length);

// taken a TestHarness returned from createTestHarness, a seed, and a boolean verbose
// runs the test and returns true if it passed and false if it failed
// depending on the Verbosity of verbose, prints information about the test
bool runTestHarness(TestHarness* harness, int seed, Verbosity verbose);

// utility functions

// print the string if the verbosity is ALL_INFO
void report(TestResult* result, const char* str, ...);

// assert, need to use a different name because assert exists in <assert.h>
void affirm(TestResult* result, bool valid, const char* fail_text);

// if there are no errors, write success_text to the result
void onSuccess(TestResult* result, const char* success_text);

// if there are errors, write fail_text to the result
void onFailure(TestResult* result, const char* fail_text);

#endif // HARNESS_H
