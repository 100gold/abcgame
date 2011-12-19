#pragma once

class KeyboardInputEvent
{
public:
	virtual void key_released(const OIS::KeyEvent &e) = 0;
	virtual void key_pressed(const OIS::KeyEvent &e) = 0;
};

class KeyboardSimpleInputEvent : public KeyboardInputEvent
{
	void key_pressed(const OIS::KeyEvent &e)
	{
	}
};

template<int key>
class KeyInputEvent : public KeyboardSimpleInputEvent
{
	virtual void activate() = 0;
	void key_released(const OIS::KeyEvent &e)
	{
		if (e.key == key)
		{
			activate();
		}
	}
};

class MouseInputEvent
{
public:
	virtual void mouse_moved(const OIS::MouseEvent &e) = 0;
	virtual void mouse_clicked(const OIS::MouseEvent &e, OIS::MouseButtonID id) = 0;
};

class InputGrabber : public OIS::KeyListener, public OIS::MouseListener
{
	OIS::InputManager* m_imgr;
	OIS::Mouse* m_mousedev; 
	OIS::Keyboard* m_keyboarddev;

	std::list<KeyboardInputEvent*> m_keyboard_events;
	std::list<MouseInputEvent*> m_mouse_events;

	bool keyPressed(const OIS::KeyEvent &e);
	bool keyReleased(const OIS::KeyEvent &e);

	bool mouseMoved(const OIS::MouseEvent &e);
	bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
public:
	InputGrabber(Ogre::RenderWindow* rwin);
	~InputGrabber();

	void capture();
	void stop_input();
	void inject_listener(KeyboardInputEvent* event);
	void inject_listener(MouseInputEvent* event);
	void drop_listener(KeyboardInputEvent* event);
	void drop_listener(MouseInputEvent* event);
};