#include "SceneSimulation.h"
#include "GameEngine.h"


#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

void SceneSimulation::init(const std::string& levelPath)
{
	std::cout << "Simulation started" << std::endl;

	loadLevel(levelPath);
}

void SceneSimulation::startBFS()
{
	m_visited.assign(m_adjacent.size(), false);
	m_parent.assign(m_adjacent.size(), nullptr);
	while (!m_BFSqueue.empty()) m_BFSqueue.pop();

	m_BFSqueue.push(m_start);
	m_visited[m_start->id()] = true;
	m_bfsActive = true;
	m_stepClock.restart();
}

bool SceneSimulation::bfsStep() {
	if (m_BFSqueue.empty()) { m_bfsActive = false; return true; }

	m_fringe = m_BFSqueue.front();
	m_BFSqueue.pop();

	for (auto& adj : m_adjacent[m_fringe->id()])
	{
		if (adj != nullptr)
		{
			if (adj->id() == m_end->id())
			{
				m_parent[adj->id()] = m_fringe; // <-- make sure the target knows its parent

				std::shared_ptr<Entity> curr = m_end;
				while (curr != nullptr) {
					m_path.push_back(curr);
					curr = m_parent[curr->id()];
				}
				std::reverse(m_path.begin(), m_path.end());

				std::cout << "Path: ";
				for (auto& node : m_path)
				{
					std::cout << node->id() << " ";
					node->getComponent<CBoundingBox>().rectangle.setFillColor(sf::Color::White);
				}
				std::cout << std::endl;
				m_bfsActive = false;
				return true;
			}
			if (!m_visited[adj->id()])
			{
				m_visited[adj->id()] = true;
				m_parent[adj->id()] = m_fringe;
				m_BFS.push_back(adj);
				m_BFSqueue.push(adj);
				adj->getComponent<CBoundingBox>().rectangle.setFillColor(sf::Color::Red);
			}
		}
	}
	return false;
}

Vec2 SceneSimulation::windowToWorld(const Vec2& windowPos) const
{
	auto& view = m_game->m_window.getView();

	float wx = view.getCenter().x - (m_game->m_window.getSize().x / 2);
	float wy = view.getCenter().y - (m_game->m_window.getSize().y / 2);

	return Vec2(windowPos.x + wx, windowPos.y + wy);
}

Vec2 SceneSimulation::gridtoMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
	if (!entity->hasComponent<CBoundingBox>()) {
		throw std::runtime_error("Entity missing required CAnimation component.");
	}

	auto& tileSize = entity->getComponent<CBoundingBox>().rectangle.getSize();

	float centerX = gridX + (tileSize.x / (2.f * m_gridSize.x));
	float centerY = (gridY + 1) - (tileSize.y / (2.f * m_gridSize.y));

	return Vec2(centerX * m_gridSize.x, centerY * m_gridSize.y);
}

void SceneSimulation::loadLevel(const std::string& level_path)
{
	std::cout << level_path << std::endl;
	std::fstream myFiles(level_path);
	if (!myFiles.is_open())
	{
		std::cerr << "Failed to open the file: " << level_path << std::endl;
		return;
	}

	std::string line;
	while (std::getline(myFiles, line))
	{
		if (line.empty())
		{
			continue;
		}

		std::istringstream lineStream(line);
		std::string nameAsset;
		float xAsset, yAsset;

		if (lineStream >> nameAsset >> xAsset >> yAsset)
		{
			auto entity = m_entities.addEntity(nameAsset);

			entity->addComponent<CBoundingBox>(m_gridSize);
			if (nameAsset == "Land")
			{
				entity->getComponent<CBoundingBox>().rectangle.setFillColor(sf::Color::Green);
			}
			else if (nameAsset == "Rock")
			{
				entity->getComponent<CBoundingBox>().rectangle.setFillColor(sf::Color(128,128,128));
			}
			else if (nameAsset == "Sea")
			{
				entity->getComponent<CBoundingBox>().rectangle.setFillColor(sf::Color::Blue);
			}
			entity->addComponent<CTransform>(gridtoMidPixel(xAsset, yAsset, entity));
			m_colors.push_back(nameAsset[0]);

		}
		else
		{
			std::cerr << "Malformed line" << std::endl;
		}
	}
}

void SceneSimulation::getAdjacentTiles()
{
	auto& entities = m_entities.getEntities();
	std::cout << "Number of entities: " << entities.size() << std::endl;
	std::cout << "Number of colors: " << m_colors.size() << std::endl;
	for (int i = 0; i < entities.size(); i++)
	{
		if (i == 0)
		{
			if (m_colors[i + 1] == m_colors[i])
			{
				m_adjacent.push_back({ entities[i + 1] });
			}
			else
			{
				m_adjacent.push_back({ nullptr });
			}
			if (m_colors[i + 64] == m_colors[i])
			{
				m_adjacent[i].push_back(entities[i + 64]);
			}
			else
			{
				m_adjacent[i].push_back(nullptr);
			}
		}
		else if (i == 63)
		{
			if (m_colors[i - 1] == m_colors[i])
			{
				m_adjacent.push_back({ entities[i - 1] });
			}
			else
			{
				m_adjacent.push_back({ nullptr });
			}
			if (m_colors[i + 64] == m_colors[i])
			{
				m_adjacent[i].push_back(entities[i + 64]);
			}
			else
			{
				m_adjacent[i].push_back(nullptr);
			}
		}
		else if (i == 4032)
		{
			if (m_colors[i - 64] == m_colors[i])
			{
				m_adjacent.push_back({ entities[i - 64] });
			}
			else
			{
				m_adjacent.push_back({ nullptr });
			}
			if (m_colors[i + 1] == m_colors[i])
			{
				m_adjacent[i].push_back(entities[i + 1]);
			}
			else
			{
				m_adjacent[i].push_back(nullptr);
			}
		}
		else if (i == 4095)
		{
			if (m_colors[i - 64] == m_colors[i])
			{
				m_adjacent.push_back({ entities[i - 64] });
			}
			else
			{
				m_adjacent.push_back({ nullptr });
			}
			if (m_colors[i - 1] == m_colors[i])
			{
				m_adjacent[i].push_back(entities[i - 1]);
			}
			else
			{
				m_adjacent[i].push_back(nullptr);
			}
		}
		else if (i > 0 && i < 63)
		{
			if (m_colors[i - 1] == m_colors[i])
			{
				m_adjacent.push_back({ entities[i - 1] });
			}
			else
			{
				m_adjacent.push_back({ nullptr });
			}
			if (m_colors[i + 1] == m_colors[i])
			{
				m_adjacent[i].push_back(entities[i + 1]);
			}
			else
			{
				m_adjacent[i].push_back(nullptr);
			}
			if (m_colors[i + 64] == m_colors[i])
			{
				m_adjacent[i].push_back(entities[i + 64]);
			}
			else
			{
				m_adjacent[i].push_back(nullptr);
			}
		}
		else if (i > 4032 && i < 4095)
		{
			if (m_colors[i - 64] == m_colors[i])
			{
				m_adjacent.push_back({ entities[i - 64] });
			}
			else
			{
				m_adjacent.push_back({ nullptr });
			}
			if (m_colors[i - 1] == m_colors[i])
			{
				m_adjacent[i].push_back(entities[i - 1]);
			}
			else
			{
				m_adjacent[i].push_back(nullptr);
			}
			if (m_colors[i + 1] == m_colors[i])
			{
				m_adjacent[i].push_back(entities[i + 1]);
			}
			else
			{
				m_adjacent[i].push_back(nullptr);
			}
		}
		else if (i % 64 == 0)
		{
			if (m_colors[i - 64] == m_colors[i])
			{
				m_adjacent.push_back({ entities[i - 64] });
			}
			else
			{
				m_adjacent.push_back({ nullptr });
			}
			if (m_colors[i + 1] == m_colors[i])
			{
				m_adjacent[i].push_back(entities[i + 1]);
			}
			else
			{
				m_adjacent[i].push_back(nullptr);
			}
			if (m_colors[i + 64] == m_colors[i])
			{
				m_adjacent[i].push_back(entities[i + 64]);
			}
			else
			{
				m_adjacent[i].push_back(nullptr);
			}
		}
		else if (i % 64 == 63)
		{
			if (m_colors[i - 64] == m_colors[i])
			{
				m_adjacent.push_back({ entities[i - 64] });
			}
			else
			{
				m_adjacent.push_back({ nullptr });
			}
			if (m_colors[i - 1] == m_colors[i])
			{
				m_adjacent[i].push_back(entities[i - 1]);
			}
			else
			{
				m_adjacent[i].push_back(nullptr);
			}
			if (m_colors[i + 64] == m_colors[i])
			{
				m_adjacent[i].push_back(entities[i + 64]);
			}
			else
			{
				m_adjacent[i].push_back(nullptr);
			}
		}
		else
		{
			if (m_colors[i - 64] == m_colors[i])
			{
				m_adjacent.push_back({ entities[i - 64] });
			}
			else
			{
				m_adjacent.push_back({ nullptr });
			}
			if (m_colors[i - 1] == m_colors[i])
			{
				m_adjacent[i].push_back(entities[i - 1]);
			}
			else
			{
				m_adjacent[i].push_back(nullptr);
			}
			if (m_colors[i + 1] == m_colors[i])
			{
				m_adjacent[i].push_back(entities[i + 1]);
			}
			else
			{
				m_adjacent[i].push_back(nullptr);
			}
			if (m_colors[i + 64] == m_colors[i])
			{
				m_adjacent[i].push_back(entities[i + 64]);
			}
			else
			{
				m_adjacent[i].push_back(nullptr);
			}
		}
	}
}

SceneSimulation::SceneSimulation(std::shared_ptr<GameEngine> game, const std::string& level_path)
	: Scene(std::move(game))
	, m_levelPath(level_path)
{
	init(m_levelPath);
}

void SceneSimulation::sDebug()
{
	for (auto& entity : m_entities.getEntities())
	{
		entity->getComponent<CBoundingBox>().rectangle.setPosition(entity->getComponent<CTransform>().pos.x, entity->getComponent<CTransform>().pos.y);
		m_game->m_window.draw(entity->getComponent<CBoundingBox>().rectangle);
	}
}

bool IsInside(Vec2 pos, std::shared_ptr<Entity> e)
{
	auto ePos = e->getComponent<CTransform>().pos;
	auto size = e->getComponent<CBoundingBox>().rectangle.getSize();

	float dx = fabs(pos.x - ePos.x);
	float dy = fabs(pos.y - ePos.y);

	return (dx <= size.x / 2) && (dy <= size.y / 2);
}

void SceneSimulation::sDoAction(const Action& action)
{
	if (action.type() == "START")
	{
		if (action.name() == "LEFT_CLICK")
		{
			if (m_chooseNext)
			{
				Vec2 worldPos = windowToWorld(action.pos());
				for (auto& e : m_entities.getEntities())
				{
					if (IsInside(worldPos, e))
					{
						if (!m_pointToggle)
						{
							for (auto& entity : m_entities.getEntities("Land"))
							{
								entity->getComponent<CBoundingBox>().rectangle.setFillColor(sf::Color::Green);
							}
							for (auto& entity : m_entities.getEntities("Rock"))
							{
								entity->getComponent<CBoundingBox>().rectangle.setFillColor(sf::Color(128,128,128));
							}
							for (auto& entity : m_entities.getEntities("Sea"))
							{
								entity->getComponent<CBoundingBox>().rectangle.setFillColor(sf::Color::Blue);
							}
							m_path.clear();
							m_start = e;
							std::cout << "Start: " << m_start->id() << std::endl;
							m_start->getComponent<CBoundingBox>().rectangle.setFillColor(sf::Color::White);

						}
						if (m_pointToggle)
						{
							if(e->tag() == m_start->tag())
							{
								m_end = e;
								std::cout << "End: " << m_end->id() << std::endl;
								m_end->getComponent<CBoundingBox>().rectangle.setFillColor(sf::Color::White);
								m_chooseNext = !m_chooseNext;
								startBFS();
							}
							else
							{
								std::cout << "Cannot traverse from one type of grid to another." << std::endl;
							}
						}
						m_pointToggle = !m_pointToggle;
					}
				}
			}
		}

		if (action.name() == "MOUSE_MOVE")
		{
			m_mPos = action.pos();
			m_mShape.setPosition(m_mPos.x, m_mPos.y);
		}
	}

	if (action.type() == "END")
	{
		
	}
}

const ActionMap& SceneSimulation::getActionMap() const
{
	return m_actionMap;
}


void SceneSimulation::update() {
	m_entities.update();
	if (m_firstTime)
	{
		getAdjacentTiles();
		m_firstTime = false;
	}
	
	if (!m_chooseNext) {
		
		if (m_bfsActive && m_stepClock.getElapsedTime().asMilliseconds() >= 0) {
			bfsStep();
			m_stepClock.restart();
		}

		if (!m_bfsActive && !m_chooseNext) {
			m_chooseNext = true;
		}
	}
	
}


void SceneSimulation::onEnd()
{
	for (auto& entity : m_entities.getEntities())
	{
		entity->destroy();
	}
	m_game->m_window.close();
}

void SceneSimulation::sRender()
{
	m_game->m_window.clear(sf::Color(225, 225, 225));
	sDebug();

	m_mShape.setFillColor(sf::Color::Yellow);
	m_mShape.setRadius(4);
	m_mShape.setOrigin(2, 2);

	Vec2 worldPos = windowToWorld(m_mPos);
	m_mShape.setPosition(worldPos.x, worldPos.y);
	m_game->m_window.draw(m_mShape);

	m_game->m_window.display();
}