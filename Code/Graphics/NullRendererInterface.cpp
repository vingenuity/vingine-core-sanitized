#include "../BuildPreferences.hpp"

#if defined( RENDERER_INTERFACE_USE_NULL )
#include "NullRendererInterface.hpp"

//These are used to fill the renderer constants.
//There are multiple because these may be used in switch statements (and thus can't share numbers)
static const unsigned int NULL_CONSTANT_0 = 0;
static const unsigned int NULL_CONSTANT_1 = 1;
static const unsigned int NULL_CONSTANT_2 = 2;
static const unsigned int NULL_CONSTANT_3 = 3;
static const unsigned int NULL_CONSTANT_4 = 4;
static const unsigned int NULL_CONSTANT_5 = 5;
static const unsigned int NULL_CONSTANT_6 = 6;
static const unsigned int NULL_CONSTANT_7 = 7;

#pragma region Renderer Constant Definitions
//-----------------------------------------------------------------------------------------------
STATIC const RendererInterface::ArrayType RendererInterface::COLOR_ARRAYS			= NULL_CONSTANT_0;
STATIC const RendererInterface::ArrayType RendererInterface::TEXTURE_COORD_ARRAYS	= NULL_CONSTANT_1;
STATIC const RendererInterface::ArrayType RendererInterface::VERTEX_ARRAYS			= NULL_CONSTANT_2;

STATIC const RendererInterface::BufferType RendererInterface::ARRAY_BUFFER = NULL_CONSTANT_0;
STATIC const RendererInterface::BufferType RendererInterface::INDEX_BUFFER = NULL_CONSTANT_1;

STATIC const RendererInterface::ColorComponents RendererInterface::COMPONENTS_DEPTH		= NULL_CONSTANT_0;
STATIC const RendererInterface::ColorComponents RendererInterface::COMPONENTS_STENCIL	= NULL_CONSTANT_1;
STATIC const RendererInterface::ColorComponents RendererInterface::ARGB					= NULL_CONSTANT_2;
STATIC const RendererInterface::ColorComponents RendererInterface::RGB					= NULL_CONSTANT_3;
STATIC const RendererInterface::ColorComponents RendererInterface::RGBA					= NULL_CONSTANT_4;
STATIC const RendererInterface::ColorComponents RendererInterface::RGB_16_BIT			= NULL_CONSTANT_5;
STATIC const RendererInterface::ColorComponents RendererInterface::RGBA_16_BIT			= NULL_CONSTANT_6;

STATIC const RendererInterface::ColorBlendingMode RendererInterface::NO_COLOR						= NULL_CONSTANT_0;
STATIC const RendererInterface::ColorBlendingMode RendererInterface::CONSTANT_ONE					= NULL_CONSTANT_1;
STATIC const RendererInterface::ColorBlendingMode RendererInterface::ONE_MINUS_DESTINATION_COLOR	= NULL_CONSTANT_2;
STATIC const RendererInterface::ColorBlendingMode RendererInterface::ONE_MINUS_SOURCE_ALPHA			= NULL_CONSTANT_3;
STATIC const RendererInterface::ColorBlendingMode RendererInterface::SOURCE_ALPHA					= NULL_CONSTANT_4;
STATIC const RendererInterface::ColorBlendingMode RendererInterface::ONE_MINUS_SOURCE_COLOR			= NULL_CONSTANT_5;
STATIC const RendererInterface::ColorBlendingMode RendererInterface::SOURCE_COLOR					= NULL_CONSTANT_6;

STATIC const RendererInterface::CoordinateType RendererInterface::TYPE_SHORT			= NULL_CONSTANT_0;
STATIC const RendererInterface::CoordinateType RendererInterface::TYPE_INT				= NULL_CONSTANT_1;
STATIC const RendererInterface::CoordinateType RendererInterface::TYPE_FLOAT			= NULL_CONSTANT_2;
STATIC const RendererInterface::CoordinateType RendererInterface::TYPE_DOUBLE			= NULL_CONSTANT_3;
STATIC const RendererInterface::CoordinateType RendererInterface::TYPE_UNSIGNED_BYTE	= NULL_CONSTANT_4;
STATIC const RendererInterface::CoordinateType RendererInterface::TYPE_UNSIGNED_INTEGER	= NULL_CONSTANT_5;
STATIC const RendererInterface::CoordinateType RendererInterface::TYPE_UNSIGNED_SHORT	= NULL_CONSTANT_6;
STATIC const RendererInterface::CoordinateType RendererInterface::TYPE_FOUR_BYTES_AS_INT	= NULL_CONSTANT_7;

STATIC const RendererInterface::Feature RendererInterface::COLOR_BLENDING			= NULL_CONSTANT_0;
STATIC const RendererInterface::Feature RendererInterface::DEPTH_TESTING			= NULL_CONSTANT_1;
STATIC const RendererInterface::Feature RendererInterface::FACE_CULLING				= NULL_CONSTANT_2;
STATIC const RendererInterface::Feature RendererInterface::SHAPE_RESTART_INDEXING	= NULL_CONSTANT_3;
STATIC const RendererInterface::Feature RendererInterface::TEXTURES_2D				= NULL_CONSTANT_4;

STATIC const RendererInterface::QualityLevel RendererInterface::FASTEST = NULL_CONSTANT_0;

STATIC const RendererInterface::Shader RendererInterface::GEOMETRY_SHADER		= NULL_CONSTANT_0;
STATIC const RendererInterface::Shader RendererInterface::PIXEL_FRAGMENT_SHADER = NULL_CONSTANT_1;
STATIC const RendererInterface::Shader RendererInterface::VERTEX_SHADER			= NULL_CONSTANT_2;

STATIC const RendererInterface::Shape RendererInterface::LINES			= NULL_CONSTANT_0;
STATIC const RendererInterface::Shape RendererInterface::LINE_LOOP		= NULL_CONSTANT_1;
STATIC const RendererInterface::Shape RendererInterface::LINE_STRIP		= NULL_CONSTANT_2;
STATIC const RendererInterface::Shape RendererInterface::TRIANGLES		= NULL_CONSTANT_3;
STATIC const RendererInterface::Shape RendererInterface::TRIANGLE_STRIP	= NULL_CONSTANT_4;
STATIC const RendererInterface::Shape RendererInterface::POINTS			= NULL_CONSTANT_5;
STATIC const RendererInterface::Shape RendererInterface::QUADS			= NULL_CONSTANT_6;

STATIC const RendererInterface::TextureFilteringMethod RendererInterface::NEAREST_NEIGHBOR = NULL_CONSTANT_0;
STATIC const RendererInterface::TextureFilteringMethod RendererInterface::LINEAR_INTERPOLATION = NULL_CONSTANT_1;
STATIC const RendererInterface::TextureFilteringMethod RendererInterface::NEAREST_MIPMAP_NEAREST_TEXTURE = NULL_CONSTANT_2;
STATIC const RendererInterface::TextureFilteringMethod RendererInterface::NEAREST_MIPMAP_INTERPOLATE_TEXTURES = NULL_CONSTANT_3;
STATIC const RendererInterface::TextureFilteringMethod RendererInterface::INTERPOLATE_MIPMAPS_NEAREST_TEXTURE = NULL_CONSTANT_4;
STATIC const RendererInterface::TextureFilteringMethod RendererInterface::INTERPOLATE_MIPMAPS_INTERPOLATE_TEXTURES = NULL_CONSTANT_5;

STATIC const RendererInterface::TextureWrapMode RendererInterface::CLAMP_TO_EDGE		= NULL_CONSTANT_0;
STATIC const RendererInterface::TextureWrapMode RendererInterface::REPEAT_OVER_GEOMETRY = NULL_CONSTANT_1;
STATIC const RendererInterface::TextureWrapMode RendererInterface::REPEAT_FLIPPED		= NULL_CONSTANT_2;
#pragma endregion //Renderer Constant Definitions

#endif // defined( RENDERER_INTERFACE_USE_NULL )
