//
//  PlayerCoop.cpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 12/10/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#include "PlayerCoop.hpp"

#include <math.h>

//------CREATION-------

//Creates a rectangle shape to represent a player
void PlayerCoop::createPlayer(std::string textureStr)
{
    
    playerTexture=utilityManager.loadTexture(textureStr);
    width = playerTexture.getSize().x;
    height = playerTexture.getSize().y;
    
    
    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTexture);
    playerSprite.setOrigin(playerTexture.getSize().x/2, playerTexture.getSize().y/2);
    playerSprite.setPosition(300, 270);
    
    playerS=playerSprite;
};

//------SETTERS--------
void PlayerCoop::setStartingPos(float posX, float posY)
{
    playerS.setPosition(posX, posY);
}

//------GETTERS--------
sf::Sprite PlayerCoop::getPlayer()
{
    return playerS;
};

float PlayerCoop::getSpeed()
{
    return currentSpeed;
}


//------MOVEMENT--------
void PlayerCoop::moveRelated()
{
    movePlayer();
    
    if(bullet.isEnabled)
        bullet.moveBullet();
}


void PlayerCoop::movePlayer()
{
    
    //Move RIGHT
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        //Rotation
        playerS.rotate(rotationSpeed);
    }
    
    //Move LEFT
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        //Rotation
        playerS.rotate(-rotationSpeed);
    }
    
    //Move DOWN
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        //Move in a direction defied by sprite rotation
        moveInADirection();
        
        //Slow down if possible
        PlayerBase::slowDown();
    }
    
    //MOVE UP
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        //Move in a direction defied by sprite rotation
        moveInADirection();
        
        //Accelerate if possible
        PlayerBase::accelerate();
        
    }
    else //NO Movement
    {
        currentSpeed=0;
    }
    
    shoot();
}

void PlayerCoop::moveInADirection()
{
    //Convert degrees to radians
    float radians = utilityManager.degreesToRads(playerS.getRotation());
    
    //Change psition
    playerS.setPosition(
                        currentSpeed * cos(radians) + playerS.getPosition().x,
                        currentSpeed * sin(radians) + playerS.getPosition().y);
    
    
    //Checks for screen boundaries
    if(!PlayerBase::isOnTrack(playerTexture.getSize().y,
                              playerS.getPosition().x,
                              playerS.getPosition().y))
    {
        playerS.setPosition(
                            playerS.getPosition().x - currentSpeed * cos(radians),
                            playerS.getPosition().y - currentSpeed * sin(radians)); //Move back
    }
    
    
}

void PlayerCoop::shoot()
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::E) && !bullet.isEnabled)
    {
        bullet.instantiateBullet(playerS.getPosition().x, playerS.getPosition().y, playerS.getRotation());
    }
}





