//
//  main.cpp
//  t01
//
//  Created by Cordry, Julien on 30/09/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//


#include "GameLoop.hpp"
#include <SFML/Network.hpp>

int main()
{
    //--------------------------------------
    //GAME LOGIC
    
    //Create a game loop class
    GameLoop gameLoop;
    
    //Create a starting menu / lobbie
    gameLoop.OpenLobbie();
    
    //Start game
    gameLoop.StartGame();
    
    return 0;
}

