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
    
    //Checks if the car is on track
    bool isOnTrack(int textureY, int posX, int posY);
    
private:
    //Check boundaries
    bool isWithinScreen(int textureY, int posX, int posY);
    bool isWithinRoad(int textureY, int posX, int posY);
    
    //Temp
    Bullet bullet;
};

#endif /* PlayerBase_hpp */
