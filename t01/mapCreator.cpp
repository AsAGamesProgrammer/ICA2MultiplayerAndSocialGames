//
//  mapCreator.cpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 03/10/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#include "mapCreator.hpp"
#include <iostream>

//----INTERFACE---

void mapCreator::loadTiles()
{

    loadTexture();
    
    
    float tilesWidth = sf::VideoMode::getDesktopMode().width / tileTexture.getSize().x;
    float curX = 0;
    float curY = 0;
    
    for(int i=0; i<3; i++)
    {
        loadTile(curX, curY);
        curX +=100;
    }

}

//----CODE BEHIND---
//Load a texture
void mapCreator::loadTexture()
{
    //Texture
    sf::Texture texture;
    
    //*********ASK ASK ASK ASK ASK************
    if (!texture.loadFromFile("../../../../../../../../Desktop/ICA2MultiplayerAndSocialGames/road_dirt01.png"))
    {
        std::cout<<"texture not loaded";
    }
    tileTexture=texture;
}

//Create a single tile sprite
void mapCreator::loadTile(float startX, float startY)
{
    sf::Sprite tileSpr;
    tileSpr.setTexture(tileTexture);
    tileSpr.setOrigin(tileTexture.getSize().x/2, tileTexture.getSize().y/2);
    
    tileSpr.setPosition(startX, startX);
    
    
    tiles[tileNumber]=tileSpr;
    tileNumber++;
    
    //test
    /*sf::Sprite tileSpr2;
    tileSpr2.setTexture(tileTexture);
    tileSpr2.setOrigin(tileTexture.getSize().x/2, tileTexture.getSize().y/2);
    
    tileSpr2.setPosition(startX, startX);
    
    //tileSprite=tileSpr;
    
    
    tiles[tileNumber]=&tileSprite2;
    tileNumber++; */
    
};
