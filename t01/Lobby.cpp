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
    
    //Button
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(500, 100));
    rectangle.setPosition(1020, 400);
    
    button = rectangle;
    
    //Button 2
    sf::RectangleShape rectangle2;
    rectangle2.setSize(sf::Vector2f(500, 100));
    rectangle2.setPosition(1020, 600);
    
    button2 = rectangle2;
    
    btn1Label = uiUtilities.writeString("Training");
    btn1Label.move(1020, 400);
    btn1Label.setFillColor(sf::Color::Black);
    
    btn2Label = uiUtilities.writeString("Local coop");
    btn2Label.move(1020, 600);
    btn2Label.setFillColor(sf::Color::Black);
    
}
