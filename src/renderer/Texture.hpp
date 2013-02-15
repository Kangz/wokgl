#ifndef RENDERER_TEXTURE_HPP
#define RENDERER_TEXTURE_HPP

#include <string>
#include <SDL.h>
#include "enums.hpp"

namespace renderer{

class ActiveTextureManager;
class Buffer;

class Texture{ //TODO Change this to Texture2D sometime
    public:
        Texture();
        Texture(TextureFormat format);
        ~Texture();
        
        int activate();
        int activateAsImage();
        Texture& bind();
        Texture& dataFromBuffer(const Buffer& buf);
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
        Texture& setTarget(TextureTarget target);
        Texture& applyWrapAndFilter();

        GLuint getHandle();
        
        operator GLuint();

    private:
        Texture(Texture&);
        Texture& operator=(Texture&);

        GLuint _handle;
        int _texUnit;
        TextureTarget _target;
        TextureFormat _format;
        TextureWrap _wrapS = TextureWrap::Repeat;
        TextureWrap _wrapT = TextureWrap::Repeat;
        TextureFilter _magFilter = TextureFilter::Linear;
        TextureFilter _minFilter = TextureFilter::Linear;
};

}

#endif //RENDERER_TEXTURE_HPP
