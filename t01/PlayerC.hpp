//
//  PlayerC.hpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 28/09/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#ifndef PlayerC_hpp
#define PlayerC_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>

#include "Bullet.hpp"

class PlayerC
{
    //-----PUBLIC-----
    public:
    PlayerC();                          //Constructor

    //Getters
    sf::Sprite getPlayer();     //Return the shape of a player
    sf::Sprite getBullet() {return bullet.getSprite();};
    
    
    float getSpeed();

    //Functionality
    void moveRelated();                  //Movement
    
    //-----PRIVATE-----
    private:

    //Variables
    sf::Sprite playerS;     //Shape of a player
    sf::Texture playerTexture;
    
    
    float currentSpeed =0.5f;           //Current speed
    float acceleration = 0.005f;
    float maxSpeed = 0.8f;
    
    float rotationSpeed = 0.5f;         //Rotation responsivness
    
    //Functionality
    void createPlayer();                //Player creation function
    void movePlayer();                  //Movement
    
    //Check boundaries
    bool isWithinScreen();
    bool isWithinRoad();
    
    void shoot();
    
    //Temp
    Bullet bullet;
};


#endif /* PlayerC_hpp */


