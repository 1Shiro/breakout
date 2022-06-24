#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL_image.h>

const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 720;
const int VELOCITY = 10;
    
SDL_Window* window = NULL;
SDL_Surface* surface = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture*  texture = NULL;


void startSDL(){

    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    window = SDL_CreateWindow("Atari breakout",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
}


void end(){
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Texture* loadMedia(const char *file, SDL_Renderer *renderer){
    surface = IMG_Load(file);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void move();
void explote();
void bounce();
void life();
void points();
void lose();





int main()
{
    startSDL();
    bool closeGame = false;
    SDL_Event event;

    SDL_Texture *background = loadMedia("./images/pxArt.png",renderer);
    SDL_Texture *paddle_texture = loadMedia("./images/green.png",renderer);


    SDL_Rect paddle_object;
    paddle_object.x=340;
    paddle_object.y=490;
    paddle_object.w=40;
    paddle_object.h=10;


        while (!closeGame)
        {
            while (SDL_PollEvent(&event) != 0)
            {
                if(event.type == SDL_QUIT){
                    closeGame = true;
                }

                if(event.type == SDL_KEYDOWN)
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_RIGHT:
                        if (paddle_object.x<490)
                        {
                            paddle_object.x += VELOCITY;
                        }
                        break;
                    case SDLK_LEFT:
                        if(paddle_object.x>200)
                        {
                            paddle_object.x -= VELOCITY;
                        }
                        break;
                    default:
                        break;
                    }//switch teclas
                }//if eventos teclas
            }//while eventos

            SDL_RenderCopy(renderer,background,NULL,NULL);
            SDL_RenderCopy(renderer,paddle_texture,NULL,&paddle_object);
            SDL_Delay(60);
            SDL_RenderPresent(renderer);
            
        }//while principal

    end();
    return 0;
}//main


