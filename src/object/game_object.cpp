#include "object/game_object.hpp"

GameObject::GameObject() :
	m_uid(),
	m_schedule_for_remove(false)
{}

GameObject::~GameObject()
{}

UID GameObject::get_uid() const { return m_uid; }
void GameObject::clear_uid() { m_uid = UID(); }
void GameObject::set_uid(const UID& uid) { m_uid = uid; }

std::string GameObject::class_name() { return "game-object"; }
std::string GameObject::get_class_name() const { return class_name(); };

void GameObject::remove_me() { m_schedule_for_remove = true; }
bool GameObject::is_valid() const { return !m_schedule_for_remove; }

bool GameObject::is_singleton() const { return false; }