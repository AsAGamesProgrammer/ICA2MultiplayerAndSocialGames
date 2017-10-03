//
//  mapCreator.hpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 03/10/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#ifndef mapCreator_hpp
#define mapCreator_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <list>

class mapCreator
{
    //-----PUBLIC-----
    public:
    void loadTile();
    
    sf::Sprite* tiles [20];
    sf::Sprite tileSprite;
    
    
    
    //-----PRIVATE-----
    private:
    sf::Texture tileTexture;
    //sf::Sprite tileSprite;

};

#endif /* mapCreator_hpp */
