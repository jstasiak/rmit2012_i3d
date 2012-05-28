#include "precompile.h"

#include "font.h"

Font::Font(std::string fileName)
	: fileName(fileName),
	fontSizes()
{
}

Font::~Font() {
	BOOST_FOREACH(auto it, this->fontSizes) {
		TTF_CloseFont((TTF_Font*) it.second);
	}

	this->fontSizes.clear();
}

void Font::drawText(const glm::vec2& position, int size, const std::string& text) {
	assert(this);
	assert(size > 0);

	if(!TTF_WasInit()) {
		TTF_Init();
	}

	// We want to leave attrib state as it was
	glPushAttrib(GL_TEXTURE_2D | GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	auto font = this->getFontForSize(size);
	SDL_Color color = {255, 255, 255};
	auto originalSurface = TTF_RenderText_Blended(font, text.c_str(), color);

	// In order to create OpenGL texture we need surface with dimensions which
	// are all power of 2
	int w = pow(2, ceil( log((float) originalSurface->w)/log(2.0f) ) );
	int h = pow(2, ceil( log((float) originalSurface->h)/log(2.0f) ) );

	auto textSurface = 
	  SDL_CreateRGBSurface(SDL_SRCALPHA, w, h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	SDL_BlitSurface(originalSurface, 0, textSurface, 0);


	unsigned int texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textSurface->w, textSurface->h,
		0, GL_BGRA, GL_UNSIGNED_BYTE, textSurface->pixels);



	auto x = position.x;
	auto y = position.y;

	glColor3f(1, 1, 1);

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex2f(x, y);

		glTexCoord2f(1, 0);
		glVertex2f(x + textSurface->w, y);

		glTexCoord2f(1, 1);
		glVertex2f(x + textSurface->w, y - textSurface->h);

		glTexCoord2f(0, 1);
		glVertex2f(x, y - textSurface->h);
	glEnd();


	glDeleteTextures(1, &texture);
	SDL_FreeSurface(textSurface);
	SDL_FreeSurface(originalSurface);

	glPopAttrib();
}

TTF_Font* Font::getFontForSize(int size) {
	assert(this);
	assert(size > 0);

	auto it = this->fontSizes.find((int) size);
	
	if(! (it != this->fontSizes.end() ) ) {
		auto font = TTF_OpenFont(this->fileName.c_str(), size);

		if(!font) {
			throw new std::exception("Font not found");
		}

		this->fontSizes[size] = (int) font;
	}

	return (TTF_Font*) this->fontSizes[size];
}

