//
//  Bullet.hpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 05/10/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#ifndef Bullet_hpp
#define Bullet_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>

#include "Utilities.hpp"

class Bullet
{
public:
  
    void moveBullet();
    Bullet();
    sf::Sprite getSprite() {return bulletS;}
    bool isEnabled = false;
    
    void instantiateBullet(float x, float y, float angle);
    
private:
    float speed = 2;
    sf::Sprite bulletS;
    sf::Texture bulletTexture;
    
    float direction;
    Utilities utilityManager;
};


#endif /* Bullet_hpp */
