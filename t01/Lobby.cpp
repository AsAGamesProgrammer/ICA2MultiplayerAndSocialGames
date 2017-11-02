//
//  Lobby.cpp
//  t01
//
//  Created by BLINOVA, KRISTINA on 02/11/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#include "Lobby.hpp"

Lobby::Lobby()
{
    lobbieText = uiUtilities.writeString("Press enter to begin");
    lobbieText.setCharacterSize(60);
    lobbieText.move(1000, 500);
}
