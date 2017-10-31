#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include "drawline.h"
#include "triangle.h"
#include "object.h" 


/*
 * Return a newly created object based on the arguments provided.
 */
object_t *create_object(SDL_Surface *surface, triangle_t *model, int numtriangles)
{   

    //Declares two random floats;
    int sx = rand() % 60 + 1;
    int sy = rand() % 60 + 1;

    
    //Allocates space for the object and triangles
    object_t *object = malloc(sizeof(object_t));
    object->model = malloc(sizeof(triangle_t) * numtriangles);

    //Assigns values to parameters
    object->scale = 0.1;
    object->rotation = 2000;
    
    object->ty = 500 * object->scale;
    object->tx = 500 * object->scale;

    object->speedx = sx;
    object->speedy = sy;
    object->ttl = 5000; 

    object->numtriangles = numtriangles;
    memcpy(object->model, model, sizeof(triangle_t) * numtriangles);
    object->surface = surface;

    

    return object;
}

/*
 * Destroy the object, freeing the memory.
 */

void destroy_object(object_t *object)
{   
    free(object);
    free(object->model);
    
}

/*
 * Draw the object on its surface.
 */
void draw_object(object_t *object)
{

    //Draws the triangles who makes up the ball
    int i;
    for(i = 0; i < object->numtriangles; i++){
       object->model[i].tx = object->tx;
       object->model[i].ty = object->ty;
       object->model[i].rotation = object->rotation;
       object->model[i].scale = object->scale;

        draw_triangle(object->surface, &object->model[i]);
    }
} 
