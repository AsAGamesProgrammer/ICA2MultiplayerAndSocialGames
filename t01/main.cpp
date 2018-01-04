//
//  main.cpp
//  t01
//
//  Created by Cordry, Julien on 30/09/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//


#include "GameLoop.hpp"
#include <SFML/Network.hpp>
#include <thread>
#include <chrono>

int main()
{
    //--------------------------------------
    //GAME LOGIC
    
    //Create a game loop class
    GameLoop gameLoop;
    
    std::thread tcpRecThread (&GameLoop::receiveTCP, &gameLoop);
    std::thread udpRecThread (&GameLoop::receiveUDP, &gameLoop);
    
    //Create a starting menu / lobbie
    int mode = gameLoop.OpenLobbie();
    
    switch(mode)
    {
        case 0:
            //Start training game
            gameLoop.StartGame();
            break;
            
        case 1:
            //Start coopGame
            gameLoop.StartCoopGame();
            break;
            
        case 2:
            //Start Networking game
            gameLoop.StartGame();
            break;
            
        default:
            gameLoop.StartGame();
            break;
    }
    
    
    tcpRecThread.join();
    udpRecThread.join();
    
    return 0;
}

