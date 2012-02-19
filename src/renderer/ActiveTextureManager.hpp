#ifndef RENDERER_ACTIVETEXTUREMANAGER_HPP
#define RENDERER_ACTIVETEXTUREMANAGER_HPP

#include "Texture.hpp"

namespace renderer{

class Texture;

class ActiveTextureManager{
    public:
        static ActiveTextureManager& getInstance();
        int activate(Texture* tex);
    
    private:
        ActiveTextureManager();
        //~ActiveTextureManager(); //who wants to destroy it ?
        static ActiveTextureManager* _instance;

        int _activeUnit;
        int _nTexUnits;
        int* _lastBoundTime;
        Texture** _boundTexture;
        int _count; //this won't overflow 2M texture changes is enough ... 
};

}

#endif //RENDERER_ACTIVETEXTUREMANAGER_HPP
