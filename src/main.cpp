#include <iostream>
#include <SFML/Graphics.hpp>
#include "Events/EventManager.h"
#include "Application/Application.h"
#include <memory>
#include "ResourceManagement/ResourceManager.h"


int main()
{
    auto game = std::make_unique<Application>();

    while (game->GetWindow().IsDone() == false)
    {
        game->Update();
        game->Render();
        game->PostUpdate();
    }
}

