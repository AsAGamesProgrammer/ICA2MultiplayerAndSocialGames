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
// Threads
// Send a custom msg
// Make a class for msgs
// Store log in on the server
//
//


#include "GameLoop.hpp"
#include <stdio.h>

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
    
    
    //SEND TCP DATA
    sendTCPData("test TCP lalala");
    receiveTCP();
    
    //SEND UDP DATA
    sendUDPUpdata("testing broadcast P");
    receiveUDP();
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
    //ISSUE!!!
    //------NEW NEW NEW---------
    //This should be in a thread
    //Receives data once
    
    char inData[100] = "None";
    std::size_t received;
    // TCP socket:
    if (socketTCP.receive(inData, 100, received) != sf::Socket::Done)
    {
        std::cout << "Failed to receive (TCP)"<< std::endl;
    }
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
    char data[100];
    std::size_t received;
    sf::IpAddress sender;
    unsigned short port;
    if (socketUDP.receive(data, 100, received, sender, port) != sf::Socket::Done)
    {
        std::cout<<"Failed to receive UDP"<<std::endl;
    }
    std::cout << "Received "<<data<< " which is " << received << " bytes from " << sender << " on port " << port << std::endl;
}


//----------------------------------------
//              LOBBY
//----------------------------------------

void GameLoop::OpenLobbie()
{
    window.clear();
    
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
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            lobby.selectNextBtn();
        }
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            lobby.selectPrevBtn();
        }
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            return;
        
        //Networking test
        //sendUDPUpdata("test");
        //receiveUDP();
    }
}


void GameLoop::StartGame()
{
    
    //PRE-PROCESSING
    mapManager.loadTiles();
    player.createPlayer("carYS3.png");      //Player 1
    otherPlayer.createPlayer("carBS5.png"); //Player 2
    
    
    //TEMP
    otherPlayer.setStartingPos(500, 200);
    
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

void GameLoop::Render()
{
    //Can be skipped because background is rerendered anyways
    //window.clear();
    
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
    
    //Player
    window.draw(player.getPlayer());
    window.draw(otherPlayer.getPlayer());
    
    //Bullet
    window.draw(player.getBullet());
    window.draw(otherPlayer.getBullet());
    
    //UI - changing labels
    for(int i=0; i<uiManager.gameLabels.size(); i++)
    {
        window.draw(uiManager.gameLabels.front());
        uiManager.gameLabels.pop_front();
    }
    
    //UI - constant labels
    for(int i=0; i<uiManager.numberOfConstText; i++)
        window.draw(uiManager.constantText[i]);

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
