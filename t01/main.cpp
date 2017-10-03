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
#include "mapCreator.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height),
                            "P4076882 - Kristina Blinova - The Racing Game!");
    
    //Create a car
    PlayerC player;
    UIManager uiManager;
    mapCreator mapManager;
    
    //Load map
    mapManager.loadTile();
    
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
        
        //Update Interface
        uiManager.displaySpeed(player.getSpeed());
    
        window.clear();
        
        //----DRAW-----
        //Track
        for(int i=0; i<1; i++)
         {
            sf::Sprite sprite = *mapManager.tiles[i];
            window.draw(sprite);
         }
        
        //Player
        window.draw(player.getPlayer());
        
        //UI
        for(int i=0; i<uiManager.gameLabels.size(); i++)
        {
            window.draw(uiManager.gameLabels.front());
            uiManager.gameLabels.pop_front();
        }
        
        
        window.display();
        
    }
    
    return 0;
}
