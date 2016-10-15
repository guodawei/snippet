#include <stdlib.h>

typedef struct _Hash {
    List *value;
    size_t size;
} Hash;

/*==================================================================*/
/* bucket number for hash table */
/*==================================================================*/
const static size_t prime_num [20] = {
    67,    97,    151,   197,   257,
    293,   397,   557,   797,   1103,
    1597,  2297,  3187,  6287,  9257,
    12289, 24593, 49157, 98317, 25165843
};

static size_t calc_bucket_num (size_t num)
{
    for (size_t i = 0; i < 20U; ++i) {
        if (prime_num[i] >= num)
            return prime_num[i];
    }
    return (~0x0);
}

/*==================================================================*/
/* simple hash function for string type */
/*==================================================================*/
static size_t hash_function (const char *s, size_t len)
{
    size_t key = 0;
    while (len--)
        key = key^9 + *s++;
    return key;
}

/*==================================================================*/
/* implementation for Hash */
/*==================================================================*/
Hash *hash_create (size_t size)
{
    size_t bucket = calc_bucket_num(size);

    Hash *hash = (Hash*)malloc(sizeof(Hash));
    hash->value = (List*)malloc(bucket * sizeof(List));
    memset(hash->value, 0, bucket *sizeof(List));
    hash->size = bucket;
    return hash;
}

void hash_destroy (Hash *hash)
{
    for (size_t i = 0; i < hash->size; ++i)
        list_clear(&hash->value[i]);
    free(hash->value);
    free(hash);
}

size_t hash_key (Hash *hash, Node *node)
{
    String *str = node->value;
    size_t val = hash_function(str->ptr, str->len);
    return val % hash->size;
}

List *hash_lookup (Hash *hash, Node *node)
{
    size_t key = hash_key(hash, node);
    return &hash->value[key];
}

void hash_insert (Hash *hash, Node *node)
{
    List *lst = hash_lookup(hash, node);
    list_append(lst, node);
}

