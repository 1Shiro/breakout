#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>


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

void darFormaPaletaCenter(Paddle *paddle)
{
    paddle->forma.x=360;
    paddle->forma.y=670;
    paddle->forma.w=30;
    paddle->forma.h=10;
}
void darFormaPaletaLeft(Paddle *paddle)
{
    paddle->forma.x=340;
    paddle->forma.y=670;
    paddle->forma.w=20;
    paddle->forma.h=10;
}
void darFormaPaletaRight(Paddle *paddle)
{
    paddle->forma.x=390;
    paddle->forma.y=670;
    paddle->forma.w=20;
    paddle->forma.h=10;   
}

void darFormaPelota(Ball *ball)
{
    ball->forma.x = 360;
    ball->forma.y = 670;
    ball->forma.w = 15;
    ball->forma.h = 15;
    ball->vel_x = 2;
    ball->vel_y = -3;
}

Brick *crearLadrillos(int cantidadLadrillos)
{
    Brick *arrayLadrillos;
    arrayLadrillos = malloc((cantidadLadrillos) * sizeof(Brick));
    int aux_x = 25;
    for(int i=0; i<cantidadLadrillos; i++)
    {
        arrayLadrillos[i].forma.x = aux_x;
        arrayLadrillos[i].forma.y = 100;
        arrayLadrillos[i].forma.w = 58;
        arrayLadrillos[i].forma.h = 38;
        aux_x += 70;
    }
    return arrayLadrillos;
}

Brick *crearLadrillos2(int cantidadLadrillos)
{
    Brick *arrayLadrillos;
    arrayLadrillos = malloc((cantidadLadrillos) * sizeof(Brick));
    int aux_x = 25;
    for(int i=0; i<cantidadLadrillos; i++)
    {
        arrayLadrillos[i].forma.x = aux_x;
        arrayLadrillos[i].forma.y = 160;
        arrayLadrillos[i].forma.w = 58;
        arrayLadrillos[i].forma.h = 38;
        aux_x += 70;
    }
    return arrayLadrillos;
}

Brick *crearLadrillos3(int cantidadLadrillos)
{
    Brick *arrayLadrillos;
    arrayLadrillos = malloc((cantidadLadrillos) * sizeof(Brick));
    int aux_x = 25;
    for(int i=0; i<cantidadLadrillos; i++)
    {
        arrayLadrillos[i].forma.x = aux_x;
        arrayLadrillos[i].forma.y = 220;
        arrayLadrillos[i].forma.w = 58;
        arrayLadrillos[i].forma.h = 38;
        aux_x += 70;
    }
    return arrayLadrillos;
}




int main()
{
    startSDL();
    TTF_Init();
    bool closeGame = false;
    bool startGame = false;
    bool restartGame = false;
    bool closeMenu = false;
    bool loser = false;
    bool pause = false;
    bool victory = false;
    int points = 0;
    int lifes = 3;

    char scoreText[13];
    char lifeText[13];

    SDL_Rect rPoints;
    rPoints.x=580;
    rPoints.y=200;
    rPoints.w=70;
    rPoints.h=75;

    SDL_Rect rLifes;
    rLifes.x=580;
    rLifes.y=510;
    rLifes.w=70;
    rLifes.h=75;


    SDL_Event event;

    SDL_Surface *icon = IMG_Load("./images/icon.png");
    SDL_SetWindowIcon(window,icon);

    Paddle paletaCenter;
    darFormaPaletaCenter(&paletaCenter);
    Paddle paletaLeft;
    darFormaPaletaLeft(&paletaLeft);
    Paddle paletaRight;
    darFormaPaletaRight(&paletaRight);

    Ball pelota;
    darFormaPelota(&pelota);

    int cantidadLadrillos = 7;
    Brick *arrayLadrillosGreen,*arrayLadrillosRed,*arrayLadrillosBlue;
    arrayLadrillosGreen = crearLadrillos(cantidadLadrillos);
    arrayLadrillosRed = crearLadrillos2(cantidadLadrillos);
    arrayLadrillosBlue = crearLadrillos3(cantidadLadrillos);



    SDL_Texture *background = loadMedia("./images/fondo_juego.png",renderer);
    SDL_Texture *paddle_textureCenter = loadMedia("./images/paddle5.png",renderer);
    SDL_Texture *paddle_textureLeft = loadMedia("./images/paddleleft.png",renderer);
    SDL_Texture *paddle_textureRigth = loadMedia("./images/paddleright.png",renderer);
    SDL_Texture *ball_texture = loadMedia("./images/pokeball.png",renderer);
    SDL_Texture *brick_textureGreen[cantidadLadrillos];
    SDL_Texture *brick_textureRed[cantidadLadrillos];
    SDL_Texture *brick_textureBlue[cantidadLadrillos];
    SDL_Texture *menuBackground = loadMedia("./images/menubackground.png",renderer);
    SDL_Texture *loserTexture = loadMedia("./images/loser.png",renderer);
    SDL_Texture *pauseTexture = loadMedia("./images/Pause.png",renderer);
    SDL_Texture *victoryTexture = loadMedia("./images/victory.png",renderer);


    for(int i=0; i<cantidadLadrillos; i++)
    {
        brick_textureGreen[i] = loadMedia("./images/block1.png",renderer);
        brick_textureRed[i] = loadMedia("./images/block2.png",renderer);
        brick_textureBlue[i] = loadMedia("./images/block3.png",renderer);
    }

    Mix_Music *backgroundSong = Mix_LoadMUS("./music/theme.ogg");
    Mix_Music *menuMusic = Mix_LoadMUS("./music/menuMusic.mp3");
    Mix_Chunk *paddleSong = Mix_LoadWAV("./music/paddle.wav");
    Mix_Chunk *brickSong = Mix_LoadWAV("./music/bricks.wav");

    TTF_Font *font = TTF_OpenFont("./fonts/OpenSans-Bold.ttf",50);
    SDL_Color color[2] = {{0,0,0},{255,0,5}}; //negro - rojo


    int select = 0;
    char *labels[3] = {"Start","Options","Exit"};

    SDL_Surface *menu[3];
    SDL_Surface *rmenu[3];

    menu[0] = TTF_RenderText_Solid(font,labels[0],color[0]);
    menu[1] = TTF_RenderText_Solid(font,labels[1],color[0]);
    menu[2] = TTF_RenderText_Solid(font,labels[2],color[0]);

    rmenu[0] = TTF_RenderText_Solid(font,labels[0],color[1]);
    rmenu[1] = TTF_RenderText_Solid(font,labels[1],color[1]);
    rmenu[2] = TTF_RenderText_Solid(font,labels[2],color[1]);

    SDL_Texture *menu1 = SDL_CreateTextureFromSurface(renderer, menu[0]);
    SDL_FreeSurface(menu[0]);

    SDL_Texture *menu2 = SDL_CreateTextureFromSurface(renderer, menu[1]);
    SDL_FreeSurface(menu[1]);

    SDL_Texture *menu3 = SDL_CreateTextureFromSurface(renderer, menu[2]);
    SDL_FreeSurface(menu[2]);


    SDL_Texture *rmenu1 = SDL_CreateTextureFromSurface(renderer, rmenu[0]);
    SDL_FreeSurface(rmenu[0]);

    SDL_Texture *rmenu2 = SDL_CreateTextureFromSurface(renderer, rmenu[1]);
    SDL_FreeSurface(rmenu[1]);

    SDL_Texture *rmenu3 = SDL_CreateTextureFromSurface(renderer, rmenu[2]);
    SDL_FreeSurface(rmenu[2]);

    SDL_Rect pos[3];
    
    pos[0].x = SCREEN_WIDTH/2 -50;
    pos[0].y = SCREEN_HEIGHT/2-50;
    pos[0].h = 40;
    pos[0].w = 80;
    
    pos[1].x = SCREEN_WIDTH/2 -50;
    pos[1].y = SCREEN_HEIGHT/2-20;
    pos[1].h = 40;
    pos[1].w = 110;

    pos[2].x = SCREEN_WIDTH/2 -50;
    pos[2].y = SCREEN_HEIGHT/2+10;
    pos[2].h = 40;
    pos[2].w = 60;

    Mix_PlayMusic(menuMusic,-1);

    while(!closeMenu)
    {
        while(SDL_PollEvent(&event) != 0)
        {
            if(event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_DOWN:
                    if(select<3)
                    {
                        select++;
                    }
                    break;
                case SDLK_UP:
                    if(select>1)
                    {
                        select--;
                    }
                    break;
                case SDLK_RETURN:
                    if(select==1)
                    {
                        closeMenu=true;
                    }
                    if(select==2)
                    {

                    }
                    if(select==3)
                    {
                        closeGame=closeMenu=true;
                    }
                    break;
                default:
                    break;
                }
            }
            if(event.type == SDL_QUIT)
            {
                closeGame=closeMenu=true;
            }
        }

        SDL_RenderCopy(renderer,menuBackground,NULL,NULL);

        SDL_RenderCopy(renderer,menu1,NULL,&pos[0]);
        SDL_RenderCopy(renderer,menu2,NULL,&pos[1]);
        SDL_RenderCopy(renderer,menu3,NULL,&pos[2]);

        if (select==1)
        {
            SDL_RenderCopy(renderer,rmenu1,NULL,&pos[0]);
        }else if (select==2)
        {
            SDL_RenderCopy(renderer,rmenu2,NULL,&pos[1]);
        }else if (select==3)
        {
            SDL_RenderCopy(renderer,rmenu3,NULL,&pos[2]);
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(50);

    }//while menu


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
                        if (paletaRight.forma.x<490)
                        {
                            if(!startGame)
                            {
                                paletaRight.forma.x +=VELOCITY;
                                paletaCenter.forma.x +=VELOCITY;
                                paletaLeft.forma.x +=VELOCITY;
                                pelota.forma.x = paletaCenter.forma.x;
                            }else
                            {
                                paletaCenter.forma.x += VELOCITY;
                                paletaRight.forma.x += VELOCITY;
                                paletaLeft.forma.x += VELOCITY;
                            }
                        }
                        break;
                    case SDLK_LEFT:
                        Mix_PlayChannel(-1,paddleSong,0);
                        if(paletaLeft.forma.x>30)
                        {
                            if(!startGame)
                            {
                                paletaLeft.forma.x -=VELOCITY;
                                paletaCenter.forma.x -=VELOCITY;
                                paletaRight.forma.x -=VELOCITY;
                                pelota.forma.x = paletaCenter.forma.x;
                            }else
                            {
                                paletaLeft.forma.x -= VELOCITY;
                                paletaCenter.forma.x -= VELOCITY;
                                paletaRight.forma.x -= VELOCITY;
                            }
                        }
                        break;
                    case SDLK_RETURN:
                        startGame = true;
                        pelota.forma.x += pelota.vel_x;
                        pelota.forma.y += pelota.vel_y;
                        break;
                    case SDLK_p:
                        pause = true;
                        break;
                    default:
                        break;
                    }//switch teclas
                }//if eventos teclas
            }//while eventos

            if(restartGame)
            {
                pelota.forma.x = paletaCenter.forma.x+paletaCenter.forma.h;
                pelota.forma.y = paletaCenter.forma.y;
                arrayLadrillosGreen = crearLadrillos(cantidadLadrillos);
                arrayLadrillosRed = crearLadrillos2(cantidadLadrillos);
                arrayLadrillosBlue = crearLadrillos3(cantidadLadrillos);
                lifes = 3;
                points = 0;
                restartGame=false;

            }

            if(pelota.forma.x>=485+pelota.forma.w)
            {
                pelota.forma.x = 485+pelota.forma.w;
                pelota.vel_x = -pelota.vel_x;
            }
            if(pelota.forma.x<=35-pelota.forma.w)
            {
                pelota.forma.x = 35-pelota.forma.w;
                pelota.vel_x = -pelota.vel_x;
            }
            if(pelota.forma.y>=690-pelota.forma.h)
            {  
                lifes--;
                if(lifes == 0)
                {
                    loser=true;
                }
                pelota.forma.x = paletaCenter.forma.x;
                pelota.forma.y = paletaCenter.forma.y - paletaCenter.forma.h;
                startGame = false;
            }
            if(pelota.forma.y<=10+pelota.forma.h)
            {
                pelota.forma.y = 10+pelota.forma.h;
                pelota.vel_y = -pelota.vel_y;
            }

            if(SDL_HasIntersection(&pelota.forma,&paletaRight.forma))
            {
                if(pelota.vel_x>0){
                    pelota.vel_y = -pelota.vel_y;
                }
                pelota.forma.y = paletaRight.forma.y-pelota.forma.h;
                pelota.vel_y = -pelota.vel_y;
                pelota.vel_x = -pelota.vel_x;
            }

            if(SDL_HasIntersection(&pelota.forma,&paletaCenter.forma))
            {
                pelota.forma.y = paletaCenter.forma.y-pelota.forma.h;
                pelota.vel_y = -pelota.vel_y;
            }
            if(SDL_HasIntersection(&pelota.forma,&paletaLeft.forma))
            {
                if(pelota.vel_x<0){
                    pelota.vel_y = -pelota.vel_y;
                }
                pelota.forma.y = paletaLeft.forma.y-pelota.forma.h;
                pelota.vel_y = -pelota.vel_y;
                pelota.vel_x = -pelota.vel_x;
            }

            for(int i=0; i<cantidadLadrillos; i++)
            {
                if(SDL_HasIntersection(&pelota.forma,&arrayLadrillosGreen[i].forma))
                {
                    Mix_PlayChannel(-1,brickSong,0);
                    points++;
                    if(pelota.vel_y<0)
                    {
                        arrayLadrillosGreen[i].forma.h = 0;
                        arrayLadrillosGreen[i].forma.w = 0;
                    }else
                    {
                        arrayLadrillosGreen[i].forma.h = 0;
                        arrayLadrillosGreen[i].forma.w = 0;
                    }
                    pelota.vel_y = -pelota.vel_y;
                }
                if(SDL_HasIntersection(&pelota.forma,&arrayLadrillosRed[i].forma))
                {
                    Mix_PlayChannel(-1,brickSong,0);
                    points++;
                    if(pelota.vel_y<0)
                    {
                        arrayLadrillosRed[i].forma.h = 0;
                        arrayLadrillosRed[i].forma.w = 0;
                    }else
                    {
                        arrayLadrillosRed[i].forma.h = 0;
                        arrayLadrillosRed[i].forma.w = 0;
                    }
                    pelota.vel_y = -pelota.vel_y;
                }
                if(SDL_HasIntersection(&pelota.forma,&arrayLadrillosBlue[i].forma))
                {
                    Mix_PlayChannel(-1,brickSong,0);
                    points++;
                    if(pelota.vel_y<0)
                    {
                        arrayLadrillosBlue[i].forma.h = 0;
                        arrayLadrillosBlue[i].forma.w = 0;
                    }else
                    {
                        arrayLadrillosBlue[i].forma.h = 0;
                        arrayLadrillosBlue[i].forma.w = 0;
                    }
                    pelota.vel_y = -pelota.vel_y;
                }
            }

            if(startGame)
            {
                pelota.forma.x += pelota.vel_x;
                pelota.forma.y += pelota.vel_y;

            }

            if(points==21)
            {
                victory=true;
            }

            while(loser)
            {
                SDL_RenderCopy(renderer,loserTexture,NULL,NULL);
                while(SDL_PollEvent(&event) != 0)
                {
                    if(event.type == SDL_KEYDOWN)
                    {
                        switch (event.key.keysym.sym)
                        {
                        case SDLK_r:
                            restartGame = true;
                            loser = false;
                            break;
                        
                        default:
                            break;
                        }
                    }
                    if(event.type == SDL_QUIT)
                    {
                        closeGame=true;
                        loser=false;
                    }
                }
                SDL_RenderPresent(renderer);
            }
            while(pause)
            {
                SDL_RenderCopy(renderer,pauseTexture,NULL,NULL);
                while(SDL_PollEvent(&event) != 0)
                {
                    if(event.type == SDL_KEYDOWN)
                    {
                        switch (event.key.keysym.sym)
                        {
                        case SDLK_p:
                            pause = false;
                            break;
                        
                        default:
                            break;
                        }
                    }
                    if(event.type == SDL_QUIT)
                    {
                        closeGame=true;
                        pause=false;
                    }
                }
                SDL_RenderPresent(renderer);
            }
            while(victory)
            {
                SDL_RenderCopy(renderer,victoryTexture,NULL,NULL);
                while(SDL_PollEvent(&event) != 0)
                {
                    if(event.type == SDL_KEYDOWN)
                    {
                        switch (event.key.keysym.sym)
                        {
                        case SDLK_RETURN:
                            victory = false;
                            closeMenu = false;
                            restartGame=true;
                            break;
                        
                        default:
                            break;
                        }
                    }
                    if(event.type == SDL_QUIT)
                    {
                        closeGame=true;
                        victory=false;
                    }
                }
                SDL_RenderPresent(renderer);
            }

            sprintf(scoreText,"%d",points);
            SDL_Surface *scoreSurface = TTF_RenderText_Solid(font,scoreText,color[2]);
            SDL_Texture *scoreTexture = SDL_CreateTextureFromSurface(renderer,scoreSurface);
            SDL_FreeSurface(scoreSurface);

            sprintf(lifeText,"%d",lifes);
            SDL_Surface *lifesSurface = TTF_RenderText_Solid(font,lifeText,color[2]);
            SDL_Texture *lifesTexture = SDL_CreateTextureFromSurface(renderer,lifesSurface);
            SDL_FreeSurface(lifesSurface);

            SDL_RenderCopy(renderer,background,NULL,NULL);
            SDL_RenderCopy(renderer,paddle_textureLeft,NULL,&paletaLeft.forma);
            SDL_RenderCopy(renderer,paddle_textureRigth,NULL,&paletaRight.forma);
            SDL_RenderCopy(renderer,paddle_textureCenter,NULL,&paletaCenter.forma);
            for(int i=0; i<cantidadLadrillos; i++)
            {
                SDL_RenderCopy(renderer,brick_textureGreen[i],NULL,&arrayLadrillosGreen[i].forma);
                SDL_RenderCopy(renderer,brick_textureRed[i],NULL,&arrayLadrillosRed[i].forma);
                SDL_RenderCopy(renderer,brick_textureBlue[i],NULL,&arrayLadrillosBlue[i].forma);
            }
            SDL_RenderCopy(renderer,ball_texture,NULL,&pelota.forma);
            SDL_RenderCopy(renderer,scoreTexture,NULL,&rPoints);
            SDL_RenderCopy(renderer,lifesTexture,NULL,&rLifes);
            SDL_Delay(1000/144);
            SDL_RenderPresent(renderer);
            
        }//while principal

    end();
    return 0;
}//main

