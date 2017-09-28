//
//  PlayerC.cpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 28/09/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#include "PlayerC.hpp"

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
    player.setPosition(10, 10);
    
    playerShape=player;
};

sf::RectangleShape PlayerC::getPlayer()
{
    return playerShape;
};

void PlayerC::movePlayer()
{
    //Move right
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        playerShape.setPosition(playerShape.getPosition().x + currentSpeed, playerShape.getPosition().y);
        playerShape.setRotation(180);
    }
    
    //Move left
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        playerShape.setPosition(playerShape.getPosition().x - currentSpeed, playerShape.getPosition().y);
        playerShape.setRotation(180);
    }
     
     //Move down
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        playerShape.setPosition(playerShape.getPosition().x, playerShape.getPosition().y + currentSpeed);
        playerShape.setRotation(90);
    }
     
     //Move up
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        playerShape.setPosition(playerShape.getPosition().x, playerShape.getPosition().y - currentSpeed);
        playerShape.setRotation(90);
    }
}

