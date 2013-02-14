#include <GL/glew.h>

#include "Texture.hpp"
#include "ActiveTextureManager.hpp"

namespace renderer{

ActiveTextureManager& ActiveTextureManager::getInstance(ActiveManagerType type){
    int index = static_cast<int>(type);
    if(!_activeManagerInstances[index]){
        int nUnits = ActiveTextureManager::getMaxUnits(type);
        _activeManagerInstances[index] = new ActiveTextureManager(nUnits);
    }
    return *_activeManagerInstances[index];
}

ActiveTextureManager::ActiveTextureManager(int nUnits)
: _activeUnit(0), _nUnits(nUnits), _count(0){
    _lastBoundTime = new int[_nUnits];
    _boundTexture = new Texture*[_nUnits];

    glActiveTexture(GL_TEXTURE0);
    for(int i=0; i<_nUnits; i++){
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
    for(int i=1; i<_nUnits; i++){
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

int ActiveTextureManager::getMaxUnits(ActiveManagerType type){
    int result = 0;
    if(type == ActiveManagerType::Sampler){
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &result);
    }else if(type == ActiveManagerType::Image){
        glGetIntegerv(GL_MAX_IMAGE_UNITS, &result);
    }

    return result;
}

}
