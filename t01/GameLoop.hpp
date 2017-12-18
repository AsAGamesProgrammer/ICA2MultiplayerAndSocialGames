//
//  GameLoop.hpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 10/10/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#ifndef GameLoop_hpp
#define GameLoop_hpp

#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>

#include <iostream>
#include "PlayerC.hpp"
#include "UIManager.hpp"
#include "mapCreator.hpp"
#include "Checkpoint.hpp"
#include "GameLoop.hpp"
#include "PlayerCoop.hpp"
#include "Lobby.hpp"

//Networking
#include <SFML/Network.hpp>

class GameLoop
{

public:
    GameLoop();
    void StartGame();
    void OpenLobbie();
    
    void receiveTCP();
    void receiveUDP();
    
private:
    PlayerC player;
    PlayerCoop otherPlayer;
    
    UIManager uiManager;
    mapCreator mapManager;
    Checkpoint chpManager;
    
    void Update();
    void Render();
    
    //Collision
    void checkPointPassed(int index);
    void checkBulletCollision();
    
    //Lobby
    Lobby lobby;
    
    //-----------------------
    //      NETWORKING
    //-----------------------
    //Sockets
    sf::TcpSocket socketTCP;
    sf::UdpSocket socketUDP;
    
    //Send
    void sendUDPUpdata(std::string msg);
    void sendTCPData(std::string msg);

    void receiveTCPOnce();
    void receiveUDPOnce();
    
    //Loop condition
    bool runThreads=true;
    
};

#endif /* GameLoop_hpp */
