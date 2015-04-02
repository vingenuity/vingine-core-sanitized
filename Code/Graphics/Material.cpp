#include "Material.hpp"

#include "../Graphics/RendererInterface.hpp"
#include "../AssertionError.hpp"


//-----------------------------------------------------------------------------------------------
void Material::BindMatrixToShader( MatrixReturningFunction matrixUpdater, const char* shaderVariableName )
{
	FATAL_ASSERTION( pipeline != nullptr, "Material Error", "Cannot bind shader variables to a material without a shader pipeline." );

	CachingShaderLoader* shaderLoader = RendererInterface::GetShaderLoader();
	ShaderVariable* shaderVariable = shaderLoader->GetUniformVariable( pipeline, shaderVariableName );

	matrixBindings.push_back( ShaderBinding< Float4x4Matrix >( shaderVariable, matrixUpdater ) );
}

//-----------------------------------------------------------------------------------------------
int Material::GetValidAttributeIDFromNameOrDie( const std::string& /*attributeName*/ ) const
{
	int attributeID = 0;//RendererInterface::GetAttributeLocation( program, attributeName );
	assert( attributeID != -1 );
	return attributeID;
}

//-----------------------------------------------------------------------------------------------
int Material::GetValidUniformIDFromNameOrDie( const std::string& /*uniformName*/ ) const
{
	int uniformID = 0;//RendererInterface::GetUniformVariableLocation( program, uniformName );
	assert( uniformID != -1 );
	return uniformID;
}

//-----------------------------------------------------------------------------------------------
void Material::SetFloatUniform( const std::string& /*uniformName*/, float /*value*/ )
{
	//int uniformID = GetValidUniformIDFromNameOrDie( uniformName );

	//RendererInterface::SetUniformVariable( uniformID, value );
}

//-----------------------------------------------------------------------------------------------
void Material::SetFloatUniform( const std::string& /*uniformName*/, const FloatVector3& /*vector*/ )
{
	//int uniformID = GetValidUniformIDFromNameOrDie( uniformName );

	//RendererInterface::SetUniformVariable( uniformID, vector );
}

//-----------------------------------------------------------------------------------------------
void Material::SetFloatUniform( const std::string& /*uniformName*/, const FloatVector4& /*vector*/ )
{
// 	int uniformID = GetValidUniformIDFromNameOrDie( uniformName );
// 
// 	RendererInterface::SetUniformVariable( uniformID, vector );
}

//-----------------------------------------------------------------------------------------------
void Material::SetIntegerUniform( const std::string& /*uniformName*/, int /*value*/ )
{
// 	int uniformID = GetValidUniformIDFromNameOrDie( uniformName );
// 
// 	RendererInterface::SetUniformVariable( uniformID, value );
}

//-----------------------------------------------------------------------------------------------
void Material::SetLightUniform( const std::string& /*uniformName*/, const Light& /*light*/ )
{
// 	int uniformID = GetValidUniformIDFromNameOrDie( uniformName );
// 
// 	uniformID = program->GetUniformVariableIDFromName( uniformName + ".position" );
// 	RendererInterface::SetUniformVariable( uniformID, light.m_position );
// 
// 	uniformID = program->GetUniformVariableIDFromName( uniformName + ".directionInWorld" );
// 	RendererInterface::SetUniformVariable( uniformID, light.m_direction );
// 
// 	uniformID = program->GetUniformVariableIDFromName( uniformName + ".colorAndBrightness" );
// 	RendererInterface::SetUniformVariable( uniformID, light.m_colorAndBrightness );
// 
// 	uniformID = program->GetUniformVariableIDFromName( uniformName + ".outerRadiusOfZeroIntensity" );
// 	RendererInterface::SetUniformVariable( uniformID, light.m_outerRadiusOfZeroIntensity );
// 
// 	uniformID = program->GetUniformVariableIDFromName( uniformName + ".inverseSizeOfDistanceAttenuationZone" );
// 	RendererInterface::SetUniformVariable( uniformID, light.m_inverseSizeOfDistanceAttenuationZone );
// 
// 	uniformID = program->GetUniformVariableIDFromName( uniformName + ".outerApertureAngleAsDotProduct" );
// 	RendererInterface::SetUniformVariable( uniformID, light.m_outerApertureAngleAsDotProduct );
// 
// 	uniformID = program->GetUniformVariableIDFromName( uniformName + ".inverseSizeOfApertureAttenuationZone" );
// 	RendererInterface::SetUniformVariable( uniformID, light.m_inverseSizeOfApertureAttenuationZone );
// 
// 	uniformID = program->GetUniformVariableIDFromName( uniformName + ".percentOfLightIsAmbientZeroToOne" );
// 	RendererInterface::SetUniformVariable( uniformID, light.m_percentOfLightIsAmbientZeroToOne );
// 
// 	uniformID = program->GetUniformVariableIDFromName( uniformName + ".positionWasGiven" );
// 	RendererInterface::SetUniformVariable( uniformID, light.m_positionWasGiven );
}

//-----------------------------------------------------------------------------------------------
void Material::SetMatrixUniform( const std::string& /*uniformName*/, const Float4x4Matrix& /*matrix*/ )
{
// 	int uniformID = GetValidUniformIDFromNameOrDie( uniformName );
// 	RendererInterface::SetUniformVariable( uniformID, matrix );
}

//-----------------------------------------------------------------------------------------------
void Material::SetTextureUniform( const std::string& /*uniformName*/, int /*textureUnitID*/, const std::string& /*textureFileLocation*/, 
	Texture::FilteringMethod /*filteringMethod*/, Texture::WrappingMode /*wrappingMode*/ )
{
// 	int uniformID = GetValidUniformIDFromNameOrDie( uniformName );
// 
// 	TextureInfo texInfo;
// 
// 	texInfo.textureUnitID = textureUnitID;
// 	RendererInterface::SetActiveTextureUnit( textureUnitID );
// 
// 	//FIX: Multiple declarations of same texture WILL take on the same characteristics!
// 	texInfo.texture = RendererInterface::GetTextureManager()->CreateOrGetTexture( textureFileLocation.c_str(), filteringMethod, wrappingMode );
// 	RendererInterface::SetUniformVariable( uniformID, textureUnitID );
// 
// 	texInfo.samplerUniformID = uniformID;
// 	infoForTextures.push_back( texInfo );
}
