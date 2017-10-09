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

class Bullet
{
public:
  
    void moveBullet(float angle);
    Bullet();
    sf::Sprite getSprite() {return bulletS;}
    bool isEnabled = false;
    
    void instantiateBullet(float x, float y);
    
private:
    float speed = 3;
    sf::Sprite bulletS;
    sf::Texture bulletTexture;
};


#endif /* Bullet_hpp */
