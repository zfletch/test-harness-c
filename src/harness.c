#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include "./harness.h"

// maximum size of automatic test names when NULL is passed in
static const size_t max_string_size = 256;

// initializes the fields in result to their default values
static void initTestResult(TestResult* result, Verbosity verbose, const char* name);

// prints the error passed in and exits
static void handleChildSignal (int signum);

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

	// since fork() is called, stdout has to be unbuffered or redirecting output
	// will print the same line multiple times
	setbuf(stdout, NULL);

	// fork is an expensive call, but it's the easiest and safest way to guarantee
	// that test harness runs do not interfere with each other's memory
	// the child runs the test and the parent waits for it to exit before continuing
	// if the exit status is non-zero then the test has either failed or died
	// exit status of EXIT_FAILURE means it failed and any other non-zero value means it died
	pid_t pid = fork();
	if (pid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid == 0) {
		if (verbose == BASIC_INFO || verbose == MINIMAL_INFO || verbose == ALL_INFO) {
			// this is probably the hackiest part of the program
			// in the case where the test dies but the exit status is EXIT_FAILURE,
			// then we do not want the user to think the test completed but failed
			// so in the case of EXIT_FAILURE occurring during the test,
			// the [DIED] message is printed by the child before it exits
			struct sigaction sa;
			sigemptyset(&sa.sa_mask);
			sa.sa_handler = handleChildSignal;
			if (sigaction(EXIT_FAILURE, &sa, NULL) == -1) {
				perror("sigaction");
				exit(SIGINT);
			}
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

		if (!valid) {
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);

	} else {

		int status;
		pid_t wait_err = waitpid(pid, &status, WUNTRACED | WCONTINUED);
		if (wait_err == -1) {
			perror("waitpid");
			exit(EXIT_FAILURE);
		}

		// if the user types ^C during a test, we should exit the test harness as well
		if (status == SIGINT) {
			exit(SIGINT);
		}

		if (status != EXIT_SUCCESS) {
			valid = false;
		}

		if (verbose == BASIC_INFO || verbose == MINIMAL_INFO || verbose == ALL_INFO) {

			if (status != EXIT_SUCCESS && status != EXIT_FAILURE) {
				printf("[DIED] Exit status: %d\n", status);
			}

			if (valid) {
				printf("[success] \"%s\" with seed %d\n", harness->name, seed);
			} else {
				printf("[FAILURE] \"%s\" with seed %d\n", harness->name, seed);
			}
		}

		if (verbose == BASIC_INFO || verbose == ALL_INFO) {
			printf("[end]\n");
		}
	}

	return valid;
}

// similar to runTestHarness except that it does not take a TestHarness*
// instead it creates one given the rest of the arguments and runs the test once
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


// printf the string passed in if verbosity is ALL_INFO
void report(TestResult* result, const char* str, ...)
{
	if (result->verbose == ALL_INFO) {
		va_list args;
		va_start(args, str);
		printf("    [report] ");
		vprintf(str, args);
		printf("\n");
		va_end(args);
	}
}

// basically assert, need to use a different name because assert exists in <assert.h>
// if the bool passed in (valid) is true, then this function does nothing
// if it is false, then it writes the string passed in to result->fail_text
// (unless something is already written there)
// this means that the fail text printed in the output is:
// 1. the most recently called onFailure if there is one
// 2. the least recently (the one called first) affirm that failed if there is one
// 3. NULL
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

// write success_text to the result
void onSuccess(TestResult* result, const char* success_text)
{
	result->success_text = success_text;
}

// write fail_text to the result (overrides affirm)
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

static void handleChildSignal (int status)
{
	printf("[DIED] Exit status: %d\n", status);
	exit(EXIT_FAILURE);
}
