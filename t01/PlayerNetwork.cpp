//
//  PlayerNetwork.cpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 04/01/2018.
//  Copyright © 2018 Cordry, Julien. All rights reserved.
//

#include "PlayerNetwork.hpp"

//------CREATION-------

//Creates a rectangle shape to represent a player
void PlayerNetwork::createPlayer(std::string textureStr)
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

sf::Sprite PlayerNetwork::getPlayer()
{
    return playerS;
}

//------SETTERS--------
void PlayerNetwork::setStartingPos(float posX, float posY)
{
    playerS.setPosition(posX, posY);
}
