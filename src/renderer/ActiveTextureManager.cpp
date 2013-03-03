#include <GL/glew.h>

#include "Texture.hpp"
#include "ActiveTextureManager.hpp"

#include <iostream>

namespace renderer{

ActiveTextureManager& ActiveTextureManager::getInstance(ActiveManagerType type){
    int index = static_cast<int>(type);
    if(!_activeManagerInstances[index]){
        int nUnits = ActiveTextureManager::getMaxUnits(type);
        _activeManagerInstances[index] = new ActiveTextureManager(nUnits, type);
    }
    return *_activeManagerInstances[index];
}

ActiveTextureManager::ActiveTextureManager(int nUnits, ActiveManagerType type)
:_type(type), _activeUnit(0), _nUnits(nUnits), _count(0){
    _lastBoundTime = new int[_nUnits];
    _boundTexture = new Texture*[_nUnits];

    glActiveTexture(GL_TEXTURE0);
    for(int i=0; i<_nUnits; i++){
        _lastBoundTime[i] = 0;
        _boundTexture[i] = NULL;
    }
}

int ActiveTextureManager::activate(Texture* tex){
    int unit;
    if(_type == ActiveManagerType::Sampler){
        unit = tex->getLastTextureUnit();
    }else{
        unit = tex->getLastImageUnit();
    }
    _count ++;
    if(unit >= 1 && _boundTexture[unit] == tex){
        _lastBoundTime[unit] = _count;
        return unit;
    }
    int minUnit = 1;
    for(int i=1; i<_nUnits; i++){
        if(_lastBoundTime[i] < _lastBoundTime[minUnit]){
            minUnit = i;
        }
    }

    _boundTexture[minUnit] = tex;
    _lastBoundTime[minUnit] = _count;
    _activeUnit = minUnit;
    if(_type == ActiveManagerType::Sampler){
        glActiveTexture(GL_TEXTURE0 + minUnit);
        tex->bind();
        glActiveTexture(GL_TEXTURE0);
        tex->unbind();
    }else{
        glBindImageTexture(minUnit, *tex, 0, GL_FALSE, 0, GL_READ_WRITE,
                static_cast<int>(tex->getFormat()));
    }

    std::cout << tex << "    " << minUnit << "    " << static_cast<int>(_type) << std::endl;

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
