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
    if (!fontA.loadFromFile("simon.otf"))
    {
        std::cout<<"Font could not be loaded";
    };
    
    font1 = fontA;
}

//----INTERFACE----
void UIManager::displaySpeed(float number)
{
    sf::Text speedLabel = writeString("Speed ");
    speedLabel.move(10, 10);
    
    sf::Text speedText = writeFloat(number);
    speedText.move(80, 10);
    
    gameLabels.push_back(speedLabel);
    gameLabels.push_back(speedText);

}


void UIManager::displayCheckPoints()
{

    //Point A
    sf::Text chckPointA = writeString("Check point A: ");
    chckPointA.move(500, 500);
    
    sf::Text isCheckedA = writeString(checkedPoints[0]);
    isCheckedA.move(700, 500);
    
    //Point B
    sf::Text chckPointB = writeString("Check point B: ");
    chckPointB.move(500, 600);
    
    sf::Text isCheckedB = writeString(checkedPoints[1]);
    isCheckedB.move(700, 600);
    
    //Point C
    sf::Text chckPointC = writeString("Check point C: ");
    chckPointC.move(500, 700);
    
    sf::Text isCheckedC = writeString(checkedPoints[2]);
    isCheckedC.move(700, 700);
    
    //Point D
    sf::Text chckPointD = writeString("Check point D: ");
    chckPointD.move(500, 800);
    
    sf::Text isCheckedD = writeString(checkedPoints[3]);
    isCheckedD.move(700, 800);
    
    //Push all the UI elements in a pool
    gameLabels.push_back(chckPointA);
    gameLabels.push_back(isCheckedA);
    
    gameLabels.push_back(chckPointB);
    gameLabels.push_back(isCheckedB);
    gameLabels.push_back(chckPointC);
    gameLabels.push_back(isCheckedC);
    gameLabels.push_back(chckPointD);
    gameLabels.push_back(isCheckedD);

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

