#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/*
 * List implementation
 */

typedef struct listnode listnode_t;

struct listnode {
    listnode_t  *next;
    void        *item;
};

struct list {
    listnode_t *head;
    int numitems;
};

/*
 * Returns a newly created, empty list.
 */
list_t *list_create(void)
{   

    //Declaring and allocation space
    list_t *list = NULL;
    list = malloc(sizeof(list_t));


    //Assigning values
    list->head = NULL;
    list->numitems = 0;
    return list;
}
/*
 * Frees the list; list and nodes, but not the items it holds.
 */
void list_destroy(list_t *list){
    free(list);

}

/*
 * Adds an item first in the provided list.
 */
void list_addfirst(list_t *list, void *item)
{   

    //Declaring and allocation space
    listnode_t *new_node;
    new_node = malloc(sizeof(listnode_t));


    //Assigning values
    new_node->item = item;
    new_node->next = list->head;
    list->head = new_node;
    list->numitems += 1;

}

/*
 * Adds an item last in the provided list.
 */
void list_addlast(list_t *list, void *item)
{   

    //Declaring and allocation space
    listnode_t *current = malloc(sizeof(listnode_t));
    current->next = list->head;
    
    
    //Checks if the list is empty
    if(list->head == NULL){

        //Assigning values
        list->head = malloc(sizeof(listnode_t));   
        list->head->item = item;
        list->head->next = NULL;
    }
    

    //Iterates to the end of the list
    else {
    while(current->next != NULL){
        current = current->next;
    }


    //Assigning values
    current->next = malloc(sizeof(listnode_t));
    current->next->item = item;
    current->next->next = NULL;
}
    list->numitems += 1;
}

/*
 * Removes an item from the provided list, only freeing the node.
 */
void list_remove(list_t *list, void *item)
{   
    listnode_t *current = list->head;
    
    //Checks if the first item is going to be removed
    if(current->item == item){
        list->head = current->next;
        free(current);
        list->numitems -= 1;

    }else{
    
    //Iterates to the node before the item that will be removed 
    while(current->next->item != item){
        current = current->next;
        }
    listnode_t *temp = current->next;
    current->next = current->next->next;
    free(temp);
    list->numitems -= 1;
}  
}

/*
 * Return the number of items in the list.
 */
int list_size(list_t *list)
{
    return list->numitems;
}



/*
 * Iterator implementation
 */
 

struct list_iterator {
    listnode_t *next;
    list_t *list;
};


/*
 * Return a newly created list iterator for the given list.
 */
list_iterator_t *list_createiterator(list_t *list)
{
    //Declaring and allocating space
    list_iterator_t *iter = malloc(sizeof(list_iterator_t));


    //Prints an error if something goes wrong
    if(iter == NULL){
        printf("An error has occured while creating the iterator");
        return NULL;
    }


    //Assigning values
    iter->list = list;
    
    iter->next = list->head;
    
    return iter;
}


/*
 * Free the memory for the given list iterator.
 */
void list_destroyiterator(list_iterator_t *iter)
{
    free(iter);
}


/*
 * Move iterator to next item in list and return current.
 */
void *list_next(list_iterator_t *iter)
{   

    //Checks if the iterator has reached the end of the list
    if(iter->next == NULL){
        return NULL;
    }else{
    
    //Iterates forward and returns an item
    listnode_t *current = iter->next;
    iter->next = iter->next->next;
    return current->item;
    }
}


/*
 * Let iterator point to first item in list again.
 */
void list_resetiterator(list_iterator_t *iter)
{
    iter->next = iter->list->head;
}
