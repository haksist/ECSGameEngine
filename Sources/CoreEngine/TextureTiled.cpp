
#include <iostream>
#include "TextureTiled.h"

TextureTiled::TextureTiled(const GLvoid*pixels,
	int width, int height,
	int rows, int columns, int tilesCount,
	bool generateMipmaps,
	GLenum internalFormat,
	GLenum format,
	GLenum dataType)
:Texture2D(pixels, width, height, generateMipmaps,internalFormat,format,dataType), rows(rows), columns(columns), tiles_count(tilesCount)
{
	frameCount = tilesCount;
	textures = new TextureRegion[tilesCount];

	float tile_w = (float)width / columns;
	float tile_h = (float)height / rows;
	int r = 0, c = 0;

	for (int i = 0; i < tilesCount; i++)
	{
		r = i / columns;
		c = i % columns;

/*([6],[7])----------([4],[5])
		   |		|
		   |		|
		   |		|
  ([0],[1])----------([2],[3])*/
		textures[i].uv[0] = c*tile_w/width; 
		textures[i].uv[1] = (rows-1-r)*tile_h/height;

		textures[i].uv[2] = (c+1)*tile_w/width;
		textures[i].uv[3] = (rows - 1 - r)*tile_h/height;

		textures[i].uv[4] = (c+1)*tile_w/width;
		textures[i].uv[5] = (rows-r)*tile_h/height;

		textures[i].uv[6] = c*tile_w/width;
		textures[i].uv[7] = (rows - r)*tile_h/height;
	}
}

TextureTiled::~TextureTiled()
{
	std::cout << "TextureTiled: ~~~deleted~~~" << std::endl;
}