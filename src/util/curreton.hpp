#ifndef HEADER_ASAED_UTIL_CURRETON_HPP
#define HEADER_ASAED_UTIL_CURRETON_HPP

/** It's like singleton but without handling the object construction (only and global but better) */
template<class C>
class Currenton {
private:
	static Currenton<C>* s_current;

protected:
	Currenton() {
		s_current = this; 
	}

	virtual ~Currenton() {
		if (s_current == this) {
			s_current = nullptr;
		}
	}

public:
	static C* current() {
		return static_cast<C*>(s_current);
	}
};

template<class C>
Currenton<C>* Currenton<C>::s_current = nullptr; 

#endif