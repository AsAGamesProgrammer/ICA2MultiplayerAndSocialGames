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

    //Texture
    sf::Texture texture;
    if (!texture.loadFromFile("../../../../../../../../Desktop/ICA2MultiplayerAndSocialGames/carY.png"))
    {
        std::cout<<"texture not loaded";
    }
    playerTexture=texture;
    
    
    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTexture);
    playerSprite.setOrigin(playerTexture.getSize().x/2, playerTexture.getSize().y/2);
    playerSprite.setPosition(300, 300);
    
    playerS=playerSprite;
};

//------GETTERS--------
sf::Sprite PlayerC::getPlayer()
{
    return playerS;
};


//------MOVEMENT--------
void PlayerC::movePlayer()
{
    //Move RIGHT
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        //Rotation
        playerS.rotate(rotationSpeed);
        
    }
    
    //Move LEFT
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        //Rotation
        playerS.rotate(-rotationSpeed);
    }
     
     //Move DOWN
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        //Convert degrees to radians
        float radians = playerS.getRotation() * 2 * M_PI/360;
        
        //Change positon
        playerS.setPosition(
                                -currentSpeed * cos(radians) + playerS.getPosition().x,
                                -currentSpeed * sin(radians) + playerS.getPosition().y);
    }
     
     //Move UP
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        //Convert degrees to radians
        float radians = playerS.getRotation() * 2 * M_PI/360;
        
        //Change psition
        playerS.setPosition(
                                currentSpeed * cos(radians) + playerS.getPosition().x,
                                currentSpeed * sin(radians) + playerS.getPosition().y);
        
    }
}

