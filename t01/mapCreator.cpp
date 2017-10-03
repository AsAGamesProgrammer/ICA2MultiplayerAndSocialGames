//
//  mapCreator.cpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 03/10/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#include "mapCreator.hpp"
#include <iostream>
#include <fstream>

//----INTERFACE---

void mapCreator::loadTiles()
{

    //Load the texture
    loadTextures();
    textureW =tileTexture[0].getSize().x;
    textureH = tileTexture[0].getSize().y;
    
    float tilesWidth = sf::VideoMode::getDesktopMode().width / textureW;
    float tilesHeight = sf::VideoMode::getDesktopMode().height / textureH;
    float curX = 0;
    float curY = 0;
    
    
    //Load map
    std::string line;
    //******ASK******
    std::ifstream myfile ("../../../../../../../../Desktop/ICA2MultiplayerAndSocialGames/map.txt");
    
    if (myfile.is_open())
    {

        
        for (int i=0; i<tilesHeight; i++)
        {
            std::string line;
            std::getline (myfile,line);
  
            int flag =0;
            for(int j=0; j<tilesWidth; j++)
            {
                char lineId = line[flag];
                int iLineId = int(lineId)-48; //char to int conversion
                std::cout<<lineId;
                
                loadTile(curX, curY, iLineId);
                curX +=textureW;
                flag+=2;
            }
            curY += textureH;
            curX=0;
        }
        myfile.close();
    }
    
   

}

//----CODE BEHIND---
//Load a texture
void mapCreator::loadTexture(std::string name, int idx)
{
    //Texture
    sf::Texture texture;
    

    if (!texture.loadFromFile(name))
    {
        std::cout<<"texture not loaded";
    }
    
    tileTexture[idx]=texture;
}

void mapCreator::loadTextures()
{
    int idx=0;
    //*********ASK ASK ASK ASK ASK************
    loadTexture("../../../../../../../../Desktop/ICA2MultiplayerAndSocialGames/road_dirt05.png", idx);
    
    idx++;
    loadTexture("../../../../../../../../Desktop/ICA2MultiplayerAndSocialGames/road_dirt03.png", idx);
    
    
    
}

//Create a single tile sprite
void mapCreator::loadTile(float startX, float startY, int tileId)
{
    sf::Sprite tileSpr;                         //create a sprite
    tileSpr.setTexture(tileTexture[tileId]);            //set texture
    tileSpr.setPosition(startX, startY);        //set position
    
    
    tiles[tileNumber]=tileSpr;
    tileNumber++;
 
};
