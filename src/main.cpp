#include <iostream>
#include "Events/EventManager.h"
#include "Application/Application.h"
#include <memory>
#include "ResourceManagement/ResourceManager.h"


int main()
{
    auto game = std::make_unique<Application>();

    while (!game->GetWindow().IsDone())
    {
        game->Update();
        game->Render();
        game->PostUpdate();
    }
}

