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
#define PDBMFONT_DEFINE
#define PDBMFONT_TEXT
#define PDBMFONT_BINARY
#define PDBMFONT_XML
#include "PDBMFont.hpp"
#include "SDL_image.h"

TextRenderer::TextRenderer(SDL_Renderer* const renderer, const std::filesystem::path& fontLocation) : renderer(renderer) {
	if (!font.read(fontLocation.string())) {
		std::cerr << "Failed reading font." << std::endl;
		exit(EXIT_FAILURE);
	}

	std::filesystem::path fontDirectory = fontLocation;
	fontDirectory.remove_filename();

	pageTextures.resize(font.pages.size(), nullptr);
	for (std::size_t i = 0u; i < font.pages.size(); i++) {
		SDL_Surface* const surface = IMG_Load((fontDirectory / font.pages[i]).string().c_str());
		if (!surface) {
			std::cerr << "Failed to load image into a surface." << std::endl;
			exit(EXIT_FAILURE);
		}
		pageTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
		if (!pageTextures[i]) {
			std::cerr << "Failed to create texture from surface." << std::endl;
			exit(EXIT_FAILURE);
		}
		SDL_FreeSurface(surface);
	}
}

TextRenderer::~TextRenderer() {
	for (const auto texture : pageTextures) {
		SDL_DestroyTexture(texture);
	}
}

void TextRenderer::print(const std::string& text, const int x, const int y) {
	int printX = x - font.chars[text[0]].xoffset;
	int printY = y;
	for (std::size_t i = 0u; i < text.size(); i++) {
		if (text[i] == '\n') {
			if (text[i + 1] != '\0') {
				printX = x - font.chars[text[i + 1]].xoffset;
				printY += font.common.lineHeight;
			}
			continue;
		}

		const PDBMFont::BMFont::Char& ch = font.chars[text[i]];
		const SDL_Rect srcrect = {
			int(ch.x), int(ch.y),
			int(ch.width), int(ch.height)
		};
		const SDL_Rect dstrect = {
			printX + ch.xoffset, printY + ch.yoffset,
			int(ch.width), int(ch.height)
		};

		SDL_RenderCopy(renderer, pageTextures[ch.page], &srcrect, &dstrect);

		printX += int(ch.xadvance);
		if (text[i + 1] != '\0' && font.kernings.count({text[i], text[i + 1]})) {
			printX += font.kernings[{text[i], text[i + 1]}];
		}
	}
}
