#ifndef FONT_H
#define FONT_h

#include <map>

#include <boost/smart_ptr.hpp>
#include <glm/glm.hpp>
#include <SDL_ttf.h>


class Font {

private: std::map< int, int > fontSizes;
private: std::string fileName;

public: Font(std::string fileName);
public: ~Font();

public: void drawText(const glm::vec2& position, int size, const std::string& text);

private: TTF_Font* getFontForSize(int size);
};

#endif