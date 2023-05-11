#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "x86.h"


SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;


using namespace std;
int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;

    return 1;
  }

  window = SDL_CreateWindow("x86.bin", 100, 100, 160, 144, SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;

    SDL_Quit();

    return 1;
  }

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == nullptr) {
    std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 1;
  }

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                              SDL_TEXTUREACCESS_STREAMING, 160, 144);

  SDL_Event event;
  bool quit = false;


  if (argc != 2) {
    printf("please run with the following invocation:\n");
    printf("%s <bios path>\n", argv[0]);
    return 0;
  }

  if(!X86_LLE_BIOS_Load(argv[1])) {
    printf("Error loading BIOS!\n");
    return 1;
  }

  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
        printf("quitting!\n");

      }
    }

    if (!X86_CPU_Step()) {
        exit(-1);
    }


    // SDL_RenderClear(renderer);

    // // memset(GAMEBOY_GPU_FrameBuffer, 0, sizeof(GAMEBOY_GPU_FrameBuffer));

    // GAMEBOY_HOST_DrawFramebuffer();
    // // renderTextures
    // SDL_RenderPresent(renderer);
  }


  return 0;
}
