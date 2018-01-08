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
    
    //LAP INFORMATION
    sf::Text LapA = uiUtilities.writeString("YELLOW laps: ");
    LapA.move(1600, 500);
    //Point B
    sf::Text LapB = uiUtilities.writeString("PINK laps: ");
    LapB.move(1600, 600);
    //Point C
    sf::Text LapC = uiUtilities.writeString("BLUE laps: ");
    LapC.move(1600, 700);
    //Point D
    sf::Text LapD = uiUtilities.writeString("ORANGE laps: ");
    LapD.move(1600, 800);
    
    constantText[0]=speedLabel;
    constantText[1] = chckPointA;
    constantText[2]=chckPointB;
    constantText[3]=chckPointC;
    constantText[4]=chckPointD;
    constantText[5]=LapA;
    constantText[6]=LapB;
    constantText[7]=LapC;
    constantText[8]=LapD;
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
    bannerText.move(1150, 10);
    
    gameLabels.push_back(bannerText);
    
}

//Infrmation about all the laps
void UIManager::displayLaps()
{
    //Point A
    sf::Text YellowLaps = uiUtilities.writeString(laps[0]);
    YellowLaps.move(1900, 500);
    
    //Point B
    sf::Text PinkLaps = uiUtilities.writeString(laps[1]);
    PinkLaps.move(1900, 600);
    
    //Point C
    sf::Text BlueLaps = uiUtilities.writeString(laps[2]);
    BlueLaps.move(1900, 700);
    
    //Point D
    sf::Text OrangeLaps = uiUtilities.writeString(laps[3]);
    OrangeLaps.move(1900, 800);
    
    //Push all the UI elements in a pool
    gameLabels.push_back(YellowLaps);
    gameLabels.push_back(PinkLaps);
    gameLabels.push_back(BlueLaps);
    gameLabels.push_back(OrangeLaps);
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


