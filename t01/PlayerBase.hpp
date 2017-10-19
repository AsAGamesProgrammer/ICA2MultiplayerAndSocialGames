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
    
    //FUNCTIONALITY
    //Checks if the car is on track
    bool isOnTrack(int textureY, int posX, int posY);
    void accelerate();
    void slowDown();
    
    //VARIABLES
    float currentSpeed =0.5f;           //Current speed
    float acceleration = 0.005f;
    float maxSpeed = 1.0f;
    float rotationSpeed = 0.5f;         //Rotation responsivness
    
    //Bullet
    Bullet bullet;
    bool checkBulletCollision=false;
    
    //Utilitis manager
    Utilities utilityManager;

    //Hit
    void onHit();
    
private:
    //Check boundaries
    bool isWithinScreen(int textureY, int posX, int posY);
    bool isWithinRoad(int textureY, int posX, int posY);
    
};

#endif /* PlayerBase_hpp */
