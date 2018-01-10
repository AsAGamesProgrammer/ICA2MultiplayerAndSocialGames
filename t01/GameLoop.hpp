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
#include "PlayerNetwork.hpp"

//Networking
#include <SFML/Network.hpp>

class GameLoop
{

public:
    GameLoop();
    void StartGame();
    void StartCoopGame();
    void StartNetworkGame();
    int OpenLobbie();
    
    void receiveTCP();
    void receiveUDP();
    
    void addNewPlayer(std::string name, int id);
    
private:
    std::string myName;
    PlayerC player;
    PlayerCoop otherPlayer;
    
    PlayerNetwork networkPlayers[4];
    
    UIManager uiManager;
    mapCreator mapManager;
    Checkpoint chpManager;
    
    void Update();
    void Render();
    void GeneralRender();
    void networkedGameRender();
    
    //Collision
    void checkPointPassed(int index);
    void checkBulletCollision();
    
    void interpretTCP(char bytes[1024]);
    void interpretUDP(char bytes[1024]);
    
    //Update networking game
    bool gameUpdate();
    bool networkingGameOn=false;
    
    //Lobby
    Lobby lobby;
    
    //SCORE
    void updateScore();
    
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
    
    int myID=-1;
    
    //Laps
    float lastSentNumberOfLaps = 0.0;
    
};

#endif /* GameLoop_hpp */
