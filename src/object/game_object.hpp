#ifndef HEADER_ASAED_OBJECT_GAME_OBJECT_HPP
#define HEADER_ASAED_OBJECT_GAME_OBJECT_HPP

#include "util/uid.hpp"

#include <string>

class DrawingContext;

class GameObject {
	friend class GameObjectManager;
private:
	// unique id
	UID m_uid;

	// flag to remove the object at the end of the frame
	bool m_schedule_for_remove;

public:
	GameObject();
	virtual ~GameObject();

private:
	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;

public:
	virtual void update(float dt_sec) = 0;
	virtual void draw(DrawingContext& drawing_context) = 0;

	UID get_uid() const;
	void clear_uid();

	static std::string class_name();
	virtual std::string get_class_name() const;

	/** schedule this object to be removed at the end of the frame */
	virtual void remove_me();
	bool is_valid() const;
	
	/** If true only a single object of this type is allowed in a given GameObjectManager */
	virtual bool is_singleton() const;

private:
	void set_uid(const UID& uid);
};

#endif