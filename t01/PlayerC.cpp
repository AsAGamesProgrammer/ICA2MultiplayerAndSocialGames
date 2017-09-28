//
//  PlayerC.cpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 28/09/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#include "PlayerC.hpp"

PlayerC::PlayerC()
{
    createPlayer();
}

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
    }
    
    //Move left
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        playerShape.setPosition(playerShape.getPosition().x - currentSpeed, playerShape.getPosition().y);
    }
     
     //Move down
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        playerShape.setPosition(playerShape.getPosition().x, playerShape.getPosition().y + currentSpeed);
        playerShape.rotate(0.1f);
    }
     
     //Move up
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        playerShape.setPosition(playerShape.getPosition().x, playerShape.getPosition().y - currentSpeed);
    }
}

