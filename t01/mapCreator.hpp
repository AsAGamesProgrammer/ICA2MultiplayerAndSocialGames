//
//  mapCreator.hpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 03/10/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#ifndef mapCreator_hpp
#define mapCreator_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <list>

#include "Utilities.hpp"

class mapCreator
{
    //-----PUBLIC-----
    public:
    void loadTiles();
    
    sf::Sprite tiles [250];
    int tileNumber=0;
    
    sf::Sprite start;
    
    
    //-----PRIVATE-----
    private:
    sf::Texture tileTexture[14];
    float textureW;
    float textureH;
    
    sf::Texture elementTexture;
    
    void loadTextures();
    void loadTile(float startX, float startY, int tileId);
    void loadElements();
    
    float startPosx = 500;
    float startPosY = 175;
    
    Utilities utilityManager;

};

#endif /* mapCreator_hpp */
