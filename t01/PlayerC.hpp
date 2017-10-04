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

class PlayerC
{
    //-----PUBLIC-----
    public:
    PlayerC();                          //Constructor

    //Getters
    sf::Sprite getPlayer();     //Return the shape of a player
    float getSpeed();

    //Functionality
    void movePlayer();                  //Movement
    
    //-----PRIVATE-----
    private:

    //Variables
    sf::Sprite playerS;     //Shape of a player
    sf::Texture playerTexture;
    
    
    float currentSpeed =0.5f;           //Current speed
    float acceleration = 0.005f;
    float maxSpeed = 1.4f;
    
    float rotationSpeed = 0.5f;         //Rotation responsivness
    
    //Functionality
    void createPlayer();                //Player creation function
};

#endif /* PlayerC_hpp */


