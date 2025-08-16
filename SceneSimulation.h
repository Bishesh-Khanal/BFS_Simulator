#pragma once
#ifndef SCENESIMULATION_H
#define SCENESIMULATION_H

#include "Scene.h"


class SceneSimulation :public Scene
{
private:

	std::string						m_levelPath;
	const Vec2						m_gridSize		= { 64, 64 };
	Vec2							m_mPos;
	sf::CircleShape					m_mShape;
	std::shared_ptr<Entity>			m_start;
	std::shared_ptr<Entity>			m_end;
	bool							m_pointToggle	= false;
	bool							m_chooseNext	= true;
	std::vector<std::vector<int>>	m_adjacent;
	std::vector<char>				m_colors;
	std::vector<int>				m_BFS;
	std::vector<int>				m_path;
	std::queue<int>					m_BFSqueue;
	int								m_fringe;


	void init(const std::string&);

	void onEnd()							override;
	void update()							override;
	void sRender()							override;
	void sDoAction(const Action&)			override;
	const ActionMap& getActionMap() const	override;
	std::vector<int> simulate()             override;

	void sDebug();

	void loadLevel(const std::string&);
	void getAdjacentTiles();
	Vec2 windowToWorld(const Vec2&) const;
	Vec2 gridtoMidPixel(float, float, std::shared_ptr<Entity>);

public:
	SceneSimulation(std::shared_ptr<GameEngine>, const std::string&);
};

#endif