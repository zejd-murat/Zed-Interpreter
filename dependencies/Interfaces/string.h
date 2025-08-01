#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

typedef struct opaque string;

string* constructStr();

void push_back(string* str, char x);

char at(string* str, size_t i);

char* GetStr(string* str);

void SetBool(string* str, bool x);

bool isNumeric(string* str);

size_t GetSize(string* str);

void push_str(string* str, int n, ...);