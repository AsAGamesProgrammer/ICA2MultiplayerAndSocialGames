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

class GameLoop
{

public:
    void StartGame();
    void OpenLobbie();
    
    
private:
    PlayerC player;
    PlayerC otherPlayer;
    
    UIManager uiManager;
    mapCreator mapManager;
    Checkpoint chpManager;
    
    void Update();
    void Render();
    
    //Collision
    void checkPointPassed(int index);
    
    
};

#endif /* GameLoop_hpp */