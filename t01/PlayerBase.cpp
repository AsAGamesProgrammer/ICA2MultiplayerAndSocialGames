//
//  PlayerBase.cpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 12/10/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#include "PlayerBase.hpp"


//---PROTECTED---
bool PlayerBase::isOnTrack(int texture, int posX, int posY)
{
    if(!isWithinScreen(texture, posX, posY) || !isWithinRoad(texture, posX, posY))
        return false;
    
    return true;
    
}

//----PRIVATE----

//Checks if the player sprite postion is valid
bool PlayerBase::isWithinScreen(int textureY, int posX, int posY)
{
    int textureOffset=10;     //10 is texture offset
    
    if(posX - textureY  + textureOffset <0 || //check for left boundary
       posY - textureY + textureOffset <0 || //check for top boundary
       posX + textureY - textureOffset > sf::VideoMode::getDesktopMode().width ||
       posY + textureY + textureOffset + 100> sf::VideoMode::getDesktopMode().height)
        
    {
        return false;
    }
    
    return true;
}

//Checks if the player sprite postion is valid
//Checks the top area
bool PlayerBase::isWithinRoad(int textureY, int posX, int posY)
{
    
    int textureOffset=10;     //10 is texture offset
    
    bool onTheLeft = false;
    bool onTheRight = false;
    bool onTheTop=false;
    
    //LEFT
    if(posX + textureY - textureOffset < 256)
        onTheLeft=true;
    
    //RIGHT
    if(posX + textureY - textureOffset > 2390)
        onTheRight=true;
    
    //TOP && BOT
    if(posY - textureY + textureOffset >290
       && posY - textureY + textureOffset < 1050)
        onTheTop=true;
    
    
    //DECISION
    if(onTheTop && !onTheLeft && !onTheRight)
    {
        return false;
    }
    
    return true;
    
}
