#include <stdio.h>
#include <stdbool.h>
#include "logging.h"
#include "args.h"

// Returns false if the buffer ran out
static bool fread_to_whitespace(FILE* file, char* buffer, int max) {
	int c = 0;
	int i = 0;
	while ((c = fgetc(file)) >= 0) {
		if (i >= max) {
			buffer[max] = '\0';
			return false;
		}
		if (c == '\r' || c == '\n' || c == ' ' || c == '\t') break;
		buffer[i] = c;
		i++;
	}
	buffer[i] = '\0';
	return i;
}

// Returns the length
static int get_directive(FILE* file, char* buffer, int max) {
	int c = 0;
	int i = 0;
	while ((c = fgetc(file)) >= 0 && i < max) {
		if (c == '\r' || c == '\n') break;
		buffer[i] = c;
		i++;
	}
	buffer[i] = '\0';
	return i;
}


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

	char dbuffer[max + 1];
	dbuffer[max] = '\0';

	int i = 0;
	int c;
	bool success = true;
	char end = '\0';

	// Helper
#   define flush(text) if (fputs(text, out) == EOF) \
	{\
		success = false;\
		break;\
	}

	// Flushes the main buffer and restarting the counter
#   define flush_main() buffer[i] = '\0'; flush(buffer); i = 0;

#   define push(char_value) buffer[i] = char_value; i++;


	// A fail means the input was massive, and not workable. So just dump it into the file and continue
#   define load_next_word_into_dbuffer() if (!fread_to_whitespace(src, dbuffer, max + 1)) {\
		push('#'); \
		flush_main(); \
		flush(dbuffer); \
		push(' '); \
		continue; \
	}

	// Read input
	while ((c = fgetc(src)) >= 0) {
		// Inside a string
		if (end != '\0') {
			if (buffer[i - 1] != '\\' && c == end) {
				end = '\0';
			}
			push(c);
		}
		else if (c == '#') {
			load_next_word_into_dbuffer();
			if (strcmp("include", dbuffer) == 0) {
				load_next_word_into_dbuffer();
				if (dbuffer[0] == '<') {
					flush_main();
					push('#');
					push('i');
					push('n');
					push('c');
					push('l');
					push('u');
					push('d');
					push('e');
					push(' ');
					flush_main();
					flush(dbuffer);
					push('\n');
				}
				else {
					todo("Handle includes: " ARGF, dbuffer);
				}
			}
			// Some unknown directive, so ignore since we only care about the includes
			else {
				push('#');
				flush_main();
				flush(dbuffer);
				push(' ');
				// buffer[i] = '?';
				// i++;
			}
		}
		else {
			push(c);
		}

		if (i == max) {
			flush_main();
		}
	}
	if (i > 0) {
		buffer[i] = '\0';
		if (fputs(buffer, out) == EOF) success = false;
	}


	fclose(src);

	return success ? 0 : 1;
}
