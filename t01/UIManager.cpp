//
//  UIManager.cpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 02/10/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#include "UIManager.hpp"
<<<<<<< HEAD
#include <iostream>

//------CONSTRUCTOR-------
//Player constructor
UIManager::UIManager()
{
    sf::Font fontA;
    
    //*********************
    if (!fontA.loadFromFile("../../../../../../../../Desktop/ICA2MultiplayerAndSocialGames/joinks_.TTF"))
    {
        std::cout<<"Font could not be loaded";
    };
    
    font1 = fontA;
}

sf::Text UIManager::writeInt(int writing)
{
    sf::Text text;
    
    // select the font
    text.setFont(font1);
    
    // set the string to display
    text.setString("Hello world");
    
    // set the character size
    text.setCharacterSize(24); // in pixels, not points!
    
    // set the color
    text.setFillColor(sf::Color::White);
    
    return text;
}
=======
>>>>>>> master
