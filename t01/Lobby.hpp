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
    //Constructor
    Lobby();
    
    //Lobbie text
    sf::Text lobbieText;
    
    //Array and number of buttons
    ButtonC btnArray[3];
    int numberOfBtns = 3;
    
    //void selectNextBtn();
    //void selectPrevBtn();
    
    //Function to set active btn and highlight it
    void setActiveButton(int n);
    
private:
    UIUtilities uiUtilities;

    int currentActiveBtn = 0;
    void deselectCurBtn();
    
    //Creates a btn and adds it to the array
    void createBtn(int posX, int posY, int lblPosX, int lblPosY, std::string title, int idx);
    
};

#endif /* Lobby_hpp */
