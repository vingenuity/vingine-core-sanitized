#ifndef INCLUDED_NAMED_DATA_BUNDLE_HPP
#define INCLUDED_NAMED_DATA_BUNDLE_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include <map>
#include <typeinfo>

#include "AssertionError.hpp"
#include "DialogInterface.hpp"
#include "HashedString.hpp"

class ParameterIterator;

//-----------------------------------------------------------------------------------------------
//Unit Testing class to confirm working functionality
void UnitTestNamedDataBundle();

//-----------------------------------------------------------------------------------------------
class NamedDataBundle
{
#pragma region Contained Parameter Class Declarations
	//-------------------------------------------------------------------------------------------
	class NamedDataItem
	{
	public:
		virtual ~NamedDataItem() { }
	};
	
	//-------------------------------------------------------------------------------------------
	template< typename DataType >
	class NamedDataItemOfType : public NamedDataItem
	{
	public:
		NamedDataItemOfType( DataType newDataItem )
		{
			dataItem = newDataItem;
		}

		DataType dataItem;
	};

	typedef std::map< HashedString, NamedDataItem* > NameToDataMap;

	//-------------------------------------------------------------------------------------------
	class Iterator
	{
			friend class NamedDataBundle;
			Iterator( NameToDataMap::iterator& startIterator ) : m_actualIterator( startIterator ) { }

		public:
			void Advance() { ++m_actualIterator; }
			void Retreat() { --m_actualIterator; }

			void GetParameterName( HashedString& out_parameterName ) const { out_parameterName = m_actualIterator->first; }
			template< typename DataType >
			void GetParameterOrDie( DataType& out_typedParameter );

		private:
			NameToDataMap::iterator m_actualIterator;
	};
#pragma endregion

public:
	~NamedDataBundle();

	void Clear() { m_nameToDataMapping.clear(); }

	template< typename DataType >
	void GetParameterOrDie( const HashedString& propertyName, DataType& out_typedParameter );

	void GetParameterOrDie( const HashedString& propertyName, unsigned int& out_unsignedInteger )
	{
		int actualIntegerTypeStored;
		GetParameterOrDie( propertyName, actualIntegerTypeStored );
		out_unsignedInteger = static_cast< unsigned int >( actualIntegerTypeStored );
	}

	template< typename DataType >
	void SetParameter( const HashedString& propertyName, const DataType& typedParameter );

	void SetParameter( const HashedString& propertyName, const char* stringLiteralParameter )
	{
		SetParameter( propertyName, std::string( stringLiteralParameter ) );
	}


private:
	NameToDataMap m_nameToDataMapping;
};

//-----------------------------------------------------------------------------------------------
inline NamedDataBundle::~NamedDataBundle()
{
	for( NameToDataMap::iterator dataPair = m_nameToDataMapping.begin();
		 dataPair != m_nameToDataMapping.end();
		 ++dataPair )
	{
		delete dataPair->second;
	}
}


//-----------------------------------------------------------------------------------------------
template< typename DataType >
inline void NamedDataBundle::GetParameterOrDie( const HashedString& propertyName, DataType& out_typedParameter )
{
	static const std::string errorMessageBoxTitle = "Vingine Error: Bad NamedParameter";
	if( m_nameToDataMapping.empty() )
	{
		std::string errorMessage = " ERROR: Attempted to get parameter \"" + propertyName.GetString() + "\" from empty NamedParameters!\n";
		PopUpSystemDialog( errorMessageBoxTitle, errorMessage );

		ImmediatelyExit( -1 );
	}

	std::map< HashedString, NamedDataItem* >::iterator parameterIterator;
	parameterIterator = m_nameToDataMapping.find( propertyName );

	if( parameterIterator == m_nameToDataMapping.end() )
	{
		std::string errorMessage = " ERROR: Parameter \"" + propertyName.GetString() + "\" not found in NamedParameters\n";
		PopUpSystemDialog( errorMessageBoxTitle, errorMessage );

		ImmediatelyExit( -1 );
	}

	NamedDataItemOfType< DataType >* parameterAsType = dynamic_cast< NamedDataItemOfType< DataType >* >( parameterIterator->second );
	//If we successfully made the dynamic cast, output the object
	if( parameterAsType != nullptr )
	{
		out_typedParameter = parameterAsType->dataItem;
	}
	//if the type requested is a superclass of our class, then we can totally make that cast
	//else if( typeid( ( ParameterType ) parameterIterator->second ) == typeid( out_typedParameter ) )
	//{
	//	
	//}
	else
	{
		std::string errorMessage = " ERROR: Parameter \"" + propertyName.GetString() + "\" in NamedParameters is not of type [" + typeid( DataType ).name() + "]\n";
		PopUpSystemDialog( errorMessageBoxTitle, errorMessage );

		ImmediatelyExit( -1 );
	}
}

//-----------------------------------------------------------------------------------------------
template< typename DataType >
inline void NamedDataBundle::SetParameter( const HashedString& propertyName, const DataType& typedParameter )
{
	m_nameToDataMapping[ propertyName ] = new NamedDataItemOfType< DataType >( typedParameter );
}

#endif //INCLUDED_NAMED_DATA_BUNDLE_HPP
