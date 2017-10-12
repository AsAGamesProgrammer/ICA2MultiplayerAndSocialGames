//
//  Checkpoint.cpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 09/10/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#include "Checkpoint.hpp"

Checkpoint::Checkpoint()
{
    //Assign texture
    checkPTexture = utilityManager.loadTexture("cp.png");
    
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
