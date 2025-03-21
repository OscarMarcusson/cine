#ifndef ARGS_H
#define ARGS_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "logging.h"

typedef struct {
	const char* input;
	const char* output;
	bool success;
} ARGS;

ARGS parse_args(const int argc, const char* argv[]) {
	ARGS args;
	args.input = NULL;
	args.output = NULL;
	args.success = false;

	for (int i = 1; i < argc; i++) {
		const char* arg = argv[i];

		// Flag parsing
		if (arg[0] == '-') {
			// Output path
			if (arg[1] == 'o' || strcmp(arg, "--output") == 0) {
				if (args.output != NULL) {
					fail("Output is already specified, remove the second " ARGF, arg);
					return args;
				}
				i++;
				if (i >= argc) {
					fail("Expected a path after " ARGF, arg);
					return args;
				}
				args.output = argv[i];
			}
			// Fallback
			else {
				fail("Unexpected argument:" ARGF, arg);
				return args;
			}
		}
		// Raw input
		else {
			if (args.input != NULL) {
				fail("Input is already specified, remove " ARGF, arg);
				return args;
			}
			args.input = arg;
		}
	}

	args.success = true;
	return args;
}

#endif
