/**
 * This is free and unencumbered software released into the public domain.
 * 
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 * 
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 * For more information, please refer to <http://unlicense.org/>
 */
#include "TextRenderer.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include "tinyxml2.h"
#include <cstdlib>
#include <vector>
#include <iostream>
#include <filesystem>

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
int main() {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	atexit([]() {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
	});

	if (!(window = SDL_CreateWindow("AngelCode Bitmap Font Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE))) {
		std::cerr << "Failed creating window." << std::endl;
		return EXIT_FAILURE;
	}

	if (!(renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED))) {
		std::cerr << "Failed creating renderer." << std::endl;
		return EXIT_FAILURE;
	}

	{
		TextRenderer textRenderer(renderer, std::filesystem::current_path() / "Font.fnt");
		constexpr int startX = 16, startY = 16;

		SDL_SetRenderDrawColor(renderer, 0x00u, 0x7Fu, 0x00u, SDL_ALPHA_OPAQUE);
		bool quit = false;
		while (!quit) {
			SDL_RenderClear(renderer);

			textRenderer.print("Hello World!\nBitmap font successfully rendered!", startX, startY);

			SDL_RenderPresent(renderer);
			SDL_Delay(10);

			SDL_Event event;
			while (SDL_PollEvent(&event) && !quit) {
				switch (event.type) {
				case SDL_QUIT:
					quit = true;
					break;
				default:
					break;
				}
			}
		}
	}

	return EXIT_SUCCESS;
}
