#include "util/line_iterator.hpp"

LineIterator::LineIterator(const std::string& text) :
	first(text.begin()),
	last(text.end()),
	line_end(text.begin()),
	started(false)
{}

LineIterator::LineIterator(std::string::const_iterator first_, std::string::const_iterator last_) :
	first(first_),
	last(last_),
	line_end(first_),
	started(false)
{}

bool LineIterator::next() {
	if (line_end == last || (std::next(line_end) == last && *first == '\n')) {
		// case1: end paragraph
		// case2: next is the end but last is '\n'
		return false;
	}
	if (started) {
		first = line_end + 1;
	}

	do {
		++ line_end;
	} while (line_end != last && *line_end != '\n');
	started = true;
	
	return true;
}

std::string LineIterator::get() { return std::string(first, line_end); }