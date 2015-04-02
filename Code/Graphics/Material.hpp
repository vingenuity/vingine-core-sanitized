#pragma once
#ifndef INCLUDED_MATERIAL_HPP
#define INCLUDED_MATERIAL_HPP

//-----------------------------------------------------------------------------------------------
#include <vector>

#include "../Math/Float4x4Matrix.hpp"

#include "Tendon.hpp"
#include "Light.hpp"
#include "CachingShaderLoader.hpp"
#include "Texture.hpp"


//-----------------------------------------------------------------------------------------------
template < typename ShaderVariableType >
struct ShaderBinding
{
	typedef const ShaderVariableType& (*VariableReturningFunction)();

	ShaderBinding( ShaderVariable* variableToUpdate, VariableReturningFunction variableUpdater )
		: variable( variableToUpdate )
		, updatingFunction( variableUpdater )
	{ }

	//Data Members
	ShaderVariable* variable;
	VariableReturningFunction updatingFunction;
};

typedef const Float4x4Matrix& (*MatrixReturningFunction)();

//-----------------------------------------------------------------------------------------------
struct Material
{
	static const unsigned short MAX_NUMBER_OF_TEXTURES = 4;
	typedef unsigned short TextureType;
	static const TextureType TEXTURE_DiffuseMap  = 0;
	static const TextureType TEXTURE_NormalMap   = 1;
	static const TextureType TEXTURE_SpecularMap = 2;
	static const TextureType TEXTURE_EmissiveMap = 3;

	struct TextureInfo
	{
		TextureInfo()
			: textureUnitID( 0 )
			, samplerUniformVariable( nullptr )
			, texture( nullptr )
		{ }
		int textureUnitID;
		ShaderVariable* samplerUniformVariable;
		const Texture* texture;
	};

	//Constructor
	Material();
	~Material();

	int GetValidAttributeIDFromNameOrDie( const std::string& attributeName ) const;
	int GetValidUniformIDFromNameOrDie( const std::string& uniformName ) const;

	void SetFloatUniform( const std::string& uniformName, float value );
	void SetFloatUniform( const std::string& uniformName, const FloatVector3& vector );
	void SetFloatUniform( const std::string& uniformName, const FloatVector4& vector );
	void SetIntegerUniform( const std::string& uniformName, int value );
	void SetLightUniform( const std::string& uniformName, const Light& light );
	void SetMatrixUniform( const std::string& uniformName, const Float4x4Matrix& matrix );
	void SetModelMatrixUniform( const std::string& uniformName );
	void SetViewMatrixUniform( const std::string& uniformName );
	void SetProjectionMatrixUniform( const std::string& uniformName );
	void SetLineWidth( float newLineWidth ) { lineWidth = newLineWidth; }
	void SetShaderPipeline( const ShaderPipeline* shaderPipeline ) { pipeline = shaderPipeline; }
	void SetTextureUniform( const std::string& uniformName, int textureUnitID, const Texture* texture );
	void SetTextureUniform( const std::string& uniformName, int textureUnitID, const std::string& textureFileLocation, 
							Texture::FilteringMethod filteringMethod, Texture::WrappingMode wrappingMode );
	void BindMatrixToShader( MatrixReturningFunction matrixUpdater, const char* shaderVariableName );

	//Data Members
	const ShaderPipeline* pipeline;
	int modelMatrixUniformLocation;
	int viewMatrixUniformLocation;
	int projectionMatrixUniformLocation;
	std::vector< TextureInfo > infoForTextures;
	float lineWidth;
	std::vector< ShaderBinding<Float4x4Matrix> > matrixBindings;
};



//-----------------------------------------------------------------------------------------------
inline Material::Material()
	: pipeline( nullptr )
	, modelMatrixUniformLocation( -1 )
	, viewMatrixUniformLocation( -1 )
	, projectionMatrixUniformLocation( -1 )
	, lineWidth( 1 )
{ }

inline Material::~Material()
{ }

//-----------------------------------------------------------------------------------------------
inline void Material::SetModelMatrixUniform( const std::string& uniformName )
{
	int uniformID = GetValidUniformIDFromNameOrDie( uniformName );

	modelMatrixUniformLocation = uniformID;
}

//-----------------------------------------------------------------------------------------------
inline void Material::SetViewMatrixUniform( const std::string& uniformName )
{
	int uniformID = GetValidUniformIDFromNameOrDie( uniformName );

	viewMatrixUniformLocation = uniformID;
}

//-----------------------------------------------------------------------------------------------
inline void Material::SetProjectionMatrixUniform( const std::string& uniformName )
{
	int uniformID = GetValidUniformIDFromNameOrDie( uniformName );

	projectionMatrixUniformLocation = uniformID;
}

//-----------------------------------------------------------------------------------------------
inline void Material::SetTextureUniform( const std::string& uniformName, int textureUnitID, const Texture* texture )
{
	TextureInfo texInfo;
	texInfo.textureUnitID = textureUnitID;
	texInfo.texture = texture;
	texInfo.samplerUniformVariable = RendererInterface::GetShaderLoader()->GetUniformVariable( pipeline, uniformName.c_str() );
	infoForTextures.push_back( texInfo );
	// FIX: We are leaking the memory for this uniform variable.
}

#endif //INCLUDED_MATERIAL_HPP
