#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>

#include "ActiveTextureManager.hpp"
#include "Buffer.hpp"
#include "Texture.hpp"

namespace renderer{

Texture::Texture(): Texture(TextureFormat::RGBA){
}

Texture::Texture(TextureFormat format):_format(format){
    glGenTextures(1, &_handle);
    _texUnit = -1;
}

Texture::~Texture(){
    glDeleteTextures(1, &_handle);
}

int Texture::activate(){
    _texUnit = ActiveTextureManager::getInstance(ActiveManagerType::Sampler).activate(this);
    return _texUnit;
}

int Texture::activateAsImage(){
    _texUnit = ActiveTextureManager::getInstance(ActiveManagerType::Image).activate(this);
    return _texUnit;
}

//TODO check it is not already bound
Texture& Texture::bind(){
	glBindTexture(static_cast<int>(_target), _handle);
    return *this;
}

Texture& Texture::dataFromBuffer(const Buffer& buf){
    this->_target = TextureTarget::TextureBuffer;
    this->bind().applyWrapAndFilter();
    glTexBuffer(GL_TEXTURE_BUFFER, static_cast<int>(_format), buf);
    return *this;
}

Texture& Texture::emptyData(int width, int height){
    this->_target = TextureTarget::Texture2D;
    this->bind().applyWrapAndFilter();
	glTexImage2D(GL_TEXTURE_2D, 0, static_cast<int>(_format), width, height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, 0);
    return *this;
}

Texture& Texture::loadSurface(SDL_Surface* surface){
    if(!surface){ //fugly
        throw "Cannot load a texture from a NULL Surface";
    }
    
    //Check that the surface size is a power of two
    if((surface->w & (surface->w - 1)) != 0 ||
        (surface->h & (surface->h - 1)) != 0){
        throw "Surface size is NPOT";
    }

    //Use SDL to convert the surface to a simple ABGR8888 format
    int bpp;
    Uint32 Rmask, Gmask, Bmask, Amask;
    SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ABGR8888, &bpp,
        &Rmask, &Gmask, &Bmask, &Amask);

    int w = surface->w;
    int h = surface->h;

    SDL_Surface *convertedSurface = SDL_CreateRGBSurface(0, w, h, bpp,
        Rmask, Gmask, Bmask, Amask);

    SDL_BlitSurface(surface, NULL, convertedSurface, NULL);

    //Upload the surface to the GPU
    this->_target = TextureTarget::Texture2D;
    this->bind().applyWrapAndFilter();
 
    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<int>(_format), w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
            convertedSurface->pixels);
    
    return *this;
}

Texture& Texture::quickFileLoad(const std::string& filename){
    SDL_Surface* surf = IMG_Load(filename.c_str());
    if(surf){
        this->loadSurface(surf);
        SDL_FreeSurface(surf);
    }else{
        throw "Could not load image: " + filename;
    }
    return *this;
}

int Texture::getLastTextureUnit(){
    return _texUnit;
}

Texture& Texture::setFilter(TextureFilter filter){
    _magFilter = _minFilter = filter;
    return *this;
}

Texture& Texture::setMagFilter(TextureFilter filter){
    _magFilter = filter;
    return *this;
}

Texture& Texture::setMinFilter(TextureFilter filter){
    _minFilter = filter;
    return *this;
}

Texture& Texture::setWrap(TextureWrap mode){
    _wrapS = _wrapT = mode;
    return *this;
}

Texture& Texture::setWrapS(TextureWrap mode){
    _wrapS = mode;
    return *this;
}

Texture& Texture::setWrapT(TextureWrap mode){
    _wrapT = mode;
    return *this;
}

Texture& Texture::setTarget(TextureTarget target){
    _target = target;
    return *this;
}

Texture& Texture::applyWrapAndFilter(){
    this->bind();

    //TODO: do a nicer check ?
    if(_target != TextureTarget::TextureBuffer){
        int target = static_cast<int>(_target);
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, static_cast<int>(_minFilter));
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, static_cast<int>(_magFilter));
        glTexParameteri(target, GL_TEXTURE_WRAP_S, static_cast<int>(_wrapS));
        glTexParameteri(target, GL_TEXTURE_WRAP_T, static_cast<int>(_wrapT));
    }
    return *this;
}

GLuint Texture::getHandle(){
    return _handle;
}

Texture::operator GLuint(){
    return _handle;
}

}
