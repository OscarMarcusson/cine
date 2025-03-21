#include <stdio.h>
#include "logging.h"
#include "args.h"

int main(const int argc, const char* argv[]) {
	if (argc == 1) {
		todo("Implement help page");
		return 0;
	}

	ARGS args = parse_args(argc, argv);
	if (!args.success) {
		return 1;
	}

	return 0;
}
