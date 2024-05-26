#include "texture.hpp"

#include "stb_image.h"

//Texture::Texture(
//	const char* path,
//	const char* unfirom_name,
//	GLenum wrap_option,
//	bool flip
//)
//{
//	_uniform_name = unfirom_name;
//
//	glGenTextures(1, &_ID);
//	glBindTexture(GL_TEXTURE_2D, _ID);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_option);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_option);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	stbi_set_flip_vertically_on_load(flip);
//
//	unsigned char *buf = stbi_load(path, &_wdt, &_hgt, &_nr_channels, 0);
//	if (not buf)
//	{
//		printf("TEXTURE::FAILED_TO_LOAD_IMAGE::%s\n", path);
//		return;
//	}
//
//	int format = get_texture_format(_nr_channels);
//	glTexImage2D(GL_TEXTURE_2D, 0, format, _wdt, _hgt, 0, format, GL_UNSIGNED_BYTE, buf);
//	glGenerateMipmap(GL_TEXTURE_2D);
//
//	stbi_image_free(buf);
//}
//
//const char* Texture::get_uniform_name()
//{
//	return _uniform_name;
//}
//
//GLuint Texture::ID()
//{
//	return _ID;
//}

unsigned load_texture_to_open_gl_and_get_id(const char* path)
{
	unsigned id;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);


	int wdt, hgt, nr_channels;
	unsigned char* buf = stbi_load(path, &wdt, &hgt, &nr_channels, 0);
	if (not buf)
	{
		printf("TEXTURE::FAILED_TO_LOAD_IMAGE::%s\n", path);
		return 0;
	}

	int format = get_texture_format(nr_channels);
	glTexImage2D(GL_TEXTURE_2D, 0, format, wdt, hgt, 0, format, GL_UNSIGNED_BYTE, buf);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(buf);

	return id;
}

void load_texture_and_add_as_entity(const char* path, const char* name)
{
	Texture* texture = new Texture;
	texture->texture_id = load_texture_to_open_gl_and_get_id(path);
	texture->name = name;
	texture->group = Entity::Group::Texture;
	add_entity(Entity::Group::Texture, (Entity*)texture);
}

int get_texture_format(int nr_channels)
{
	int format = GL_RED;
	switch (nr_channels)
	{
	case 3: format = GL_RGB; break;
	case 4: format = GL_RGBA; break;
	}
	return format;
}
