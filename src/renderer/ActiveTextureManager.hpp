#ifndef RENDERER_ACTIVETEXTUREMANAGER_HPP
#define RENDERER_ACTIVETEXTUREMANAGER_HPP

#include "enums.hpp"

namespace renderer{

class Texture;

class ActiveTextureManager{
    public:
        static ActiveTextureManager& getInstance(ActiveManagerType);
        int activate(Texture* tex);
    
    private:
        static int getMaxUnits(ActiveManagerType type);
        ActiveTextureManager(int nUnits, ActiveManagerType type);
        //~ActiveTextureManager(); //who wants to destroy it ?

        ActiveManagerType _type;
        int _activeUnit;
        int _nUnits;
        int* _lastBoundTime;
        Texture** _boundTexture;
        int _count; //this won't overflow 2M texture changes is enough ... 
};

static ActiveTextureManager* _activeManagerInstances[static_cast<int>(ActiveManagerType::Max)];

}

#endif //RENDERER_ACTIVETEXTUREMANAGER_HPP
