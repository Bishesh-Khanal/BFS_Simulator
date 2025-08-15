#pragma once
#include <iostream>
#include <map>
#include "SFML_Stuff/include/SFML/Graphics.hpp"

class Scene;

typedef std::map<std::string, std::shared_ptr<Scene>> Scenes;

class GameEngine
{
private:
	friend class SceneSimulation;

	Scenes					m_scenes;
	sf::RenderWindow		m_window;
	std::string				m_currentScene;
	size_t					m_simulationSpeed = 1;
	bool					m_running = true;
	float					m_widthW = 640.0f;
	float					m_heightW = 640.0f;
	float					m_worldWidth = 640.0f;

	void init();
	const std::shared_ptr<Scene>& currentScene();
	void update();

	void changeScene(const std::string&, std::shared_ptr<Scene>);

	void sUserInput();

public:
	GameEngine();
	void run(std::shared_ptr<GameEngine>);
	void quit();
};