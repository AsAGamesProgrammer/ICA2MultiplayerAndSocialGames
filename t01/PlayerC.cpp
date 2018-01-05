//
//  PlayerC.cpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 28/09/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#include "PlayerC.hpp"
#include <math.h>

//------CREATION-------

//Creates a rectangle shape to represent a player
void PlayerC::createPlayer(std::string textureStr)
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
void PlayerC::setStartingPos(float posX, float posY)
{
    playerS.setPosition(posX, posY);
}

//------GETTERS--------
sf::Sprite PlayerC::getPlayer()
{
    return playerS;
};

float PlayerC::getSpeed()
{
    return currentSpeed;
}


//------MOVEMENT--------
bool PlayerC::moveRelated()
{
    if(bullet.isEnabled)
        bullet.moveBullet();
    
    return movePlayer();
}


bool PlayerC::movePlayer()
{
    bool didMove=false;
    
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
        //Move in a direction defied by sprite rotation
        moveInADirection();
        
        //Slow down if possible
        PlayerBase::slowDown();
        
        didMove=true;
    }
    
         //MOVE UP
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        //Move in a direction defied by sprite rotation
        moveInADirection();
        
        //Accelerate if possible
        PlayerBase::accelerate();
        
        didMove=true;
        
    }
    else //NO Movement
    {
        currentSpeed=0;
    }
    
    shoot();
    
    return didMove;
}

void PlayerC::moveInADirection()
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

void PlayerC::shoot()
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !bullet.isEnabled)
    {
        //Instantiate a bullet
        bullet.instantiateBullet(playerS.getPosition().x, playerS.getPosition().y, playerS.getRotation());
        
        //Check for collision
        checkBulletCollision=true;
    }
}

void PlayerC::onTakingHit()
{
    PlayerBase::onHit();
}





