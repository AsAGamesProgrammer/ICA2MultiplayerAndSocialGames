//
//  main.cpp
//  t01
//
//  Created by Cordry, Julien on 30/09/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#include <iostream>
#include "PlayerC.hpp"
#include "UIManager.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1800, 1300), "P4076882 - Kristina Blinova - The Racing Game!");
    
    //Create a car
    PlayerC player;
    UIManager uiManager;
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        //Movement
        player.movePlayer();
        
        //Test

        
        window.clear();
        window.draw(player.getPlayer());
        window.draw(uiManager.writeInt(10));
        window.display();
        
    }
    
    return 0;
}
