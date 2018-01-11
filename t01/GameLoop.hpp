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

//PUBLIC
public:
    
    //Constructor
    GameLoop();
    
    //Start different game modes
    void StartGame();
    void StartCoopGame();
    void StartNetworkGame();
    
    //Opens lobbie, returns a selected mode
    int OpenLobbie();
    
    //Thread functions for receiving server msgs
    void receiveTCP();
    void receiveUDP();
    
    
//PRIVATE
private:
    
    //My player information
    std::string myName;                         //name
    int myID=-1;                                //race id
    
    //Player classes
    PlayerC player;                             //local player
    PlayerCoop otherPlayer;                     //coop game player
    PlayerNetwork networkPlayers[4];            //array of networking players
    void addNewPlayer(std::string name, int id);//adds a new networking player to a session
    
    //UI reated
    UIManager uiManager;                        //UI manager
    mapCreator mapManager;                      //Map generator class
    
    //Checkpoints and score
    Checkpoint chpManager;                      //Manager class
    float lastSentNumberOfLaps = 0.0;           //Lap information
    
    //Update functions
    void Update();
    bool gameUpdate();
    bool networkingGameOn=false;
    
    //Render functions
    void Render();
    void GeneralRender();
    void networkedGameRender();
    
    //Collision
    void checkPointPassed(int index);
    void checkBulletCollision();
    
    //Lobby
    Lobby lobby;
    
    //Collisions
    void checkNetworkBulletCollisions(int id);
    void checkAllCollsiisons();
    
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
    
    void interpretTCP(char bytes[1024]);
    void interpretUDP(char bytes[1024]);
    
    //-----------------------
    //       HELPERS
    //-----------------------
    std::string getStringFromBytes(char data[1024]);
    
};

#endif /* GameLoop_hpp */
