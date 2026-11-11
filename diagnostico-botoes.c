
#include <SDL.h>
#include <stdio.h>

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK |
                 SDL_INIT_GAMECONTROLLER) != 0) {
        fprintf(stderr, "SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Diagnostico R36S",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480, SDL_WINDOW_FULLSCREEN_DESKTOP
    );

    if (!window) {
        fprintf(stderr, "Janela: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Joystick *joy = NULL;
    if (SDL_NumJoysticks() > 0)
        joy = SDL_JoystickOpen(0);

    printf("Joysticks: %d\n", SDL_NumJoysticks());
    fflush(stdout);

    Uint32 start = SDL_GetTicks();
    int running = 1;

    while (running && SDL_GetTicks() - start < 90000) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;

            if (event.type == SDL_JOYBUTTONDOWN) {
                printf("BOTAO FISICO: %d\n",
                       event.jbutton.button);
                fflush(stdout);
            }

            if (event.type == SDL_JOYBUTTONUP) {
                printf("SOLTO: %d\n",
                       event.jbutton.button);
                fflush(stdout);
            }

            if (event.type == SDL_KEYDOWN &&
                !event.key.repeat) {
                printf("TECLA SDL: %s (%d)\n",
                       SDL_GetKeyName(event.key.keysym.sym),
                       event.key.keysym.sym);
                fflush(stdout);
            }
        }

        SDL_SetRenderDrawColor(renderer, 20, 30, 50, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    if (joy) SDL_JoystickClose(joy);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
