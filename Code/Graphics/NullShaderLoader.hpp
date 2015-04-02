#pragma once
#ifndef INCLUDED_NULL_SHADER_LOADER_HPP
#define INCLUDED_NULL_SHADER_LOADER_HPP

//-----------------------------------------------------------------------------------------------
#include "CachingShaderLoader.hpp"


//-----------------------------------------------------------------------------------------------
struct Shader
{
};

//-----------------------------------------------------------------------------------------------
struct ShaderVariable
{
};

//-----------------------------------------------------------------------------------------------
struct ShaderPipeline
{
};


//-----------------------------------------------------------------------------------------------
class NullShaderLoader : public CachingShaderLoader
{
public:
	NullShaderLoader() { }
	~NullShaderLoader() { }

	bool SupportsLanguage( ShaderLanguage language ) { return ( language == LANGUAGE_None ); }

	// Interface
	void BindVertexArrayToAttributeSlot( unsigned int slot, int numberOfVertexCoordinates,
		RendererInterface::CoordinateType coordinateType, bool normalizeData,
		unsigned int gapBetweenVertices, const void* firstVertexInArray ) { }
	void BindVertexArrayToVariable( const ShaderVariable* variable, int numberOfVertexCoordinates,
		RendererInterface::CoordinateType coordinateType, bool normalizeData,
		unsigned int gapBetweenVertices, const void* firstVertexInArray ) { }
	Shader* CompileShaderOrDie( ShaderStage shaderType, const char* sourceString ) { return nullptr; }
	ShaderPipeline* CreateOrGetShaderProgramFromFiles( const char* vertexSourceFileLocation, const char* fragmentSourceFileLocation ) { return nullptr; }
	ShaderPipeline* CreateOrGetPipelineFromShaders( const Shader* vertexShader, const Shader* geometryShader, const Shader* fragmentShader ) { return nullptr; }
	void DeletePipelineDataOnCard( ShaderPipeline* pipeline ) { }
	void DeleteShaderDataOnCard( Shader* shader ) { }
	void DetachShaderFromPipeline( const Shader* shader, ShaderPipeline* pipeline ) { }
	void DisableAttributeSlotInShader( unsigned int slot ) { }
	void EnableAttributeSlotInShader( unsigned int slot ) { }
	void DisableAttributeVariableInShader( const ShaderVariable* variable ) { }
	void EnableAttributeVariableInShader( const ShaderVariable* variable ) { }
	ShaderVariable* GetAttributeVariable( const ShaderPipeline* pipeline, const char* attributeName ) { return nullptr; }
	int GetNumberOfAttributesInPipeline( const ShaderPipeline* pipeline ) const { return 0; }
	int GetNumberOfUniformsInPipeline( const ShaderPipeline* pipeline ) const { return 0; }
	ShaderVariable* GetUniformVariable( const ShaderPipeline* pipeline, const char* uniformName ) { return nullptr; }
	void LoadSourceFromFileOrDie( char*& out_shaderData, const char* fileName ) { }
	void SetTextureUnitUniform( ShaderVariable* variable, unsigned int samplerUnitNumber ) { }
	void SetUniform( ShaderVariable* variable, int integer ) { }
	void SetUniform( ShaderVariable* variable, float floatingPointNumber ) { }
	void SetUniform( ShaderVariable* variable, const Color& color ) { }
	void SetUniform( ShaderVariable* variable, const IntVector2& vector2 ) { }
	void SetUniform( ShaderVariable* variable, const FloatVector2& vector2 ) { }
	void SetUniform( ShaderVariable* variable, const FloatVector3& vector3 ) { }
	void SetUniform( ShaderVariable* variable, const FloatVector4& vector4 ) { }
	void SetUniform( ShaderVariable* variable, const Float4x4Matrix& matrix ) { }
	void UseShaderPipeline( const ShaderPipeline* pipeline ) { }

private:
	//Data Members
};

#endif //INCLUDED_NULL_SHADER_LOADER_HPP
