//
//  PlayerC.cpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 28/09/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#include "PlayerC.hpp"
#include <math.h>
#include <iostream>


//------CONSTRUCTOR-------
//Player constructor
PlayerC::PlayerC()
{
    createPlayer();
}

//Creates a rectangle shape to represent a player
void PlayerC::createPlayer()
{
    sf::RectangleShape player;
    player.setFillColor(sf::Color::Green);  //green colour
    player.setOutlineColor(sf::Color::Black);
    player.setSize(sf::Vector2f(100,50));
    player.setPosition(300, 300);
    
    
    player.setOrigin(player.getSize().x/2, player.getSize().y/2);
    
    playerShape=player;
};

//------GETTERS--------
sf::RectangleShape PlayerC::getPlayer()
{
    return playerShape;
};


//------MOVEMENT--------
void PlayerC::movePlayer()
{
    //Move RIGHT
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        //Rotation
        playerShape.rotate(rotationSpeed);
        
    }
    
    //Move LEFT
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        //Rotation
        playerShape.rotate(-rotationSpeed);
    }
     
     //Move DOWN
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        //Convert degrees to radians
        float radians = playerShape.getRotation() * 2 * M_PI/360;
        
        //Change positon
        playerShape.setPosition(
                                -currentSpeed * cos(radians) + playerShape.getPosition().x,
                                -currentSpeed * sin(radians) + playerShape.getPosition().y);
    }
     
     //Move UP
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        //Convert degrees to radians
        float radians = playerShape.getRotation() * 2 * M_PI/360;
        
        //Change psition
        playerShape.setPosition(
                                currentSpeed * cos(radians) + playerShape.getPosition().x,
                                currentSpeed * sin(radians) + playerShape.getPosition().y);
        
    }
}

