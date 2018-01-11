//
//  PlayerNetwork.hpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 04/01/2018.
//  Copyright © 2018 Cordry, Julien. All rights reserved.
//

#ifndef PlayerNetwork_hpp
#define PlayerNetwork_hpp

#include <stdio.h>
#include "Bullet.hpp"
#include "Utilities.hpp"
#include "PlayerBase.hpp"

class PlayerNetwork:PlayerBase
{
public:
    sf::Sprite getPlayer();     //Return the shape of a player
    sf::Sprite getBullet() {return bullet.getSprite();};
    
    void createPlayer(std::string textureStr);                //Player creation function
    void setStartingPos(float posX, float posY);
    
    void instantiateBlt(float angle);
    
private:
    float width;
    float height;
    
    //Variables
    sf::Sprite playerS;     //Shape of a player
    sf::Texture playerTexture;
    
};

#endif /* PlayerNetwork_hpp */
