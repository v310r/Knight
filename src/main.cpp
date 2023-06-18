#include <iostream>
#include "Events/EventManager.h"
#include "Application/Application.h"
#include <memory>
#include "ResourceManagement/ResourceManager.h"
#include "Utilities/Profiling.h"


int main()
{
    Application* game = new Application();

    while (!game->GetWindow()->IsDone())
    {
        game->Update();
        game->Render();
        game->PostUpdate();
    }

    delete game;
}

