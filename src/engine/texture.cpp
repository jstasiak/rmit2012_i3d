#include "precompile.h"

#include "texture.h"

#include "utils.h"

Texture::Texture(std::string fileName)
	: textureId(0)
{
	this->acquireTextureId();
	this->loadFromFile(fileName);
}

void Texture::acquireTextureId() {
	glGenTextures(1, &(this->textureId));
	assert(this->textureId);
}

Texture::~Texture() {
	if(this->textureId) {
		this->releaseTextureId();
	}
}

void Texture::releaseTextureId() {
	glDeleteTextures(1, &(this->textureId));
	this->textureId = 0;
}

void Texture::loadFromFile(std::string fileName) {
	assert(this);

	auto surface = IMG_Load(fileName.c_str());
	if(!surface) {
		throw std::exception("Cannot load texture from file");
	}

	assert(isPowerOf2(surface->w));
	assert(isPowerOf2(surface->h));

	assert(this->textureId);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->textureId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h,
		0, surface->format->BytesPerPixel == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

}

void Texture::activate() {
	assert(this);
	assert(this->textureId);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->textureId);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::deactivate() {
	assert(this);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}