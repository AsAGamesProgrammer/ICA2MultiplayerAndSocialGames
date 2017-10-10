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
    checkPTexture = utilityManager.loadTexture("bar.png");
    
    //Sprite
    sf::Sprite chSprite;                         //create a sprite
    chSprite.setTexture(checkPTexture);            //set texture
    chSprite.setPosition(startPosx, startPosY);        //set position
    chSprite.setOrigin(checkPTexture.getSize().x/2, checkPTexture.getSize().y/2);
    
    checkPSprite = chSprite;
    
    sizeW = checkPTexture.getSize().x;
    sizeH = checkPTexture.getSize().y;
    
};
