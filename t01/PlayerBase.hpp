//
//  PlayerBase.hpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 12/10/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#ifndef PlayerBase_hpp
#define PlayerBase_hpp

#include <SFML/Graphics.hpp>

#include "Bullet.hpp"
#include "Utilities.hpp"

#include <stdio.h>

class PlayerBase
{
public:
    
protected:
    //Check boundaries
    bool isWithinScreen(int textureY, int posX, int posY);
    bool isWithinRoad(int textureY, int posX, int posY);
    
    //Sprite
    //sf::Sprite playerS;     //Shape of a player
};

#endif /* PlayerBase_hpp */
