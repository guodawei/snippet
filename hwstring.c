#include <string.h>
#include <stdlib.h>
#include <string.h>

typedef struct _String {
    char *ptr;
    size_t len;
} String;

/*==================================================================*/
/* interfaces for String */
/*==================================================================*/
String *string_create (const char *str);
void string_destroy (String *str);
String *string_add (String *s1, String *s2);

/*==================================================================*/
/* implementation for String */
/*==================================================================*/
inline static String *hwmalloc (char *ptr, size_t len)
{
    String *res = malloc(sizeof(String));
    res->ptr = ptr; res->len = len;
    return res;
}

String *string_create (const char *str)
{
    return hwmalloc(strdup(str), strlen(str));
}

void string_destroy (String *str)
{
    free(str->ptr); free(str);
}

String *string_add (String *s1, String *s2)
{
    const size_t sz = s1->len + s2->len;
    String *res = hwmalloc(malloc(sz+1), sz);

    memcpy(res->ptr, s1->ptr, s1->len);
    memcpy(res->ptr+s1->len, s2->ptr, s2->len+1);
    string_destroy(s1);
    string_destroy(s2);
    return res;
}

