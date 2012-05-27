#ifndef TEXTURE_H
#define TEXTURE_H

#include <gl/GL.h>

class Texture {
private: GLuint textureId;

public: Texture(std::string fileName);

private: void acquireTextureId();

public: ~Texture();

private: void releaseTextureId();

private: void loadFromFile(std::string fileName);


public: void activate();
public: void deactivate();

};

#endif