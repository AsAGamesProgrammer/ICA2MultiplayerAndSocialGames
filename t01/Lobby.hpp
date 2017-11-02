//
//  Lobby.hpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 02/11/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#ifndef Lobby_hpp
#define Lobby_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "UIUtilities.hpp"

class Lobby
{
    
public:
    sf::Text lobbieText;
    
    sf::RectangleShape button;
    sf::RectangleShape button2;
    
    sf::Text btn1Label;
    sf::Text btn2Label;
    
    Lobby();
    
private:
    UIUtilities uiUtilities;
    
};

#endif /* Lobby_hpp */
