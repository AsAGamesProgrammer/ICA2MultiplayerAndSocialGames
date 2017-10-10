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
    
    
    //Check all the checkpoints
    for (int i=0; i<chpManager.cpNumber; i++)
    {
        //Collision with a checkpoint
        checkPointPassed(i);
    }
    
}

void GameLoop::Render()
{
    //Can be skipped because background is rerendered anyways
    //window.clear();
    
    //Update Interface
    uiManager.displaySpeed(player.getSpeed());
    
    
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

void GameLoop::checkPointPassed(int index)
{
    if(player.getPlayer().getPosition().x + player.width/2 >chpManager.checkPSprites[index].getPosition().x - chpManager.sizeW/2 && //left
       player.getPlayer().getPosition().x - player.width/2 <=chpManager.checkPSprites[index].getPosition().x + chpManager.sizeW/2 &&  //right
       player.getPlayer().getPosition().y + player.height/2 > chpManager.checkPSprites[index].getPosition().y - chpManager.sizeH/2 && //top
       player.getPlayer().getPosition().y - player.height/2 <= chpManager.checkPSprites[index].getPosition().y + chpManager.sizeH/2) //bot
        
    {
        //Handle collision
        return;
    }
    
    //No collision
    return;

}
