#ifndef RENDERER_TEXTURE_HPP
#define RENDERER_TEXTURE_HPP

#include <string>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>
#include "enums.hpp"

namespace renderer{

class ActiveTextureManager;

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

        Texture& setFilter(TextureFilter filter);
        Texture& setMagFilter(TextureFilter filter);
        Texture& setMinFilter(TextureFilter filter);
        Texture& setWrap(TextureWrap mode);
        Texture& setWrapS(TextureWrap mode);
        Texture& setWrapT(TextureWrap mode);
        Texture& applyWrapAndFilter();

        GLuint getHandle();
        
        operator GLuint();

    private:
        GLuint _handle;
        int _texUnit;
        TextureFormat _format;
        TextureWrap _wrapS = TextureWrap::Repeat;
        TextureWrap _wrapT = TextureWrap::Repeat;
        TextureFilter _magFilter = TextureFilter::Linear;
        TextureFilter _minFilter = TextureFilter::Linear;
};

}

#endif //RENDERER_TEXTURE_HPP
