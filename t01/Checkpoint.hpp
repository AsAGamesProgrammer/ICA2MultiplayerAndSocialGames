//
//  Checkpoint.hpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 09/10/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#ifndef Checkpoint_hpp
#define Checkpoint_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "Utilities.hpp"

class Checkpoint
{

public:
    Checkpoint();
    //sf::Sprite getSprite() {return checkPSprite;}
    
    int const cpNumber = 4;
    sf::Sprite checkPSprites[4];
    
    float sizeW;
    float sizeH;
    
private:
    Utilities utilityManager;
    
    sf::Texture checkPTexture;
    
    void createCheckPointSprite(int index, int posX, int posY);
};

#endif /* Checkpoint_hpp */
