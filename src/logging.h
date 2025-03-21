#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>
#include "terminal.h"
#include <stdarg.h>

// Log info to the terminal
void info(const char* info, ...) {
	va_list args;
	va_start(args, info);
	vprintf(info, args);
	va_end(args);
	printf("\n");
}

// Log error to the terminal
void error(const char* info, ...) {
	printf(RED "ERROR: " RESET);
	va_list args;
	va_start(args, info);
	vprintf(info, args);
	va_end(args);
	printf("\n");
}

void error_extra(const char* info, ...) {
	printf(RED "       " RESET);
	va_list args;
	va_start(args, info);
	vprintf(info, args);
	va_end(args);
	printf("\n");
}

void todo(const char* info, ...) {
	printf(GREEN "TODO: " RESET);
	va_list args;
	va_start(args, info);
	vprintf(info, args);
	va_end(args);
	printf("\n");
}

void initiate(const char* message, ...) {
	printf(DIM "➟ " RESET);
	va_list args;
	va_start(args, message);
	vprintf(message, args);
	va_end(args);
	printf("\n");
}

void ok(const char* message, ...) {
	printf(BOLDGREEN "✓ " RESET);
	va_list args;
	va_start(args, message);
	vprintf(message, args);
	va_end(args);
	printf("\n");
}

void fail(const char* message, ...) {
	printf(BOLDRED "✕ " RESET);
	va_list args;
	va_start(args, message);
	vprintf(message, args);
	va_end(args);
	printf("\n");
}

#endif // LOGGING_H
