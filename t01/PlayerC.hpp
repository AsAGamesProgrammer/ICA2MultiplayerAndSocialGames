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
    sf::RectangleShape getPlayer();     //Return the shape of a player

    //Functionality
    void movePlayer();                  //Movement
    
    //-----PRIVATE-----
    private:

    //Variables
    sf::RectangleShape playerShape;     //Shape of a player
    float currentSpeed =0.5f;           //Current speed
    
    //Functionality
    void createPlayer();                //Player creation function
};

#endif /* PlayerC_hpp */


