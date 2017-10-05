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

    if (!texture.loadFromFile("carY.png"))
    {
        std::cout<<"texture not loaded";
    }
    playerTexture=texture;
    
    
    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTexture);
    playerSprite.setOrigin(playerTexture.getSize().x/2, playerTexture.getSize().y/2);
    playerSprite.setPosition(300, 270);
    
    playerS=playerSprite;
};

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
                                currentSpeed * cos(radians) + playerS.getPosition().x,
                                currentSpeed * sin(radians) + playerS.getPosition().y);
        
        //TEST outside screen
        if(!isWithinScreen() || !isWithinRoad())
        {
            playerS.setPosition(
                                playerS.getPosition().x - currentSpeed * cos(radians),
                                playerS.getPosition().y - currentSpeed * sin(radians)); //Move back
        }
        
        //Basic acceleration
        if(currentSpeed>0)
            currentSpeed=0;
        if(currentSpeed>=-maxSpeed)
            currentSpeed-=acceleration;
    }
    
         //MOVE UP
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        //Convert degrees to radians
        float radians = playerS.getRotation() * 2 * M_PI/360;
        
        //Change psition
        playerS.setPosition(
                                currentSpeed * cos(radians) + playerS.getPosition().x,
                                currentSpeed * sin(radians) + playerS.getPosition().y);
        
        
        //Checks for screen boundaries
        if(!isWithinScreen() || !isWithinRoad())
        {
            playerS.setPosition(
                                playerS.getPosition().x - currentSpeed * cos(radians),
                                playerS.getPosition().y - currentSpeed * sin(radians)); //Move back
        }
        
        //Basic acceleration
        if(currentSpeed<0)
            currentSpeed=0;
        if(currentSpeed<=maxSpeed)
            currentSpeed+=acceleration;
        
    }
    else //NO Movement
    {
        currentSpeed=0;
    }
}


//Checks if the player sprite postion is valid
bool PlayerC::isWithinScreen()
{
    int textureOffset=10;     //10 is texture offset
    
    if(playerS.getPosition().x - playerTexture.getSize().y  + textureOffset <0 || //check for left boundary
       playerS.getPosition().y - playerTexture.getSize().y + textureOffset <0 || //check for top boundary
       playerS.getPosition().x + playerTexture.getSize().y - textureOffset > sf::VideoMode::getDesktopMode().width ||
       playerS.getPosition().y + playerTexture.getSize().y + textureOffset + 100> sf::VideoMode::getDesktopMode().height)
    
    {
        return false;
    }

    return true;
}

//Checks if the player sprite postion is valid
//Checks the top area
bool PlayerC::isWithinRoad()
{

    int textureOffset=10;     //10 is texture offset
    
    bool onTheLeft = false;
    bool onTheRight = false;
    bool onTheTop=false;
    
    //LEFT
    if(playerS.getPosition().x + playerTexture.getSize().y - textureOffset < 256)
        onTheLeft=true;
    
    //RIGHT
    if(playerS.getPosition().x + playerTexture.getSize().y - textureOffset > 2432)
        onTheRight=true;
    
    //TOP && BOT
    if(playerS.getPosition().y - playerTexture.getSize().y + textureOffset >260
       && playerS.getPosition().y - playerTexture.getSize().y + textureOffset < 1000)
        onTheTop=true;
    
    
    //DECISION
    if(onTheTop && !onTheLeft && !onTheRight)
    {
        return false;
    }
    
    return true;

}



