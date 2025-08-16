#pragma once
#include "Vec2.h"

#include "SFML_Stuff/include/SFML/Graphics.hpp"

class CTransform
{
public:
	Vec2 pos = { 0.0f, 0.0f };
	Vec2 prevPos = { 0.0f, 0.0f };
	Vec2 velocity = { 0.0f, 0.0f };
	Vec2 scale = { 1.0f, 1.0f };
	float rotation = 0.0f;
	float speed = 0.0f;
	float angle = 1.0f;
	bool has = false;

	CTransform()
	{
	}

	CTransform(const Vec2& p, bool h = true)
		: pos(p)
		, has(h)
	{
	}

	CTransform(const Vec2& p, const Vec2& v, float r = 0, float s = 0, float a = 0, bool h = true)
		: pos(p)
		, velocity(v)
		, rotation(r)
		, speed(s)
		, angle(a)
		, has(h)
	{
	}
};

class CBoundingBox
{
public:
	bool has = false;

	Vec2 boundingbox = { 0.0f, 0.0f };
	Vec2 halfSize = { 0.0f, 0.0f };
	sf::RectangleShape rectangle;

	CBoundingBox()
	{
	}

	CBoundingBox(const Vec2& bbox, const std::string& nameAsset, bool h = true)
		: boundingbox(bbox)
		, has(h)
		, halfSize(bbox / 2)
	{
		rectangle.setSize(sf::Vector2f(bbox.x, bbox.y));
		if (nameAsset == "Grass")
		{
			rectangle.setFillColor(sf::Color::Green);
		}
		else if (nameAsset == "Block")
		{
			rectangle.setFillColor(sf::Color::Black);
		}
		rectangle.setOutlineColor(sf::Color::White);
		rectangle.setOutlineThickness(1.0f);
		rectangle.setOrigin(bbox.x / 2, bbox.y / 2);
	}
};

class CInput
{
public:
	bool has = false;

	bool pointToggle = false;

	CInput()
	{
	}
};