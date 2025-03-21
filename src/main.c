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

	FILE* src = fopen(args.input, "r");
	if (src == NULL) {
		fail("Could not find " FPATH, args.input);
		return 1;
	}

	FILE* out = fopen(args.output, "w");
	if (out == NULL) {
		fclose(src);
		fail("Failed to create " FPATH, args.output);
		return 1;
	}

	const int max = 1023;
	char buffer[max + 1];
	buffer[max] = '\0';

	int i = 0;
	int c;
	bool success = true;
	while ((c = fgetc(src)) >= 0) {
		buffer[i] = c;
		i++;
		if (i == max) {
			if (fputs(buffer, out) == EOF) {
				success = false;
				break;
			}
			i = 0;
		}
	}
	if (i > 0) {
		buffer[i] = '\0';
		if (fputs(buffer, out) == EOF) success = false;
	}


	fclose(src);

	return success ? 0 : 1;
}
