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


//Add position?
Bullet::Bullet()
{

    //Load bullet texture
    bulletTexture=utilityManager.loadTexture("bulR.png");
    
    sf::Sprite bulletSprite;
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setOrigin(bulletTexture.getSize().x/2, bulletTexture.getSize().y/2);
    
    bulletSprite.setPosition(-50, -50);
    
    bulletS=bulletSprite;
    
}

void Bullet::instantiateBullet(float x, float y, float angle)
{

    isEnabled=true;
    bulletS.setPosition(x, y);
    direction=angle;

}

//Move this to the loop?
void Bullet::moveBullet()
{
    //Convert degrees to radians
    float radians = direction * 2 * M_PI/360;
    
    //Change psition
    bulletS.setPosition(
                        speed * cos(radians) + bulletS.getPosition().x,
                        speed * sin(radians) + bulletS.getPosition().y);
    
    
    //TODO
    //Checks for screen boundaries
    if(bulletS.getPosition().x  <0 || //check for left boundary
       bulletS.getPosition().y  <0 || //check for top boundary
       bulletS.getPosition().x > sf::VideoMode::getDesktopMode().width ||
       bulletS.getPosition().y > sf::VideoMode::getDesktopMode().height)
        
    {
        bulletS.setPosition(-50, -50);
        isEnabled=false;
    }

}

