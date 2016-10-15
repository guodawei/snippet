#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define WHATEVERTYPE int*
#define WHATEVERFREE (free)
typedef struct _Node {
    struct _Node *next;
    WHATEVERTYPE value;
} Node;

typedef struct _List {
    Node *head;
    Node *tail;
    size_t len;
} List;

/*==================================================================*/
/* interfaces for Node and List*/
/*==================================================================*/
Node *node_create (WHATEVERTYPE val);
void node_destroy (Node *node);

List *list_create (void);
Node *list_top (List *lst);
int list_empty (List *lst);
void list_pop (List *lst);
void list_destroy (List *lst);
void list_append (List *lst, Node *node);

/*==================================================================*/
/* implementation for Node and List */
/*==================================================================*/
Node *node_create (WHATEVERTYPE val)
{
    Node *node = (Node*)malloc(sizeof(Node));
    node->value = val; node->next = NULL;
    return node;
}

void node_destroy (Node *node)
{
    WHATEVERFREE(node->value);
    free(node);
}

/*==================================================================*/
List *list_create (void)
{
    List *lst = (List*)malloc(sizeof(List));
    lst->head = NULL; lst->tail = NULL; lst->len = 0;
    return lst;
}

void list_clear (List *lst)
{
    while (list_empty(lst) == 0)
        list_pop(lst);
}

void list_destroy (List *lst)
{
    list_clear(lst);
    free(lst);
}

int list_empty (List *lst)
{
    return lst->len == 0;
}

Node *list_top (List *lst)
{
    return lst->head;
}

void list_pop (List *lst)
{
    assert(list_empty(lst) == 0);
    Node *top = list_top(lst);
    if (top == lst->tail) {
        lst->head = lst->tail = NULL;
        assert(lst->len  == 1);
    }
    else
        lst->head = top->next;
    node_destroy(top);
    lst->len -= 1;
}

void list_append (List *lst, Node *node)
{
    if (lst->head == NULL) lst->head = node;
    if (lst->tail != NULL) lst->tail->next = node;
    lst->tail = node;
    lst->len += 1;
}

