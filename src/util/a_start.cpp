#include "util/a_start.hpp"

#include <algorithm>
#include <assert.h>
#include <set>

#include "asaed/constants.hpp"
#include "object/tile.hpp"

AStart::AStart(const TileMap *tile_map, Vector start, Vector end) :
	m_tile_map(tile_map)
{
	m_width = m_tile_map->get_width();
	m_height = m_tile_map->get_height();

	m_start = m_tile_map->get_tile_fake_position(start);
	m_end = m_tile_map->get_tile_fake_position(end);

	m_ability.resize(m_width, std::vector<Heuristic>(m_height, Heuristic()));
	m_trace.resize(m_width, std::vector<Index>(m_height, Index(-1, -1)));
	m_close.resize(m_width, std::vector<uint8_t>(m_height, 0));
}

bool AStart::isValid(int u, int v) { return (0 <= u && u < m_width && 0 <= v && v < m_height); }

bool AStart::isFreeBlock(int u, int v) {
	const Tile& tile = m_tile_map->get_tile(u, v);
	return !tile.is_solid();
}

bool AStart::isDestination(int u, int v) { return (u == m_end.first && v == m_end.second); }

float AStart::calculate_heuristic(int u, int v) {
	return math::distance(Vector(static_cast<float>(u), static_cast<float>(v)), 
	                      Vector(static_cast<float>(m_end.first), static_cast<float>(m_end.second)));
}

std::vector<Vector> AStart::smart_path() {
	runAStart();
	std::vector<Vector> m_path;

	while (m_end.first != -1 || m_end.second != -1) {
		int u = m_end.first, v = m_end.second;
		// middle position :>
		m_path.push_back(m_tile_map->get_tile_position(u, v) + Vector(BLOCK_SIZE, BLOCK_SIZE) / 2.0f);
		m_end = m_trace[u][v];
	}
	assert(m_end.first == -1);
	assert(m_end.second == -1);
	return m_path;
}

void AStart::runAStart() {
	std::set<State> st;

	m_ability[m_start.first][m_start.second].f = 0.0f;
	m_ability[m_start.first][m_start.second].g = 0.0f;
	m_ability[m_start.first][m_start.second].h = 0.0f;
	st.insert(State(m_start.first, m_start.second, 0.0f));

	while (!st.empty()) {
		State node = *st.begin();
		st.erase(st.begin());

		int u = node.u, v = node.v;
		if (m_ability[u][v].f != node.f) {
			continue;
		}

		m_close[u][v] = true;
		for (int dx = -1; dx <= 1; ++ dx) {
			for (int dy = -1; dy <= 1; ++ dy) {
				if (dx == 0 && dy == 0) continue;
				if (dx * dy != 0) continue;
				int x = u + dx, y = v + dy;
				if (!isValid(x, y) || !isFreeBlock(x, y)) {
					continue;
				}

				if (isDestination(x, y)) {
					m_trace[x][y] = Index(u, v);
					break;
				}

				float g_new = m_ability[u][v].g + 1.0f;
				float h_new = calculate_heuristic(x, y);
				float f_new = g_new + h_new;
				if (!m_close[x][y] && m_ability[x][y].f > f_new) {
					m_ability[x][y] = Heuristic(f_new, g_new, h_new);
					m_trace[x][y] = Index(u, v);
					st.insert(State(x, y, f_new));
				}
			}
		}
	}
}