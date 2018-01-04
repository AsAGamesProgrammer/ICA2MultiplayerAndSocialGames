//
//  GameLoop.cpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 10/10/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//
//
//-------------------------------------------------------
//TODO:

//Make "Network player" class
//Make a message class and somewhat a game?
//Database
//Security

//-------------------------------------------------------
//  NETWORKING CODES:
//  REG: - register with a server
//  JOI: - join a networking game
//-------------------------------------------------------


#include "GameLoop.hpp"
#include <stdio.h>
#include <thread>
#include <chrono>

//GLOBAL VARIABLES
sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height),
                        "P4076882 - Kristina Blinova - The Racing Game!");


GameLoop::GameLoop()
{
    
    //--------------------------------------
    //CONNECTIONS

    //TCP
    //Create a socket
    sf::Socket::Status status = socketTCP.connect("152.105.5.139", 7578);
    
    //Bind
    if (status != sf::Socket::Done)
    {
        std::cout<<"Failed to connect TCP"<<std::endl;
    }
    else
        std::cout<<"TCP Connected"<<std::endl;
    
    
    //REGISTER
    std::cout<<"Hi driver! What is your name? "<<std::endl;
    
    std::string userName;
    std::cin>>userName;
    myName=userName;
    
    //Register TCP
    sendTCPData("REG " + userName);
    receiveTCPOnce();
    
    //Register UDP
    sendUDPUpdata("REG " + userName);
}

//----------------------------------------
//              NETWORKING
//----------------------------------------

//Send TCP
void GameLoop::sendTCPData(std::string msg)
{
    std::string combinedString = msg + "\n";
    char data[255];
    strcpy(data, combinedString.c_str());
    printf("Sending TCP: %s\n", data);
    
    if (socketTCP.send(data, combinedString.length()) != sf::Socket::Done)
    {
        std::cout<<"Failed to send a msg (TCP)"<<std::endl;
    }

    //send worked
    

}

void GameLoop::receiveTCP()
{
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    while(true)
    {
        char inData[100] = "None";
        std::size_t received;
        // TCP socket:
        if (socketTCP.receive(inData, 100, received) != sf::Socket::Done)
        {
            std::cout << "Failed to receive (TCP)"<< std::endl;
        }
 
        //Print only if received text
        char firstLit = inData[0];
        if(firstLit != '\0')
        {
            std::cout << "Received " << inData <<" with "<<received<<" bytes" << std::endl;
            
            //Convert bytes to a string
            std::string sub;
            for (int i=0; i<100; i++)
            {
                char nextChar = inData[i];
                sub +=nextChar;
            }
            
            std::string code = sub.substr(0, 3);
            
            //RECEIVED: JOI
            if(sub == "JOI")
                addNewPlayer(sub.substr(4, code.length()));
        }
        
        
        //If received REG: add player
    }
}

void GameLoop::receiveTCPOnce()
{
    char inData[100] = "None";
    std::size_t received;
    // TCP socket:
    if (socketTCP.receive(inData, 100, received) != sf::Socket::Done)
    {
        std::cout << "Failed to receive (TCP)"<< std::endl;
    }
    
    //Print only if received text
    char firstLit = inData[0];
    if(firstLit != '\0')
        std::cout << "Received " << inData <<" with "<<received<<" bytes" << std::endl;
}

//Send UDP
void GameLoop::sendUDPUpdata(std::string msg)
{
    std::string combinedString = msg + "\n";
    char data[255];
    strcpy(data, combinedString.c_str());

    printf("Sending UDP: %s\n", data);
    
    sf::IpAddress recipient = "255.255.255.255";
    unsigned short port = 7576;
    if (socketUDP.send(data, combinedString.length(), recipient, port) != sf::Socket::Done)
    {
        std::cout<<"Failed to send a msg (UDP)"<<std::endl;
    }
}

void GameLoop::receiveUDP()
{
    while(true)
    {
        char data[100];
        std::size_t received;
        sf::IpAddress sender;
        unsigned short port;
        if (socketUDP.receive(data, 100, received, sender, port) != sf::Socket::Done)
        {
            std::cout<<"Failed to receive UDP"<<std::endl;
        }
        
        //Print only if received text
        char firstLit = data[0];
        if(firstLit != '\0')
        std::cout << "Received "<<data<< " which is " << received << " bytes from " << sender << " on port " << port << std::endl;
    }
}

void GameLoop::receiveUDPOnce()
{
    char data[100];
    std::size_t received;
    sf::IpAddress sender;
    unsigned short port;
    if (socketUDP.receive(data, 100, received, sender, port) != sf::Socket::Done)
    {
        std::cout<<"Failed to receive UDP"<<std::endl;
    }
    
    //Print only if received text
    char firstLit = data[0];
    if(firstLit != '\0')
        std::cout << "Received "<<data<< " which is " << received << " bytes from " << sender << " on port " << port << std::endl;
    
}


//----------------------------------------
//              LOBBY
//----------------------------------------

int GameLoop::OpenLobbie()
{
    window.clear();
    int selectedMode=0;
    
    //THREADS
    //Listens to TCP
    //std::thread tcpRecThread (&GameLoop::receiveTCP, this);
    //std::thread udpRecThread (&GameLoop::receiveUDP, this);
    
    //GAME LOOP
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        //Lobby
        window.draw(lobby.lobbieText);
        
        for (int i=0; i<lobby.numberOfBtns; i++)
        {
            window.draw(lobby.btnArray[i].shape);
            window.draw(lobby.btnArray[i].label);
        }
        
        window.display();
        
        //if pressed up - choose option 1
        //if pressed 2 - choose option 2
        //if return - return
        //make funtion return int, not void
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
        {
            lobby.setActiveButton(0);
            sendTCPData("0 pressed");
            selectedMode=0;
        }
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        {
            lobby.setActiveButton(1);
            sendUDPUpdata("1 pressed");
            selectedMode=1;
        }
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        {
            lobby.setActiveButton(2);
            sendUDPUpdata("2 pressed");
            selectedMode=2;
        }
           
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            //tcpRecThread.join();
            //udpRecThread.join();
            
            //std::terminate();
            return selectedMode;
        }
        
    }
    
    return selectedMode;
    
    //tcpRecThread.join();
    //udpRecThread.join();
}

//----------------------------------------
//              NETWROK MODE
//----------------------------------------
void GameLoop::StartNetworkGame()
{
    sendTCPData("JOI " + myName);
    mapManager.loadTiles();
    player.createPlayer("../../../../Users/p4076882/Desktop/ICA2MultiplayerAndSocialGames//carYS3.png");      //Player 1
    player.setStartingPos(500, 180);
    
    // Notify server
    // Create all registered player
    // Draw all the players
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        

        GeneralRender();
        
        //Player
        window.draw(player.getPlayer());
        
        window.display();
    }
}

void GameLoop::addNewPlayer(std::string name)
{
    if(name !=myName)
    {
        networkPlayer.createPlayer("../../../../Users/p4076882/Desktop/ICA2MultiplayerAndSocialGames/carBS5.png");
        std::cout<<"NEW PLAYER ADDED"<<std::endl;
    }
}

//----------------------------------------
//              GAME MODES
//----------------------------------------

void GameLoop::StartCoopGame()
{
    //Prepare everything for the other player
    otherPlayer.createPlayer("../../../../Users/p4076882/Desktop/ICA2MultiplayerAndSocialGames/carBS5.png"); //Player 2
    //TEMP
    otherPlayer.setStartingPos(500, 200);
    
    
    StartGame();
}

void GameLoop::StartGame()
{
    
    //PRE-PROCESSING
    mapManager.loadTiles();
    player.createPlayer("../../../../Users/p4076882/Desktop/ICA2MultiplayerAndSocialGames//carYS3.png");      //Player 1
    //otherPlayer.createPlayer("../../../../Users/p4076882/Desktop/ICA2MultiplayerAndSocialGames/carBS5.png"); //Player 2
    
    
    //TEMP
    //otherPlayer.setStartingPos(500, 200);
    
    //GAME LOOP
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        //Update movement
        Update();

        //Render everything
        Render();
        
    }

}

void GameLoop::Update()
{
    //Movement
    player.moveRelated();
    otherPlayer.moveRelated();
    
    
    //Check all the checkpoints
    for (int i=0; i<chpManager.cpNumber; i++)
    {
        //Collision with a checkpoint
        checkPointPassed(i);
    }
    
    //Bullets
    checkBulletCollision();
    
}

void GameLoop::GeneralRender()
{
    //Update Interface
    uiManager.displaySpeed(player.getSpeed());
    uiManager.displayCheckPoints();
    
    //Track tiles
    for(int i=0; i<mapManager.tileNumber; i++)
    {
        window.draw(mapManager.tiles[i]);
    }
    
    //CheckPoint
    for (int i=0; i<chpManager.cpNumber; i++)
        window.draw(chpManager.checkPSprites[i]);
    
    //UI - changing labels
    for(int i=0; i<uiManager.gameLabels.size(); i++)
    {
        window.draw(uiManager.gameLabels.front());
        uiManager.gameLabels.pop_front();
    }
    
    //UI - constant labels
    for(int i=0; i<uiManager.numberOfConstText; i++)
        window.draw(uiManager.constantText[i]);
}

void GameLoop::Render()
{
    GeneralRender();
    
    //Player
    window.draw(player.getPlayer());
    window.draw(otherPlayer.getPlayer());
    
    //Bullet
    window.draw(player.getBullet());
    window.draw(otherPlayer.getBullet());
    
    window.display();
}



//TODO: bullet collision for otherPlayer
void GameLoop::checkBulletCollision()
{
    if(otherPlayer.getCheckBulletColision())
    {
        if(player.getPlayer().getPosition().x + player.width/2 >otherPlayer.getBullet().getPosition().x - 28 && //left
           player.getPlayer().getPosition().x - player.width/2 <=otherPlayer.getBullet().getPosition().x + 28 &&  //right
           player.getPlayer().getPosition().y + player.height/2 > otherPlayer.getBullet().getPosition().y - 28 && //top
           player.getPlayer().getPosition().y - player.height/2 <= otherPlayer.getBullet().getPosition().y + 28) //bot
            
        {
            std::cout<<"Hit"<<std::endl;
            player.onTakingHit();
            otherPlayer.setCheckBulletColision(false);
            
            return;
        }
    }
    
    //No collision
    return;

}

//TODO: do this for each player

void GameLoop::checkPointPassed(int index)
{
    if(player.getPlayer().getPosition().x + player.width/2 >chpManager.checkPSprites[index].getPosition().x - chpManager.sizeW/2 && //left
       player.getPlayer().getPosition().x - player.width/2 <=chpManager.checkPSprites[index].getPosition().x + chpManager.sizeW/2 &&  //right
       player.getPlayer().getPosition().y + player.height/2 > chpManager.checkPSprites[index].getPosition().y - chpManager.sizeH/2 && //top
       player.getPlayer().getPosition().y - player.height/2 <= chpManager.checkPSprites[index].getPosition().y + chpManager.sizeH/2) //bot
        
    {
        //Handle collision
        chpManager.checkedPoints[index]=true;
        uiManager.checkedPoints[index]="Checked!";
        
        return;
    }
    
    //No collision
    return;

}
