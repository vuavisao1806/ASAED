#ifndef HEADER_ASAED_WEAPON_PROJECTILE_PROJECTILE_SET_HPP
#define HEADER_ASAED_WEAPON_PROJECTILE_PROJECTILE_SET_HPP

#include "util/currenton.hpp"

#include <memory>
#include <vector>
#include <string>

class ReaderData;
class Projectile;

enum ProjectileType {
	LINE = 1,
	CYCLE,
	CYCLOID,
	BOOMERANG
};

class ProjectileSet final : public Currenton<ProjectileSet> {
	std::string m_filename;
	std::vector<std::unique_ptr<Projectile>> m_projectiles;

public:
	ProjectileSet();
	~ProjectileSet() override;

private:
	ProjectileSet(const ProjectileSet&) = delete;
	ProjectileSet& operator=(const ProjectileSet&) = delete;
	
public:
	void add_projectile(uint32_t id, std::unique_ptr<Projectile> weapon);
	const Projectile& get(const uint32_t id) const;

public:
	void parse();
	void parse_projectile(const ReaderData* data);
};

#endif