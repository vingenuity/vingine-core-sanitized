#include "Math/FloatVector3.hpp"
#include "NamedDataBundle.hpp"

//-----------------------------------------------------------------------------------------------
void UnitTestNamedDataBundle()
{
	NamedDataBundle testParameters;

	//These functions should pass
	testParameters.SetParameter( "name", "Vincent" );
	testParameters.SetParameter( "heightInInches", 76 );
	testParameters.SetParameter( "weightInPounds", 193.2f );
	testParameters.SetParameter( "normal", FloatVector3( 0.f, 0.f, 1.f ) );

	//Tricky! const char* vs. string
	std::string name;
	testParameters.GetParameterOrDie( "name", name );

	int height;
	testParameters.GetParameterOrDie( "heightInInches", height );

	//Tricky! unsigned int != int, this is handled as a special case
	unsigned int heightB;
	testParameters.GetParameterOrDie( "heightInInches", heightB ); 

	float weight;
	testParameters.GetParameterOrDie( "weightInPounds", weight );

	FloatVector3 zNormal;
	testParameters.GetParameterOrDie( "normal", zNormal );



	//These functions should fail
	//Fails because name isn't in parameters
	//testParameters.GetParameterOrDie( "weightInNewtons", weight );

	//Fails because type is incorrect
	//testParameters.GetParameterOrDie( "heightInInches", weight );
	//testParameters.GetParameterOrDie( "weightInPounds", heightB ); //Second test to verify special case handles errors

	//Fails because parameters are empty; also tests clear function
	testParameters.Clear();
	//testParameters.GetParameterOrDie( "name", name );
}
