//
//  UIUtilities.cpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 02/11/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#include "UIUtilities.hpp"
#include <sstream> // stringstream
#include <iomanip> // setprecision

//----HELPING FUNCTIONS----
UIUtilities::UIUtilities()
{
    sf::Font fontA;
    
    if (!fontA.loadFromFile("../../../../Users/p4076882/Desktop/ICA2MultiplayerAndSocialGames/simon.otf"))
    {
        std::cout<<"Font could not be loaded";
    };
    
    font1 = fontA;
}

sf::Text UIUtilities::writeFloat(float writing)
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

sf::Text UIUtilities::writeString(std::string writing)
{
    sf::Text text;
    
    // select the font
    text.setFont(font1);
    
    // set the string to display
    text.setString(writing);
    
    // set the character size
    text.setCharacterSize(24); // in pixels, not points!
    
    // set the color
    text.setFillColor(sf::Color::White);
    
    return text;
}
