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
    void displaySpeed(float number);
    
    void displayCheckPoints();
    
    //List of UI elements to render
    std::list<sf::Text> gameLabels;
    
    std::string checkedPoints[4] = {"Do it!", "Do it!", "Do it!", "Do it!"};
    
    sf::Text lobbieText;
    
private:
    sf::Font font1;
    sf::Text writeFloat(float writing);
    sf::Text writeString(std::string writing);
    
    
};

#endif /* UIManager_hpp */
