//
//  GameLoop.cpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 10/10/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//
//
//-------------------------------------------------------

//-------------------------------------------------------
//  NETWORKING CODES:
//          TCP
//  REG - register with a server
//  JOI - join a networking game
//  STR - start game
//  REP - repeat joining proccess
//  SCR - score
//  BLT - bullet
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


//--------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//--------------------------------------------------------------------------------
GameLoop::GameLoop()
{

    //COONNECT TCP
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
    
    //Player information
    std::cout<<"Hi driver! What is your name? "<<std::endl;     //Ask for log in
    std::string userName;                                       //Take input
    std::cin>>userName;
    myName=userName;                                            //Save name to a variable
    
    //Register TCP
    sendTCPData("REG " + myName + " ");
    receiveTCPOnce();                                           //Wait for a response
    
}

//--------------------------------------------------------------------------------
//                                  NETWORKING
//--------------------------------------------------------------------------------

//--------------------------------------------------------
//                      TCP
//--------------------------------------------------------

//SEND TCP
//Send a tcp message - string - to a server
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
}

//RECEIVE TCP
//This function is called in a thread ONLY and has a forever while loop
void GameLoop::receiveTCP()
{
    //Sleep for one second before before starting listening
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    //Forever loop
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
            
            //Interpret data
            interpretTCP(inData);
        }
    }
}

//RECEIVE TCP ONCE
// This function is used only once and only for a registration
// An idea is that client will wait for a ssuccessful registartion and won't enter a game illegly
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
        std::string sub = getStringFromBytes(inData);
        
        //Get the first three characters which represent a code
        std::string code = sub.substr(0, 3);
        
        //RECEIVED REG
        if(code =="REG")
        {
            //Register UDP if TCP registration is complete
            sendUDPUpdata("REG " + myName + " ");
        }
        
    }
}

//INTERPRET TCP
//This function substracts a code fro the message and calls respected functions
void GameLoop::interpretTCP(char bytes[1024])
{
    //Convert bytes to a string
    std::string sub = getStringFromBytes(bytes);

    //Get a 3 character code
    std::string code = sub.substr(0, 3);
    
        //RECEIVED: JOI for join
    if(code == "JOI")
    {
        std::string raceId = sub.substr(4,5);
        int raceIdInt = atoi(raceId.c_str());
        addNewPlayer(sub.substr(5, sub.length()), raceIdInt);
    }
    else //RECEIVED STR for Start game
    if(code =="STR")
    {
        networkingGameOn=true;
    }
    else //RECEIVED SCR for Score
    if(code =="SCR")
    {
        //Player id
        std::string raceId = sub.substr(4,5);
        int raceIdInt = atoi(raceId.c_str());
        if(raceIdInt == myID)
            return;
        
        //Score
        std::string stringScore = sub.substr(6, 8);

        //Transfer string to a float and then float to a string
        //This is required for the corrct sf:Text display
        
        std::string::size_type sz;     // alias of size_t
        float scoreInFloat = std::stof (stringScore,&sz);
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << scoreInFloat;
        uiManager.laps[raceIdInt] = ss.str();
        
        //Provide feedback to the user
        std::cout<<"SCORE!!!!!"<<std::endl;
        std::cout<<"Race id is "<<raceIdInt<<" and score is "<<stringScore<<std::endl;
        
    }
    else //RECEIVED BLT for Bullet launched
    if(code == "BLT")
    {
        //Player id
        std::string raceId = sub.substr(4,5);
        int raceIdInt = atoi(raceId.c_str());
        
        //Return if it is my id
        if(raceIdInt == myID)
            return;
        
        //Extract bullet angle from the msg
        std::string angleString = sub.substr(6, 8);
        std::string::size_type sz;     // alias of size_t
        
        //Transfer angle to a float
        float angleFloat = std::stof (angleString,&sz);

        //Instantiate a bullet and start moving it
        networkPlayers[raceIdInt].instantiateBlt(angleFloat);
        
        //Start collision checks
        networkPlayers[raceIdInt].setCheckBulletColision(true);
    }
}

//--------------------------------------------------------
//                      UDP
//--------------------------------------------------------
//SEND UDP
//Send a udp msg - string - to a server
void GameLoop::sendUDPUpdata(std::string msg)
{
    //Add an end of line character to the end of the msg
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

//INTERPRET UDP
//Interprets a udp message and calls respected functions
void GameLoop::interpretUDP(char bytes[1024])
{
    //Convert bytes to a string
    std::string sub = getStringFromBytes(bytes);
    
    //Extract a three character code
    std::string code = sub.substr(0, 3);
    
    //RECEIVED: POS for position
    if(code == "POS")
    {
        //ID string
        std::string racerIDString;

        //Two flags used for interpriting a string
        bool posXFound=false;
        bool posYFound=false;
        
        //Two strings which will become positions
        std::string posXString;
        std::string posYString;
        
        //Iterate a string starting at the third character
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
            
            //Interpret a result if a space character was not found
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
        
        //If id is not mine
        if(racerId!=myID)
        {
            int newPosX = atoi(posXString.c_str());
            int newPosY = atoi(posYString.c_str());
        
            networkPlayers[racerId].setStartingPos(newPosX, newPosY);
            std::cout<<"Interpreted as: id "<<racerId<<" posX "<<newPosX<<" posY "<<newPosX<<std::endl;
        }
    }
}

//RECEIVE UDP
//This function is called in a thread ONLY and has a forever while loop
void GameLoop::receiveUDP()
{
    //Infinite loop
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
            
            //Interpret received udp data
            interpretUDP(data);
            
        }
    }
}

//----------------------------------------
//             HELPERS
//----------------------------------------

//Make a string from an array of bytes
std::string GameLoop::getStringFromBytes(char data[1024])
{
    //Convert bytes to a string
    std::string sub;
    int i=0;
    
    while(i<1024)
    {
        char nextChar = data[i];
        
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
    
    return sub;
}

//--------------------------------------------------------------------------------
//                                  LOBBY
//--------------------------------------------------------------------------------

//Lobby is a menu system which allows choosing one o three game modes

//OPEN LOBBIE
int GameLoop::OpenLobbie()
{
    //Clear the window
    window.clear();
    
    //Initially selected mode is Training because it is the higest button
    int selectedMode=0;
    
    //Game loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        //RENDER
        //Draw lobby UI elements
        window.draw(lobby.lobbieText);                  //Draw text
        
        for (int i=0; i<lobby.numberOfBtns; i++)        //Draw buttons
        {
            window.draw(lobby.btnArray[i].shape);
            window.draw(lobby.btnArray[i].label);
        }
        
        //Display drawn elements
        window.display();
        
        
        //MECHANICS
        //if pressed up - choose option 1
        //if pressed 2 - choose option 2
        //if return - return
        //make funtion return int, not void
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
        {
            lobby.setActiveButton(0);
            selectedMode=0;
        }
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        {
            lobby.setActiveButton(1);
            selectedMode=1;
        }
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        {
            lobby.setActiveButton(2);
            selectedMode=2;
        }
           
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            return selectedMode;
        }
        
    }
    
    return selectedMode;
}

//--------------------------------------------------------------------------------
//                                 GAMEPLAY
//--------------------------------------------------------------------------------
//----------------------------------------
//              NETWROK MODE
//----------------------------------------
//This function starts and controls a netowrking game

void GameLoop::StartNetworkGame()
{
    //Send a TCP message with a code for JOIN
    sendTCPData("JOI " + myName + " ");
    
    //Load map tiles
    mapManager.loadTiles();
    
    //GameLoop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        //GAME STARTED-----------------------------------------------------------------------------------------------
        
        //Update the game only if it officially started
        if(networkingGameOn)
        {
            //PLAYER MOVEMENT
            //Allow player to move or shoot. Function returns true if a movement key was pressed
            if(gameUpdate())
            {
                //Get postion of the player to local variables
                int x = player.getPlayer().getPosition().x;
                int y = player.getPlayer().getPosition().y;

                //Have a composite position string and send it to a server using UDP
                std::string movementInfo = "POS "+std::to_string(x) +" "+ std::to_string(y)+" "+myName + " ";
                sendUDPUpdata(movementInfo);
            }
            
            //Display a "Race Started" label at the top of the screen
            uiManager.displayBanner("Race STARTED!");
            
            
            //BULLET
            //Check if player did shoot
            if(player.didShoot)
            {
                //Send notification to a server
                sendTCPData("BLT " + myName + " " + std::to_string (player.getPlayer().getRotation()) + " ");
                
                //Set flag to false to avoid sending the message twice
                player.didShoot=false;
                
                //Debug log
                std::cout<<"Bullet info sent"<<std::endl;
            }
        }
        else ///GAME DID NOT START ----------------------------------------------------------------------------------------
        {
            //Display banner
            uiManager.displayBanner("Waiting for other players");
            
            //Press left shift to rejoin
            //This is used to compensate for some descrepencies
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            {
                //Repeat joining in proccess
                sendTCPData("REP");
            }
        }

        //get information about score
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << chpManager.getLap();
        
        //Display score
        uiManager.laps[myID] = ss.str();
        
        //Render
        networkedGameRender();
        
        //Draw bullet on top of all the other rendering
        window.draw(player.getBullet());

        //Display window
        window.display();
    }
}

//NETWORKING GAME UPDATE
bool GameLoop::gameUpdate()
{
    //Check all the checkpoints
    for (int i=0; i<chpManager.cpNumber; i++)
    {
        //Collision with a checkpoint
        checkPointPassed(i);
    }
    
    //Check if player moved
    bool didMove = player.moveRelated();
    
    //Check other player bullets
    for (int i=0; i<4; i++)
    {
        networkPlayers[i].moveBullet();
    }
    
    //Check all the bullet collisions
    checkAllCollsiisons();
    
    return didMove;
}

//ADD NEW PLAYER
//Adds a new player to a game
void GameLoop::addNewPlayer(std::string name, int id)
{
    //Decid which texture to use for a car depending on passed id
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
    
    //Flag to see if the passed name is ours
    bool nameIsMine=false;
    
    //If NAME is not longer than MYNAME
    if(name[myName.length()]=='\0')
       {
           nameIsMine=true;
           
           for(int i=0; i<myName.length(); i++)
           {
               if(myName[i] !=name[i])
                   nameIsMine=false;
           }
       }

    //If name is not mine
    if(!nameIsMine)
    {
        //Create a networking player
        networkPlayers[id].createPlayer(textureAd);
        std::cout<<"("<<id<<")"<<"NEW PLAYER "<< name <<" ADDED"<<std::endl;
        networkPlayers[id].setStartingPos(500, 180 + id*50);
    }
    else
    {
        //Create a player
        player.createPlayer(textureAd);
        player.setStartingPos(500, 180 + id*50);
        myID = id;
    }
    
    //Cout my id
    std::cout<<"MyID is "<<myID<<std::endl;

}

//----------------------------------------
//          OTHER GAME MODES
//----------------------------------------

//START COOP GAME
void GameLoop::StartCoopGame()
{
    //Prepare everything for the other player
    otherPlayer.createPlayer("../../../../Users/p4076882/Desktop/ICA2MultiplayerAndSocialGames/carBS5.png"); //Player 2
    
    //Other player starts at a different position
    otherPlayer.setStartingPos(500, 200);
    
    
    StartGame();
}

//START GAME
void GameLoop::StartGame()
{
    
    //PRE-PROCESSING
    mapManager.loadTiles();
    player.createPlayer("../../../../Users/p4076882/Desktop/ICA2MultiplayerAndSocialGames//carYS3.png");      //Player 1
    
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

//GENRAL RENDER
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

//NETWORK GAME RENDER
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
        window.draw(networkPlayers[i].getBullet());
    }
    
    uiManager.displayLaps();
}

//RENDER
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


//----------------------------------------
//            BULLET COLLISIONS
//----------------------------------------

//CHECK FOR BULLET COLLISION
void GameLoop::checkBulletCollision()
{
    if(otherPlayer.getCheckBulletColision())
    {
        if(player.getPlayer().getPosition().x + player.width/2 >otherPlayer.getBullet().getPosition().x - 28 && //left
           player.getPlayer().getPosition().x - player.width/2 <=otherPlayer.getBullet().getPosition().x + 28 &&  //right
           player.getPlayer().getPosition().y + player.height/2 > otherPlayer.getBullet().getPosition().y - 28 && //top
           player.getPlayer().getPosition().y - player.height/2 <= otherPlayer.getBullet().getPosition().y + 28) //bot
            
        {
            player.onTakingHit();
            otherPlayer.setCheckBulletColision(false);
            
            return;
        }
    }
    
    //No collision
    return;

}

//CHECK FOR ALL THE BULLET COLLISIONS
//This function checks if player's bullet hit any networked player
void GameLoop::checkAllCollsiisons()
{
    for (int i=0; i<4; i++)
    {
        checkNetworkBulletCollisions(i);
    }
}

//CHECK FOR NETWORK BULLET COLLISION
void GameLoop::checkNetworkBulletCollisions(int id)
{
    if(networkPlayers[id].getCheckBulletColision())
    {
        //std::cout<<"CheckingCollisions"<<std::endl;
        
        if(player.getPlayer().getPosition().x + player.width/2 >networkPlayers[id].getBullet().getPosition().x - 28 && //left
           player.getPlayer().getPosition().x - player.width/2 <=networkPlayers[id].getBullet().getPosition().x + 28 &&  //right
           player.getPlayer().getPosition().y + player.height/2 > networkPlayers[id].getBullet().getPosition().y - 28 && //top
           player.getPlayer().getPosition().y - player.height/2 <= networkPlayers[id].getBullet().getPosition().y + 28) //bot
            
        {
            std::cout<<"Hit"<<std::endl;
            
            chpManager.reduceScore();
            sendTCPData("SCR "+myName + " " + std::to_string(chpManager.getLap()) + " ");
            //player.onTakingHit();
            networkPlayers[id].setCheckBulletColision(false);
            
            return;
        }
    }
    
    //No collision
    return;
}

//CECKPOINT PASSED
//Checks if a checkpoint was passed and adds score
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



