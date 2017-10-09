//
//  Utilities.cpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 09/10/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#include "Utilities.hpp"
#include <iostream>

//LOAD TETXTURE
sf::Texture Utilities::loadTexture(std::string name)
{
    //Texture
    sf::Texture texture;
    
    if (!texture.loadFromFile(name))
    {
        std::cout<<"texture not loaded";
    }
    
    return texture;

}

