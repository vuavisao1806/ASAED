#ifndef HEADER_ASAED_SURFACE_CREATOR_HPP
#define HEADER_ASAED_SURFACE_CREATOR_HPP

#include <SDL.h>
#include <string>

/** This class wraps SDL_Surface* to do things */

class SDLSurfacePtr final {
private:
	SDL_Surface* m_surface;

public:
	SDLSurfacePtr() :
		m_surface(nullptr)
	{}

	~SDLSurfacePtr()
	{
		SDL_FreeSurface(m_surface);
	}

	explicit SDLSurfacePtr(SDL_Surface* surface) :
		m_surface(surface)
	{}

	SDLSurfacePtr(SDLSurfacePtr&& other) noexcept :
		m_surface(other.m_surface)
	{}

	SDLSurfacePtr& operator=(SDLSurfacePtr&& other) noexcept {
		if(this != &other) {
			m_surface = other.m_surface;
			other.m_surface = nullptr;
		}
		return (*this);
	}

private:
	SDLSurfacePtr(const SDLSurfacePtr&) = delete;
	SDLSurfacePtr& operator=(const SDLSurfacePtr&) = delete;

public:

	SDL_Surface& operator*() {
		return *m_surface; 
	}
	
	const SDL_Surface& operator*() const {
		return *m_surface; 
	}

	SDL_Surface* operator->() {
		return m_surface;
	}

	const SDL_Surface* operator->() const {
		return m_surface;
	}
	
	void reset(SDL_Surface* surface) {
		SDL_FreeSurface(m_surface);
		m_surface = surface;
	}

	SDL_Surface* get() {
		return m_surface;
	}

	operator bool() {
		return m_surface;
	}
};

/**
 * This class to create SDLSurfacePtr from image load
 * It isn't manager because it doesn't contain responsibility to save
*/

class SurfaceCreator final {
public:
	static SDLSurfacePtr from_file(const std::string& filename);
};

#endif