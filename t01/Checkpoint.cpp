//
//  Checkpoint.cpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 09/10/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#include "Checkpoint.hpp"
#include <iostream>

Checkpoint::Checkpoint()
{
    //Assign texture
    checkPTexture = utilityManager.loadTexture("../../../../Users/p4076882/Desktop/ICA2MultiplayerAndSocialGames/cp.png");
    
    //Measure size
    sizeW = checkPTexture.getSize().x;
    sizeH = checkPTexture.getSize().y;
    

    createCheckPointSprite(0, 500, 255);    //First
    createCheckPointSprite(1, 2000, 255);    //Second
    createCheckPointSprite(2, 500, 1150);    //Third
    createCheckPointSprite(3, 2000, 1150);    //Fourth
    
};

void Checkpoint::createCheckPointSprite(int index, int posX, int posY)
{

    //Sprite
    sf::Sprite chSprite;                         //create a sprite
    chSprite.setTexture(checkPTexture);            //set texture
    chSprite.setPosition(posX, posY);        //set position
    chSprite.setOrigin(checkPTexture.getSize().x/2, checkPTexture.getSize().y/2);
    
    checkPSprites[index] = chSprite;

}

bool Checkpoint::checkpointPassed(int number)
{
    checkedPoints[number] = true;
    
    bool allChecked=true;
    
    lap=0;
    
    //See if everything is checked
    for (int i=0; i<4; i++)
    {
        if(checkedPoints[i]==false)
        {
            allChecked=false;
        }
        else
            lap +=0.25;
    }

    
    //Reset
    if(allChecked)
    {
        lapBase +=1.0;
        
        for (int i=0; i<4; i++)
        {
            checkedPoints[i]=false;
        }
        
        return true;
    }
    
    return false;
}

void Checkpoint::reduceScore()
{
    lapBase -=0.5;
}

float Checkpoint::getLap()
{
    return lap + lapBase;
}
