#include "HashedString.hpp"

#include "HashFunctions.hpp"



#pragma region Construction
//-----------------------------------------------------------------------------------------------
HashedString::HashedString()
	: m_hash( 0 )
	, m_string( "" )
{
}

//-----------------------------------------------------------------------------------------------
HashedString::HashedString( const std::string& string )
	: m_hash( 0 )
	, m_string( string )
{
	m_hash = HashWithDJB2( m_string.c_str() );
}

//-----------------------------------------------------------------------------------------------
HashedString::HashedString( const char* cString )
	: m_hash( 0 )
	, m_string( cString )
{
	m_hash = HashWithDJB2( cString );
}
#pragma endregion



#pragma region Operators
//-----------------------------------------------------------------------------------------------
bool HashedString::operator==(const HashedString& rhs) const
{
	if( this->m_hash == rhs.m_hash )
	{
		#ifdef CHECK_FOR_HASH_COLLISIONS
		if( this->m_string.compare( rhs.m_string ) != 0 )
		{
			RECOVERABLE_ERROR( "String Hashing Error", "A hash collision was found in HashedString." );
			return false;
		}
		#endif

		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------------------
bool HashedString::operator!=(const HashedString& rhs) const
{
	return !( *this == rhs );
}

//-----------------------------------------------------------------------------------------------
bool HashedString::operator<(const HashedString& rhs) const
{
	if( this->m_hash < rhs.m_hash )
	{
		return true;
	}
	return false;
}
#pragma endregion
