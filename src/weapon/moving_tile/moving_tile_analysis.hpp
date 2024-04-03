#ifndef HEADER_ASAED_WEAPON_MOVING_TILE_MOVING_TILE_ANALYSIS_HPP
#define HEADER_ASAED_WEAPON_MOVING_TILE_MOVING_TILE_ANALYSIS_HPP

#include "weapon/moving_tile/moving_tile.hpp"

#include <string>

class ReaderData;

class MovingTileAnalysis : public MovingTile {
private:
	int m_wall_bounce_count; // will appear soon

public:
	MovingTileAnalysis(const std::string& filename);

private:
	MovingTileAnalysis(const MovingTileAnalysis&) = delete;
	MovingTileAnalysis& operator=(const MovingTileAnalysis&) = delete;

public:
	static std::unique_ptr<MovingTile> from_file(const ReaderData* data);

public:
	virtual void collision_solid(const CollisionHit& hit) override;
	virtual HitResponse collision(CollisionObject& other, const CollisionHit& hit) override;
	virtual void collision_tile(uint32_t tile_attributes) override;
	
	static std::string class_name();
	virtual std::string get_class_name() const override;

	virtual std::unique_ptr<MovingTile> clone(const Vector& pos, uint32_t hurt_attributes, float angle) const override;
};

#endif