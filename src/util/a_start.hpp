#ifndef HEADER_ASAED_UTIL_A_START_HPP
#define HEADER_ASAED_UTIL_A_START_HPP

#include <numeric>
#include <limits>
#include <tuple>
#include <vector>

#include "math/vector.hpp"
#include "object/tile_map.hpp"

class AStart {
	struct State {
	public:
		int u, v;
		float f;

	public:
		State() :
			u(0),
			v(0),
			f(std::numeric_limits<float>::max())
		{}
		
		State(int u_, int v_, float f_) :
			u(u_),
			v(v_),
			f(f_)
		{}

	public:
		bool operator<(const State& other) const {
			if (this->f != other.f) return this->f < other.f;
			if (this->u != other.u) return this->u < other.u;
			return this->v < other.v;
		}
	};

	struct Heuristic {
	public:
		float f, g, h;

	public:
		Heuristic() :
			f(std::numeric_limits<float>::max()),
			g(std::numeric_limits<float>::max()),
			h(std::numeric_limits<float>::max())
		{}
		Heuristic(float f_, float g_, float h_) :
			f(f_),
			g(g_),
			h(h_)
		{}
	};

private:
	const TileMap *m_tile_map;

	int m_width, m_height;

	std::vector<std::vector<Heuristic>> m_ability;
	std::vector<std::vector<uint8_t>> m_close;

	using Index = std::pair<int, int>;
	std::vector<std::vector<Index>> m_trace;
	Index m_start, m_end;
	

public:
	AStart(const TileMap *tile_map, Vector start, Vector end);

private:
	AStart(const AStart&) = delete;
	AStart& operator=(const AStart&) = delete;

public:
	bool isValid(int u, int v);
	bool isFreeBlock(int u, int v);
	bool isDestination(int u, int v);

	float calculate_heuristic(int u, int v);

	std::vector<Vector> smart_path();
	void runAStart();
};


#endif