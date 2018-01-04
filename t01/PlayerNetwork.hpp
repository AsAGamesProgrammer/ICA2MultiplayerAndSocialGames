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
    void createPlayer(std::string textureStr);                //Player creation function
    
private:
    float width;
    float height;
    
    //Variables
    sf::Sprite playerS;     //Shape of a player
    sf::Texture playerTexture;
    
};

#endif /* PlayerNetwork_hpp */