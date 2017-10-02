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
#include <SFML/Graphics.hpp>

class UIManager
{

public:
    UIManager();
    sf::Text writeInt(float writing);
    
private:
    sf::Font font1;
    
};

#endif /* UIManager_hpp */
