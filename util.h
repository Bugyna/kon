#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t
#define i8 int8_t
#define i16 int16_t
#define i32 int32_t
#define i64 int64_t

#define CTRL(c) ((c) & 037)


enum
{
	E_CANNOT_READ_FILE=1
};


#define DEBUG(s, ...) printf(s __VA_ARGS__); printf("\n\n");
#define KON_WARNING(...) DEBUG("warning: ", __VA_ARGS__)

#define KON_ERROR(...) DEBUG("ERROR: ", __VA_ARGS__); exit(-1)
#define KON_ERROR_ERR_CODE(n, ...) DEBUG(#n ": ", __VA_ARGS__); exit(n)


#define KON_ASSERT(expr, err) if (expr) { KON_ERROR(err); }
#define KON_ASSERT_ERR_CODE(expr, err, err_code) if (expr) { KON_ERROR_ERR_CODE(err_code, err); }


char* read_file(const char path[])
{
	char* dst = "";
	FILE* file;
	file = fopen(path, "r");
	if (file == NULL) {
		KON_ERROR_ERR_CODE(E_CANNOT_READ_FILE, "failed to read file %s", path);
	}
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	dst = malloc(size+2);
	fseek(file, 0, SEEK_SET);


	int i = 0, c;
	for (;;i++) {
		c = fgetc(file);
		if (c == EOF)
			break;

		dst[i] = c;

	}
	dst[i] = '\0';
	fclose(file);
	return dst;
	// printf("s: %s\n", dst);
	
}

int hash(const char* key)
{
	KON_ASSERT(key == NULL, "trying to hash an empty key");

	int mask = 1 << strlen(key);
	for (unsigned int i = 0; i < strlen(key); i++) {
		mask += (int)key[i];
	}
	return mask ^ (mask >> 4);
}