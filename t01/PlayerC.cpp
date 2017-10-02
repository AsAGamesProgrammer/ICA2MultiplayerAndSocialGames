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

sf::RectangleShape PlayerC::getPlayer()
{
    return playerShape;
};

void PlayerC::movePlayer()
{
    //Move RIGHT
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        //Movement
        //playerShape.setPosition(playerShape.getPosition().x + currentSpeed, playerShape.getPosition().y);
        
        //Rotation
        playerShape.rotate(rotationSpeed);
        
         std::cout<<playerShape.getPosition().x<<" "<<playerShape.getPosition().y<<" ";
        
    }
    
    //Move LEFT
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        //Movement
        //playerShape.setPosition(playerShape.getPosition().x - currentSpeed, playerShape.getPosition().y);
        
        //Rotation
        playerShape.rotate(-rotationSpeed);
    }
     
     //Move DOWN
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
       // playerShape.move(currentSpeed * cos(playerShape.getRotation()*2), - currentSpeed * sin(playerShape.getRotation()*2));
        
        //playerShape.setPosition(cos(playerShape.getRotation()) + playerShape.getPosition().x, -sin(playerShape.getRotation() + playerShape.getPosition().y));
        //playerShape.setRotation(90);
    }
     
     //Move UP
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        float radians = playerShape.getRotation() * 2 * M_PI/360;
        playerShape.setPosition(
                                currentSpeed * cos(radians) + playerShape.getPosition().x,
                                currentSpeed * sin(radians) + playerShape.getPosition().y);
        
        //playerShape.setPosition(playerShape.getRotation());
        //playerShape.setRotation(90);
    }
}

