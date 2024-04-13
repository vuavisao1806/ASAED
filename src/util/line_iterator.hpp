#ifndef HEADER_ASAED_UTIL_LINE_ITERATOR_HPP
#define HEADER_ASAED_UTIL_LINE_ITERATOR_HPP

#include <string>

class LineIterator final {
private:
	std::string::const_iterator first;
	std::string::const_iterator last;
	std::string::const_iterator line_end;
	bool started;

public:
	LineIterator(const std::string& text);
	LineIterator(std::string::const_iterator first_, std::string::const_iterator last_);

public:
	/** return false when no existing characters in the string are left, true otherwise */
	bool next();

	/** return current line */
	std::string get();
};

#endif