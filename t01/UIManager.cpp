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

//TODO:
/*
 Optimise by making another pool of UI elements which are cnstant
 Have it as an array
 
 */

//------CONSTRUCTOR-------
//Player constructor
UIManager::UIManager()
{
    loadConstText();
}

void UIManager::loadConstText()
{
    //Speed label
    sf::Text speedLabel = uiUtilities.writeString("Speed ");
    speedLabel.move(10, 10);
    
    //CHECK POINTS
    //Point A
    sf::Text chckPointA = uiUtilities.writeString("Check point A: ");
    chckPointA.move(500, 500);
    //Point B
    sf::Text chckPointB = uiUtilities.writeString("Check point B: ");
    chckPointB.move(500, 600);
    //Point C
    sf::Text chckPointC = uiUtilities.writeString("Check point C: ");
    chckPointC.move(500, 700);
    //Point D
    sf::Text chckPointD = uiUtilities.writeString("Check point D: ");
    chckPointD.move(500, 800);
    
    constantText[0]=speedLabel;
    constantText[1] = chckPointA;
    constantText[2]=chckPointB;
    constantText[3]=chckPointC;
    constantText[4]=chckPointD;
}

//----INTERFACE----
void UIManager::displaySpeed(float number)
{
    sf::Text speedText = uiUtilities.writeFloat(number);
    speedText.move(80, 10);
    
    gameLabels.push_back(speedText);

}

//BANNER
void UIManager::displayBanner(std::string title)
{
    sf::Text bannerText = uiUtilities.writeString(title);
    bannerText.move(900, 10);
    
    gameLabels.push_back(bannerText);
    
}

void UIManager::displayLap(int lapNumber)
{
    sf::Text speedText = uiUtilities.writeFloat(lapNumber);
    speedText.move(80, 10);
    
    gameLabels.push_back(speedText);
}

//Information of wether check points were checked
void UIManager::displayCheckPoints()
{
    //Point A
    sf::Text isCheckedA = uiUtilities.writeString(checkedPoints[0]);
    isCheckedA.move(700, 500);
    
    //Point B
    sf::Text isCheckedB = uiUtilities.writeString(checkedPoints[1]);
    isCheckedB.move(700, 600);
    
    //Point C
    sf::Text isCheckedC = uiUtilities.writeString(checkedPoints[2]);
    isCheckedC.move(700, 700);
    
    //Point D
    sf::Text isCheckedD = uiUtilities.writeString(checkedPoints[3]);
    isCheckedD.move(700, 800);
    
    //Push all the UI elements in a pool
    gameLabels.push_back(isCheckedA);
    gameLabels.push_back(isCheckedB);
    gameLabels.push_back(isCheckedC);
    gameLabels.push_back(isCheckedD);

}

//----HELPING FUNCTIONS----
//sf::Text UIManager::writeFloat(float writing)
//{
//    sf::Text text;
//    
//    // select the font
//    text.setFont(font1);
//    
//    //Convert float to string
//    std::stringstream ss;
//    ss << std::fixed << std::setprecision(1) << writing;
//    
//    // set the string to display
//    text.setString(ss.str());
//    
//    
//    // set the character size
//    text.setCharacterSize(24); // in pixels, not points!
//    
//    // set the color
//    text.setFillColor(sf::Color::White);
//    
//    return text;
//}
//
//sf::Text UIManager::writeString(std::string writing)
//{
//    sf::Text text;
//    
//    // select the font
//    text.setFont(font1);
//    
//    // set the string to display
//    text.setString(writing);
//    
//    // set the character size
//    text.setCharacterSize(24); // in pixels, not points!
//    
//    // set the color
//    text.setFillColor(sf::Color::White);
//    
//    return text;
//}

