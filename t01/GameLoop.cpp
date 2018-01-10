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

// ROTATE cars inside the clients

//-------------------------------------------------------
//  NETWORKING CODES:
//          TCP
//  REG - register with a server
//  JOI - join a networking game
//  STR - start game
//  REP - repeat joining proccess
//  SCR - score
//
//          UDP
//  POS - change position of a player
//-------------------------------------------------------


#include "GameLoop.hpp"
#include <stdio.h>
#include <thread>
#include <chrono>
#include <sstream> // stringstream
#include <iomanip> // setprecision

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
    sendTCPData("REG " + myName + " ");
    receiveTCPOnce();
    

}

//--------------------------------------------------------------------------------
//                                  NETWORKING
//--------------------------------------------------------------------------------

//--------------------------------------------------------
//                      TCP
//--------------------------------------------------------
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
        char inData[1024] = "None";
        std::size_t received;
        // TCP socket:
        if (socketTCP.receive(inData, 1024, received) != sf::Socket::Done)
        {
            std::cout << "Failed to receive (TCP)"<< std::endl;
        }
 
        //Print only if received text
        char firstLit = inData[0];
        if(firstLit != '\0')
        {
            std::cout << "Received " << inData <<" with "<<received<<" bytes" << std::endl;
            
            interpretTCP(inData);

        }
        
    }
}

void GameLoop::interpretTCP(char bytes[1024])
{
    //Convert bytes to a string
    std::string sub;
    int i=0;
    
    while(i<1024)
    {
        char nextChar = bytes[i];
        
        if(nextChar !='\n')
        {
            sub +=nextChar;
            i++;
        }
        else
        {
            i=1025;
        }
    }
    
    std::string code = sub.substr(0, 3);
    
    //RECEIVED: JOI
    if(code == "JOI")
    {
        std::string raceId = sub.substr(4,5);
        int raceIdInt = atoi(raceId.c_str());
        addNewPlayer(sub.substr(5, sub.length()), raceIdInt);
    }
    else
    if(code =="STR")
    {
        networkingGameOn=true;
    }
    else
    if(code =="SCR")
    {
        
    }
}

void GameLoop::receiveTCPOnce()
{
    char inData[1024] = "None";
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
        int i=0;
        
        while(i<1024)
        {
            char nextChar = inData[i];
            
            if(nextChar !='\n')
            {
                sub +=nextChar;
                i++;
            }
            else
            {
                i=110;
            }
        }
        
        std::string code = sub.substr(0, 3);
        
        //RECEIVED REG
        if(code =="REG")
        {
            //Register UDP if TCP registration is complete
            sendUDPUpdata("REG " + myName + " ");
        }
        
    }
}

//--------------------------------------------------------
//                      UDP
//--------------------------------------------------------
void GameLoop::sendUDPUpdata(std::string msg)
{
    std::string combinedString = msg + "\n";
    char data[1024];
    strcpy(data, combinedString.c_str());

    printf("Sending UDP: %s\n", data);
    
    sf::IpAddress recipient = "255.255.255.255";
    unsigned short port = 7576;
    if (socketUDP.send(data, combinedString.length(), recipient, port) != sf::Socket::Done)
    {
        std::cout<<"Failed to send a msg (UDP)"<<std::endl;
    }
}

void GameLoop::interpretUDP(char bytes[1024])
{
    //Convert bytes to a string
    std::string sub;
    int i=0;
    
    while(i<1024)
    {
        char nextChar = bytes[i];
        
        if(nextChar !='\n')
        {
            sub +=nextChar;
            i++;
        }
        else
        {
            i=1025;
        }
    }
    
    std::string code = sub.substr(0, 3);
    
    //RECEIVED: JOI
    if(code == "POS")
    {
        //ID
        std::string racerIDString;

        //Two flags
        bool posXFound=false;
        bool posYFound=false;
        
        //Two strings which will become positions
        std::string posXString;
        std::string posYString;
        
        for(int i=3; i<sub.length()-1; i++)
        {
            char nextChar = sub[i];
            
            //Space between x and y found
            if(nextChar == ' ')
            {
                if(!posXFound)
                    posXFound=true;
                else
                    posYFound=true;
                continue;
            }
            
            if(!posXFound)
            {
                posXString +=nextChar;
            }
            else if(!posYFound)
            {
                posYString +=nextChar;
            }
            else
            {
                racerIDString +=sub[i];
                break;
            }
        }
        
        //Results
        //Ints
        int racerId = atoi(racerIDString.c_str());
        std::cout<<"Received position from id "<<racerId<<" where my ID is "<<myID<<std::endl;
        
        if(racerId!=myID)
        {
            int newPosX = atoi(posXString.c_str());
            int newPosY = atoi(posYString.c_str());
        
            networkPlayers[racerId].setStartingPos(newPosX, newPosY);
            std::cout<<"Interpreted as: id "<<racerId<<" posX "<<newPosX<<" posY "<<newPosX<<std::endl;
        }
    }
}



void GameLoop::receiveUDP()
{
    while(true)
    {
        char data[1024];
        std::size_t received;
        sf::IpAddress sender;
        unsigned short port;
        if (socketUDP.receive(data, 1024, received, sender, port) != sf::Socket::Done)
        {
            std::cout<<"Failed to receive UDP"<<std::endl;
        }
        
        //Print only if received text
        char firstLit = data[0];
        if(firstLit != '\0')
        {
            std::cout << "Received "<<data<< " which is " << received << " bytes from " << sender << " on port " << port << std::endl;
            
            interpretUDP(data);
            
        }
    }
}

void GameLoop::receiveUDPOnce()
{
    char data[1024];
    std::size_t received;
    sf::IpAddress sender;
    unsigned short port;
    if (socketUDP.receive(data, 1024, received, sender, port) != sf::Socket::Done)
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
            //sendTCPData("0 pressed");
            selectedMode=0;
        }
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        {
            lobby.setActiveButton(1);
            //sendUDPUpdata("1 pressed");
            selectedMode=1;
        }
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        {
            lobby.setActiveButton(2);
            //sendUDPUpdata("2 pressed");
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
    sendTCPData("JOI " + myName + " ");
    mapManager.loadTiles();
    

    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        //Update();
        //gameUpdate();
        
        //Update the game if it officially started
        if(networkingGameOn)
        {
            if(gameUpdate())
            {
                int x = player.getPlayer().getPosition().x;
                int y = player.getPlayer().getPosition().y;

                std::string movementInfo = "POS "+std::to_string(x) +" "+ std::to_string(y)+" "+myName + " ";
                sendUDPUpdata(movementInfo);
            }
            
            uiManager.displayBanner("Race STARTED!");
        }
        else
        {
            //Display banner
            uiManager.displayBanner("Waiting for other players");
            
            //Rejoin option
            
            //Press left shift to rejoin
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            {
                //Repeat joining in proccess
                sendTCPData("REP");
            }
        }

        //uiManager.displayLap(chpManager.getLap());
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << chpManager.getLap();
        
        // set the string to display
        //text.setString(ss.str());
        uiManager.laps[myID] = ss.str();
        
        //Render
        networkedGameRender();

        window.display();
    }
}

bool GameLoop::gameUpdate()
{
    //Check all the checkpoints
    for (int i=0; i<chpManager.cpNumber; i++)
    {
        //Collision with a checkpoint
        checkPointPassed(i);
    }
    
    return player.moveRelated();
}

void GameLoop::addNewPlayer(std::string name, int id)
{
    std::string textureAd;
    std::cout<<"ID is "<<id<<std::endl;
    
    if(id==0)
        textureAd = "../../../../Users/p4076882/Desktop/ICA2MultiplayerAndSocialGames/carYS3.png";
            
    else if(id==1)
        textureAd ="../../../../Users/p4076882/Desktop/ICA2MultiplayerAndSocialGames/carPS5 copy.png";
            
    else if(id==2)
        textureAd ="../../../../Users/p4076882/Desktop/ICA2MultiplayerAndSocialGames/carBS5.png";
            
    else if(id==3)
        textureAd ="../../../../Users/p4076882/Desktop/ICA2MultiplayerAndSocialGames/carOS5 copy.png";
    
    
    bool nameIsMine=false;
    
    //If NAME is nt longer than MYNAME
    if(name[myName.length()]=='\0')
       {
           nameIsMine=true;
           
           for(int i=0; i<myName.length(); i++)
           {
               if(myName[i] !=name[i])
                   nameIsMine=false;
           }
       }

    if(!nameIsMine)
    {
        networkPlayers[id].createPlayer(textureAd);
        std::cout<<"("<<id<<")"<<"NEW PLAYER "<< name <<" ADDED"<<std::endl;
        networkPlayers[id].setStartingPos(500, 180 + id*50);
    }
    else
    {
        player.createPlayer(textureAd);
        player.setStartingPos(500, 180 + id*50);
        myID = id;
    }
    
    std::cout<<"MyID is "<<myID<<std::endl;

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

//----------------------------------------
//             UPDATE
//----------------------------------------

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

//----------------------------------------
//              RENDER
//----------------------------------------
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

void GameLoop::networkedGameRender()
{
    //Draw other stuff
    GeneralRender();
    
    //Draw player
    window.draw(player.getPlayer());
    
    //Draw all the opponents
    for (int i=0; i<4; i++)
    {
        window.draw(networkPlayers[i].getPlayer());
    }
    
    uiManager.displayLaps();
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


void GameLoop::checkPointPassed(int index)
{
    if(player.getPlayer().getPosition().x + player.width/2 >chpManager.checkPSprites[index].getPosition().x - chpManager.sizeW/2 && //left
       player.getPlayer().getPosition().x - player.width/2 <=chpManager.checkPSprites[index].getPosition().x + chpManager.sizeW/2 &&  //right
       player.getPlayer().getPosition().y + player.height/2 > chpManager.checkPSprites[index].getPosition().y - chpManager.sizeH/2 && //top
       player.getPlayer().getPosition().y - player.height/2 <= chpManager.checkPSprites[index].getPosition().y + chpManager.sizeH/2) //bot
        
    {
        
        bool allChecked = chpManager.checkpointPassed(index);
        
        if(allChecked)
        {
            for(int i=0; i<4; i++)
            {
                uiManager.checkedPoints[i]="Do it!";
            }
        }
        else if (uiManager.checkedPoints[index] !="Checked!")
        {
            uiManager.checkedPoints[index]="Checked!";
        }
        
        //Send a code, a name and a score
        if(chpManager.getLap() !=lastSentNumberOfLaps)
        {
            lastSentNumberOfLaps = chpManager.getLap();
            sendTCPData("SCR "+myName + " " + std::to_string(chpManager.getLap()) + " ");
        }
        
        return;
    }
    
    //No collision
    return;

}

void GameLoop::updateScore()
{
    
}


