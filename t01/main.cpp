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
    //std::thread udpRecThread (&GameLoop::receiveUDP, &gameLoop);
    
    //Create a starting menu / lobbie
    gameLoop.OpenLobbie();
    
    //Start game
    gameLoop.StartGame();
    
    
    tcpRecThread.join();
    //udpRecThread.join();
    
    return 0;
}

