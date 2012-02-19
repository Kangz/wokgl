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
}

Texture& Texture::loadSurface(const SDL_Surface& surface){
    if(!&surface){ //fugly
        throw "Cannot load a taxture from a NULL Surface";
    }
    
    //Check that the surface size is a power of two
    if((surface.w & (surface.w - 1)) != 0 ||
        (surface.h & (surface.h - 1)) != 0){
        throw "Surface size is NPOT";
    }

    int nChannels = surface.format->BytesPerPixel;
    int format;
    
    //If it has alpha
    if(nChannels == 4){
        format = (surface.format->Rmask == 0x000000ff) ? GL_RGBA: GL_BGRA;
    }else if(nChannels == 3){
        format = (surface.format->Rmask == 0x000000ff) ? GL_RGB: GL_BGR;
    }else{
        throw "Surface is not at least RGB";
    }
 
    this->bind();

    //FIXME move this somewhere else ?
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
 
	glTexImage2D(GL_TEXTURE_2D, 0, nChannels, surface.w, surface.h, 0,
	             format, GL_UNSIGNED_BYTE, surface.pixels);
    
    return *this;
}

Texture& Texture::emptyData(int width, int height){
    this->bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

Texture& Texture::quickFileLoad(const std::string& filename){
    SDL_Surface* surf = SDL_LoadBMP(filename.c_str());
    if(surf){
        this->loadSurface(*surf);
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
};

Texture::operator GLuint(){
    return _handle;
}

}
