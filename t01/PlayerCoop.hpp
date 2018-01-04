//
//  PlayerCoop.hpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 12/10/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#ifndef PlayerCoop_hpp
#define PlayerCoop_hpp

#include <stdio.h>

#include "Bullet.hpp"
#include "Utilities.hpp"
#include "PlayerBase.hpp"

class PlayerCoop:PlayerBase
{
    //-----PUBLIC-----
public:
    //Getters
    sf::Sprite getPlayer();     //Return the shape of a player
    sf::Sprite getBullet() {return bullet.getSprite();};
    
    bool getCheckBulletColision(){return checkBulletCollision;}
    
    //Setter
    void setStartingPos(float posX, float posY);
    void setCheckBulletColision(bool toCheck){checkBulletCollision = toCheck;}
    
    float width;
    float height;
    float getSpeed();
    
    //Functionality
    void moveRelated();                  //Movement
    
    void createPlayer(std::string textureStr);                //Player creation function
    
    //-----PRIVATE-----
private:
    
    //Variables
    sf::Sprite playerS;     //Shape of a player
    sf::Texture playerTexture;
    
    //Functionality
    
    void movePlayer();                  //Movement
    
    void shoot();
    
    void moveInADirection();
    
    bool isActive=false;
    
};

#endif /* PlayerCoop_hpp */
