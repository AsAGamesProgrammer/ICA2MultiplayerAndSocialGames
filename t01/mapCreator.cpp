//
//  mapCreator.cpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 03/10/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#include "mapCreator.hpp"
#include <iostream>


//Creates a rectangle shape to represent a player
void mapCreator::loadTile()
{
    
    //Texture
    sf::Texture texture;
    
    //*********ASK ASK ASK ASK ASK************
    if (!texture.loadFromFile("../../../../../../../../Desktop/ICA2MultiplayerAndSocialGames/road_dirt01.png"))
    {
        std::cout<<"texture not loaded";
    }
    tileTexture=texture;
    
    
    sf::Sprite tileSpr;
    tileSpr.setTexture(tileTexture);
    tileSpr.setOrigin(tileTexture.getSize().x/2, tileTexture.getSize().y/2);
    tileSpr.setPosition(500, 500);
    
    tileSprite=tileSpr;
    
    tiles[0]=&tileSprite;
};
