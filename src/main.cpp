#include <iostream>
#include <SFML/Graphics.hpp>
#include "Events/EventManager.h"
#include "GameLogic/Game.h"
#include <memory>


int main()
{
    auto game = std::make_unique<Game>();

    while (game->GetWindow().IsDone() == false)
    {
        game->Update();
        game->Render();
        game->PostUpdate();
    }
}

