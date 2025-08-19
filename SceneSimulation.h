#pragma once
#ifndef SCENESIMULATION_H
#define SCENESIMULATION_H

#include "Scene.h"


class SceneSimulation :public Scene
{
private:

	std::string											m_levelPath;
	const Vec2											m_gridSize		= { 10, 10 };
	Vec2												m_mPos;
	sf::CircleShape										m_mShape;
	std::shared_ptr<Entity>								m_start;
	std::shared_ptr<Entity>								m_end;
	bool												m_pointToggle	= false;
	bool												m_chooseNext	= true;
	bool												m_firstTime		= true;
	std::vector<std::vector<std::shared_ptr<Entity>>>	m_adjacent;
	std::vector<char>									m_colors;
	std::vector<std::shared_ptr<Entity>>				m_BFS;
	std::vector<std::shared_ptr<Entity>>				m_path;
	std::queue<std::shared_ptr<Entity>>					m_BFSqueue;
	std::vector<bool>									m_visited;
	std::vector<std::shared_ptr<Entity>>				m_parent;
	std::shared_ptr<Entity>								m_fringe;
	bool												m_bfsActive		= false;
	sf::Clock											m_stepClock;


	void init(const std::string&);

	void onEnd()												override;
	void update()												override;
	void sRender()												override;
	void sDoAction(const Action&)								override;
	const ActionMap& getActionMap() const						override;

	void sDebug();

	void startBFS();
	bool bfsStep();

	void loadLevel(const std::string&);
	void getAdjacentTiles();
	Vec2 windowToWorld(const Vec2&) const;
	Vec2 gridtoMidPixel(float, float, std::shared_ptr<Entity>);

public:
	SceneSimulation(std::shared_ptr<GameEngine>, const std::string&);
};

#endif