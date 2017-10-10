//
//  GameLoop.cpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 10/10/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#include "GameLoop.hpp"

//GLOBAL VARIABLES
sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height),
                        "P4076882 - Kristina Blinova - The Racing Game!");


void GameLoop::StartGame()
{
    
    //PRE-PROCESSING
    mapManager.loadTiles();
    
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
    
    //Collision with a checkpoint
    checkPointPassed();
    
}

void GameLoop::Render()
{

    //Update Interface
    uiManager.displaySpeed(player.getSpeed());
    
    //window.clear();
    
    //Track tiles
    for(int i=0; i<mapManager.tileNumber; i++)
    {
        window.draw(mapManager.tiles[i]);
    }
    
    //CheckPoint
    window.draw(chpManager.getSprite());
    
    //Player
    window.draw(player.getPlayer());
    
    //Bullet
    window.draw(player.getBullet());
    
    //UI
    for(int i=0; i<uiManager.gameLabels.size(); i++)
    {
        window.draw(uiManager.gameLabels.front());
        uiManager.gameLabels.pop_front();
    }
    

    
    window.display();

}

bool GameLoop::checkPointPassed()
{
    if(player.getPlayer().getPosition().x + player.width/2 >chpManager.getSprite().getPosition().x - chpManager.sizeW/2 &&
       player.getPlayer().getPosition().x - player.width/2 <=chpManager.getSprite().getPosition().x + chpManager.sizeW/2 )
       //player.getPlayer().getPosition().y + player.height/2 > chpManager.getSprite().getPosition().y - chpManager.sizeH/2)
        
    {
        std::cout<<"Collision"<<std::endl;
        return true;
    }
    
    std::cout<<"No Collided"<<std::endl;
    
    return false;

}
