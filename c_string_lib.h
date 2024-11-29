#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>

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

/*
 * Internal function
 *
 * allocs the string and addign it's size and capacity to `size`
 */
string* _string_alloc(size_t size, size_t capacity)
{
    string *s = (string *) malloc(sizeof(string));
    if (!s)
        return NULL;

    s->size = size;
    s->capacity = capacity;
    s->str = NULL;

    s->str = (char *) malloc(capacity + 1);
    if (s->str == NULL)
    {
        free(s);
        return NULL;
    }
    
    return s;
}

/*
 * Internal function
 *
 * reallocs the string but do not change the capacity and size
 */
string_status_t _string_realloc(string *s, size_t size, size_t capacity)
{
    char *tmp = (char *) realloc(s->str, capacity + 1);

    if (!tmp)
        return STRING_ALLOCATION_ERROR;

    s->str = tmp;
    s->capacity = capacity;
    s->size = size;
    
    return STRING_SUCCESS;
}

/*
 * Creates a new string object initialized with the content of the provided string.
 * This new string must be deallocated with string_free()
 *
 * Parameters:
 *   - str:      string to be copied that will be assigned to the new string. 
 *   - capacity: The maximum capacity of the new string, including space for the null terminator.
 *               If the specified capacity is less than the length of `str`, 
 *               the capacity will be adjusted to match the length of `str`.
 *               If you don't wish to allocate a larger capacity, it is recommended
 *               to use `0` for this parameter.
 *
 * Returns:
 *   - A pointer to a newly allocated `string` object containing the copied string.
 *   The `size` field of the string will be set to the length of `str`,
 *   - If memory allocation fails, or if `str` is NULL, the function returns NULL.
 */
string* new_string(const char *str, size_t capacity)
{
    if (!str)
        return NULL;

    string *s = NULL;
    size_t size = strlen(str);
    
    if (capacity < size)
        capacity = size;

    s = _string_alloc(size, capacity);

    if (!s)
        return NULL;

    s->size = size;
    memcpy(s->str, str, size);
    s->str[s->size] = '\0';

    return s;
}

/*
 * Creates a new string object initialized with the content of the provided string.
 * This new string must be deallocated with string_free()
 *
 * Parameters:
 *   - str:      string to be copied that will be assigned to the new string. 
 *   - capacity: The maximum capacity of the new string, including space for the null terminator.
 *               If the specified capacity is less than the length of `str`, 
 *               the capacity will be adjusted to match the length of `str`.
 *               If you don't wish to allocate a larger capacity, it is recommended
 *               to use `0` for this parameter.
 *
 * Returns:
 *   - A pointer to a newly allocated `string` object containing the copied string.
 *   The `size` field of the string will be set to the length of `str`,
 *   - If memory allocation fails the function returns NULL.
 *   - if `str` or it's content is NULL, the function returns NULL.
 */
string* new_string_s(const string *str, size_t capacity)
{
    if (!str)
        return NULL;

    string *s = NULL;
    
    if (capacity < str->size)
        capacity = str->size;

    s = _string_alloc(str->size, capacity);

    if (!s)
        return NULL;

    s->size = str->size;
    memcpy(s->str, str, str->size);
    s->str[s->size] = '\0';

    return s;
}

/* 
 * Releases the memory of the string `s`
 *
 * Returns:
 * - `STRING_NULL_ARG_ERROR` if `s` or it's content is NULL
 * - STRING_SUCCESS if there was no error
 */
string_status_t string_free(string **s)
{
    if (s && *s)
    {
        free((*s)->str);
        free(*s);
        *s = NULL;

        return STRING_SUCCESS;
    }

    return STRING_NULL_ARG_ERROR;
}

/*
 * Resizes the string to the specified size.
 * If the size is greater, the new characters are uninitialized.
 * If the size is smaller, the string is truncated.
 * 
 * Parameters:
 * - `s`: The string to be resized
 * - `size`: The new size of the string
 * 
 * Returns:
 * - `STRING_NULL_ARG_ERROR` if the argument is NULL
 * - STRING_ALLOCATION_ERROR if there was an error allocating memory
 * - STRING_SUCCESS if there was no error
 */
string_status_t string_resize(string *s, size_t size)
{
    if (!s || !s->str)
        return STRING_NULL_ARG_ERROR;

    if (size == s->size)
        return STRING_SUCCESS;

    char *tmp = (char *) malloc(sizeof(char) * (size + 1));
    if (!tmp)
        return STRING_ALLOCATION_ERROR;

    if (size < s->size)
    {
        memcpy(tmp, s->str, size);
        s->size = size;
    }
    else if (size > s->size)
        memcpy(tmp, s->str, s->size);

    tmp[size] = '\0';

    free(s->str);
    s->str = tmp;
    tmp = NULL;

    s->capacity = size;

    return STRING_SUCCESS;
}

/*
 * Appends the src string to the end of dest.
 * The function handles any necessary memory allocation.
 * Parameters:
 * - `dest`: The string that will be appended by src
 * - `src`: The string whose content will be appended to dest
 * 
 * Returns:
 * - `STRING_NULL_ARG_ERROR` if any argument is `NULL`
 * - STRING_ALLOCATION_ERROR if there was an error reallocating
 * - STRING_SUCCESS if there was no error
 */
string_status_t string_append(string *dest, const char *src)
{
    if (!dest || !src || !dest->str)
        return STRING_NULL_ARG_ERROR;

    size_t src_size = strlen(src);

    if (dest->capacity - dest->size < src_size)
    {
        if (_string_realloc(dest, dest->size + dest->capacity + src_size, dest->capacity + src_size) == STRING_ALLOCATION_ERROR)
            return STRING_ALLOCATION_ERROR;
        dest->capacity += src_size;
    }
    
    memcpy(dest->str + dest->size, src, src_size);
    dest->str[dest->size] = '\0';
    
    return STRING_SUCCESS;
}

/*
 * Appends the src string to the end of dest.
 * The function handles any necessary memory allocation.
 * Parameters:
 * - `dest`: The string that will be appended by src
 * - `src`: The string whose content will be appended to dest
 * 
 * Returns:
 * - `STRING_NULL_ARG_ERROR` if any argument is `NULL`
 * - STRING_ALLOCATION_ERROR if there was an error reallocating
 * - STRING_SUCCESS if there was no error
 */
string_status_t string_append_s(string *dest, const string *src)
{
    if (!dest || !src || !dest->str || !src->str)
        return STRING_NULL_ARG_ERROR;

    if (dest->capacity - dest->size < src->size)
    {
        if (_string_realloc(dest, dest->size + dest->capacity + src->size, dest->capacity + src->size) == STRING_ALLOCATION_ERROR)
            return STRING_ALLOCATION_ERROR;
        dest->capacity += src->size;
    }
    
    memcpy(dest->str + dest->size, src->str, src->size);

    dest->size += src->size;
    dest->str[dest->size] = '\0';

    return STRING_SUCCESS;
}

/* 
 * Assigns the src string to dest.
 * The function handles any necessary memory allocation.
 * 
 * Parameters:
 * - `dest`: The string that will be assigned by src
 * - `src`: The string whose content will be assigned to dest
 * 
 * Returns:
 * - `STRING_NULL_ARG_ERROR` if any argument is `NULL`
 * - STRING_ALLOCATION_ERROR if there was an error reallocating
 * - STRING_SUCCESS if there was no error
 */
string_status_t string_assign_s(string *dest, const string *src)
{
    if (!dest || !src || !dest->str || !src->str)
        return STRING_NULL_ARG_ERROR;

    if (dest->capacity < src->size)
    {
        if (_string_realloc(dest, src->size, src->size) == STRING_ALLOCATION_ERROR)
            return STRING_ALLOCATION_ERROR;
    }
    
    memcpy(dest->str, src->str, src->size);
    dest->str[dest->size] = '\0';

    return STRING_SUCCESS;
}

/* 
 * Assigns the src string to dest.
 * The function handles any necessary memory allocation.
 * 
 * Parameters:
 * - `dest`: The string that will be assigned by src
 * - `src`: The string whose content will be assigned to dest
 * 
 * Returns:
 * - `STRING_NULL_ARG_ERROR` if any argument is `NULL`
 * - STRING_ALLOCATION_ERROR if there was an error reallocating
 * - STRING_SUCCESS if there was no error
 */
string_status_t string_assign(string *dest, const char *src)
{
    if (!dest || !dest->str || !src)
        return STRING_NULL_ARG_ERROR;

    size_t src_size = strlen(src);

    if (dest->capacity < src_size)
    {
        if (_string_realloc(dest, src_size, src_size) == STRING_ALLOCATION_ERROR)
            return STRING_ALLOCATION_ERROR;
    }

    memcpy(dest->str, src, src_size);
    dest->str[dest->size] = '\0';

    return STRING_SUCCESS;
}

/*
 * Inserts `src` into `dest` at the position `pos`.
 *
 * Parameters:
 * - `dest`: the string object where the source string will be inserted. 
 * - `src`: The string that will be inserted into the destination string.
 * - `pos`: The index in the destination string where the source string will be inserted.
 * 
 * Returns:
 * - `STRING_NULL_ARG_ERROR` if any argument is `NULL`
 * - STRING_OUT_OF_RANGE if `pos` is bigger than `dest`'s size
 * - STRING_ALLOCATION_ERROR if there was an error reallocating
 * - STRING_SUCCESS if there was no error
 */
string_status_t string_insert(string *dest, const char *src, size_t pos)
{
    if (!dest || !dest->str || !src)
        return STRING_NULL_ARG_ERROR;
    
    if (pos > dest->size)
        return STRING_OUT_OF_RANGE;

    size_t src_size = strlen(src);
    if (dest->capacity - dest->size < src_size)
    {
        string_status_t status = string_resize(dest, dest->size + src_size);
        if (status != STRING_SUCCESS)
            return status;
    }

    size_t tmp_size = dest->size - pos;
    
    char *tmp = (char *) malloc((tmp_size) * sizeof(char));
    if (!tmp)
        return STRING_ALLOCATION_ERROR;
    
    memcpy(tmp, dest->str + pos, tmp_size);
    tmp[tmp_size] = '\0';

    memcpy(dest->str + pos, src, src_size);
    memcpy(dest->str + pos + src_size, tmp, tmp_size);
    
    dest->size += src_size;
    dest->str[dest->size] = '\0';
    
    free(tmp);
    return STRING_SUCCESS;
}

/*
 * Inserts `src` into `dest` at the position `pos`.
 *
 * Parameters:
 * - `dest`: the string object where the source string will be inserted. 
 * - `src`: The string that will be inserted into the destination string.
 * - `pos`: The index in the destination string where the source string will be inserted.
 * 
 * Returns:
 * - `STRING_NULL_ARG_ERROR` if any argument is `NULL`
 * - STRING_OUT_OF_RANGE if `pos` is bigger than `dest`'s size
 * - STRING_ALLOCATION_ERROR if there was an error reallocating
 * - STRING_SUCCESS if there was no error
 */
string_status_t string_insert_s(string *dest, const string *src, size_t pos)
{
    if (!dest || !dest->str || !src || !src->str)
        return STRING_NULL_ARG_ERROR;
    
    if (pos > dest->size)
        return STRING_OUT_OF_RANGE;

    if (dest->capacity - dest->size < src->size)
    {
        string_status_t status = string_resize(dest, dest->size + src->size);
        if (status != STRING_SUCCESS)
            return status;
    }

    size_t tmp_size = dest->size - pos;
    
    char *tmp = (char *) malloc((tmp_size) * sizeof(char));
    if (!tmp)
        return STRING_ALLOCATION_ERROR;
    
    memcpy(tmp, dest->str + pos, tmp_size);
    tmp[tmp_size] = '\0';

    memcpy(dest->str + pos, src->str, src->size);
    memcpy(dest->str + pos + src->size, tmp, tmp_size);
    
    dest->size += src->size;
    dest->str[dest->size] = '\0';
    
    free(tmp);
    return STRING_SUCCESS;
}

/*
 * Erases the content of the string
 * but the capacity is still the same
 * 
 * Returns:
 * - `STRING_NULL_ARG_ERROR` if the argument is NULL
 * - STRING_SUCCESS if there was no error
 */
string_status_t string_clear(string *s)
{
    if (!s || !s->str)
        return STRING_NULL_ARG_ERROR;

    s->size = 0;
    s->str[0] = '\0';

    return STRING_SUCCESS;
}

/*
 * Compares the content of str1 with str2.
 *
 * Returns:
 * -  0  if both strings are equal
 * -  1  if str1 is lexicographically greater than str2
 * - -1  if str1 is lexicographically less than str2
 * - `STRING_NULL_ARG_ERROR` if any argument is `NULL`
 */
int string_compare(const string *str1, const char *str2)
{
    if (!str1 || !str1->str || !str2)
        return STRING_NULL_ARG_ERROR;

    size_t size2 = strlen(str2);
    size_t min = str1->size < size2 ? str1->size : size2;

    unsigned char c1, c2;

    const char *str1_ptr = str1->str;
    const char *str2_ptr = str2;

	while (min--)
    {
		c1 = (unsigned char) *str1_ptr++;
		c2 = (unsigned char) *str2++;

		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
	}

    if (str1->size > size2)
        return 1;
    else if (str1->size < size2)
        return -1;
    
	return 0;
}

/*
 * Compares the content of str1 with str2.
 *
 * Returns:
 * -  0  if both strings are equal
 * -  1  if str1 is lexicographically greater than str2
 * - -1  if str1 is lexicographically less than str2
 * - `STRING_NULL_ARG_ERROR` if any argument is `NULL`
 */
int string_compare_s(const string *str1, const string *str2)
{
    if (!str1 || !str1->str || !str2 || !str2->str)
        return STRING_NULL_ARG_ERROR;

    size_t min = str1->size < str2->size ? str1->size : str2->size;

    unsigned char c1, c2;

    const char *str1_ptr = str1->str;
    const char *str2_ptr = str2->str;

	while (min--)
    {
		c1 = (unsigned char) *str1_ptr++;
		c2 = (unsigned char) *str2_ptr++;

		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
	}

    if (str1->size > str2->size)
        return 1;
    else if (str1->size < str2->size)
        return -1;
    
	return 0;
}

/*
 * Compares the content of `str1` with `str2` up to `size`.
 * If `size` is bigger than `str1` and `str2` sizes,
 * it will be adjuted to be the minimum of them both
 * 
 * Returns:
 * -  0  if both strings are equal
 * -  1  if `str1` is lexicographically greater than `str2`
 * - -1  if `str1` is lexicographically less than `str2`
 * - `STRING_NULL_ARG_ERROR` if any argument is `NULL`
 */
int string_compare_buffer(const string *str1, const char *str2, size_t size)
{
    if (!str1 || !str1->str || !str2)
        return STRING_NULL_ARG_ERROR;
    
    size_t str2_size = strlen(str2);

    if (size > str1->size)
        size = str1->size;

    if (size > str2_size)
        size = str2_size;

    unsigned char c1, c2;

    const char *str1_ptr = str1->str;
    const char *str2_ptr = str2;

	while (size--)
    {
		c1 = (unsigned char) *str1_ptr++;
		c2 = (unsigned char) *str2++;

		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
	}

    if (str1->size > str2_size)
        return 1;
    else if (str1->size < str2_size)
        return -1;
    
	return 0;
}

/*
 * Compares the content of `str1` with `str2` up to `size`.
 * If `size` is bigger than `str1` and `str2` sizes,
 * it will be adjuted to be the minimum of them both
 * 
 * Returns:
 * -  0  if both strings are equal
 * -  1  if `str1` is lexicographically greater than `str2`
 * - -1  if `str1` is lexicographically less than `str2`
 * - `STRING_NULL_ARG_ERROR` if any argument is `NULL`
 */
int string_compare_buffer_s(const string *str1, const string *str2, size_t size)
{
    if (!str1 || !str1->str || !str2)
        return STRING_NULL_ARG_ERROR;

    if (size > str1->size)
        size = str1->size;

    if (size > str2->size)
        size = str2->size;

    unsigned char c1, c2;

    const char *str1_ptr = str1->str;
    const char *str2_ptr = str2->str;
    
	while (size--)
    {
		c1 = (unsigned char) *str1_ptr++;
		c2 = (unsigned char) *str2_ptr++;

		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
	}

    if (str1->size > str2->size)
        return 1;
    else if (str1->size < str2->size)
        return -1;
    
	return 0;
}

/*
 * Converts all characters in the string to lowercase.
 * Parameter:
 * `s`: The string that will be lowercased
 * 
 * Returns:
 * - `STRING_NULL_ARG_ERROR` if the argument is NULL
 * - STRING_SUCCESS if there was no error
 */
string_status_t string_lower(string *s)
{
    if (!s || !s->str)
        return STRING_NULL_ARG_ERROR;

    for (size_t i = 0; i < s->size; i++)
        s->str[i] = tolower((unsigned char) s->str[i]);

    return STRING_SUCCESS;
}

/*
 * Converts all characters in the string to uppercase.
 * Parameter:
 * `s`: The string that will be uppercased
 * 
 * Returns:
 * - `STRING_NULL_ARG_ERROR` if the argument is NULL
 * - STRING_SUCCESS if there was no error
 */
string_status_t string_upper(string *s)
{
    if (!s || !s->str)
        return STRING_NULL_ARG_ERROR;

    for (size_t i = 0; i < s->size; i++)
        s->str[i] = toupper((unsigned char) s->str[i]);

    return STRING_SUCCESS;
}

/*
 * Assigns the substring from start to end of src to dest.
 *
 * Parameters:
 * - `dest`: the string that will that will be assigned by the sub string
 * - `src`: the string that contains the sub string
 * - `start`: the starting position of the sub string (inclusive)
 * - `end`: the ending position of the sub string (exclusive)
 * 
 * Returns:
 * - `STRING_NULL_ARG_ERROR` if any argument is `NULL`
 * - STRING_OUT_OF_RANGE if start or/and end are out of the string range
 * Ex: string size is 10, if end is 12, it is out of range
 * - STRING_ALLOCATION_ERROR if there was an error allocating memory
 * - STRING_SUCCESS if there was no error
 */
string_status_t string_substr(string *dest, const string *src, size_t start, size_t end)
{
    if (!src || !src->str)
        return STRING_NULL_ARG_ERROR;
    
    if (start >= src->size || end > src->size || start > end)
        return STRING_OUT_OF_RANGE;

    size_t substr_size = end - start;
    if (dest->capacity < substr_size)
    {
        if (string_resize(dest, substr_size) == STRING_ALLOCATION_ERROR)
            return STRING_ALLOCATION_ERROR;
        
        dest->capacity = substr_size;
    }

    for (size_t i = 0, j = start; j < end; i++, j++)
        dest->str[i] = src->str[j];

    dest->size = substr_size;
    dest->str[dest->size] = '\0';

    return STRING_SUCCESS;
}

/* 
 * Splits the source string `src` into an array of strings using the specified delimiter.
 * 
 * Parameters:
 * - `src`: The source string to split.
 * - `delimiter`: The character used to split the string.
 * - `count`: Pointer to a size_t variable to store the number of substrings created.
 * - `status`: Pointer to store the result status of the operation.
 *
 * Returns:
 * - An array of strings (`string**`) representing the split substrings.
 * - Sets `status` to:
 *   - `STRING_NULL_ARG_ERROR` if `src` or its internal string is NULL.
 *   - STRING_ALLOCATION_ERROR if memory allocation fails.
 *   - STRING_SUCCESS if the operation succeeds.
 *
 * Notes:
 * - If the delimiter appears consecutively, empty substrings are ignored.
 * - Memory for the array of substrings and each substring must be freed by the caller.
 */
string** string_split(const string *src, const char delimiter, size_t *count, string_status_t *status)
{
    if (!src || !src->str)
    {
        *status = STRING_NULL_ARG_ERROR;
        return NULL;
    }

    *count = 0;

    size_t ocurrences = 0;
    for (size_t i = 0; i < src->size; i++)
    {
        if (src->str[i] == delimiter || src->str[i] == '\0')
        {
            if (i == 0 || src->str[i-1] == delimiter)
                continue;

            ocurrences++;
        }
    }

    if (src->size > 0 && src->str[src->size - 1] != delimiter)
        ocurrences++;

    string **s = (string **) malloc(sizeof(string *) * ocurrences);
    if (!s)
    {
        *status = STRING_ALLOCATION_ERROR;
        return NULL;
    }

    size_t start = 0, split_index = 0;

    for (size_t i = 0; i <= src->size; i++)
    {
        if (src->str[i] == delimiter || src->str[i] == '\0')
        {
            if (start == i)
            {
                start = i + 1;
                continue;
            }

            size_t size = i - start;
            if (size == 0)
                continue;

            string *substr = _string_alloc(size, size);
            if (!substr)
            {
                // Clean up already allocated strings
                for (size_t j = 0; j < split_index; j++)
                    string_free(&s[j]);
                free(s);

                *status = STRING_ALLOCATION_ERROR;
                return NULL;
            }

            memcpy(substr->str, src->str + start, size);
            substr->str[size] = '\0';
            substr->size = size;

            s[split_index++] = substr;
            start = i + 1;
        }
    }
    
    *count = split_index;
    *status = STRING_SUCCESS;
    return s;
}

/*
 * Joins an array of strings into a single string, inserting a delimiter between each.
 *
 * Parameters:
 * - `strings`: Array of strings (`string**`) to join.
 * - `delimiter`: The character to insert between each substring.
 * - `num_strings`: Number of strings in the `strings` array.
 * - `status`: Pointer to store the result status of the operation.
 *
 * Returns:
 * - A new string containing the concatenated result with delimiters.
 * - Sets `status` to:
 *   - `STRING_NULL_ARG_ERROR` if `strings` or its contents are NULL.
 *   - STRING_ALLOCATION_ERROR if memory allocation fails.
 *   - STRING_SUCCESS if the operation succeeds.
 *
 * Notes:
 * - The memory for the returned string must be freed by the caller.
 */
string* string_join(string **strings, char delimiter, size_t num_strings, string_status_t *status)
{
    if (!strings)
    {
        *status = STRING_NULL_ARG_ERROR;
        return NULL;
    }

    size_t size = 0;

    for (size_t i = 0; i < num_strings; i++)
        size += strings[i]->size + 1; // +1 for the delimiter

    size--; // last char can't be a delimiter
    
    string *s = _string_alloc(size, size);
    if (!s)
    {
        *status = STRING_ALLOCATION_ERROR;
        return NULL;
    }

    size_t accumulator = 0;
    for (size_t i = 0; i < num_strings; i++)
    {
        if (!strings[i]->str)
        {
            *status = STRING_NULL_ARG_ERROR;
            string_free(&s);
            return NULL;
        }

        for (size_t j = 0; j < strings[i]->size; j++)
        {
            s->str[accumulator] = strings[i]->str[j];
            accumulator++;
        }
        
        if (i != num_strings - 1)
        {
            s->str[accumulator] = delimiter;        
            accumulator++;
        }
    }

    s->size = accumulator;
    s->str[s->size] = '\0';
    *status = STRING_SUCCESS;
    return s;    
}

/*
 * Reverses the characters in the string `s` in place.
 * 
 * Parameters:
 * - `s`: The string to reverse.
 *
 * Returns:
 * - `STRING_NULL_ARG_ERROR` if the string is NULL or it's internal buffer is NULL.
 * - STRING_SUCCESS if the operation succeeds.
 */
string_status_t string_reverse(string *s)
{
    if (!s || !s->str)
        return STRING_NULL_ARG_ERROR;

    int i = 0, j = s->size - 1;
    while (i < j)
    {
        char tmp = s->str[i];
        s->str[i] = s->str[j];
        s->str[j] = tmp;

        i++;
        j--;
    }

    return STRING_SUCCESS;
}

/*
 * Finds the first occurrence of `substr` in `s`.
 *
 * Parameters:
 * - `s`: The `string` that will be searched.
 * - `substr`: The substring that is search for.
 *
 * Returns:
 * - `STRING_NULL_ARG_ERROR` if any argument is `NULL`.
 * - `-1` if `substr` is not found in `s`.
 * - The index of the first occurrence of `substr` in `s` if found, or `-1` if `substr` is not found.
 */
ssize_t string_find(const string *s, const char *substr)
{
    if (!s || !s->str || !substr)
        return STRING_NULL_ARG_ERROR;

    size_t substr_size = strlen(substr);
    if (s->size == 0 || substr_size == 0)
        return -1;

    for (size_t i = 0; i <= s->size - substr_size; i++)
    {
        if (strncmp(s->str + i, substr, substr_size) == 0)
            return i;
    }

    return -1; // Not found
}

/*
 * Finds the first occurrence of `substr` in `s`.
 *
 * Parameters:
 * - `s`: The `string` that will be searched.
 * - `substr`: The substring that is search for.
 *
 * Returns:
 * - `STRING_NULL_ARG_ERROR` if any argument is `NULL`.
 * - `-1` if `substr` is not found in `s`.
 * - The index of the first occurrence of `substr` in `s` if found, or `-1` if `substr` is not found.
 */
ssize_t string_find_s(const string *s, const string *substr)
{
    if (!s || !s->str || !substr)
        return STRING_NULL_ARG_ERROR;

    if (s->size == 0 || substr->size == 0)
        return -1;

    for (size_t i = 0; i <= s->size - substr->size; i++)
    {
        if (strncmp(s->str + i, substr, substr->size) == 0)
            return i;
    }

    return -1; // Not found
}