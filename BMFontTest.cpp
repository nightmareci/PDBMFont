/**
 * This source file is available as public domain; its author disclaims any
 * copyright to it, if you choose to license as public domain.
 *
 * You may credit the author, Brandon McGriff, when licensing as public domain.
 *
 * You may also license this library under the MIT license; this is intended as
 * an option for countries that have no laws for explicit submission to the
 * public domain:
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Brandon McGriff
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
#define PDBMFONT_TEXT
#define PDBMFONT_BINARY
#define PDBMFONT_XML
#include "PDBMFont.hpp"
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_image.h"
#include "tinyxml2.h"
#include <cstdlib>
#include <vector>
#include <iostream>

int main() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("AngelCode Bitmap Font Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Surface* windowSurface = SDL_GetWindowSurface(window);
	PDBMFont::BMFont font;
	if (!font.read("Font.fnt")) {
		std::cerr << "Failed parsing font." << std::endl;
		return EXIT_FAILURE;
	}
	std::vector<SDL_Texture*> fontPageTextures(font.pages.size(), nullptr);
	IMG_Init(IMG_INIT_PNG);
	for (std::size_t i = 0u; i < font.pages.size(); i++) {
		SDL_Surface* surface = IMG_Load(font.pages[i].c_str());
		fontPageTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}
	IMG_Quit();

	std::string message = "Hello World!\nBitmap font successfully rendered!";
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		SDL_RenderClear(renderer);

		int y = 0;
		int x = -font.chars[message[0]].xoffset;
		for (std::size_t i = 0u; i < message.size(); i++) {
			if (message[i] == '\n') {
				if (message[i + 1] != '\0') {
					x = -font.chars[message[i + 1]].xoffset;
					y += font.common.lineHeight;
				}
				continue;
			}
			PDBMFont::BMFont::Char& fontChar = font.chars[message[i]];
			SDL_Rect srcrect = { (int)fontChar.x, (int)fontChar.y, (int)fontChar.width, (int)fontChar.height };
			SDL_Rect dstrect = { x + fontChar.xoffset, y + fontChar.yoffset, (int)fontChar.width, (int)fontChar.height };
			SDL_RenderCopy(renderer, fontPageTextures[fontChar.page], &srcrect, &dstrect);
			x += (long)fontChar.xadvance;
			if (message[i + 1] != '\0' && font.kernings.count({ message[i], message[i + 1] })) {
				x += font.kernings[{ message[i], message[i + 1] }];
			}
		}

		SDL_RenderPresent(renderer);

		SDL_Delay(10);
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

	for (auto texture : fontPageTextures) {
		SDL_DestroyTexture(texture);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}
