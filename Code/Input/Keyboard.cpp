#include "Keyboard.hpp"

#include "../EngineMacros.hpp"
#include "../PlatformSpecificHeaders.hpp"


//-----------------------------------------------------------------------------------------------
STATIC const Keyboard::KeyModifier Keyboard::MODIFIER_NONE			= 0;
STATIC const Keyboard::KeyModifier Keyboard::MODIFIER_CONTROL		= 1;
STATIC const Keyboard::KeyModifier Keyboard::MODIFIER_SHIFT			= 2;
STATIC const Keyboard::KeyModifier Keyboard::MODIFIER_CTRL_SHIFT	= 3;
STATIC const Keyboard::KeyModifier Keyboard::MODIFIER_ALT			= 4;
STATIC const Keyboard::KeyModifier Keyboard::MODIFIER_CTRL_ALT		= 5;
STATIC const Keyboard::KeyModifier Keyboard::MODIFIER_ALT_SHIFT		= 6;
STATIC const Keyboard::KeyModifier Keyboard::MODIFIER_CTRL_ALT_SHIFT= 7;



#pragma region Key Definitions
//-----------------------------------------------------------------------------------------------

#if defined( PLATFORM_WINDOWS )
	#pragma region Windows Keys
	//-----------------------------------------------------------------------------------------------
	STATIC const Keyboard::Key Keyboard::A = 'A';
	STATIC const Keyboard::Key Keyboard::B = 'B';
	STATIC const Keyboard::Key Keyboard::C = 'C';
	STATIC const Keyboard::Key Keyboard::D = 'D';
	STATIC const Keyboard::Key Keyboard::E = 'E';
	STATIC const Keyboard::Key Keyboard::F = 'F';
	STATIC const Keyboard::Key Keyboard::G = 'G';
	STATIC const Keyboard::Key Keyboard::H = 'H';
	STATIC const Keyboard::Key Keyboard::I = 'I';
	STATIC const Keyboard::Key Keyboard::J = 'J';
	STATIC const Keyboard::Key Keyboard::K = 'K';
	STATIC const Keyboard::Key Keyboard::L = 'L';
	STATIC const Keyboard::Key Keyboard::M = 'M';
	STATIC const Keyboard::Key Keyboard::N = 'N';
	STATIC const Keyboard::Key Keyboard::O = 'O';
	STATIC const Keyboard::Key Keyboard::P = 'P';
	STATIC const Keyboard::Key Keyboard::Q = 'Q';
	STATIC const Keyboard::Key Keyboard::R = 'R';
	STATIC const Keyboard::Key Keyboard::S = 'S';
	STATIC const Keyboard::Key Keyboard::T = 'T';
	STATIC const Keyboard::Key Keyboard::U = 'U';
	STATIC const Keyboard::Key Keyboard::V = 'V';
	STATIC const Keyboard::Key Keyboard::W = 'W';
	STATIC const Keyboard::Key Keyboard::X = 'X';
	STATIC const Keyboard::Key Keyboard::Y = 'Y';
	STATIC const Keyboard::Key Keyboard::Z = 'Z';

	STATIC const Keyboard::Key Keyboard::GRAVE = VK_OEM_3;
	STATIC const Keyboard::Key Keyboard::TILDE = VK_OEM_3;
	STATIC const Keyboard::Key Keyboard::NUMBER_1 = '1';
	STATIC const Keyboard::Key Keyboard::NUMBER_2 = '2';
	STATIC const Keyboard::Key Keyboard::NUMBER_3 = '3';
	STATIC const Keyboard::Key Keyboard::NUMBER_4 = '4';
	STATIC const Keyboard::Key Keyboard::NUMBER_5 = '5';
	STATIC const Keyboard::Key Keyboard::NUMBER_6 = '6';
	STATIC const Keyboard::Key Keyboard::NUMBER_7 = '7';
	STATIC const Keyboard::Key Keyboard::NUMBER_8 = '8';
	STATIC const Keyboard::Key Keyboard::NUMBER_9 = '9';
	STATIC const Keyboard::Key Keyboard::NUMBER_0 = '0';

	STATIC const Keyboard::Key Keyboard::FUNCTION_1  = VK_F1;
	STATIC const Keyboard::Key Keyboard::FUNCTION_2  = VK_F2;
	STATIC const Keyboard::Key Keyboard::FUNCTION_3  = VK_F3;
	STATIC const Keyboard::Key Keyboard::FUNCTION_4  = VK_F4;
	STATIC const Keyboard::Key Keyboard::FUNCTION_5  = VK_F5;
	STATIC const Keyboard::Key Keyboard::FUNCTION_6  = VK_F6;
	STATIC const Keyboard::Key Keyboard::FUNCTION_7  = VK_F7;
	STATIC const Keyboard::Key Keyboard::FUNCTION_8  = VK_F8;
	STATIC const Keyboard::Key Keyboard::FUNCTION_9  = VK_F9;
	STATIC const Keyboard::Key Keyboard::FUNCTION_10 = VK_F10;
	STATIC const Keyboard::Key Keyboard::FUNCTION_11 = VK_F11;
	STATIC const Keyboard::Key Keyboard::FUNCTION_12 = VK_F12;

	STATIC const Keyboard::Key Keyboard::BACKSPACE		= VK_BACK;
	STATIC const Keyboard::Key Keyboard::SPACEBAR		= VK_SPACE;
	STATIC const Keyboard::Key Keyboard::ENTER			= VK_RETURN;
	STATIC const Keyboard::Key Keyboard::ESCAPE			= VK_ESCAPE;
	STATIC const Keyboard::Key Keyboard::TAB			= VK_TAB;
	STATIC const Keyboard::Key Keyboard::LEFT_ALT		= VK_LMENU;
	STATIC const Keyboard::Key Keyboard::RIGHT_ALT		= VK_RMENU;
	STATIC const Keyboard::Key Keyboard::LEFT_CONTROL	= VK_LCONTROL;
	STATIC const Keyboard::Key Keyboard::RIGHT_CONTROL	= VK_RCONTROL;
	STATIC const Keyboard::Key Keyboard::LEFT_SHIFT		= VK_LSHIFT;
	STATIC const Keyboard::Key Keyboard::RIGHT_SHIFT	= VK_RSHIFT;

	STATIC const Keyboard::Key Keyboard::INSERT		= VK_INSERT;
	STATIC const Keyboard::Key Keyboard::DELETE_KEY	= VK_DELETE;
	STATIC const Keyboard::Key Keyboard::HOME		= VK_HOME;
	STATIC const Keyboard::Key Keyboard::END		= VK_END;
	STATIC const Keyboard::Key Keyboard::PAGE_UP	= VK_PRIOR;
	STATIC const Keyboard::Key Keyboard::PAGE_DOWN	= VK_NEXT;

	STATIC const Keyboard::Key Keyboard::ARROW_UP	 = VK_UP;
	STATIC const Keyboard::Key Keyboard::ARROW_DOWN	 = VK_DOWN;
	STATIC const Keyboard::Key Keyboard::ARROW_LEFT	 = VK_LEFT;
	STATIC const Keyboard::Key Keyboard::ARROW_RIGHT = VK_RIGHT;

	STATIC const Keyboard::Key Keyboard::NUMPAD_0 = VK_NUMPAD0;
	STATIC const Keyboard::Key Keyboard::NUMPAD_1 = VK_NUMPAD1;
	STATIC const Keyboard::Key Keyboard::NUMPAD_2 = VK_NUMPAD2;
	STATIC const Keyboard::Key Keyboard::NUMPAD_3 = VK_NUMPAD3;
	STATIC const Keyboard::Key Keyboard::NUMPAD_4 = VK_NUMPAD4;
	STATIC const Keyboard::Key Keyboard::NUMPAD_5 = VK_NUMPAD5;
	STATIC const Keyboard::Key Keyboard::NUMPAD_6 = VK_NUMPAD6;
	STATIC const Keyboard::Key Keyboard::NUMPAD_7 = VK_NUMPAD7;
	STATIC const Keyboard::Key Keyboard::NUMPAD_8 = VK_NUMPAD8;
	STATIC const Keyboard::Key Keyboard::NUMPAD_9 = VK_NUMPAD9;
	STATIC const Keyboard::Key Keyboard::NUMPAD_ENTER	= VK_RETURN;
	STATIC const Keyboard::Key Keyboard::NUMPAD_PERIOD	= VK_DECIMAL;
	STATIC const Keyboard::Key Keyboard::NUMPAD_PLUS	= VK_ADD;
	STATIC const Keyboard::Key Keyboard::NUMPAD_MINUS	= VK_SUBTRACT;
	STATIC const Keyboard::Key Keyboard::NUMPAD_TIMES	= VK_MULTIPLY;
	STATIC const Keyboard::Key Keyboard::NUMPAD_DIVIDE	= VK_DIVIDE;
	#pragma endregion //Windows Keys
#elif defined( PLATFORM_HTML5 )
	//-----------------------------------------------------------------------------------------------
	#include <SDL/SDL.h>

	#pragma region SDL Keys
	//-----------------------------------------------------------------------------------------------
	STATIC const Keyboard::Key Keyboard::A = SDL_SCANCODE_A;
	STATIC const Keyboard::Key Keyboard::B = SDL_SCANCODE_B;
	STATIC const Keyboard::Key Keyboard::C = SDL_SCANCODE_C;
	STATIC const Keyboard::Key Keyboard::D = SDL_SCANCODE_D;
	STATIC const Keyboard::Key Keyboard::E = SDL_SCANCODE_E;
	STATIC const Keyboard::Key Keyboard::F = SDL_SCANCODE_F;
	STATIC const Keyboard::Key Keyboard::G = SDL_SCANCODE_G;
	STATIC const Keyboard::Key Keyboard::H = SDL_SCANCODE_H;
	STATIC const Keyboard::Key Keyboard::I = SDL_SCANCODE_I;
	STATIC const Keyboard::Key Keyboard::J = SDL_SCANCODE_J;
	STATIC const Keyboard::Key Keyboard::K = SDL_SCANCODE_K;
	STATIC const Keyboard::Key Keyboard::L = SDL_SCANCODE_L;
	STATIC const Keyboard::Key Keyboard::M = SDL_SCANCODE_M;
	STATIC const Keyboard::Key Keyboard::N = SDL_SCANCODE_N;
	STATIC const Keyboard::Key Keyboard::O = SDL_SCANCODE_O;
	STATIC const Keyboard::Key Keyboard::P = SDL_SCANCODE_P;
	STATIC const Keyboard::Key Keyboard::Q = SDL_SCANCODE_Q;
	STATIC const Keyboard::Key Keyboard::R = SDL_SCANCODE_R;
	STATIC const Keyboard::Key Keyboard::S = SDL_SCANCODE_S;
	STATIC const Keyboard::Key Keyboard::T = SDL_SCANCODE_T;
	STATIC const Keyboard::Key Keyboard::U = SDL_SCANCODE_U;
	STATIC const Keyboard::Key Keyboard::V = SDL_SCANCODE_V;
	STATIC const Keyboard::Key Keyboard::W = SDL_SCANCODE_W;
	STATIC const Keyboard::Key Keyboard::X = SDL_SCANCODE_X;
	STATIC const Keyboard::Key Keyboard::Y = SDL_SCANCODE_Y;
	STATIC const Keyboard::Key Keyboard::Z = SDL_SCANCODE_Z;

	STATIC const Keyboard::Key Keyboard::GRAVE = SDL_SCANCODE_GRAVE;
	STATIC const Keyboard::Key Keyboard::TILDE = SDL_SCANCODE_GRAVE;
	STATIC const Keyboard::Key Keyboard::NUMBER_1 = SDL_SCANCODE_1;
	STATIC const Keyboard::Key Keyboard::NUMBER_2 = SDL_SCANCODE_2;
	STATIC const Keyboard::Key Keyboard::NUMBER_3 = SDL_SCANCODE_3;
	STATIC const Keyboard::Key Keyboard::NUMBER_4 = SDL_SCANCODE_4;
	STATIC const Keyboard::Key Keyboard::NUMBER_5 = SDL_SCANCODE_5;
	STATIC const Keyboard::Key Keyboard::NUMBER_6 = SDL_SCANCODE_6;
	STATIC const Keyboard::Key Keyboard::NUMBER_7 = SDL_SCANCODE_7;
	STATIC const Keyboard::Key Keyboard::NUMBER_8 = SDL_SCANCODE_8;
	STATIC const Keyboard::Key Keyboard::NUMBER_9 = SDL_SCANCODE_9;
	STATIC const Keyboard::Key Keyboard::NUMBER_0 = SDL_SCANCODE_0;
	STATIC const Keyboard::Key Keyboard::MINUS	  = SDL_SCANCODE_MINUS;
	STATIC const Keyboard::Key Keyboard::EQUALS   = SDL_SCANCODE_EQUALS;

	STATIC const Keyboard::Key Keyboard::FUNCTION_1  = SDL_SCANCODE_F1;
	STATIC const Keyboard::Key Keyboard::FUNCTION_2  = SDL_SCANCODE_F2;
	STATIC const Keyboard::Key Keyboard::FUNCTION_3  = SDL_SCANCODE_F3;
	STATIC const Keyboard::Key Keyboard::FUNCTION_4  = SDL_SCANCODE_F4;
	STATIC const Keyboard::Key Keyboard::FUNCTION_5  = SDL_SCANCODE_F5;
	STATIC const Keyboard::Key Keyboard::FUNCTION_6  = SDL_SCANCODE_F6;
	STATIC const Keyboard::Key Keyboard::FUNCTION_7  = SDL_SCANCODE_F7;
	STATIC const Keyboard::Key Keyboard::FUNCTION_8  = SDL_SCANCODE_F8;
	STATIC const Keyboard::Key Keyboard::FUNCTION_9  = SDL_SCANCODE_F9;
	STATIC const Keyboard::Key Keyboard::FUNCTION_10 = SDL_SCANCODE_F10;
	STATIC const Keyboard::Key Keyboard::FUNCTION_11 = SDL_SCANCODE_F11;
	STATIC const Keyboard::Key Keyboard::FUNCTION_12 = SDL_SCANCODE_F12;

	STATIC const Keyboard::Key Keyboard::BACKSPACE		= SDL_SCANCODE_BACKSPACE;
	STATIC const Keyboard::Key Keyboard::SPACEBAR		= SDL_SCANCODE_SPACE;
	STATIC const Keyboard::Key Keyboard::ENTER			= SDL_SCANCODE_RETURN;
	STATIC const Keyboard::Key Keyboard::ESCAPE			= SDL_SCANCODE_ESCAPE;
	STATIC const Keyboard::Key Keyboard::TAB			= SDL_SCANCODE_TAB;
	STATIC const Keyboard::Key Keyboard::LEFT_ALT		= SDL_SCANCODE_LALT;
	STATIC const Keyboard::Key Keyboard::RIGHT_ALT		= SDL_SCANCODE_RALT;
	STATIC const Keyboard::Key Keyboard::LEFT_CONTROL	= SDL_SCANCODE_LCTRL;
	STATIC const Keyboard::Key Keyboard::RIGHT_CONTROL	= SDL_SCANCODE_RCTRL;
	STATIC const Keyboard::Key Keyboard::LEFT_SHIFT		= SDL_SCANCODE_LSHIFT;
	STATIC const Keyboard::Key Keyboard::RIGHT_SHIFT	= SDL_SCANCODE_RSHIFT;

	STATIC const Keyboard::Key Keyboard::INSERT		= SDL_SCANCODE_INSERT;
	STATIC const Keyboard::Key Keyboard::DELETE_KEY	= SDL_SCANCODE_DELETE;
	STATIC const Keyboard::Key Keyboard::HOME		= SDL_SCANCODE_HOME;
	STATIC const Keyboard::Key Keyboard::END		= SDL_SCANCODE_END;
	STATIC const Keyboard::Key Keyboard::PAGE_UP	= SDL_SCANCODE_PAGEUP;
	STATIC const Keyboard::Key Keyboard::PAGE_DOWN	= SDL_SCANCODE_PAGEDOWN;

	STATIC const Keyboard::Key Keyboard::ARROW_UP	 = SDL_SCANCODE_UP;
	STATIC const Keyboard::Key Keyboard::ARROW_DOWN	 = SDL_SCANCODE_DOWN;
	STATIC const Keyboard::Key Keyboard::ARROW_LEFT	 = SDL_SCANCODE_LEFT;
	STATIC const Keyboard::Key Keyboard::ARROW_RIGHT = SDL_SCANCODE_RIGHT;

	STATIC const Keyboard::Key Keyboard::NUMPAD_0 = SDL_SCANCODE_KP_0;
	STATIC const Keyboard::Key Keyboard::NUMPAD_1 = SDL_SCANCODE_KP_1;
	STATIC const Keyboard::Key Keyboard::NUMPAD_2 = SDL_SCANCODE_KP_2;
	STATIC const Keyboard::Key Keyboard::NUMPAD_3 = SDL_SCANCODE_KP_3;
	STATIC const Keyboard::Key Keyboard::NUMPAD_4 = SDL_SCANCODE_KP_4;
	STATIC const Keyboard::Key Keyboard::NUMPAD_5 = SDL_SCANCODE_KP_5;
	STATIC const Keyboard::Key Keyboard::NUMPAD_6 = SDL_SCANCODE_KP_6;
	STATIC const Keyboard::Key Keyboard::NUMPAD_7 = SDL_SCANCODE_KP_7;
	STATIC const Keyboard::Key Keyboard::NUMPAD_8 = SDL_SCANCODE_KP_8;
	STATIC const Keyboard::Key Keyboard::NUMPAD_9 = SDL_SCANCODE_KP_9;
	STATIC const Keyboard::Key Keyboard::NUMPAD_ENTER	= SDL_SCANCODE_KP_ENTER;
	STATIC const Keyboard::Key Keyboard::NUMPAD_PERIOD	= SDL_SCANCODE_KP_PERIOD;
	STATIC const Keyboard::Key Keyboard::NUMPAD_PLUS	= SDL_SCANCODE_KP_PLUS;
	STATIC const Keyboard::Key Keyboard::NUMPAD_MINUS	= SDL_SCANCODE_KP_MINUS;
	STATIC const Keyboard::Key Keyboard::NUMPAD_TIMES	= SDL_SCANCODE_KP_MULTIPLY;
	STATIC const Keyboard::Key Keyboard::NUMPAD_DIVIDE	= SDL_SCANCODE_KP_DIVIDE;
	#pragma endregion //SDL Keys

#elif defined( PLATFORM_ANDROID )
	//-----------------------------------------------------------------------------------------------
	#include <android/keycodes.h>

	#pragma region Android Keys
	//-----------------------------------------------------------------------------------------------
	STATIC const Keyboard::Key Keyboard::A = AKEYCODE_A;
	STATIC const Keyboard::Key Keyboard::B = AKEYCODE_B;
	STATIC const Keyboard::Key Keyboard::C = AKEYCODE_C;
	STATIC const Keyboard::Key Keyboard::D = AKEYCODE_D;
	STATIC const Keyboard::Key Keyboard::E = AKEYCODE_E;
	STATIC const Keyboard::Key Keyboard::F = AKEYCODE_F;
	STATIC const Keyboard::Key Keyboard::G = AKEYCODE_G;
	STATIC const Keyboard::Key Keyboard::H = AKEYCODE_H;
	STATIC const Keyboard::Key Keyboard::I = AKEYCODE_I;
	STATIC const Keyboard::Key Keyboard::J = AKEYCODE_J;
	STATIC const Keyboard::Key Keyboard::K = AKEYCODE_K;
	STATIC const Keyboard::Key Keyboard::L = AKEYCODE_L;
	STATIC const Keyboard::Key Keyboard::M = AKEYCODE_M;
	STATIC const Keyboard::Key Keyboard::N = AKEYCODE_N;
	STATIC const Keyboard::Key Keyboard::O = AKEYCODE_O;
	STATIC const Keyboard::Key Keyboard::P = AKEYCODE_P;
	STATIC const Keyboard::Key Keyboard::Q = AKEYCODE_Q;
	STATIC const Keyboard::Key Keyboard::R = AKEYCODE_R;
	STATIC const Keyboard::Key Keyboard::S = AKEYCODE_S;
	STATIC const Keyboard::Key Keyboard::T = AKEYCODE_T;
	STATIC const Keyboard::Key Keyboard::U = AKEYCODE_U;
	STATIC const Keyboard::Key Keyboard::V = AKEYCODE_V;
	STATIC const Keyboard::Key Keyboard::W = AKEYCODE_W;
	STATIC const Keyboard::Key Keyboard::X = AKEYCODE_X;
	STATIC const Keyboard::Key Keyboard::Y = AKEYCODE_Y;
	STATIC const Keyboard::Key Keyboard::Z = AKEYCODE_Z;

	STATIC const Keyboard::Key Keyboard::GRAVE = AKEYCODE_GRAVE;
	STATIC const Keyboard::Key Keyboard::TILDE = AKEYCODE_GRAVE;
	STATIC const Keyboard::Key Keyboard::NUMBER_1 = AKEYCODE_1;
	STATIC const Keyboard::Key Keyboard::NUMBER_2 = AKEYCODE_2;
	STATIC const Keyboard::Key Keyboard::NUMBER_3 = AKEYCODE_3;
	STATIC const Keyboard::Key Keyboard::NUMBER_4 = AKEYCODE_4;
	STATIC const Keyboard::Key Keyboard::NUMBER_5 = AKEYCODE_5;
	STATIC const Keyboard::Key Keyboard::NUMBER_6 = AKEYCODE_6;
	STATIC const Keyboard::Key Keyboard::NUMBER_7 = AKEYCODE_7;
	STATIC const Keyboard::Key Keyboard::NUMBER_8 = AKEYCODE_8;
	STATIC const Keyboard::Key Keyboard::NUMBER_9 = AKEYCODE_9;
	STATIC const Keyboard::Key Keyboard::NUMBER_0 = AKEYCODE_0;
	STATIC const Keyboard::Key Keyboard::MINUS	  = AKEYCODE_MINUS;
	STATIC const Keyboard::Key Keyboard::EQUALS   = AKEYCODE_EQUALS;

	STATIC const Keyboard::Key Keyboard::FUNCTION_1  = AKEYCODE_F1;
	STATIC const Keyboard::Key Keyboard::FUNCTION_2  = AKEYCODE_F2;
	STATIC const Keyboard::Key Keyboard::FUNCTION_3  = AKEYCODE_F3;
	STATIC const Keyboard::Key Keyboard::FUNCTION_4  = AKEYCODE_F4;
	STATIC const Keyboard::Key Keyboard::FUNCTION_5  = AKEYCODE_F5;
	STATIC const Keyboard::Key Keyboard::FUNCTION_6  = AKEYCODE_F6;
	STATIC const Keyboard::Key Keyboard::FUNCTION_7  = AKEYCODE_F7;
	STATIC const Keyboard::Key Keyboard::FUNCTION_8  = AKEYCODE_F8;
	STATIC const Keyboard::Key Keyboard::FUNCTION_9  = AKEYCODE_F9;
	STATIC const Keyboard::Key Keyboard::FUNCTION_10 = AKEYCODE_F10;
	STATIC const Keyboard::Key Keyboard::FUNCTION_11 = AKEYCODE_F11;
	STATIC const Keyboard::Key Keyboard::FUNCTION_12 = AKEYCODE_F12;

	STATIC const Keyboard::Key Keyboard::BACKSPACE		= AKEYCODE_BACK;
	STATIC const Keyboard::Key Keyboard::SPACEBAR		= AKEYCODE_SPACE;
	STATIC const Keyboard::Key Keyboard::ENTER			= AKEYCODE_ENTER;
	STATIC const Keyboard::Key Keyboard::ESCAPE			= AKEYCODE_ESCAPE;
	STATIC const Keyboard::Key Keyboard::TAB			= AKEYCODE_TAB;
	STATIC const Keyboard::Key Keyboard::LEFT_ALT		= AKEYCODE_ALT_LEFT;
	STATIC const Keyboard::Key Keyboard::RIGHT_ALT		= AKEYCODE_ALT_RIGHT;
	STATIC const Keyboard::Key Keyboard::LEFT_CONTROL	= AKEYCODE_POUND;
	STATIC const Keyboard::Key Keyboard::RIGHT_CONTROL	= AKEYCODE_STAR;
	STATIC const Keyboard::Key Keyboard::LEFT_SHIFT		= AKEYCODE_SHIFT_LEFT;
	STATIC const Keyboard::Key Keyboard::RIGHT_SHIFT	= AKEYCODE_SHIFT_RIGHT;

	STATIC const Keyboard::Key Keyboard::INSERT		= AKEYCODE_INSERT;
	STATIC const Keyboard::Key Keyboard::DELETE_KEY	= AKEYCODE_DEL;
	STATIC const Keyboard::Key Keyboard::HOME		= AKEYCODE_HOME;
	STATIC const Keyboard::Key Keyboard::END		= AKEYCODE_POUND;
	STATIC const Keyboard::Key Keyboard::PAGE_UP	= AKEYCODE_PAGE_UP;
	STATIC const Keyboard::Key Keyboard::PAGE_DOWN	= AKEYCODE_PAGE_DOWN;

	STATIC const Keyboard::Key Keyboard::ARROW_UP	 = AKEYCODE_DPAD_UP;
	STATIC const Keyboard::Key Keyboard::ARROW_DOWN	 = AKEYCODE_DPAD_DOWN;
	STATIC const Keyboard::Key Keyboard::ARROW_LEFT	 = AKEYCODE_DPAD_LEFT;
	STATIC const Keyboard::Key Keyboard::ARROW_RIGHT = AKEYCODE_DPAD_RIGHT;

	STATIC const Keyboard::Key Keyboard::NUMPAD_0 = AKEYCODE_POUND;
	STATIC const Keyboard::Key Keyboard::NUMPAD_1 = AKEYCODE_POUND;
	STATIC const Keyboard::Key Keyboard::NUMPAD_2 = AKEYCODE_POUND;
	STATIC const Keyboard::Key Keyboard::NUMPAD_3 = AKEYCODE_POUND;
	STATIC const Keyboard::Key Keyboard::NUMPAD_4 = AKEYCODE_POUND;
	STATIC const Keyboard::Key Keyboard::NUMPAD_5 = AKEYCODE_POUND;
	STATIC const Keyboard::Key Keyboard::NUMPAD_6 = AKEYCODE_POUND;
	STATIC const Keyboard::Key Keyboard::NUMPAD_7 = AKEYCODE_POUND;
	STATIC const Keyboard::Key Keyboard::NUMPAD_8 = AKEYCODE_POUND;
	STATIC const Keyboard::Key Keyboard::NUMPAD_9 = AKEYCODE_POUND;
	STATIC const Keyboard::Key Keyboard::NUMPAD_ENTER	= AKEYCODE_POUND;
	STATIC const Keyboard::Key Keyboard::NUMPAD_PERIOD	= AKEYCODE_POUND;
	STATIC const Keyboard::Key Keyboard::NUMPAD_PLUS	= AKEYCODE_POUND;
	STATIC const Keyboard::Key Keyboard::NUMPAD_MINUS	= AKEYCODE_POUND;
	STATIC const Keyboard::Key Keyboard::NUMPAD_TIMES	= AKEYCODE_POUND;
	STATIC const Keyboard::Key Keyboard::NUMPAD_DIVIDE	= AKEYCODE_POUND;
#pragma endregion //Android Keys

#elif defined( PLATFORM_PS3 )

#pragma region PS3 Keys
//-----------------------------------------------------------------------------------------------
#include <cell/keyboard.h>

//-----------------------------------------------------------------------------------------------
STATIC const Keyboard::Key Keyboard::A = CELL_KEYC_A;
STATIC const Keyboard::Key Keyboard::B = CELL_KEYC_B;
STATIC const Keyboard::Key Keyboard::C = CELL_KEYC_C;
STATIC const Keyboard::Key Keyboard::D = CELL_KEYC_D;
STATIC const Keyboard::Key Keyboard::E = CELL_KEYC_E;
STATIC const Keyboard::Key Keyboard::F = CELL_KEYC_F;
STATIC const Keyboard::Key Keyboard::G = CELL_KEYC_G;
STATIC const Keyboard::Key Keyboard::H = CELL_KEYC_H;
STATIC const Keyboard::Key Keyboard::I = CELL_KEYC_I;
STATIC const Keyboard::Key Keyboard::J = CELL_KEYC_J;
STATIC const Keyboard::Key Keyboard::K = CELL_KEYC_K;
STATIC const Keyboard::Key Keyboard::L = CELL_KEYC_L;
STATIC const Keyboard::Key Keyboard::M = CELL_KEYC_M;
STATIC const Keyboard::Key Keyboard::N = CELL_KEYC_N;
STATIC const Keyboard::Key Keyboard::O = CELL_KEYC_O;
STATIC const Keyboard::Key Keyboard::P = CELL_KEYC_P;
STATIC const Keyboard::Key Keyboard::Q = CELL_KEYC_Q;
STATIC const Keyboard::Key Keyboard::R = CELL_KEYC_R;
STATIC const Keyboard::Key Keyboard::S = CELL_KEYC_S;
STATIC const Keyboard::Key Keyboard::T = CELL_KEYC_T;
STATIC const Keyboard::Key Keyboard::U = CELL_KEYC_U;
STATIC const Keyboard::Key Keyboard::V = CELL_KEYC_V;
STATIC const Keyboard::Key Keyboard::W = CELL_KEYC_W;
STATIC const Keyboard::Key Keyboard::X = CELL_KEYC_X;
STATIC const Keyboard::Key Keyboard::Y = CELL_KEYC_Y;
STATIC const Keyboard::Key Keyboard::Z = CELL_KEYC_Z;

STATIC const Keyboard::Key Keyboard::GRAVE = CELL_KEYC_ACCENT_CIRCONFLEX_106;
STATIC const Keyboard::Key Keyboard::TILDE = CELL_KEYC_ACCENT_CIRCONFLEX_106;
STATIC const Keyboard::Key Keyboard::NUMBER_1 = CELL_KEYC_1;
STATIC const Keyboard::Key Keyboard::NUMBER_2 = CELL_KEYC_2;
STATIC const Keyboard::Key Keyboard::NUMBER_3 = CELL_KEYC_3;
STATIC const Keyboard::Key Keyboard::NUMBER_4 = CELL_KEYC_4;
STATIC const Keyboard::Key Keyboard::NUMBER_5 = CELL_KEYC_5;
STATIC const Keyboard::Key Keyboard::NUMBER_6 = CELL_KEYC_6;
STATIC const Keyboard::Key Keyboard::NUMBER_7 = CELL_KEYC_7;
STATIC const Keyboard::Key Keyboard::NUMBER_8 = CELL_KEYC_8;
STATIC const Keyboard::Key Keyboard::NUMBER_9 = CELL_KEYC_9;
STATIC const Keyboard::Key Keyboard::NUMBER_0 = CELL_KEYC_0;
STATIC const Keyboard::Key Keyboard::MINUS	  = CELL_KEYC_MINUS;
STATIC const Keyboard::Key Keyboard::EQUALS   = CELL_KEYC_EQUAL_101;

STATIC const Keyboard::Key Keyboard::FUNCTION_1  = CELL_KEYC_F1;
STATIC const Keyboard::Key Keyboard::FUNCTION_2  = CELL_KEYC_F2;
STATIC const Keyboard::Key Keyboard::FUNCTION_3  = CELL_KEYC_F3;
STATIC const Keyboard::Key Keyboard::FUNCTION_4  = CELL_KEYC_F4;
STATIC const Keyboard::Key Keyboard::FUNCTION_5  = CELL_KEYC_F5;
STATIC const Keyboard::Key Keyboard::FUNCTION_6  = CELL_KEYC_F6;
STATIC const Keyboard::Key Keyboard::FUNCTION_7  = CELL_KEYC_F7;
STATIC const Keyboard::Key Keyboard::FUNCTION_8  = CELL_KEYC_F8;
STATIC const Keyboard::Key Keyboard::FUNCTION_9  = CELL_KEYC_F9;
STATIC const Keyboard::Key Keyboard::FUNCTION_10 = CELL_KEYC_F10;
STATIC const Keyboard::Key Keyboard::FUNCTION_11 = CELL_KEYC_F11;
STATIC const Keyboard::Key Keyboard::FUNCTION_12 = CELL_KEYC_F12;

STATIC const Keyboard::Key Keyboard::BACKSPACE		= CELL_KEYC_BS;
STATIC const Keyboard::Key Keyboard::SPACEBAR		= CELL_KEYC_SPACE;
STATIC const Keyboard::Key Keyboard::ENTER			= CELL_KEYC_ENTER;
STATIC const Keyboard::Key Keyboard::ESCAPE			= CELL_KEYC_ESCAPE;
STATIC const Keyboard::Key Keyboard::TAB			= CELL_KEYC_TAB;
STATIC const Keyboard::Key Keyboard::LEFT_ALT		= CELL_KB_MKEY_L_ALT;
STATIC const Keyboard::Key Keyboard::RIGHT_ALT		= CELL_KB_MKEY_R_ALT;
STATIC const Keyboard::Key Keyboard::LEFT_CONTROL	= CELL_KB_MKEY_L_CTRL;
STATIC const Keyboard::Key Keyboard::RIGHT_CONTROL	= CELL_KB_MKEY_R_CTRL;
STATIC const Keyboard::Key Keyboard::LEFT_SHIFT		= CELL_KB_MKEY_L_SHIFT;
STATIC const Keyboard::Key Keyboard::RIGHT_SHIFT	= CELL_KB_MKEY_R_SHIFT;

STATIC const Keyboard::Key Keyboard::INSERT		= CELL_KEYC_INSERT;
STATIC const Keyboard::Key Keyboard::DELETE_KEY	= CELL_KEYC_DELETE;
STATIC const Keyboard::Key Keyboard::HOME		= CELL_KEYC_HOME;
STATIC const Keyboard::Key Keyboard::END		= CELL_KEYC_END;
STATIC const Keyboard::Key Keyboard::PAGE_UP	= CELL_KEYC_PAGE_UP;
STATIC const Keyboard::Key Keyboard::PAGE_DOWN	= CELL_KEYC_PAGE_DOWN;

STATIC const Keyboard::Key Keyboard::ARROW_UP	 = CELL_KEYC_UP_ARROW;
STATIC const Keyboard::Key Keyboard::ARROW_DOWN	 = CELL_KEYC_DOWN_ARROW;
STATIC const Keyboard::Key Keyboard::ARROW_LEFT	 = CELL_KEYC_LEFT_ARROW;
STATIC const Keyboard::Key Keyboard::ARROW_RIGHT = CELL_KEYC_RIGHT_ARROW;

STATIC const Keyboard::Key Keyboard::NUMPAD_0 = CELL_KEYC_KPAD_0;
STATIC const Keyboard::Key Keyboard::NUMPAD_1 = CELL_KEYC_KPAD_1;
STATIC const Keyboard::Key Keyboard::NUMPAD_2 = CELL_KEYC_KPAD_2;
STATIC const Keyboard::Key Keyboard::NUMPAD_3 = CELL_KEYC_KPAD_3;
STATIC const Keyboard::Key Keyboard::NUMPAD_4 = CELL_KEYC_KPAD_4;
STATIC const Keyboard::Key Keyboard::NUMPAD_5 = CELL_KEYC_KPAD_5;
STATIC const Keyboard::Key Keyboard::NUMPAD_6 = CELL_KEYC_KPAD_6;
STATIC const Keyboard::Key Keyboard::NUMPAD_7 = CELL_KEYC_KPAD_7;
STATIC const Keyboard::Key Keyboard::NUMPAD_8 = CELL_KEYC_KPAD_8;
STATIC const Keyboard::Key Keyboard::NUMPAD_9 = CELL_KEYC_KPAD_9;
STATIC const Keyboard::Key Keyboard::NUMPAD_ENTER	= CELL_KEYC_KPAD_ENTER;
STATIC const Keyboard::Key Keyboard::NUMPAD_PERIOD	= CELL_KEYC_KPAD_PERIOD;
STATIC const Keyboard::Key Keyboard::NUMPAD_PLUS	= CELL_KEYC_KPAD_PLUS;
STATIC const Keyboard::Key Keyboard::NUMPAD_MINUS	= CELL_KEYC_KPAD_MINUS;
STATIC const Keyboard::Key Keyboard::NUMPAD_TIMES	= CELL_KEYC_KPAD_ASTERISK;
STATIC const Keyboard::Key Keyboard::NUMPAD_DIVIDE	= CELL_KEYC_KPAD_SLASH;
#pragma endregion //PS3 Keys

#else

#pragma region Platforms Without Keyboards
//-----------------------------------------------------------------------------------------------
STATIC const Keyboard::Key Keyboard::A = 'A';
STATIC const Keyboard::Key Keyboard::B = 'B';
STATIC const Keyboard::Key Keyboard::C = 'C';
STATIC const Keyboard::Key Keyboard::D = 'D';
STATIC const Keyboard::Key Keyboard::E = 'E';
STATIC const Keyboard::Key Keyboard::F = 'F';
STATIC const Keyboard::Key Keyboard::G = 'G';
STATIC const Keyboard::Key Keyboard::H = 'H';
STATIC const Keyboard::Key Keyboard::I = 'I';
STATIC const Keyboard::Key Keyboard::J = 'J';
STATIC const Keyboard::Key Keyboard::K = 'K';
STATIC const Keyboard::Key Keyboard::L = 'L';
STATIC const Keyboard::Key Keyboard::M = 'M';
STATIC const Keyboard::Key Keyboard::N = 'N';
STATIC const Keyboard::Key Keyboard::O = 'O';
STATIC const Keyboard::Key Keyboard::P = 'P';
STATIC const Keyboard::Key Keyboard::Q = 'Q';
STATIC const Keyboard::Key Keyboard::R = 'R';
STATIC const Keyboard::Key Keyboard::S = 'S';
STATIC const Keyboard::Key Keyboard::T = 'T';
STATIC const Keyboard::Key Keyboard::U = 'U';
STATIC const Keyboard::Key Keyboard::V = 'V';
STATIC const Keyboard::Key Keyboard::W = 'W';
STATIC const Keyboard::Key Keyboard::X = 'X';
STATIC const Keyboard::Key Keyboard::Y = 'Y';
STATIC const Keyboard::Key Keyboard::Z = 'Z';

STATIC const Keyboard::Key Keyboard::GRAVE = '`';
STATIC const Keyboard::Key Keyboard::TILDE = '~';
STATIC const Keyboard::Key Keyboard::NUMBER_1 = '1';
STATIC const Keyboard::Key Keyboard::NUMBER_2 = '2';
STATIC const Keyboard::Key Keyboard::NUMBER_3 = '3';
STATIC const Keyboard::Key Keyboard::NUMBER_4 = '4';
STATIC const Keyboard::Key Keyboard::NUMBER_5 = '5';
STATIC const Keyboard::Key Keyboard::NUMBER_6 = '6';
STATIC const Keyboard::Key Keyboard::NUMBER_7 = '7';
STATIC const Keyboard::Key Keyboard::NUMBER_8 = '8';
STATIC const Keyboard::Key Keyboard::NUMBER_9 = '9';
STATIC const Keyboard::Key Keyboard::NUMBER_0 = '0';
STATIC const Keyboard::Key Keyboard::MINUS	  = '-';
STATIC const Keyboard::Key Keyboard::EQUALS   = '=';

STATIC const Keyboard::Key Keyboard::FUNCTION_1  = 0x1;
STATIC const Keyboard::Key Keyboard::FUNCTION_2  = 0x2;
STATIC const Keyboard::Key Keyboard::FUNCTION_3  = 0x3;
STATIC const Keyboard::Key Keyboard::FUNCTION_4  = 0x4;
STATIC const Keyboard::Key Keyboard::FUNCTION_5  = 0x5;
STATIC const Keyboard::Key Keyboard::FUNCTION_6  = 0x6;
STATIC const Keyboard::Key Keyboard::FUNCTION_7  = 0x7;
STATIC const Keyboard::Key Keyboard::FUNCTION_8  = 0x8;
STATIC const Keyboard::Key Keyboard::FUNCTION_9  = 0x9;
STATIC const Keyboard::Key Keyboard::FUNCTION_10 = 0x10;
STATIC const Keyboard::Key Keyboard::FUNCTION_11 = 0x11;
STATIC const Keyboard::Key Keyboard::FUNCTION_12 = 0x12;

STATIC const Keyboard::Key Keyboard::BACKSPACE		= 0x13;
STATIC const Keyboard::Key Keyboard::SPACEBAR		= 0x14;
STATIC const Keyboard::Key Keyboard::ENTER			= 0x15;
STATIC const Keyboard::Key Keyboard::ESCAPE			= 0x16;
STATIC const Keyboard::Key Keyboard::TAB			= 0x17;
STATIC const Keyboard::Key Keyboard::LEFT_ALT		= 0x18;
STATIC const Keyboard::Key Keyboard::RIGHT_ALT		= 0x19;
STATIC const Keyboard::Key Keyboard::LEFT_CONTROL	= 0x20;
STATIC const Keyboard::Key Keyboard::RIGHT_CONTROL	= 0x21;
STATIC const Keyboard::Key Keyboard::LEFT_SHIFT		= 0x22;
STATIC const Keyboard::Key Keyboard::RIGHT_SHIFT	= 0x23;

STATIC const Keyboard::Key Keyboard::INSERT		= 0x24;
STATIC const Keyboard::Key Keyboard::DELETE_KEY	= 0x25;
STATIC const Keyboard::Key Keyboard::HOME		= 0x26;
STATIC const Keyboard::Key Keyboard::END		= 0x27;
STATIC const Keyboard::Key Keyboard::PAGE_UP	= 0x28;
STATIC const Keyboard::Key Keyboard::PAGE_DOWN	= 0x29;

STATIC const Keyboard::Key Keyboard::ARROW_UP	 = '\'';
STATIC const Keyboard::Key Keyboard::ARROW_DOWN	 = ';';
STATIC const Keyboard::Key Keyboard::ARROW_LEFT	 = '<';
STATIC const Keyboard::Key Keyboard::ARROW_RIGHT = '>';

STATIC const Keyboard::Key Keyboard::NUMPAD_0 = ')';
STATIC const Keyboard::Key Keyboard::NUMPAD_1 = '!';
STATIC const Keyboard::Key Keyboard::NUMPAD_2 = '@';
STATIC const Keyboard::Key Keyboard::NUMPAD_3 = '#';
STATIC const Keyboard::Key Keyboard::NUMPAD_4 = '$';
STATIC const Keyboard::Key Keyboard::NUMPAD_5 = '%';
STATIC const Keyboard::Key Keyboard::NUMPAD_6 = '^';
STATIC const Keyboard::Key Keyboard::NUMPAD_7 = '&';
STATIC const Keyboard::Key Keyboard::NUMPAD_8 = '*';
STATIC const Keyboard::Key Keyboard::NUMPAD_9 = '(';
STATIC const Keyboard::Key Keyboard::NUMPAD_ENTER	= '\\';
STATIC const Keyboard::Key Keyboard::NUMPAD_PERIOD	= '.';
STATIC const Keyboard::Key Keyboard::NUMPAD_PLUS	= '+';
STATIC const Keyboard::Key Keyboard::NUMPAD_MINUS	= '_';
STATIC const Keyboard::Key Keyboard::NUMPAD_TIMES	= ',';
STATIC const Keyboard::Key Keyboard::NUMPAD_DIVIDE	= '/';
#pragma endregion //PS3 Keys

#endif

#pragma endregion //Key Definitions

//-----------------------------------------------------------------------------------------------
void Keyboard::SetKeyDown( Key key )
{
	switch( key )
	{
	case LEFT_ALT:
	case RIGHT_ALT:
		SetBitInMask( MODIFIER_ALT, m_keyModifiers );
		break;

	case LEFT_CONTROL:
	case RIGHT_CONTROL:
		SetBitInMask( MODIFIER_CONTROL, m_keyModifiers );
		break;

	case LEFT_SHIFT:
	case RIGHT_SHIFT:
		SetBitInMask( MODIFIER_SHIFT, m_keyModifiers );
		break;

	default:
		break;
	}

	m_isKeyDown[ key ] = true;
}

//-----------------------------------------------------------------------------------------------
void Keyboard::SetKeyUp( Key key )
{
	switch( key )
	{
	case LEFT_ALT:
	case RIGHT_ALT:
		UnsetBitInMask( MODIFIER_ALT, m_keyModifiers );
		break;

	case LEFT_CONTROL:
	case RIGHT_CONTROL:
		UnsetBitInMask( MODIFIER_CONTROL, m_keyModifiers );
		break;

	case LEFT_SHIFT:
	case RIGHT_SHIFT:
		UnsetBitInMask( MODIFIER_SHIFT, m_keyModifiers );
		break;

	default:
		break;
	}

	m_isKeyDown[ key ] = false;
	m_wasKeyDown[ key ] = false;
}
