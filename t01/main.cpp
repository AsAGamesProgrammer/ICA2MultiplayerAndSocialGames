//
//  main.cpp
//  t01
//
//  Created by Cordry, Julien on 30/09/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#include <iostream>
#include "PlayerC.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");
    
    //Create a car
    PlayerC player;
    
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
        
        
        window.clear();
        window.draw(player.getPlayer());
        window.display();
        
    }
    
    return 0;
}
