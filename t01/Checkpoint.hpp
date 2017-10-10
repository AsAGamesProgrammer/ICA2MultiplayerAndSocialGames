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
    sf::Sprite getSprite() {return checkPSprite;}
    
    float sizeW;
    float sizeH;
    
private:
    Utilities utilityManager;
    
    sf::Texture checkPTexture;
    sf::Sprite checkPSprite;

    //temp
    float startPosx = 500;
    float startPosY = 255;
};

#endif /* Checkpoint_hpp */
