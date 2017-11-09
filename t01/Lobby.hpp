//
//  Lobby.hpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 02/11/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#ifndef Lobby_hpp
#define Lobby_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "UIUtilities.hpp"
#include "ButtonC.hpp"

class Lobby
{
    
public:
    sf::Text lobbieText;
    
    //ButtonC btn1;
    //ButtonC btn2;
    
    ButtonC btnArray[2];
    int numberOfBtns = 2;
    
    Lobby();
    
    int currentActiveBtn;
    
private:
    UIUtilities uiUtilities;
    void setActiveButton(int n);
    
    //Creates a btn and adds it to the array
    void createBtn(int posX, int posY, int lblPosX, int lblPosY, std::string title, int idx);
    
};

#endif /* Lobby_hpp */
