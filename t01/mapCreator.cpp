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

    //Load the texture
    loadTexture();
    
    
    float tilesWidth = sf::VideoMode::getDesktopMode().width / tileTexture.getSize().x;
    float curX = 0;
    float curY = 0;
    
    for(int i=0; i<tilesWidth; i++)
    {
        loadTile(curX, curY);
        curX +=tileTexture.getSize().x;
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
    sf::Sprite tileSpr;                         //create a sprite
    tileSpr.setTexture(tileTexture);            //set texture
    tileSpr.setPosition(startX, startY);        //set position
    
    
    tiles[tileNumber]=tileSpr;
    tileNumber++;
 
};
