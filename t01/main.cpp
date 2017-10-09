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
    mapManager.loadTiles();
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        //Movement
        player.moveRelated();
        
        //Update Interface
        uiManager.displaySpeed(player.getSpeed());
    
        window.clear();
        
        //----DRAW-----
        //Track
        for(int i=0; i<mapManager.tileNumber; i++)
         {
            window.draw(mapManager.tiles[i]);
         }
        
        //Player
        window.draw(player.getPlayer());
        
        //Bullet
        window.draw(player.getBullet());
        
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
