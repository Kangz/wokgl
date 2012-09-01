#ifndef RENDERER_TEXTURE_HPP
#define RENDERER_TEXTURE_HPP

#include <string>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>
#include "enums.hpp"

//Should I move it inside Texture.cpp ?
#include "ActiveTextureManager.hpp"

namespace renderer{

class Texture{ //TODO Change this to Texture2D sometime
    public:
        Texture();
        Texture(TextureFormat format);
        ~Texture();
        
        int activate();
        Texture& bind();
        Texture& emptyData(int width, int height);
        Texture& loadSurface(SDL_Surface* surface);
        Texture& quickFileLoad(const std::string& filename);
        int getLastTextureUnit();

        GLuint getHandle();
        
        operator GLuint();

    private:
        GLuint _handle;
        int _texUnit;
        TextureFormat _format;
};

}

#endif //RENDERER_TEXTURE_HPP
