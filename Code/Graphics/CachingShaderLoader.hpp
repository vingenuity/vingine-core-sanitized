#pragma once
#ifndef INCLUDED_CACHING_SHADER_LOADER_HPP
#define INCLUDED_CACHING_SHADER_LOADER_HPP

//-----------------------------------------------------------------------------------------------
#include "../EngineMacros.hpp"

#include "../Color.hpp"
#include "../Math/FloatVector2.hpp"
#include "../Math/FloatVector3.hpp"
#include "../Math/FloatVector4.hpp"
#include "../Math/Float4x4Matrix.hpp"
#include "../Math/IntVector2.hpp"
#include "RendererInterface.hpp"


//-----------------------------------------------------------------------------------------------
/* Each implementation of this loader will define what it needs to store in these variables.*/
struct Shader;
struct ShaderVariable;
struct ShaderPipeline;


//-----------------------------------------------------------------------------------------------
/* This is an enumeration of all of the shader language stage types.
 * It is not defined as an enum because each shader loader needs to define its constants for them.*/
typedef unsigned short ShaderStage;
extern const ShaderStage STAGE_Null;
extern const ShaderStage STAGE_Domain;
extern const ShaderStage STAGE_Fragment;
extern const ShaderStage STAGE_Geometry;
extern const ShaderStage STAGE_Hull;
extern const ShaderStage STAGE_Vertex;
static const unsigned int NUMBER_OF_SHADER_STAGES = 5; // Not including Null

//-----------------------------------------------------------------------------------------------
enum ShaderLanguage
{
	LANGUAGE_None,
	LANGUAGE_Cg,
	LANGUAGE_GLSL,
	LANGUAGE_HLSL,
	NUMBER_OF_SHADER_LANGUAGES
};


//-----------------------------------------------------------------------------------------------
ABSTRACT class CachingShaderLoader
{
public:
	virtual ~CachingShaderLoader() { }
	virtual bool SupportsLanguage( ShaderLanguage /*language*/ ) { return false; }

	// Interface
	virtual void BindVertexArrayToAttributeSlot( unsigned int slot, int numberOfVertexCoordinates,
		RendererInterface::CoordinateType coordinateType, bool normalizeData,
		unsigned int gapBetweenVertices, const void* firstVertexInArray ) = 0;
	virtual void BindVertexArrayToVariable( const ShaderVariable* variable, int numberOfVertexCoordinates,
		RendererInterface::CoordinateType coordinateType, bool normalizeData,
		unsigned int gapBetweenVertices, const void* firstVertexInArray ) = 0;
	virtual Shader* CompileShaderOrDie( ShaderStage shaderType, const char* sourceString ) = 0;
	virtual ShaderPipeline* CreateOrGetShaderProgramFromFiles( const char* vertexSourceFileLocation, const char* fragmentSourceFileLocation ) = 0;
	virtual ShaderPipeline* CreateOrGetPipelineFromShaders( const Shader* vertexShader, const Shader* geometryShader, const Shader* fragmentShader ) = 0;
	virtual void DeletePipelineDataOnCard( ShaderPipeline* pipeline ) = 0;
	virtual void DeleteShaderDataOnCard( Shader* shader ) = 0;
	virtual void DetachShaderFromPipeline( const Shader* shader, ShaderPipeline* pipeline ) = 0;
	virtual void DisableAttributeSlotInShader( unsigned int slot ) = 0;
	virtual void EnableAttributeSlotInShader( unsigned int slot ) = 0;
	virtual void DisableAttributeVariableInShader( const ShaderVariable* variable ) = 0;
	virtual void EnableAttributeVariableInShader( const ShaderVariable* variable ) = 0;
	virtual ShaderVariable* GetAttributeVariable( const ShaderPipeline* pipeline, const char* attributeName ) = 0;
	virtual int GetNumberOfAttributesInPipeline( const ShaderPipeline* pipeline ) const = 0;
	virtual int GetNumberOfUniformsInPipeline( const ShaderPipeline* pipeline ) const = 0;
	virtual ShaderVariable* GetUniformVariable( const ShaderPipeline* pipeline, const char* uniformName ) = 0;
	virtual void LoadSourceFromFileOrDie( char*& out_shaderData, const char* fileName ) = 0;
	virtual void SetTextureUnitUniform( ShaderVariable* variable, unsigned int samplerUnitNumber ) = 0;
	virtual void SetUniform( ShaderVariable* variable, int integer ) = 0;
	virtual void SetUniform( ShaderVariable* variable, float floatingPointNumber ) = 0;
	virtual void SetUniform( ShaderVariable* variable, const Color& color ) = 0;
	virtual void SetUniform( ShaderVariable* variable, const IntVector2& vector2 ) = 0;
	virtual void SetUniform( ShaderVariable* variable, const FloatVector2& vector2 ) = 0;
	virtual void SetUniform( ShaderVariable* variable, const FloatVector3& vector3 ) = 0;
	virtual void SetUniform( ShaderVariable* variable, const FloatVector4& vector4 ) = 0;
	virtual void SetUniform( ShaderVariable* variable, const Float4x4Matrix& matrix ) = 0;
	virtual void UseShaderPipeline( const ShaderPipeline* pipeline ) = 0;

protected:
	CachingShaderLoader() { }
};

#endif //INCLUDED_CACHING_SHADER_LOADER_HPP
