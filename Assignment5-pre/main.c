#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL.h>
#include "drawline.h"
#include "triangle.h"
#include "list.h"
#include "teapot_data.h"
#include "sphere_data.h"
#include "object.h"
#include <time.h>//For srand

/* Two macro's that find the lesser or greater of two values */
#define MIN(x,y) (x < y ? x : y)
#define MAX(x,y) (x > y ? x : y)

/*
 * Clear the surface by filling it with 0x00000000(black).
 */
void clear_screen(SDL_Surface *surface)
{
    if(SDL_FillRect(surface, NULL, 0x00000000) < 0){
        fprintf(stderr, "Unable to clear the surface. Error returned: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

/*
 * Accelerate the object; altering its speed based on the boost given.
 */
void accelerate_object(object_t *a, float boost, float maxspeed)
{
    float s;
    float news;
    
    /* Calculate lenght of speed vector */
    s = sqrtf(a->speedx * a->speedx + a->speedy * a->speedy);

    /* Boost speed */
    news = s * boost;
    if (news < 0.0)
        news = 0.0;
    if (news > maxspeed)
        news = maxspeed;    
    a->speedx *= news/s;
    a->speedy *= news/s;
}

/*
 * Animate bouncing balls on the screen.
 */
void bouncing_balls(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Surface *surface)
{   
    time_t t;
    srand((unsigned) time(&t));
    object_t *object = create_object(surface, sphere_model, SPHERE_NUMTRIANGLES );
    object_t *object1 = create_object(surface, sphere_model, SPHERE_NUMTRIANGLES );
    object_t *object2 = create_object(surface, sphere_model, SPHERE_NUMTRIANGLES );

    list_t *theList = list_create();
    list_iterator_t *theIter = list_createiterator()
    
    

    float radius = 500 * object->scale;
    int done = 0;
    SDL_Event event;

    object->ty = radius;
    object->tx = radius;
    object1->ty = radius;
    object1->tx = radius;

    float gravity = 0.1;
    float boost = 0.8;     
    float maxspeed = 100;


    while (!done){
        draw_object(object);
        object->speedy += gravity;
        object->ty += object->speedy;
        object->tx += object->speedx;
        object->rotation += object->speedx;

        if(object->ty >= 900 - radius){
            object->ty = 900 - radius;
            accelerate_object(object, boost, maxspeed);
            object->speedy = object->speedy * -1;
        }

        if(object->tx >= 1600 - radius){
            object->tx = 1600 - radius;
            accelerate_object(object, boost, maxspeed);
            object->speedx = object->speedx * -1;
        }

        if(object->tx <= radius){
            object->tx = radius;
            accelerate_object(object, boost, maxspeed);
            object->speedx = object->speedx * -1;
        }

        if(object->ty <= radius){
            object->ty = radius;
            accelerate_object(object,boost, maxspeed);
            object->speedy = object->speedy * -1;
        }

        draw_object(object1);
        object1->speedy += gravity;
        object1->ty += object1->speedy;
        object1->tx += object1->speedx;
        object1->rotation += object1->speedx;

        if(object1->ty >= 900 - radius){
            object1->ty = 900 - radius;
            accelerate_object(object1, boost, maxspeed);
            object1->speedy = object1->speedy * -1;
        }

        if(object1->tx >= 1600 - radius){
            object1->tx = 1600 - radius;
            accelerate_object(object1, boost, maxspeed);
            object1->speedx = object1->speedx * -1;
        }

        if(object1->tx <= radius){
            object1->tx = radius;
            accelerate_object(object1, boost, maxspeed);
            object1->speedx = object1->speedx * -1;
        }

        if(object1->ty <= radius){
            object1->ty = radius;
            accelerate_object(object1,boost, maxspeed);
            object1->speedy = object1->speedy * -1;
        }

        SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        clear_screen(surface);

        while(SDL_PollEvent(&event));
        {
            if (event.type == SDL_QUIT)
            {
                done = 1;
                break;
            }
        }
    }   
}
   



int main(void)
{
    const size_t bufsize = 100;
    
    /* Change the screen width and height to your own liking */
    const int screen_w = 1600;
    const int screen_h = 900;

    char errmsg[bufsize];
    SDL_Surface *surface;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *texture;
    
    /* Initialize SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < -1) {
        snprintf(errmsg, bufsize, "Unable to initialize SDL.");
        goto error;
    }
    
    /* Create a 1600x900 window */
    window = SDL_CreateWindow("The Amazing Bouncing Balls",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              screen_w, screen_h,
                              0);
    if(!window) {
        snprintf(errmsg, bufsize, "Unable to get video surface.");
        goto error;
    }

    /* Create window renderer */
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer) {
        snprintf(errmsg, bufsize, "Unable to create window renderer.");
        goto error;
    }

    /* Create the texture that contain the currentrly drawn pixels */
    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STATIC,
                                screen_w, screen_h);
    if(!texture) {
        snprintf(errmsg, bufsize, "Unable to create texture.");
        goto error;
    }

    /* Create the suface in RAM that we manipulate the pixels of */
    surface = SDL_CreateRGBSurface(0, screen_w, screen_h, 32,
                                   0x00FF0000,
                                   0x0000FF00,
                                   0x000000FF,
                                   0xFF000000);
    if(!surface) {
        snprintf(errmsg, bufsize, "Unable to create surface.");
        goto error;
    }

    /* Start bouncing some balls */
    bouncing_balls(renderer, texture, surface);

    /* Shut down SDL */
    SDL_Quit();

    /* Wait a little bit jic something went wrong (so that printfs can be read) */
    SDL_Delay(5000);
    
    return 0;

    /* Upon an error, print message and quit properly */
error:
    fprintf(stderr, "%s Error returned: %s\n", errmsg, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}
