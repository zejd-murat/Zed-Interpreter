#include <string.h>
#include <stdlib.h>
#include "../Interfaces/string.h"
#define INITIAL_CAP 3
typedef struct opaque {
	size_t size;
	size_t capacity;
	char* data;
	bool isNumeric;
} string;

string* constructStr() {
	string* obj = (string*)malloc(sizeof(string));
	obj->size = 0;
	obj->capacity = INITIAL_CAP + 1;
	obj->data= (char*)malloc(obj->capacity);
	obj->isNumeric = 0;
	return obj;
}

void SetBool(string* str, bool x) {
	str->isNumeric = x;
}

bool isNumeric(string* str) {
	return str->isNumeric;
}
void ReAlloc(string* str)
{
	str->capacity = str->capacity * 2 + 1;
	char* NewData = (char*)malloc(str->capacity);
	for (size_t i = 0; i < str->size; i++)
		NewData[i] = str->data[i];

	free((void*)str->data);
	str->data = NewData;
}

void ReAlloc2(string* str, size_t s) {
	str->capacity = str->capacity + s  + 1;

	char* NewData = (char*)malloc(str->capacity);

	for (size_t i = 0; i < str->size; i++)
		NewData[i] = str->data[i];
	
	free(str->data);
	str->data = NewData;

}
void push_back(string* str, char x) {
	if (str->size < str->capacity - 1)
	{
		str->data[str->size] = x;
		str->size += 1;
		str->data[str->size] = '\0';
	}

	else
	{
		ReAlloc(str);
		str->data[str->size++] = x;
		str->data[str->size] = '\0';
	}
}

void push_str(string* str, int n, ...) {
	va_list args;

	va_start(args, n);

	const char* s = NULL;

	for (int i = 0; i < n; i++) {
		s = va_arg(args, const char*);

		for (size_t x = 0; x < strlen(s); x++) {
			push_back(str, s[x]);
		} 
	}

	va_end(args);
}

char* GetStr(string* str) {
	return str->data;
}

char at(string* str, size_t i) {
	return str->data[i];
}

size_t GetSize(string* str) {
	return str->size;
}
// We need to garbage collect