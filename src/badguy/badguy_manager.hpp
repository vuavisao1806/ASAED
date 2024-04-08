#include <map>
#include <memory>
#include <string>

#include "badguy/badguy.hpp"
#include "util/currenton.hpp"

class BadGuy;
class ReaderData;

/** Class to hold all types of badguy to help clone when needed */

class BadGuyManager final : public Currenton<BadGuyManager> {
private:
	std::string m_filename;
	std::map<std::string, std::unique_ptr<BadGuy>> m_badguys;

public:
	BadGuyManager();
	~BadGuyManager() override;

private:
	BadGuyManager(const BadGuyManager&) = delete;
	BadGuyManager& operator=(const BadGuyManager&) = delete;

private:
	void parse_badguy(const ReaderData* data);

public:
	const BadGuy& get(const std::string& filename) const;
};