#pragma once

class SelectAction
{
public:
	virtual void select(ViewableObject* obj, const Ogre::Vector3& point)
	{
	}
};
typedef boost::shared_ptr<SelectAction> SelectActionPtr;

class MouseTracer : public MouseInputEvent
{
	Ogre::RaySceneQuery* m_ray_query;
	Ogre::Camera* m_camera;
	SelectActionPtr m_select_action;

	std::hash_map<Ogre::String, ViewableObject*> m_registred_events;

	void mouse_moved(const OIS::MouseEvent &e);
	void mouse_clicked(const OIS::MouseEvent &e, OIS::MouseButtonID id);

public:
	MouseTracer(Ogre::SceneManager* sc_mgr, Ogre::Camera* camera);
	~MouseTracer();

	void reg_obj(Ogre::String objname, ViewableObject* obj);
	void unreg_obj(Ogre::String objname);

	void set_select_action(SelectActionPtr act);
};