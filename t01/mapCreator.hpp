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

class mapCreator
{
    //-----PUBLIC-----
    public:
    void loadTiles();
    
    sf::Sprite tiles [250];
    int tileNumber=0;
    
    
    //-----PRIVATE-----
    private:
    sf::Texture tileTexture[2];
    float textureW;
    float textureH;
    
    void loadTexture(std::string name, int idx);
    void loadTextures();
    void loadTile(float startX, float startY, int tileId);

};

#endif /* mapCreator_hpp */
