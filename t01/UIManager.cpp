//
//  UIManager.cpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 02/10/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#include "UIManager.hpp"
#include <iomanip> // setprecision
#include <sstream> // stringstream


//------CONSTRUCTOR-------
//Player constructor
UIManager::UIManager()
{
    sf::Font fontA;
    
    //*********************
    if (!fontA.loadFromFile("../../../../../../../../ICA2/simon.otf"))
    {
        std::cout<<"Font could not be loaded";
    };
    
    font1 = fontA;
}

//----INTERFACE----
sf::Text UIManager::displaySpeed(float number)
{
    sf::Text speedLabel = writeString("Speed ");
    speedLabel.move(10, 10);
    
    sf::Text speedText = writeFloat(number);
    speedText.move(80, 10);
    
    gameLabels.push_back(speedLabel);
    gameLabels.push_back(speedText);
    
    return speedText;
}

//----HELPING FUNCTIONS----
sf::Text UIManager::writeFloat(float writing)
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

sf::Text UIManager::writeString(std::string writing)
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

