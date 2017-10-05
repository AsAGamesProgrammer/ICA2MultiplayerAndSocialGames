//
//  Bullet.cpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 05/10/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#include "Bullet.hpp"
#include <math.h>
#include <iostream>


Bullet::Bullet()
{
    //Texture
    sf::Texture texture;
    
    if (!texture.loadFromFile("bulY.png"))
    {
        std::cout<<"texture not loaded";
    }
    
    bulletTexture=texture;
    
    
    sf::Sprite bulletSprite;
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setOrigin(bulletTexture.getSize().x/2, bulletTexture.getSize().y/2);
    bulletSprite.setPosition(300, 270);
    
    bulletS=bulletSprite;
}

void Bullet::moveBullet(float angle)
{
    //Convert degrees to radians
    float radians = angle * 2 * M_PI/360;
    
    //Change psition
    bulletS.setPosition(
                        speed * cos(radians) + bulletS.getPosition().x,
                        speed * sin(radians) + bulletS.getPosition().y);
    
    
    //TODO
    //Checks for screen boundaries
   // if(!isWithinScreen() || !isWithinRoad())
   // {
    //    playerS.setPosition(
     //                       playerS.getPosition().x - currentSpeed * cos(radians),
    //                        playerS.getPosition().y - currentSpeed * sin(radians)); //Move back
   // }

}
