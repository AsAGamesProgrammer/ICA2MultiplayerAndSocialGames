//
//  Lobby.cpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 02/11/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#include "Lobby.hpp"

Lobby::Lobby()
{
    lobbieText = uiUtilities.writeString("Press enter to begin");
    lobbieText.setCharacterSize(60);
    lobbieText.move(1000, 300);
    
    //Creating buttons
    createBtn(1020, 400, 1150, 410, "Training", 0);
    createBtn(1020, 600, 1130, 610, "Local coop", 1);
    createBtn(1020, 800, 1130, 810, "Multiplayer", 2);
    
    //Set active
    setActiveButton(currentActiveBtn);
        
}


//Creates a button
void Lobby::createBtn(int posX, int posY, int lblPosX, int lblPosY, std::string title, int idx)
{
    ButtonC newBtn;
    
    //Shape
    newBtn.shape.setSize(sf::Vector2f(500, 100));
    newBtn.shape.setPosition(posX, posY);
    
    //Label
    newBtn.label = uiUtilities.writeString(title);
    newBtn.label.move(lblPosX, lblPosY);
    newBtn.label.setFillColor(sf::Color::Black);
    newBtn.label.setCharacterSize(60);
    
    //Array
    btnArray[idx] = newBtn;
}

void Lobby::selectNextBtn()
{
    btnArray[currentActiveBtn].shape.setFillColor(sf::Color::White);
    
    if(currentActiveBtn+1<numberOfBtns)
        currentActiveBtn++;
    else
        currentActiveBtn=0;
    
    btnArray[currentActiveBtn].shape.setFillColor(sf::Color::Blue);
}

void Lobby::selectPrevBtn()
{
    if(currentActiveBtn-1>=0)
        currentActiveBtn--;
    else
        currentActiveBtn=numberOfBtns-1;
    
    setActiveButton(currentActiveBtn);
}

void Lobby::setActiveButton(int n)
{
    deselectCurBtn();
    currentActiveBtn = n;
    btnArray[n].shape.setFillColor(sf::Color::Blue);
    
}

void Lobby::deselectCurBtn()
{
    btnArray[currentActiveBtn].shape.setFillColor(sf::Color::White);
}


