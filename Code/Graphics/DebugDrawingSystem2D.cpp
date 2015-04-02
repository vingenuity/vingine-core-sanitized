#include "DebugDrawingSystem2D.hpp"

#include "../Entity.hpp"
#include "../Font/BitmapFont.hpp"
#include "Material.hpp"
#include "MeshComponent.hpp"
#include "Mesh2DGeneration.hpp"
#include "MeshGenerationText.hpp"
#include "RendererInterface.hpp"


//-----------------------------------------------------------------------------------------------
void DebugDrawingSystem2D::OnAttachment( SystemManager* /*manager*/ )
{
	m_debugMeshOwningEntity = new Entity();

	std::string fontTextureLocation( "Data/Font/MainFont_EN_00.png" );
	//m_debugFont = new BitmapFont( "Data/Font/MainFont_EN.FontDef.xml", &fontTextureLocation, 1 );

// 	m_debugMeshMaterial = RendererInterface::CreateOrGetNewMaterial( L"DebugMaterial" );
// 	m_debugMeshMaterial->SetShaderProgram( ShaderProgram::CreateOrGetShaderProgram( "Shaders/BasicNoTexture.110.vertex.glsl", "Shaders/BasicNoTexture.110.fragment.glsl" ) );
// 	m_debugMeshMaterial->SetModelMatrixUniform( "u_modelMatrix" );
// 	m_debugMeshMaterial->SetViewMatrixUniform( "u_viewMatrix" );
// 	m_debugMeshMaterial->SetProjectionMatrixUniform( "u_projectionMatrix" );
// 
// 	m_debugTextMaterial = RendererInterface::CreateOrGetNewMaterial( L"DebugTextMaterial" );
// 	m_debugTextMaterial->SetShaderProgram( ShaderProgram::CreateOrGetShaderProgram( "Shaders/Basic.110.vertex.glsl", "Shaders/Basic.110.fragment.glsl" ) );
// 	m_debugTextMaterial->SetModelMatrixUniform( "u_modelMatrix" );
// 	m_debugTextMaterial->SetViewMatrixUniform( "u_viewMatrix" );
// 	m_debugTextMaterial->SetProjectionMatrixUniform( "u_projectionMatrix" );
}

//-----------------------------------------------------------------------------------------------
void DebugDrawingSystem2D::OnEndFrame()
{
	for( unsigned int i = 0; i < m_meshes.size(); ++i )
	{
		if( m_meshes[ i ]->secondsLeftUntilDestroyed < 0.f )
		{
			CleanupTimedMesh( m_meshes[ i ] );
			m_meshes.erase( m_meshes.begin() + i );
		}
	}

	//Clear "ownership" of components (the entity doesn't actually care about the components, the components just need an owner during creation)
	m_debugMeshOwningEntity->attachedComponents.clear();
}

//-----------------------------------------------------------------------------------------------
void DebugDrawingSystem2D::OnRender() const
{
	RendererInterface::DisableFeature( RendererInterface::DEPTH_TESTING );
	RendererInterface::DisableDepthBufferWriting();
	RendererInterface::SetViewMatrixToIdentity();
	RendererInterface::SetOrthographicProjection( m_leftXEdge, m_rightXEdge, m_bottomYEdge, m_topYEdge, 0.f, 1.f );

	for( unsigned int i = 0; i < m_meshes.size(); ++i )
	{
		RenderMeshComponent( m_meshes[ i ]->meshComponent );
	}

	RendererInterface::EnableDepthBufferWriting();
	RendererInterface::EnableFeature( RendererInterface::DEPTH_TESTING );
}

//-----------------------------------------------------------------------------------------------
void DebugDrawingSystem2D::OnUpdate( float deltaSeconds )
{
	for( unsigned int i = 0; i < m_meshes.size(); ++i )
	{
		m_meshes[ i ]->secondsLeftUntilDestroyed -= deltaSeconds;
	}
}

//-----------------------------------------------------------------------------------------------
void DebugDrawingSystem2D::OnDestruction()
{
	for( unsigned int i = 0; i < m_meshes.size(); ++i )
	{
		CleanupTimedMesh( m_meshes[ i ] );
	}
	m_meshes.clear();
	delete m_debugMeshOwningEntity;
	//delete m_debugFont;
}

//-----------------------------------------------------------------------------------------------
void DebugDrawingSystem2D::CleanupTimedMesh( TimedMesh* timedMesh )
{
	timedMesh->meshComponent->material = nullptr; //Save our static material from destruction
	delete timedMesh->meshComponent;
	delete timedMesh;
}

//-----------------------------------------------------------------------------------------------
void DebugDrawingSystem2D::RenderMeshComponent( const MeshComponent* mesh ) const
{
	RendererInterface::ApplyMaterial( mesh->material );
	//RendererInterface::BindVertexDataToShader( mesh->vertexData, mesh->material->pipeline );

	RendererInterface::SetLineWidth( 5.f );

	RendererInterface::RenderVertexArray( mesh->vertexData->shape, 0, mesh->vertexData->numberOfVertices );

	//RendererInterface::UnbindVertexDataFromShader( mesh->vertexData, mesh->material->pipeline );
	RendererInterface::RemoveMaterial( mesh->material );
}

//-----------------------------------------------------------------------------------------------
void DebugDrawingSystem2D::DrawPointForTime( const FloatVector2& centerPosition, float size, 
											 const Color& color, float lifetimeSeconds )
{
	MeshComponent* newMesh = new MeshComponent();
	newMesh->owner = m_debugMeshOwningEntity;
	Generate2DPoint( *newMesh->vertexData, centerPosition, size, color );
	m_meshes.push_back( new TimedMesh( lifetimeSeconds, newMesh ) );
	m_meshes.back()->meshComponent->material = m_debugMeshMaterial;
}

//-----------------------------------------------------------------------------------------------
void DebugDrawingSystem2D::DrawTextForTime( const std::string& textString, const FloatVector2& position, const Color& color, 
											float fontHeight, float lifetimeSeconds )
{
	MeshComponent* newMesh = new MeshComponent();
	newMesh->owner = m_debugMeshOwningEntity;
	newMesh->vertexData = new VertexData();
	GenerateTextMesh( *newMesh->vertexData, textString, position, color, m_debugFont, fontHeight );
	m_meshes.push_back( new TimedMesh( lifetimeSeconds, newMesh ) );
	m_meshes.back()->meshComponent->material = m_debugTextMaterial;
}

