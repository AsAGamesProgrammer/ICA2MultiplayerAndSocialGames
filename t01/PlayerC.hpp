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

#include <iostream>

#include "Bullet.hpp"
#include "Utilities.hpp"
#include "PlayerBase.hpp"

class PlayerC : PlayerBase
{
    //-----PUBLIC-----
    public:
    //Getters
    sf::Sprite getPlayer();     //Return the shape of a player
    sf::Sprite getBullet() {return bullet.getSprite();};
    
    //Setter
    void setStartingPos(float posX, float posY);
    
    float width;
    float height;
    float getSpeed();

    //Functionality
    void moveRelated();                  //Movement
    
    void createPlayer(std::string textureStr);                //Player creation function
    
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

    void movePlayer();                  //Movement
    
    void shoot();
    
    void moveInADirection();
    
    //Temp
    Bullet bullet;
    
    Utilities utilityManager;
    
};


#endif /* PlayerC_hpp */


