#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>


typedef struct Paddle
{
    SDL_Rect forma;
    double vel_x;
}Paddle;

typedef struct Brick
{
    SDL_Rect forma;
    int hardness;
}Brick;

typedef struct Ball
{
    SDL_Rect forma;
    int vel_x;
    int vel_y;
    int strenght;
}Ball;


const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 720;
const int VELOCITY = 20;


SDL_Window *window = NULL;
SDL_Surface *surface = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;


void startSDL()
{
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024);
    window = SDL_CreateWindow("Atari breakout",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
}


void end()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Texture* loadMedia(const char *file, SDL_Renderer *renderer)
{
    surface = IMG_Load(file);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}


void explote();
void bounce();
void life();
void points();
void lose();


void darFormaPaleta(Paddle *paddle)
{
    paddle->forma.x=340;
    paddle->forma.y=490;
    paddle->forma.w=40;
    paddle->forma.h=10;
}

void darFormaPelota(Ball *ball)
{
    ball->forma.x = 350;
    ball->forma.y = 475;
    ball->forma.w = 15;
    ball->forma.h = 15;
    ball->vel_x = 1;
    ball->vel_y = -1;
}

Brick *crearLadrillos(int cantidadLadrillos)
{
    Brick *arrayLadrillos;
    arrayLadrillos = malloc((cantidadLadrillos) * sizeof(Brick));
    int aux_x = 208;
    for(int i=0; i<cantidadLadrillos; i++)
    {
        arrayLadrillos[i].forma.x = aux_x;
        arrayLadrillos[i].forma.y = 100;
        arrayLadrillos[i].forma.w = 32;
        arrayLadrillos[i].forma.h = 18;
        aux_x += 40;
    }
    return arrayLadrillos;
}

Brick *crearLadrillos2(int cantidadLadrillos)
{
    Brick *arrayLadrillos;
    arrayLadrillos = malloc((cantidadLadrillos) * sizeof(Brick));
    int aux_x = 208;
    for(int i=0; i<cantidadLadrillos; i++)
    {
        arrayLadrillos[i].forma.x = aux_x;
        arrayLadrillos[i].forma.y = 123;
        arrayLadrillos[i].forma.w = 32;
        arrayLadrillos[i].forma.h = 18;
        aux_x += 40;
    }
    return arrayLadrillos;
}

int main()
{
    startSDL();
    bool closeGame = false;
    bool startGame = false;
    bool restartGame = false;
    int points = 0;
    SDL_Event event;

    Paddle paleta;
    darFormaPaleta(&paleta);

    Ball pelota;
    darFormaPelota(&pelota);

    int cantidadLadrillos = 2;
    Brick *arrayLadrillosGreen, *arrayLadrillosRed;
    arrayLadrillosGreen = crearLadrillos(cantidadLadrillos);
    arrayLadrillosRed = crearLadrillos2(cantidadLadrillos);

    SDL_Texture *background = loadMedia("./images/pxArt.png",renderer);
    SDL_Texture *paddle_texture = loadMedia("./images/green.png",renderer);
    SDL_Texture *ball_texture = loadMedia("./images/pokeball.png",renderer);
    SDL_Texture *brick_textureGreen[cantidadLadrillos];
    SDL_Texture *brick_textureRed[cantidadLadrillos];
    for(int i=0; i<cantidadLadrillos; i++)
    {
        brick_textureGreen[i] = loadMedia("./images/green.png",renderer);
        brick_textureRed[i] = loadMedia("./images/red.png",renderer);

    }

    Mix_Music *backgroundSong = Mix_LoadMUS("./music/theme.ogg");
    Mix_Chunk *paddleSong = Mix_LoadWAV("./music/paddle.wav");
    Mix_Chunk *brickSong = Mix_LoadWAV("./music/bricks.wav");
    Mix_PlayMusic(backgroundSong,-1);


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
                        Mix_PlayChannel(-1,paddleSong,0);
                        if (paleta.forma.x<480)
                        {
                            if(!startGame)
                            {
                                paleta.forma.x +=VELOCITY;
                                pelota.forma.x = paleta.forma.x+paleta.forma.h;
                            }else
                            {
                                paleta.forma.x += VELOCITY;
                            }
                        }
                        break;
                    case SDLK_LEFT:
                        Mix_PlayChannel(-1,paddleSong,0);
                        if(paleta.forma.x>200)
                        {
                            if(!startGame)
                            {
                                paleta.forma.x -=VELOCITY;
                                pelota.forma.x = paleta.forma.x+paleta.forma.h;
                            }else
                            {
                                paleta.forma.x -= VELOCITY;
                            }
                        }
                        break;
                    case SDLK_RETURN:
                        startGame = true;
                        pelota.forma.x += pelota.vel_x;
                        pelota.forma.y += pelota.vel_y;
                        break;
                    case SDLK_y:
                        restartGame = true;
                        startGame = false;
                        break;
                    default:
                        break;
                    }//switch teclas
                }//if eventos teclas
            }//while eventos

            if(restartGame)
            {
                pelota.forma.x = paleta.forma.x+paleta.forma.h;
                pelota.forma.y = paleta.forma.y;
                arrayLadrillosGreen = crearLadrillos(cantidadLadrillos);
                arrayLadrillosRed = crearLadrillos2(cantidadLadrillos);
                restartGame=false;
            }

            if(pelota.forma.x>=500+pelota.forma.w)
            {
                pelota.forma.x = 500+pelota.forma.w;
                pelota.vel_x = -pelota.vel_x;
            }
            if(pelota.forma.x<=202-pelota.forma.w)
            {
                pelota.forma.x = 202-pelota.forma.w;
                pelota.vel_x = -pelota.vel_x;
            }
            if(pelota.forma.y>=510-pelota.forma.h)
            {
                startGame = false;
            }
            if(pelota.forma.y<=40+pelota.forma.h)
            {
                pelota.forma.y = 40+pelota.forma.h;
                pelota.vel_y = -pelota.vel_y;
            }

            if(SDL_HasIntersection(&pelota.forma,&paleta.forma))
            {
                pelota.forma.y = paleta.forma.y-pelota.forma.h;
                pelota.vel_y = -pelota.vel_y;
            }
            for(int i=0; i<cantidadLadrillos; i++)
            {
                if(SDL_HasIntersection(&pelota.forma,&arrayLadrillosGreen[i].forma))
                {
                    Mix_PlayChannel(-1,brickSong,0);
                    points++;
                    arrayLadrillosGreen[i].forma.h = 0;
                    arrayLadrillosGreen[i].forma.w = 0;
                    if(pelota.vel_y<0)
                    {
                        pelota.forma.y = arrayLadrillosGreen[i].forma.y+pelota.forma.h;
                    }else
                    {
                        pelota.forma.y = arrayLadrillosGreen[i].forma.y-pelota.forma.h;
                    }
                    pelota.vel_y = -pelota.vel_y;
                }
                if(SDL_HasIntersection(&pelota.forma,&arrayLadrillosRed[i].forma))
                {
                    Mix_PlayChannel(-1,brickSong,0);
                    points++;
                    arrayLadrillosRed[i].forma.h = 0;
                    arrayLadrillosRed[i].forma.w = 0;
                    if(pelota.vel_y<0)
                    {
                        pelota.forma.y = arrayLadrillosRed[i].forma.y+pelota.forma.h;
                    }else
                    {
                        pelota.forma.y = arrayLadrillosRed[i].forma.y-pelota.forma.h;
                    }
                    pelota.vel_y = -pelota.vel_y;
                }
            }

            if(startGame)
            {
                pelota.forma.x += pelota.vel_x;
                pelota.forma.y += pelota.vel_y;

            }

            SDL_RenderCopy(renderer,background,NULL,NULL);
            SDL_RenderCopy(renderer,paddle_texture,NULL,&paleta.forma);
            for(int i=0; i<cantidadLadrillos; i++)
            {
                SDL_RenderCopy(renderer,brick_textureGreen[i],NULL,&arrayLadrillosGreen[i].forma);
                SDL_RenderCopy(renderer,brick_textureRed[i],NULL,&arrayLadrillosRed[i].forma);
            }
            SDL_RenderCopy(renderer,ball_texture,NULL,&pelota.forma);
            SDL_Delay(1000/144);
            SDL_RenderPresent(renderer);
            
        }//while principal

    end();
    return 0;
}//main


