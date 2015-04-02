#pragma once
#ifndef INCLUDED_CG_GL_SHADER_LOADER_HPP
#define INCLUDED_CG_GL_SHADER_LOADER_HPP

//-----------------------------------------------------------------------------------------------
#include <map>
#include <string>
#include <vector>

#include <Cg/cg.h>
#include <Cg/cgGL.h>

#include "CachingShaderLoader.hpp"


//-----------------------------------------------------------------------------------------------
struct Shader
{
	Shader() : shaderPointer( nullptr ) { }
	Shader( CGprogram shaderPtr ) : shaderPointer( shaderPtr ) { }

	CGprogram shaderPointer;
};

//-----------------------------------------------------------------------------------------------
struct ShaderVariable
{
	ShaderVariable( CGparameter shaderParameter ) : parameter( shaderParameter ) { }

	CGparameter parameter;
};

//-----------------------------------------------------------------------------------------------
struct ShaderPipeline
{
	ShaderPipeline()
		: vertexShader( nullptr )
		, geometryShader( nullptr )
		, fragmentShader( nullptr )
		, combinedProgram( nullptr )
	{ }


	bool operator==( const ShaderPipeline& rhs ) const 
	{
		if( this->vertexShader == rhs.vertexShader &&
			this->fragmentShader == rhs.fragmentShader &&
			this->geometryShader == rhs.geometryShader )
		{
			return true;
		}
		return false;
	}

	// Data Members
	const Shader* vertexShader;
	const Shader* geometryShader;
	const Shader* fragmentShader;
	CGprogram combinedProgram;
};


//-----------------------------------------------------------------------------------------------
class CgGLShaderLoader : public CachingShaderLoader
{
	// This is used to index the shader profiles
	static const ShaderStage LOWEST_SHADER_STAGE = CG_GL_VERTEX;

public:
	CgGLShaderLoader();
	~CgGLShaderLoader();

	bool SupportsLanguage( ShaderLanguage language );

	// Interface
	void BindVertexArrayToAttributeSlot( unsigned int slot, int numberOfVertexCoordinates,
		RendererInterface::CoordinateType coordinateType, bool normalizeData,
		unsigned int gapBetweenVertices, const void* firstVertexInArray );
	void BindVertexArrayToVariable( const ShaderVariable* variable, int numberOfVertexCoordinates,
		RendererInterface::CoordinateType coordinateType, bool normalizeData,
		unsigned int gapBetweenVertices, const void* firstVertexInArray );
	Shader* CompileShaderOrDie( ShaderStage shaderType, const char* sourceString );
	ShaderPipeline* CreateOrGetShaderProgramFromFiles( const char* vertexSourceFileLocation, const char* fragmentSourceFileLocation );
	ShaderPipeline* CreateOrGetPipelineFromShaders( const Shader* vertexShader, const Shader* geometryShader, const Shader* fragmentShader );
	void DeletePipelineDataOnCard( ShaderPipeline* pipeline );
	void DeleteShaderDataOnCard( Shader* shader );
	void DetachShaderFromPipeline( const Shader* shader, ShaderPipeline* pipeline );
	void DisableAttributeSlotInShader( unsigned int slot );
	void EnableAttributeSlotInShader( unsigned int slot );
	void DisableAttributeVariableInShader( const ShaderVariable* variable );
	void EnableAttributeVariableInShader( const ShaderVariable* variable );
	ShaderVariable* GetAttributeVariable( const ShaderPipeline* pipeline, const char* attributeName );
	int GetNumberOfAttributesInPipeline( const ShaderPipeline* pipeline ) const;
	int GetNumberOfUniformsInPipeline( const ShaderPipeline* pipeline ) const;
	ShaderVariable* GetUniformVariable( const ShaderPipeline* pipeline, const char* uniformName );
	void LoadSourceFromFileOrDie( char*& out_shaderData, const char* fileName );
	void SetTextureUnitUniform( ShaderVariable* variable, unsigned int samplerUnitNumber );
	void SetUniform( ShaderVariable* variable, int integer );
	void SetUniform( ShaderVariable* variable, float floatingPointNumber );
	void SetUniform( ShaderVariable* variable, const Color& color );
	void SetUniform( ShaderVariable* variable, const IntVector2& vector2 );
	void SetUniform( ShaderVariable* variable, const FloatVector2& vector2 );
	void SetUniform( ShaderVariable* variable, const FloatVector3& vector3 );
	void SetUniform( ShaderVariable* variable, const FloatVector4& vector4 );
	void SetUniform( ShaderVariable* variable, const Float4x4Matrix& matrix );
	void UseShaderPipeline( const ShaderPipeline* pipeline );

private:
	//Helpers
	CGerror GetCGErrorCode();
	const char* GetCGErrorString( CGerror errorCode );
	void VerifyNoCgErrorsHaveOccurredOrDie();

	//Data Members
	CGcontext m_shaderContainer;
	std::vector< ShaderPipeline* > m_pipelineCache;
	std::map< std::string, Shader > m_shaderCache;
};

#endif //INCLUDED_CG_GL_SHADER_LOADER_HPP
