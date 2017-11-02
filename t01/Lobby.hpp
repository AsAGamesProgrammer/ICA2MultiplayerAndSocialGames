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
    Lobby();
    
private:
    UIUtilities uiUtilities;
    
};

#endif /* Lobby_hpp */
