#pragma once

#include "Renderer.h"

class Texture
{
	unsigned int rendererId;
	std::string filepath;
	unsigned char* localBuffer;
	int width;
	int height;
	int bitsPerPixel;

public:
	Texture(const std::string& filepath);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int getWidth() const { return width; }
	inline int getHeight() const { return height; }
};