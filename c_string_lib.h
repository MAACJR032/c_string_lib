#pragma once

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct string
{
    size_t size;       // Number of characters in the string
    size_t capacity;   // Allocated spaces + 1 (for the null terminator)
    char   *str;       // Array of characters
} string;

typedef enum {
    STRING_SUCCESS          =  0,
    STRING_NULL_ARG_ERROR   = -2,
    STRING_ALLOCATION_ERROR = -3,
    STRING_OUT_OF_RANGE     = -4
} string_status_t;

char* string_to_char(string *s);
string* char_to_string(char *s);

string* new_string(const char *str, size_t capacity);
string* new_string_s(const string *str, size_t capacity);
string_status_t string_free(string **s);

string_status_t string_reserve(string *s, size_t capacity);
string_status_t string_resize(string *s, size_t size);

string_status_t string_append(string *dest, const char *src);
string_status_t string_append_s(string *dest, const string *src);

string_status_t string_assign(string *dest, const char *src);
string_status_t string_assign_s(string *dest, const string *src);

string_status_t string_insert(string *dest, const char *src, size_t pos);
string_status_t string_insert_s(string *dest, const string *src, size_t pos);

string_status_t string_clear(string *s);

int string_compare(const string *str1, const char *str2);
int string_compare_s(const string *str1, const string *str2);
int string_compare_buffer(const string *str1, const char *str2, size_t size);
int string_compare_buffer_s(const string *str1, const string *str2, size_t size);

string_status_t string_lower(string *s);
string_status_t string_upper(string *s);

string_status_t string_substr(string *dest, const string *src, size_t start, size_t end);

string** string_split(const string *src, const char delimiter, size_t *count, string_status_t *status);
string* string_join(string **strings, char delimiter, size_t num_strings, string_status_t *status);

string_status_t string_reverse(string *s);

ssize_t string_find(const string *s, const char *substr);
ssize_t string_find_s(const string *s, const string *substr);

typedef struct string_iterator
{
    const char *current;
    const char *end;
} string_iterator;

typedef struct string_reverse_iterator
{
    const char *current;
    const char *start;
} string_reverse_iterator;

string_iterator new_string_iter(const string *s);
string_reverse_iterator new_string_reverse_iter(const string *s);

bool string_iter_next(string_iterator *it);
bool string_reverse_iter_next(string_reverse_iterator *it);

char* string_get_curr_iter(string_iterator *it);
char* string_get_curr_reverse_iter(string_reverse_iterator *it);