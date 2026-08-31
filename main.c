#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        printf("Blad SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

if (!(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG))
{
    printf("Blad SDL_image: %s\n", IMG_GetError());
    SDL_Quit();
    return 1;
}

    if ((Mix_Init(MIX_INIT_MP3) & MIX_INIT_MP3) == 0)
    {
        printf("Blad SDL_mixer: %s\n", Mix_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("Blad audio: %s\n", Mix_GetError());
        Mix_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }


    SDL_Window *window = SDL_CreateWindow(
        "Modlitwa do Izraela",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window)
    {
        printf("Blad okna: %s\n", SDL_GetError());
        Mix_CloseAudio();
        Mix_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer)
    {
        printf("Blad renderera: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        Mix_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Surface *jpgSurface = IMG_Load("sciana.jpg");

    if (!jpgSurface)
    {
        printf("Nie mozna wczytac sciana.jpg: %s\n",
               IMG_GetError());

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        Mix_Quit();
        IMG_Quit();
        SDL_Quit();

        return 1;
    }

    SDL_Texture *jpgTexture =
        SDL_CreateTextureFromSurface(renderer, jpgSurface);

    SDL_FreeSurface(jpgSurface);

    if (!jpgTexture)
    {
        printf("Blad tekstury JPG: %s\n",
               SDL_GetError());

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        Mix_Quit();
        IMG_Quit();
        SDL_Quit();

        return 1;
    }

    IMG_Animation *gif = NULL;
    SDL_Texture **gifTextures = NULL;
    Mix_Music *music = NULL;

    int clicked = 0;
    int running = 1;

    int currentFrame = 0;
    Uint32 lastFrameTime = SDL_GetTicks();

    SDL_Event event;

    SDL_Rect imageRect = {
        0,
        0,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    };


    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN &&
                event.button.button == SDL_BUTTON_LEFT)
            {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                if (mouseX >= imageRect.x &&
                    mouseX < imageRect.x + imageRect.w &&
                    mouseY >= imageRect.y &&
                    mouseY < imageRect.y + imageRect.h)
                {
                    if (!clicked)
                    {
                        clicked = 1;
                        printf("+7000\n");
                        printf("271k>6000000");

                        gif = IMG_LoadAnimation("200w.gif");

                        if (!gif)
                        {
                            printf(
                                "Nie mozna wczytac 200w.gif: %s\n",
                                IMG_GetError()
                            );
                        }
                        else
                        {
                            gifTextures =
                                malloc(
                                    sizeof(SDL_Texture *) *
                                    gif->count
                                );

                            if (!gifTextures)
                            {
                                printf("Brak pamieci!\n");
                            }
                            else
                            {
                                for (int i = 0;
                                     i < gif->count;
                                     i++)
                                {
                                    gifTextures[i] =
                                        SDL_CreateTextureFromSurface(
                                            renderer,
                                            gif->frames[i]
                                        );

                                    if (!gifTextures[i])
                                    {
                                        printf(
                                            "Blad klatki GIF: %s\n",
                                            SDL_GetError()
                                        );
                                    }
                                }

                                currentFrame = 0;
                                lastFrameTime = SDL_GetTicks();
                            }

                            music = Mix_LoadMUS("HavaNagilla.mp3");

                            if (!music)
                            {
                                printf(
                                    "Nie mozna wczytac HavaNagilla.mp3: %s\n",
                                    Mix_GetError()
                                );
                            }
                            else
                            {
                                Mix_PlayMusic(music, -1);
                            }
                        }
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(
            renderer,
            0, 0, 0, 255
        );

        SDL_RenderClear(renderer);

        if (!clicked)
        {
            SDL_RenderCopy(
                renderer,
                jpgTexture,
                NULL,
                &imageRect
            );
        }


        else if (gif && gifTextures)
        {
            Uint32 now = SDL_GetTicks();

            if (now - lastFrameTime >=
                (Uint32)gif->delays[currentFrame])
            {
                currentFrame++;

                if (currentFrame >= gif->count)
                {
                    currentFrame = 0;
                }

                lastFrameTime = now;
            }

            SDL_RenderCopy(
                renderer,
                gifTextures[currentFrame],
                NULL,
                &imageRect
            );
        }

        SDL_RenderPresent(renderer);
    }

    if (music)
    {
        Mix_FreeMusic(music);
    }

    if (gifTextures)
    {
        for (int i = 0; i < gif->count; i++)
        {
            if (gifTextures[i])
            {
                SDL_DestroyTexture(gifTextures[i]);
            }
        }

        free(gifTextures);
    }

    if (gif)
    {
        IMG_FreeAnimation(gif);
    }

    SDL_DestroyTexture(jpgTexture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_CloseAudio();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}