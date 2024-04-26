#include "weapon/projectile/projectile_set.hpp"

#include "util/reader_machine.hpp"
#include "util/reader_data.hpp"
#include "util/log.hpp"
#include "weapon/projectile/projectile.hpp"
#include "weapon/projectile/projectile_line.hpp"
#include "weapon/projectile/projectile_cycle.hpp"
#include "weapon/projectile/projectile_cycloid.hpp"
#include "weapon/projectile/projectile_big.hpp"
#include "weapon/projectile/projectile_boomerang.hpp"
#include "weapon/projectile/projectile_spawn.hpp"


ProjectileSet::ProjectileSet() :
	m_filename("images/weapon/projectile-set.json"),
	m_projectiles(1)
{
	parse();
}

ProjectileSet::~ProjectileSet() {
	m_projectiles.clear();
}

void ProjectileSet::add_projectile(uint32_t id, std::unique_ptr<Projectile> projectile) {
	if (id >= static_cast<uint32_t>(m_projectiles.size())) {
		m_projectiles.resize(id + 1);
	}

	if (m_projectiles[id]) {
		log_warning << "Projectile id " << id << " redefined\n";
	}
	else {
		m_projectiles[id] = std::move(projectile);
	}
}

const Projectile& ProjectileSet::get(const uint32_t id) const {
	if (id >= static_cast<uint32_t>(m_projectiles.size())) {
		log_warning << "Projectile id " << id << " is invalid\n";
		return *m_projectiles[0].get();
	}
	else {
		Projectile* projectile = m_projectiles[id].get();
		if (projectile) {
			return *projectile;
		}
		else {
			log_warning << "Projectile id " << id << " is invalid\n";
			return *m_projectiles[0].get();
		}
	}
}

void ProjectileSet::parse() {
	ReaderMachine reader(m_filename);
	
	for (size_t i = 0; i < reader.get_size(); ++ i) {
		parse_projectile(reader.get_data(i));
	}
}

void ProjectileSet::parse_projectile(const ReaderData* data) {
	uint32_t id;

	if (!data->get("id", id)) {
		throw std::runtime_error("Missing projectile id");
	}

	uint32_t type;
	if (!data->get("type", type)) {
		throw std::runtime_error("Missing projectile type");
	}
	std::unique_ptr<Projectile> projectile;
	switch (type) {
		case ProjectileType::LINE:
			projectile = ProjectileLine::from_file(data);
			break;
		case ProjectileType::CYCLE:
			projectile = ProjectileCycle::from_file(data);
			break;
		case ProjectileType::CYCLOID:
			projectile = ProjectileCycloid::from_file(data);
			break;
		case ProjectileType::BIGSPAWN:
			projectile = ProjectileBig::from_file(data);
			break;
		
		case ProjectileType::BOOMERANG:
			projectile = ProjectileBoomerang::from_file(data);
			break;

		case ProjectileType::SPAWN:
			projectile = ProjectileSpawn::from_file(data);
			break;
		
		default:
			break;
	}
	add_projectile(id, std::move(projectile));
}