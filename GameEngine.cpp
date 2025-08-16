#include "GameEngine.h"
#include "Scene.h"
#include "SceneSimulation.h"

GameEngine::GameEngine()
{
    std::cout << "Created the game engine" << std::endl;
    init();
}

void GameEngine::init()
{
    m_window.create(sf::VideoMode(m_widthW, m_heightW), "Simulator", sf::Style::Close);
    m_window.setFramerateLimit(60);

    std::cout << "Game engine initialized successfully." << std::endl;
}


void GameEngine::run(std::shared_ptr<GameEngine> game)
{
    std::cout << "Running the game engine" << std::endl;
    changeScene("SIMULATION", std::make_shared<SceneSimulation>(game, "bin/levels/rock_sea_land.txt"));

    while (m_running)
    {
        update();
        sUserInput();
        currentScene()->sRender();

        currentScene()->m_currentFrame++;
    }
}

const std::shared_ptr<Scene>& GameEngine::currentScene()
{
    return m_scenes[m_currentScene];
}

void GameEngine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene)
{
    m_scenes[sceneName] = scene;
    m_currentScene = sceneName;
    //currentScene();
}

void GameEngine::sUserInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
            m_running = false;
        }

        auto mousePos = sf::Mouse::getPosition(m_window);
        Vec2 mpos(mousePos.x, mousePos.y);

        if (event.type == sf::Event::MouseButtonPressed)
        {
            switch (event.mouseButton.button)
            {
            case sf::Mouse::Left: { currentScene()->doAction(Action("LEFT_CLICK", "START", mpos)); break; }
            case sf::Mouse::Middle: { currentScene()->doAction(Action("MIDDLE_CLICK", "START", mpos)); break; }
            case sf::Mouse::Right: { currentScene()->doAction(Action("RIGHT_CLICK", "START", mpos)); break; }
            default: break;
            }
        }

        if (event.type == sf::Event::MouseButtonReleased)
        {
            switch (event.mouseButton.button)
            {
            case sf::Mouse::Left: { currentScene()->doAction(Action("LEFT_CLICK", "END", mpos)); break; }
            case sf::Mouse::Middle: { currentScene()->doAction(Action("MIDDLE_CLICK", "END", mpos)); break; }
            case sf::Mouse::Right: { currentScene()->doAction(Action("RIGHT_CLICK", "END", mpos)); break; }
            default: break;
            }
        }

        if (event.type == sf::Event::MouseMoved)
        {
            currentScene()->doAction(Action("MOUSE_MOVE", "START", Vec2(event.mouseMove.x, event.mouseMove.y)));
        }
    }
}

void GameEngine::update()
{
    currentScene()->update();
}