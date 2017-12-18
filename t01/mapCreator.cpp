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
    std::ifstream myfile ("../../../../Users/p4076882/Desktop/ICA2MultiplayerAndSocialGames/map.txt");
    
    if (myfile.is_open())
    {
        for (int i=0; i<tilesHeight; i++)
        {
            std::string line;
            std::getline (myfile,line);
  
            int flag =0;
            for(int j=0; j<tilesWidth; j++)
            {
                int iLineId=0;
                char lineId = line[flag];       //get a character that represents a tile
                if(lineId>='0' && lineId<='9')  //check if it is an integer
                {
                    iLineId = int(lineId)-48;   //char to int conversion
                }
                else
                {
                    switch(lineId)
                    {
                            case 'A':
                            iLineId = 10;
                            break;
                            
                            case 'B':
                            iLineId=11;
                            break;
                            
                            case 'C':
                            iLineId=12;
                            break;
                            
                            case 'D':
                            iLineId=13;
                            break;
                    };

                }
                
                loadTile(curX, curY, iLineId);
                curX +=textureW;
                flag+=2;
            }
            curY += textureH;
            curX=0;
        }
        myfile.close();
    }
    
    //loadElements();

}

//----CODE BEHIND---

void mapCreator::loadTextures()
{
    for(int i=1; i<10; i++)
    {
        std::string path ="../../../../Users/p4076882/Desktop/ICA2MultiplayerAndSocialGames/land_grass0" + std::to_string(i) +".png";
        tileTexture[i-1]=utilityManager.loadTexture(path);
    
    }
    
    for(int i=10; i<15; i++)
    {
        std::string path ="../../../../Users/p4076882/Desktop/ICA2MultiplayerAndSocialGames/land_grass" + std::to_string(i) +".png";
        tileTexture[i-1]=utilityManager.loadTexture(path);
    }
    
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

/*void mapCreator::loadElements()
{
    elementTexture = utilityManager.loadTexture("bar.png");
    
    //Sprite
    sf::Sprite startSprite;                         //create a sprite
    startSprite.setTexture(elementTexture);            //set texture
    startSprite.setPosition(startPosx, startPosY);        //set position
    
    start = startSprite;
}; */
