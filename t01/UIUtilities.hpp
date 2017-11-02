//
//  UIUtilities.hpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 02/11/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#ifndef UIUtilities_hpp
#define UIUtilities_hpp

#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>

class UIUtilities
{
public:
    UIUtilities();
    sf::Text writeFloat(float writing);
    sf::Text writeString(std::string writing);
    
    sf::Font font1;
};

#endif /* UIUtilities_hpp */
