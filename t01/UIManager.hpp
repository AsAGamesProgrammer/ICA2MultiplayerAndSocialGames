//
//  UIManager.hpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 02/10/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#ifndef UIManager_hpp
#define UIManager_hpp

#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>

class UIManager
{

public:
    UIManager();
    sf::Text displaySpeed(float number);
    std::list<sf::Text> gameLabels;
    
private:
    sf::Font font1;
    sf::Text writeFloat(float writing);
    sf::Text writeString(std::string writing);
    
};

#endif /* UIManager_hpp */
