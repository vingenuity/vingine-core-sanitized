#pragma once
#ifndef INCLUDED_HASHED_STRING_HPP
#define INCLUDED_HASHED_STRING_HPP

//-----------------------------------------------------------------------------------------------
#include <string>

//-----------------------------------------------------------------------------------------------
class HashedString
{
	typedef unsigned int Hash;

public:
	//Construction
	HashedString();
	HashedString( const std::string& string );
	HashedString( const char* cString );

	//Getters
	const std::string& GetString() const { return m_string; }

	//Operators
	bool operator==(const HashedString& rhs) const;
	bool operator!=(const HashedString& rhs) const;
	bool operator<(const HashedString& rhs) const;
	operator int() const { return m_hash; }

private:
	Hash m_hash;
	std::string m_string;
};
#endif //INCLUDED_HASHED_STRING_HPP
