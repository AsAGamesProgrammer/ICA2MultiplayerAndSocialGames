//
//  UIManager.cpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 02/10/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#include "UIManager.hpp"
#include <iostream>
#include <iomanip> // setprecision
#include <sstream> // stringstream

//------CONSTRUCTOR-------
//Player constructor
UIManager::UIManager()
{
    sf::Font fontA;
    
    //*********************
    if (!fontA.loadFromFile("../../../../../../../../Desktop/ICA2MultiplayerAndSocialGames/simon.otf"))
    {
        std::cout<<"Font could not be loaded";
    };
    
    font1 = fontA;
}

sf::Text UIManager::writeInt(float writing)
{
    sf::Text text;
    
    // select the font
    text.setFont(font1);
    
    //Convert float to string
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << writing;
    
    // set the string to display
    text.setString(ss.str());
    
    
    // set the character size
    text.setCharacterSize(24); // in pixels, not points!
    
    // set the color
    text.setFillColor(sf::Color::White);
    
    return text;
}

