#include "import.h"
#include "all.h"

InputGrabber::InputGrabber(Ogre::RenderWindow* rwin)
{
	OIS::ParamList pl;
	size_t window_handle = 0;
	std::ostringstream window_handle_str;
	rwin->getCustomAttribute("WINDOW", &window_handle);
	window_handle_str << window_handle;
	pl.insert(std::make_pair(std::string("WINDOW"), window_handle_str.str()));
#pragma message("fixit! crossplatform problem")
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
	m_imgr = OIS::InputManager::createInputSystem(pl);

	m_keyboarddev = static_cast<OIS::Keyboard*>(m_imgr->createInputObject(OIS::OISKeyboard, true));
	m_keyboarddev->setEventCallback(this);

	m_mousedev = static_cast<OIS::Mouse*>(m_imgr->createInputObject(OIS::OISMouse, true));
	m_mousedev->setEventCallback(this);
	unsigned int width, height, depth;
	int left, top;
	rwin->getMetrics(width, height, depth, left, top);
	const OIS::MouseState &mouse_state = m_mousedev->getMouseState();
	mouse_state.width  = width;
	mouse_state.height = height;
}

InputGrabber::~InputGrabber()
{
	BOOST_ASSERT(m_mouse_events.empty());
	BOOST_ASSERT(m_keyboard_events.empty());
	m_imgr->destroyInputObject(m_keyboarddev);
	m_imgr->destroyInputObject(m_mousedev);
	OIS::InputManager::destroyInputSystem(m_imgr);
}

void InputGrabber::capture()
{
	m_mousedev->capture();
	m_keyboarddev->capture();
}

bool InputGrabber::keyPressed(const OIS::KeyEvent &e)
{
	BOOST_FOREACH(auto evt, m_keyboard_events)
	{
		evt->key_pressed(e);
	}
    return true;
}

bool InputGrabber::keyReleased(const OIS::KeyEvent &e)
{
	BOOST_FOREACH(auto evt, m_keyboard_events)
	{
		evt->key_released(e);
	}
	return true;
}

bool InputGrabber::mouseMoved(const OIS::MouseEvent &e)
{
	BOOST_FOREACH(auto evt, m_mouse_events)
	{
		evt->mouse_moved(e);
	}
	return true;
}

bool InputGrabber::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	return true;
}

bool InputGrabber::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	BOOST_FOREACH(auto evt, m_mouse_events)
	{
		evt->mouse_clicked(e, id);
	}
	return true;
}

void InputGrabber::inject_listener(KeyboardInputEvent* event)
{
	m_keyboard_events.push_front(event);
}

void InputGrabber::drop_listener(KeyboardInputEvent* event)
{
	m_keyboard_events.remove_if([event](KeyboardInputEvent* i){
		return event==i;
	});
}

void InputGrabber::drop_listener(MouseInputEvent* event)
{
	m_mouse_events.remove_if([event](MouseInputEvent* i){
		return event==i;
	});
}

void InputGrabber::inject_listener(MouseInputEvent* event)
{
	m_mouse_events.push_front(event);
}

void InputGrabber::stop_input()
{
	m_keyboard_events.clear();
	m_mouse_events.clear();
}
