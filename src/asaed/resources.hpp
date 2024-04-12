#ifndef HEADER_ASAED_ASAED_RESOURCES_HPP
#define HEADER_ASAED_ASAED_RESOURCES_HPP

#include <memory>

class MouseCursor;

class Resources final {
public:
	static std::unique_ptr<MouseCursor> m_mouse_cursor;

public:
	Resources();
	~Resources();

public:
	void load();
	void unload();
};

#endif