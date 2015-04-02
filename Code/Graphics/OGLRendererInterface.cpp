#include "../BuildPreferences.hpp"

#if defined( RENDERER_INTERFACE_USE_OPENGL )

#include "OGLRendererInterface.hpp"

#pragma region OpenGL_Function_Pointer_Definitions
//-----------------------------------------------------------------------------------------------
PFNGLCLEARDEPTHFPROC			  glClearDepthf = nullptr;
//Vertex Buffers
PFNGLBINDBUFFERPROC				  glBindBuffer = nullptr;
PFNGLBUFFERDATAPROC				  glBufferData = nullptr;
PFNGLDELETEBUFFERSPROC			  glDeleteBuffers = nullptr;
PFNGLGENBUFFERSPROC				  glGenBuffers = nullptr;
PFNGLPRIMITIVERESTARTINDEXPROC	  glPrimitiveRestartIndex = nullptr;

//Framebuffers
PFNGLBINDFRAMEBUFFERPROC		 glBindFramebuffer = nullptr;
PFNGLCHECKFRAMEBUFFERSTATUSPROC	 glCheckFramebufferStatus = nullptr;
PFNGLDELETEFRAMEBUFFERSPROC		 glDeleteFramebuffers = nullptr;
PFNGLDRAWBUFFERSPROC			 glDrawBuffers = nullptr;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer = nullptr;
PFNGLFRAMEBUFFERTEXTURE2DPROC	 glFramebufferTexture2D = nullptr;
PFNGLGENFRAMEBUFFERSPROC		 glGenFramebuffers = nullptr;

//Mipmaps
PFNGLGENERATEMIPMAPPROC	glGenerateMipmap = nullptr;

//Textures
PFNGLACTIVETEXTUREPROC		glActiveTexture = nullptr;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D = nullptr;
#pragma endregion

#pragma region Renderer_to_OpenGL_Constant_Definitions
//-----------------------------------------------------------------------------------------------
STATIC const RendererInterface::ArrayType RendererInterface::COLOR_ARRAYS			= GL_COLOR_ARRAY;
STATIC const RendererInterface::ArrayType RendererInterface::TEXTURE_COORD_ARRAYS = GL_TEXTURE_COORD_ARRAY;
STATIC const RendererInterface::ArrayType RendererInterface::VERTEX_ARRAYS		= GL_VERTEX_ARRAY;

STATIC const RendererInterface::BufferType RendererInterface::ARRAY_BUFFER = GL_ARRAY_BUFFER;
STATIC const RendererInterface::BufferType RendererInterface::INDEX_BUFFER = GL_ELEMENT_ARRAY_BUFFER;

STATIC const RendererInterface::ColorComponents RendererInterface::COMPONENTS_DEPTH	= GL_DEPTH_COMPONENT;
STATIC const RendererInterface::ColorComponents RendererInterface::COMPONENTS_STENCIL = GL_STENCIL_COMPONENTS;
STATIC const RendererInterface::ColorComponents RendererInterface::ARGB				= GL_RGBA;
STATIC const RendererInterface::ColorComponents RendererInterface::RGB				= GL_RGB;
STATIC const RendererInterface::ColorComponents RendererInterface::RGBA				= GL_RGBA;
STATIC const RendererInterface::ColorComponents RendererInterface::RGB_16_BIT		= GL_RGB16;
STATIC const RendererInterface::ColorComponents RendererInterface::RGBA_16_BIT		= GL_RGBA16;

STATIC const RendererInterface::ColorBlendingMode RendererInterface::NO_COLOR						= GL_ZERO;
STATIC const RendererInterface::ColorBlendingMode RendererInterface::CONSTANT_ONE						= GL_ONE;
STATIC const RendererInterface::ColorBlendingMode RendererInterface::ONE_MINUS_DESTINATION_COLOR	= GL_ONE_MINUS_DST_COLOR;
STATIC const RendererInterface::ColorBlendingMode RendererInterface::ONE_MINUS_SOURCE_ALPHA		= GL_ONE_MINUS_SRC_ALPHA;
STATIC const RendererInterface::ColorBlendingMode RendererInterface::SOURCE_ALPHA					= GL_SRC_ALPHA;
STATIC const RendererInterface::ColorBlendingMode RendererInterface::ONE_MINUS_SOURCE_COLOR		= GL_ONE_MINUS_SRC_COLOR;
STATIC const RendererInterface::ColorBlendingMode RendererInterface::SOURCE_COLOR					= GL_SRC_COLOR;

STATIC const RendererInterface::CoordinateType RendererInterface::TYPE_SHORT		= GL_SHORT;
STATIC const RendererInterface::CoordinateType RendererInterface::TYPE_INT			= GL_INT;
STATIC const RendererInterface::CoordinateType RendererInterface::TYPE_FLOAT		= GL_FLOAT;
STATIC const RendererInterface::CoordinateType RendererInterface::TYPE_DOUBLE		= GL_DOUBLE;
STATIC const RendererInterface::CoordinateType RendererInterface::TYPE_UNSIGNED_BYTE	= GL_UNSIGNED_BYTE;
STATIC const RendererInterface::CoordinateType RendererInterface::TYPE_UNSIGNED_INTEGER	= GL_UNSIGNED_INT;
STATIC const RendererInterface::CoordinateType RendererInterface::TYPE_UNSIGNED_SHORT	= GL_UNSIGNED_SHORT;
STATIC const RendererInterface::CoordinateType RendererInterface::TYPE_FOUR_BYTES_AS_INT	= GL_UNSIGNED_INT_8_8_8_8_REV;

STATIC const RendererInterface::Feature RendererInterface::COLOR_BLENDING	= GL_BLEND;
STATIC const RendererInterface::Feature RendererInterface::DEPTH_TESTING	= GL_DEPTH_TEST;
STATIC const RendererInterface::Feature RendererInterface::FACE_CULLING	= GL_CULL_FACE;
STATIC const RendererInterface::Feature RendererInterface::SHAPE_RESTART_INDEXING	= GL_PRIMITIVE_RESTART;
STATIC const RendererInterface::Feature RendererInterface::TEXTURES_2D	= GL_TEXTURE_2D;

STATIC const RendererInterface::QualityLevel RendererInterface::FASTEST = GL_FASTEST;

STATIC const RendererInterface::Shader RendererInterface::GEOMETRY_SHADER = GL_GEOMETRY_SHADER;
STATIC const RendererInterface::Shader RendererInterface::PIXEL_FRAGMENT_SHADER = GL_FRAGMENT_SHADER;
STATIC const RendererInterface::Shader RendererInterface::VERTEX_SHADER = GL_VERTEX_SHADER;

STATIC const RendererInterface::Shape RendererInterface::LINES			= GL_LINES;
STATIC const RendererInterface::Shape RendererInterface::LINE_LOOP		= GL_LINE_LOOP;
STATIC const RendererInterface::Shape RendererInterface::LINE_STRIP		= GL_LINE_STRIP;
STATIC const RendererInterface::Shape RendererInterface::TRIANGLES		= GL_TRIANGLES;
STATIC const RendererInterface::Shape RendererInterface::TRIANGLE_STRIP	= GL_TRIANGLE_STRIP;
STATIC const RendererInterface::Shape RendererInterface::POINTS			= GL_POINT;
STATIC const RendererInterface::Shape RendererInterface::QUADS			= GL_QUADS;

STATIC const RendererInterface::TextureFilteringMethod RendererInterface::NEAREST_NEIGHBOR = GL_NEAREST;
STATIC const RendererInterface::TextureFilteringMethod RendererInterface::LINEAR_INTERPOLATION = GL_LINEAR;
STATIC const RendererInterface::TextureFilteringMethod RendererInterface::NEAREST_MIPMAP_NEAREST_TEXTURE = GL_NEAREST_MIPMAP_NEAREST;
STATIC const RendererInterface::TextureFilteringMethod RendererInterface::NEAREST_MIPMAP_INTERPOLATE_TEXTURES = GL_LINEAR_MIPMAP_NEAREST;
STATIC const RendererInterface::TextureFilteringMethod RendererInterface::INTERPOLATE_MIPMAPS_NEAREST_TEXTURE = GL_NEAREST_MIPMAP_LINEAR;
STATIC const RendererInterface::TextureFilteringMethod RendererInterface::INTERPOLATE_MIPMAPS_INTERPOLATE_TEXTURES = GL_LINEAR_MIPMAP_LINEAR;

STATIC const RendererInterface::TextureWrapMode RendererInterface::CLAMP_TO_EDGE		  = GL_CLAMP_TO_EDGE;
STATIC const RendererInterface::TextureWrapMode RendererInterface::REPEAT_OVER_GEOMETRY = GL_REPEAT;
STATIC const RendererInterface::TextureWrapMode RendererInterface::REPEAT_FLIPPED = GL_MIRRORED_REPEAT;
#pragma endregion

//-----------------------------------------------------------------------------------------------
void OGLRendererInterface::InitializeOpenGLFunctionPointers()
{
	glClearDepthf				= ( PFNGLCLEARDEPTHFPROC ) wglGetProcAddress( "glClearDepthf" );
	//Vertex Buffers
	glBindBuffer				= ( PFNGLBINDBUFFERPROC ) wglGetProcAddress( "glBindBuffer" );
	glBufferData				= ( PFNGLBUFFERDATAPROC ) wglGetProcAddress( "glBufferData" );
	glDeleteBuffers				= ( PFNGLDELETEBUFFERSPROC ) wglGetProcAddress( "glDeleteBuffers" );
	glGenBuffers				= ( PFNGLGENBUFFERSPROC ) wglGetProcAddress( "glGenBuffers" );
	glPrimitiveRestartIndex		= ( PFNGLPRIMITIVERESTARTINDEXPROC ) wglGetProcAddress( "glPrimitiveRestartIndex" );

	//Framebuffers
	glBindFramebuffer			= ( PFNGLBINDFRAMEBUFFERPROC ) wglGetProcAddress( "glBindFramebuffer" );
	glCheckFramebufferStatus	= ( PFNGLCHECKFRAMEBUFFERSTATUSPROC ) wglGetProcAddress( "glCheckFramebufferStatus" );
	glDeleteFramebuffers		= ( PFNGLDELETEFRAMEBUFFERSPROC ) wglGetProcAddress( "glDeleteFramebuffers" );
	glDrawBuffers				= ( PFNGLDRAWBUFFERSPROC ) wglGetProcAddress( "glDrawBuffers" );
	glFramebufferRenderbuffer	= ( PFNGLFRAMEBUFFERRENDERBUFFERPROC ) wglGetProcAddress( "glFramebufferRenderbuffer" );
	glFramebufferTexture2D		= ( PFNGLFRAMEBUFFERTEXTURE2DPROC ) wglGetProcAddress( "glFramebufferTexture2D" );
	glGenFramebuffers			= ( PFNGLGENFRAMEBUFFERSPROC ) wglGetProcAddress( "glGenFramebuffers" );

	//Mipmaps
	glGenerateMipmap = ( PFNGLGENERATEMIPMAPPROC ) wglGetProcAddress( "glGenerateMipmap" );

	//Textures
	glActiveTexture = ( PFNGLACTIVETEXTUREPROC ) wglGetProcAddress( "glActiveTexture" );
	glCompressedTexImage2D = ( PFNGLCOMPRESSEDTEXIMAGE2DPROC ) wglGetProcAddress( "glCompressedTexImage2D" );
}
#endif //defined( RENDERER_INTERFACE_USE_OPENGL )
