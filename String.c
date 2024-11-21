#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

string* _string_alloc(size_t size)
{
    string *s = (string *) malloc(sizeof(string));
    if (!s)
        return NULL;

    s->size = size;
    s->capacity = size;
    s->str = NULL;

    s->str = (char *) malloc(size + 1);
    if (s->str == NULL)
    {
        free(s);
        return NULL;
    }
    
    return s;
}

string_status_t _string_realloc(string *s, size_t size)
{
    char *tmp = (char *) realloc(s->str, size + 1);

    if (!tmp)
        return STRING_ALLOCATION_ERROR;

    s->str = tmp;
    
    return STRING_SUCCESS;
}

/* 
 * Returns a string with capacity and size of str size - 1 (for the null terminator)
 */
string* new_string(const char *str)
{
    // string *s = (string *) malloc(sizeof(string));
    // if (!s)
    //     return NULL;

    // size_t size = strlen(str);

    // s->size = size;
    // s->capacity = size;

    // s->str = NULL;
    // s->str = (char *) malloc(size + 1);
    
    // if (!s->str)
    // {
    //     free(s);
    //     return NULL;
    // }

    size_t size = strlen(str);
    string *s = _string_alloc(size);
    if (!s)
        return NULL;

    memcpy(s->str, str, size);
    s->str[size] = '\0';

    return s;
}

/* 
 * Returns a string with capacity and size of str size - 1 (for the null terminator)
 */
string* new_string_s(const string *str)
{
    // string *s = (string *) malloc(sizeof(string));
    // if (!s)
    //     return NULL;

    // s->size = str->size;
    // s->capacity = str->size;

    // s->str = NULL;
    // s->str = (char *) malloc(str->size + 1);
    
    // if (!s->str)
    // {
    //     free(s);
    //     return NULL;
    // }

    string *s = _string_alloc(str->size);
    if (!s)
        return NULL;

    memcpy(s->str, str->str, str->size);
    s->str[str->size] = '\0';

    return s;
}

/* 
 * Releases the memory of the string s
 * Returns STRING_NULL_ARG_ERROR if the argument is NULL
 * Returns STRING_SUCCESS if there was no error
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
 * Appends the src string to the end of dest.
 * The function handles any necessary memory allocation.
 * Returns STRING_NULL_ARG_ERROR if any argument is NULL
 * Returns STRING_ALLOCATION_ERROR if there was an error reallocating
 * Returns STRING_SUCCESS if there was no error
 */
string_status_t string_append(string *dest, const char *src)
{
    if (!dest || !src || !dest->str)
        return STRING_NULL_ARG_ERROR;

    size_t src_size = strlen(src);

    if (dest->capacity - dest->size < src_size)
    {
        // char *tmp = (char *) realloc(dest->str, dest->capacity + src_size + 1);

        // if (!tmp)
        //     return STRING_ALLOCATION_ERROR;
        
        // dest->str = tmp;
        if (_string_realloc(dest, dest->capacity + src_size) == STRING_ALLOCATION_ERROR)
            return STRING_ALLOCATION_ERROR;
        dest->capacity += src_size;
    }
    
    memcpy(dest->str + dest->size, src, src_size);

    dest->size += src_size;
    dest->str[dest->size] = '\0';
    
    return STRING_SUCCESS;
}

/*
 * Appends the src string to the end of dest.
 * The function handles any necessary memory allocation.
 * Returns STRING_NULL_ARG_ERROR if any argument is NULL
 * Returns STRING_ALLOCATION_ERROR if there was an error reallocating
 * Returns STRING_SUCCESS if there was no error
 */
string_status_t string_append_s(string *dest, const string *src)
{
    if (!dest || !src || !dest->str || !src->str)
        return STRING_NULL_ARG_ERROR;

    if (dest->capacity - dest->size < src->size)
    {
        // char *tmp = (char *) realloc(dest->str, dest->capacity + src->size + 1);

        // if (!tmp)
        //     return STRING_ALLOCATION_ERROR;

        // dest->str = tmp;

        if (_string_realloc(dest, dest->capacity + src->size) == STRING_ALLOCATION_ERROR)
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
 * Returns STRING_NULL_ARG_ERROR if any argument is NULL
 * Returns STRING_ALLOCATION_ERROR if there was an error reallocating
 * Returns STRING_SUCCESS if there was no error
 */
string_status_t string_assign_s(string *dest, const string *src)
{
    if (!dest || !src || !dest->str || !src->str)
        return STRING_NULL_ARG_ERROR;

    if (dest->capacity < src->size)
    {
        // char *tmp = (char *) realloc(dest->str, src->size + 1);

        // if (!tmp)
        //     return STRING_ALLOCATION_ERROR;

        // dest->str = tmp;

        if (_string_realloc(dest, src->size) == STRING_ALLOCATION_ERROR)
            return STRING_ALLOCATION_ERROR;
        dest->capacity = src->size;
    }
    
    memcpy(dest->str, src->str, src->size);
    
    dest->size = src->size;
    dest->str[dest->size] = '\0';

    return STRING_SUCCESS;
}

/* 
 * Assigns the src string to dest.
 * The function handles any necessary memory allocation.
 * Returns STRING_NULL_ARG_ERROR if any argument is NULL
 * Returns STRING_ALLOCATION_ERROR if there was an error reallocating
 * Returns STRING_SUCCESS if there was no error
 */
string_status_t string_assign(string *dest, const char *src)
{
    if (!dest || !dest->str || !src)
        return STRING_NULL_ARG_ERROR;

    size_t src_size = strlen(src);

    if (dest->capacity < src_size)
    {
        // char *tmp = (char *) realloc(dest->str, src_size + 1);

        // if (!tmp)
        //     return STRING_ALLOCATION_ERROR;

        // dest->str = tmp;

        if (_string_realloc(dest, src_size) == STRING_ALLOCATION_ERROR)
            return STRING_ALLOCATION_ERROR;
        dest->capacity = src_size;
    }

    memcpy(dest->str, src, src_size);

    dest->size = src_size;
    dest->str[dest->size] = '\0';

    return STRING_SUCCESS;
}

/*
 * Erases the content of the string
 * but the capacity is still the same
 * Returns STRING_NULL_ARG_ERROR if the argument is NULL
 * Returns STRING_SUCCESS if there was no error
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
 * Returns:
 *   0  if both strings are equal
 *   1  if str1 is lexicographically greater than str2
 *  -1  if str1 is lexicographically less than str2
 * 
 * Returns STRING_NULL_ARG_ERROR if any argument is NULL
 */
int string_compare(string *str1, const char *str2)
{
    if (!str1 || !str1->str || !str2)
        return STRING_NULL_ARG_ERROR;

    size_t size2 = strlen(str2);
    size_t min = str1->size < size2 ? str1->size : size2;

    unsigned char c1, c2;

    char *str1_ptr = str1->str;
	while (min)
    {
		c1 = *str1_ptr++;
		c2 = *str2++;

		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;

		min--;
	}

    if (str1->size > size2)
        return 1;
    else if (str1->size < size2)
        return -1;
    
	return 0;
}

/*
 * Compares the content of str1 with str2.
 * Returns:
 *   0  if both strings are equal
 *   1  if str1 is lexicographically greater than str2
 *  -1  if str1 is lexicographically less than str2
 * 
 * Returns STRING_NULL_ARG_ERROR if any argument is NULL
 */
int string_compare_s(string *str1, string *str2)
{
    if (!str1 || !str1->str || !str2 || !str2->str)
        return STRING_NULL_ARG_ERROR;

    size_t min = str1->size < str2->size ? str1->size : str2->size;

    unsigned char c1, c2;

    char *str1_ptr = str1->str, *str2_ptr = str2->str;
	while (min)
    {
		c1 = *str1_ptr++;
		c2 = *str2_ptr++;

		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;

		min--;
	}

    if (str1->size > str2->size)
        return 1;
    else if (str1->size < str2->size)
        return -1;
    
	return 0;
}

/*
 * Resizes the string to the specified size.
 * If the size is greater, the new characters are uninitialized.
 * If the size is smaller, the string is truncated.
 * Returns STRING_NULL_ARG_ERROR if the argument is NULL
 * Returns STRING_ALLOCATION_ERROR if there was an error allocating memory
 * Returns STRING_SUCCESS if there was no error
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
 * Converts all characters in the string to lowercase.
 * Returns STRING_NULL_ARG_ERROR if the argument is NULL
 * Returns STRING_SUCCESS if there was no error
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
 * Returns STRING_NULL_ARG_ERROR if the argument is NULL
 * Returns STRING_SUCCESS if there was no error
 */
string_status_t string_upper(string *s)
{
    if (!s || !s->str)
        return STRING_NULL_ARG_ERROR;

    for (size_t i = 0; i < s->size; i++)
        s->str[i] = toupper((unsigned char) s->str[i]);

    return STRING_SUCCESS;
}

string_status_t string_substr(string *dest, string *src, size_t start, size_t end)
{
    if (!src || !src->str)
        return STRING_NULL_ARG_ERROR;
    
    if (start >= src->size || end > src->size || start > end)
        return STRING_OUT_OF_RANGE;
    
    /* FAZER ALOCAÇÂO OU OQ QUER QUE SEJA SE DEST FOR > QUE SRC */
    size_t substr_size = end - start;
    printf("size: %zu\n", substr_size);
    if (dest->capacity < substr_size)
    {
        free(dest->str); 
        printf("malloc\n");
        dest = _string_alloc(substr_size);

        if (dest == NULL)
            return STRING_ALLOCATION_ERROR;
        
        dest->capacity = substr_size;
        dest->size = substr_size; 
    }

    for (size_t i = 0, j = start; j < end; i++, j++)
    {
        dest->str[i] = src->str[j];
        // printf("%c, %zu\n", dest.str, j);
    }

    dest->str[dest->size] = '\0';

    return STRING_SUCCESS;
}

int main()
{
    string *s = new_string("teste");
    string *s2 = new_string("adFAGDSBF");
    string_substr(s2, s, 0, 3);
    printf("%zu %zu %s\n", s->size, s->capacity, s->str);
    printf("%zu %zu %s\n", s2->size, s2->capacity, s2->str);
}

/*

Busca de Padrão (string_find):
Encontra a posição da primeira ocorrência de uma substring:
int string_find(const string *s, const char *pattern);

Inserção de Substring (string_insert):
Insere uma string em uma posição específica:
string_status_t string_insert(string *dest, size_t pos, const char *src);

Trim (string_trim):
Remove espaços em branco do início e fim da string:
string_status_t string_trim(string *s);

Replace (string_replace):
Substitui todas as ocorrências de um caractere ou substring:
string_status_t string_replace(string *s, const char *old, const char *new);

Split (string_split):
Divide a string com base em um delimitador e retorna um array de strings:
string** string_split(const string *s, char delimiter, size_t *count);

Join (string_join):
Concatena um array de strings com um delimitador:
string* string_join(string **strings, size_t count, char delimiter);

Reverse (string_reverse):
Inverte o conteúdo da string:
string_status_t string_reverse(string *s);

*/
