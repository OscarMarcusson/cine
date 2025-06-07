#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>
#include <string.h>

typedef struct {
	char* pointer;
	int length;
	int capacity;
} BUFFER;

BUFFER create_buffer(int capacity) {
	BUFFER buffer;
	buffer.length = 0;

	if (capacity <= 0) {
		buffer.pointer = NULL;
		buffer.capacity = 0;
		return buffer;
	}

	buffer.pointer = (char*)malloc(sizeof(char) * capacity);
	if (buffer.pointer == NULL) {
		buffer.capacity = 0;
		return buffer;
	}

	buffer.pointer[0] = '\0';
	buffer.capacity = capacity;
	return buffer;
}


static bool ensure_buffer_has_capacity(BUFFER* buffer, int new_length) {
	// Grow buffer if needed
	if (new_length > buffer->capacity) {
		int new_capacity = buffer->capacity * 2;
		if (new_length > new_capacity) {
			new_capacity = new_length;
		}
		char* new_pointer = (char*)realloc(buffer->pointer, new_capacity);
		if (new_pointer == NULL) {
			return false;
		}

		buffer->pointer = new_pointer;
		buffer->capacity = new_capacity;
	}
	return true;
}

bool append_to_buffer(BUFFER* buffer, const char* text) {
	int length = strlen(text);
	int new_length = buffer->length + length + 1;
	if (!ensure_buffer_has_capacity(buffer, new_length)) {
		return false;
	}

	// Append text to buffer
	strcat(buffer->pointer, text);   // strcat is safe due to the length checks above
	buffer->length = new_length - 1; // Save length without including \0
	return true;
}

bool append_char_to_buffer(BUFFER* buffer, char c) {
	int new_length = buffer->length + 1;
	if (!ensure_buffer_has_capacity(buffer, new_length)) {
		return false;
	}
	buffer->pointer[buffer->length] = c;
	buffer->pointer[new_length] = '\0';
	buffer->length = new_length;
	return true;
}

void clear_buffer(BUFFER* buffer) {
	buffer->length = 0;
	if (buffer->pointer != NULL) {
		buffer->pointer[0] = '\0';
	}
}

void destroy_buffer(BUFFER* buffer) {
	if (buffer->pointer != NULL) {
		free(buffer->pointer);
		buffer->pointer = NULL;
	}
}

#endif // BUFFER_H
