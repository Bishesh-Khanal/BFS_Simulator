#pragma once

#ifndef SCENE_H
#define SCENE_H

#include <queue>
#include <memory>

#include "Action.h"
#include "EntityManager.h"

class GameEngine;

typedef std::map<int, std::string> ActionMap;

class Scene
{
protected:
	friend class GameEngine;

	std::shared_ptr<GameEngine>		m_game;
	EntityManager					m_entities;
	ActionMap						m_actionMap;

	int								m_currentFrame			= 0;
	bool							m_paused				= false;
	bool							m_hasEnded				= false;

	virtual void update()									= 0;
	virtual void sRender()									= 0;
	virtual void sDoAction(const Action&)					= 0;
	virtual const ActionMap& getActionMap() const			= 0;
	virtual void onEnd()									= 0;
	virtual std::vector<std::shared_ptr<Entity>> simulate()	= 0;

	size_t width() const;
	size_t height() const;
	size_t currentFrame() const;

	void drawLine(const Vec2&, const Vec2&);
	void setPaused(bool);
	void doAction(const Action&);
	void registerAction(int, const std::string&);

public:
	Scene(std::shared_ptr<GameEngine>);
	virtual ~Scene() = default;
};

#endif