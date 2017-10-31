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
    //Gives rand a seed based on the current time
    time_t t;
    srand((unsigned) time(&t));


    //Initializing pointers
    object_t *iterObject = NULL;
    list_t *theList = list_create();
    list_iterator_t *theIter = list_createiterator(theList);

    //Declaring fps variables
    const int FRAMES_PER_SECOND = 200;
    int frame = 0;
    int delta = 0;
    int lastFrame = 0;

   
    //Function for calculating radius variable
    float setRadius(){
        object_t *object = create_object(surface, sphere_model, SPHERE_NUMTRIANGLES );
        float radius = 500 * object->scale;
        return radius;

    }
    
    //Assigning radius to returned variable
    float radius = setRadius();


    //Function for creating and implementing a ball into a list
    void addBall(list_t *list){
        object_t *object = create_object(surface, sphere_model, SPHERE_NUMTRIANGLES );
        object->tx = radius;
        object->ty = radius;
        list_addlast(list, object);
        
    }
    
    //A loop for creating multiple balls
    for(int i = 0; i < 5; i++){
        addBall(theList);
    }

    
    //Variables for deciding when the user has quit the program
    int done = 0;
    SDL_Event event;

    //Program physics attributes 
    float gravity = 0.3;
    float boost = 0.8;     
    float maxspeed = 3000;

    //Draws and updates balls as long as the program runs
    while (!done){

        //Resets and initalizes iterator
        list_resetiterator(theIter);
        while((iterObject = (object_t*)list_next(theIter)) != NULL){

            //Draws the balls and applies physics to them
            draw_object(iterObject);
            iterObject->speedy += gravity;
            iterObject->ty += iterObject->speedy;
            iterObject->tx += iterObject->speedx;
            iterObject->rotation += iterObject->speedx;
            
            //Prevents the balls from bouncing off screen
            if(iterObject->ty >= 900 - radius){
                    iterObject->ty = 900 - radius-0.1;
                    accelerate_object(iterObject, boost, maxspeed);
                    iterObject->speedy = iterObject->speedy * -1;
            }
        
            if(iterObject->tx >= 1600 - radius){
                    iterObject->tx = 1600 - radius-0.1;
                    accelerate_object(iterObject, boost, maxspeed);
                    iterObject->speedx = iterObject->speedx * -1;
            }
        
            if(iterObject->tx <= radius){
                    iterObject->tx = radius+0.1;
                    accelerate_object(iterObject, boost, maxspeed);
                    iterObject->speedx = iterObject->speedx * -1;
            }
        
            if(iterObject->ty <= radius){
                    iterObject->ty = radius+0.1;
                    accelerate_object(iterObject,boost, maxspeed);
                    iterObject->speedy = iterObject->speedy * -1;
            }
            
            
            //Detects when the ball has stopped moving and destroys the ball
            if(abs(iterObject->speedx) == 0 && abs(iterObject->speedy) == 0){
                printf("  (%fx|%fy)  ",iterObject->speedx, iterObject->speedy);
                    
                if(iterObject->ttl == 5000){
                        iterObject->ttl += SDL_GetTicks()-delta;
                        printf("|%d|  ", iterObject->ttl);
                        break;
                }
                            
                    
                if(iterObject->ttl <= SDL_GetTicks()){
                    printf("x%dx  ", iterObject->ttl);
                    iterObject->model = NULL;
                    destroy_object(iterObject);
                    list_remove(theList, iterObject);
                                               
                }
            
            }
                //Caps the frames at gives frames per second
                frame = SDL_GetTicks();
                delta = frame - lastFrame;
                lastFrame = frame;
                if(delta < 1000 / FRAMES_PER_SECOND) { 
                    SDL_Delay(  (1000/FRAMES_PER_SECOND) - delta); 
                }
            }   

        //Updates the screen constantly
        SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        clear_screen(surface);
        

        //Terminates program
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


    SDL_GetTicks();
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
