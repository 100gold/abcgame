#pragma once

class InputEvent
{
};

class InputGrabber : public EventHiveOwner<InputEvent>, public OIS::KeyListener, public OIS::MouseListener
{
	OIS::InputManager* m_imgr;
	OIS::Mouse* m_mousedev; 
	OIS::Keyboard* m_keyboarddev;

	bool keyPressed(const OIS::KeyEvent &e);
	bool keyReleased(const OIS::KeyEvent &e);

	bool mouseMoved(const OIS::MouseEvent &e);
	bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
public:
	InputGrabber(Ogre::RenderWindow* rwin);
	~InputGrabber();

	void capture();
};