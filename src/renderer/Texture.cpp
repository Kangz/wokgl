#include "Texture.hpp"

namespace renderer{

Texture::Texture(){
    glGenTextures(1, &_handle);
    _texUnit = -1;
}

Texture::~Texture(){
    glDeleteTextures(1, &_handle);
}

int Texture::activate(){
    _texUnit = ActiveTextureManager::getInstance().activate(this);
    return _texUnit;
}

Texture& Texture::bind(){
	glBindTexture(GL_TEXTURE_2D, _handle);
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
    this->bind();

    //FIXME move this somewhere else ?
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
            convertedSurface->pixels);
    
    return *this;
}

Texture& Texture::emptyData(int width, int height){
    this->bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
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

GLuint Texture::getHandle(){
    return _handle;
}

Texture::operator GLuint(){
    return _handle;
}

}
