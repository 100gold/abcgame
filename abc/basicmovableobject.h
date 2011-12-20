#pragma once

class BasicMovableObject : public ViewableObject
{
protected:
	MovementParameterGroup m_move_params;

public:
	BasicMovableObject(GameSectorPtr sector);

	void move(Ogre::Vector2& v);
	void abs_move(Ogre::Vector2& v);
};