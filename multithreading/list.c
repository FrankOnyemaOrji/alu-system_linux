#include <stdlib.h>
#include <stdio.h>
#include "list.h"

/**
 * node_create - Allocates and initializes a new node
 *
 * @content: Pointer to the content to store in the new node
 *
 * Return: A pointer to the newly created node
 * Author: Frank Onyema Orji
 */
node_t *node_create(void *content)
{
    node_t *node = malloc(sizeof(*node));
    if (node == NULL)
        return NULL;

    node->content = content;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

/**
 * list_add - Appends a new node to the end of a list
 *
 * @list:    Pointer to the list to which the node will be added
 * @content: Pointer to the content to store in the new node
 *
 * Return: A pointer to the newly added node
 */
node_t *list_add(list_t *list, void *content)
{
    node_t *node = node_create(content);
    if (node == NULL)
        return NULL;

    node->prev = list->tail;
    if (list->tail)
        list->tail->next = node;
    else
        list->head = node;

    list->tail = node;
    list->size++;

    return node;
}

/**
 * list_init - Sets up an empty list structure
 *
 * @list: Pointer to the list to be initialized
 *
 * Return: A pointer to the initialized list
 */
list_t *list_init(list_t *list)
{
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

/**
 * list_destroy - Frees all nodes and their contents in a list
 *
 * @list:      Pointer to the list to be cleared
 * @free_func: Function pointer used to free the content of each node
 */
void list_destroy(list_t *list, node_func_t free_func)
{
    node_t *node = list->head;
    while (node)
    {
        node_t *next_node = node->next;

        if (free_func)
            free_func(node->content);

        free(node);
        node = next_node;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

/**
 * list_each - Executes a function for each node's content in a list
 *
 * @list: Pointer to the list to iterate over
 * @func: Function pointer to be called with the content of each node
 */
void list_each(list_t *list, node_func_t func)
{
    node_t *node = list->head;
    while (node)
    {
        func(node->content);
        node = node->next;
    }
}
