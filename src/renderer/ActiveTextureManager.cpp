#include <GL/glew.h>

#include "ActiveTextureManager.hpp"

namespace renderer{

ActiveTextureManager& ActiveTextureManager::getInstance(){
    if(!_instance){
        _instance = new ActiveTextureManager();
    }
    return *_instance;
}

ActiveTextureManager::ActiveTextureManager(): _activeUnit(0), _count(0){
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_nTexUnits);
    _lastBoundTime = new int[_nTexUnits];
    _boundTexture = new Texture*[_nTexUnits];

    glActiveTexture(GL_TEXTURE0);
    for(int i=0; i<_nTexUnits; i++){
        _lastBoundTime[i] = 0;
        _boundTexture[i] = NULL;
    }
}

int ActiveTextureManager::activate(Texture* tex){
    int unit = tex->getLastTextureUnit();
    _count ++;
    if(_boundTexture[unit] == tex){//check against out of bounds ?
        _lastBoundTime[unit] = _count;
        return unit;
    }
    int minUnit = 0;
    for(int i=1; i<_nTexUnits; i++){
        if(_lastBoundTime[i] < _lastBoundTime[minUnit]){
            minUnit = i;
        }
    }

    _boundTexture[minUnit] = tex;
    _lastBoundTime[minUnit] = _count;
    _activeUnit = minUnit;
    glActiveTexture(GL_TEXTURE0 + minUnit);
    tex->bind();

    return minUnit;
}

ActiveTextureManager* ActiveTextureManager::_instance = NULL;

}
