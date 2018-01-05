//
//  PlayerC.hpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 28/09/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#ifndef PlayerC_hpp
#define PlayerC_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>

#include <iostream>

#include "Bullet.hpp"
#include "Utilities.hpp"
#include "PlayerBase.hpp"

class PlayerC : PlayerBase
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
    bool moveRelated();                  //Movement
    
    void createPlayer(std::string textureStr);                //Player creation function
    
    void onTakingHit();
    
    //-----PRIVATE-----
    private:

    //Variables
    sf::Sprite playerS;     //Shape of a player
    sf::Texture playerTexture;
        
    //Functionality

    bool movePlayer();                  //Movement
    
    void shoot();
    
    void moveInADirection();
     
};


#endif /* PlayerC_hpp */


