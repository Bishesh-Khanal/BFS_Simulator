#include "SceneSimulation.h"
#include "GameEngine.h"


#include <fstream>
#include <sstream>

void SceneSimulation::init(const std::string& levelPath)
{
	std::cout << "Simulation started" << std::endl;
	registerAction(sf::Keyboard::Escape, "QUIT");
	registerAction(sf::Keyboard::Space, "PAUSE");

	loadLevel(levelPath);
	getAdjacentTiles();
}

void SceneSimulation::simulate()
{

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
		std::string assetType, nameAsset;
		float xAsset, yAsset;

		if (lineStream >> assetType >> nameAsset >> xAsset >> yAsset)
		{
			auto entity = m_entities.addEntity(assetType);

			entity->addComponent<CBoundingBox>(m_gridSize, nameAsset);
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
	for (int i = 0; i < m_colors.size(); i++)
	{
		if (i == 0)
		{
			if (m_colors[i + 1] == m_colors[i])
			{
				m_adjacent.push_back({ i + 1 });
			}
			else
			{
				m_adjacent.push_back({ -1 });
			}
			if (m_colors[i + 10] == m_colors[i])
			{
				m_adjacent[i].push_back(i + 10);
			}
			else
			{
				m_adjacent[i].push_back(-1);
			}
		}
		else if (i == 9)
		{
			if (m_colors[i - 1] == m_colors[i])
			{
				m_adjacent.push_back({ i - 1 });
			}
			else
			{
				m_adjacent.push_back({ -1 });
			}
			if (m_colors[i + 10] == m_colors[i])
			{
				m_adjacent[i].push_back(i + 10);
			}
			else
			{
				m_adjacent[i].push_back(-1);
			}
		}
		else if (i == 90)
		{
			if (m_colors[i - 10] == m_colors[i])
			{
				m_adjacent.push_back({ i - 10 });
			}
			else
			{
				m_adjacent.push_back({ -1 });
			}
			if (m_colors[i + 1] == m_colors[i])
			{
				m_adjacent[i].push_back(i + 1);
			}
			else
			{
				m_adjacent[i].push_back(-1);
			}
		}
		else if (i == 99)
		{
			if (m_colors[i - 10] == m_colors[i])
			{
				m_adjacent.push_back({ i - 10 });
			}
			else
			{
				m_adjacent.push_back({ -1 });
			}
			if (m_colors[i - 1] == m_colors[i])
			{
				m_adjacent[i].push_back(i - 1);
			}
			else
			{
				m_adjacent[i].push_back(-1);
			}
		}
		else if (i > 0 && i < 9)
		{
			if (m_colors[i - 1] == m_colors[i])
			{
				m_adjacent.push_back({ i - 1 });
			}
			else
			{
				m_adjacent.push_back({ -1 });
			}
			if (m_colors[i + 1] == m_colors[i])
			{
				m_adjacent[i].push_back(i + 1);
			}
			else
			{
				m_adjacent[i].push_back(-1);
			}
			if (m_colors[i + 10] == m_colors[i])
			{
				m_adjacent[i].push_back(i + 10);
			}
			else
			{
				m_adjacent[i].push_back(-1);
			}
		}
		else if (i > 90 && i < 99)
		{
			if (m_colors[i - 10] == m_colors[i])
			{
				m_adjacent.push_back({ i - 10 });
			}
			else
			{
				m_adjacent.push_back({ -1 });
			}
			if (m_colors[i - 1] == m_colors[i])
			{
				m_adjacent[i].push_back(i - 1);
			}
			else
			{
				m_adjacent[i].push_back(-1);
			}
			if (m_colors[i + 1] == m_colors[i])
			{
				m_adjacent[i].push_back(i + 1);
			}
			else
			{
				m_adjacent[i].push_back(-1);
			}
		}
		else if (i % 10 == 0)
		{
			if (m_colors[i - 10] == m_colors[i])
			{
				m_adjacent.push_back({ i - 10 });
			}
			else
			{
				m_adjacent.push_back({ -1 });
			}
			if (m_colors[i + 1] == m_colors[i])
			{
				m_adjacent[i].push_back(i + 1);
			}
			else
			{
				m_adjacent[i].push_back(-1);
			}
			if (m_colors[i + 10] == m_colors[i])
			{
				m_adjacent[i].push_back(i + 10);
			}
			else
			{
				m_adjacent[i].push_back(-1);
			}
		}
		else if (i % 10 == 9)
		{
			if (m_colors[i - 10] == m_colors[i])
			{
				m_adjacent.push_back({ i - 10 });
			}
			else
			{
				m_adjacent.push_back({ -1 });
			}
			if (m_colors[i - 1] == m_colors[i])
			{
				m_adjacent[i].push_back(i - 1);
			}
			else
			{
				m_adjacent[i].push_back(-1);
			}
			if (m_colors[i + 10] == m_colors[i])
			{
				m_adjacent[i].push_back(i + 10);
			}
			else
			{
				m_adjacent[i].push_back(-1);
			}
		}
		else
		{
			if (m_colors[i - 10] == m_colors[i])
			{
				m_adjacent.push_back({ i - 10 });
			}
			else
			{
				m_adjacent.push_back({ -1 });
			}
			if (m_colors[i - 1] == m_colors[i])
			{
				m_adjacent[i].push_back(i - 1);
			}
			else
			{
				m_adjacent[i].push_back(-1);
			}
			if (m_colors[i + 1] == m_colors[i])
			{
				m_adjacent[i].push_back(i + 1);
			}
			else
			{
				m_adjacent[i].push_back(-1);
			}
			if (m_colors[i + 10] == m_colors[i])
			{
				m_adjacent[i].push_back(i + 10);
			}
			else
			{
				m_adjacent[i].push_back(-1);
			}
		}
	}
	int j = 0;
	for (auto& adjVec : m_adjacent)
	{
		std::cout << j << ": ";
		for (auto& adj : adjVec)
		{
			std::cout << adj << "  ";
		}
		j++;
		std::cout << std::endl;
	}
}

SceneSimulation::SceneSimulation(std::shared_ptr<GameEngine> game, const std::string& level_path)
	: Scene(std::move(game))
	, m_levelPath(level_path)
{
	SceneSimulation::init(m_levelPath);
}

void SceneSimulation::sDebug()
{
	for (auto& entity : m_entities.getEntities())
	{
		if (entity->hasComponent<CBoundingBox>())
		{
			entity->getComponent<CBoundingBox>().rectangle.setPosition(entity->getComponent<CTransform>().pos.x, entity->getComponent<CTransform>().pos.y);
			m_game->m_window.draw(entity->getComponent<CBoundingBox>().rectangle);
		}
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
		if (action.name() == "QUIT")
		{
			onEnd();
		}
		if (action.name() == "PAUSE")
		{
			m_paused = !m_paused;
		}

		if (action.name() == "LEFT_CLICK")
		{
			if(m_chooseNext)
			{
				Vec2 worldPos = windowToWorld(action.pos());
				for (auto& e : m_entities.getEntities())
				{
					if (IsInside(worldPos, e))
					{
						if (!m_pointToggle)
						{
							m_start = e;
							std::cout << "Start: " << m_start->id() << std::endl;
							m_start->getComponent<CBoundingBox>().rectangle.setFillColor(sf::Color::White);
						}
						if (m_pointToggle)
						{
							m_end = e;
							std::cout << "End: " << m_end->id() << std::endl;
							m_end->getComponent<CBoundingBox>().rectangle.setFillColor(sf::Color::White);
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
	/*
	if (!m_paused) {
		sMovement();
		sCollision();
		sAnimation();
		spawnBullet();
		sLifeSpan();
		sDragAndDrop();
	}
	*/
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

	m_mShape.setFillColor(sf::Color::Red);
	m_mShape.setRadius(4);
	m_mShape.setOrigin(2, 2);

	Vec2 worldPos = windowToWorld(m_mPos);
	m_mShape.setPosition(worldPos.x, worldPos.y);
	m_game->m_window.draw(m_mShape);

	m_game->m_window.display();
}