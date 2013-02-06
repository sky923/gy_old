#ifndef __gy_pil_gyEvent_h__
#define __gy_pil_gyEvent_h__

#include "gyAtomicDataType.h"

namespace gy
{
	namespace mouse
	{
#define MAX_MOUSE_BUTTON_SIZE 10
		const char GY_BUTTON_LEFT = 0; ///< left button index (= GY_BUTTON_0)
		const char GY_BUTTON_RIGHT = 1; ///< right button index (= GY_BUTTON_1)
		const char GY_BUTTON_MIDDLE = 2; ///< middle(wheel) button index (= GY_BUTTON_2)
		const char GY_BUTTON_0 = 0;
		const char GY_BUTTON_1 = 1;
		const char GY_BUTTON_2 = 2;
		const char GY_BUTTON_3 = 3;
		const char GY_BUTTON_4 = 4;
		const char GY_BUTTON_5 = 5;
		const char GY_BUTTON_6 = 6;
		const char GY_BUTTON_7 = 7;
		const char GY_BUTTON_8 = 8;
		const char GY_BUTTON_9 = 9;
		//bool is_button_down[MAX_MOUSE_BUTTON_SIZE] = {false};
	}

	namespace keyboard
	{
#define MAX_KEYBOARD_KEY_SIZE 255
		// functional key
		const char GY_KEY_BACKSPACE = 8;
		const char GY_KEY_HTAB = 9;
		const char GY_KEY_ESCAPE = 27;
		const char GY_KEY_DELETE = 127;

		// glyph characters
		const char GY_KEY_SPACE = 32; ///<  
		const char GY_KEY_EXCLAM = 33; ///< !
		const char GY_KEY_DQUOTA = 34; ///< "
		const char GY_KEY_SHARP = 35; ///< #
		const char GY_KEY_DOLLAR = 36; ///< $
		const char GY_KEY_PERCENT = 37; ///< %
		const char GY_KEY_AMPERSAND = 38; ///< &
		const char GY_KEY_SQUOTA = 39; ///< '
		const char GY_KEY_RBOPEN = 40; ///< (
		const char GY_KEY_RBCLOSE = 41; ///< )
		const char GY_KEY_ASTERISK = 42; ///< *
		const char GY_KEY_PLUS = 43; ///< +
		const char GY_KEY_COMMA = 44; ///< ,
		const char GY_KEY_MINUS = 45; ///< -
		const char GY_KEY_FULLSTOP = 46; ///< .
		const char GY_KEY_SLASH = 47; ///< /
		const char GY_KEY_0 = 48; ///< 0
		const char GY_KEY_1 = 49; ///< 1
		const char GY_KEY_2 = 50; ///< 2
		const char GY_KEY_3 = 51; ///< 3
		const char GY_KEY_4 = 52; ///< 4
		const char GY_KEY_5 = 53; ///< 5
		const char GY_KEY_6 = 54; ///< 6
		const char GY_KEY_7 = 55; ///< 7
		const char GY_KEY_8 = 56; ///< 8
		const char GY_KEY_9 = 57; ///< 9
		const char GY_KEY_COLON = 58; ///< :
		const char GY_KEY_SEMICOLON = 59; ///< ;
		const char GY_KEY_LESSTHAN = 60; ///< <
		const char GY_KEY_EQUAL = 61; ///< =
		const char GY_KEY_GREATERTHAN = 62; ///< >
		const char GY_KEY_QUESTION = 63; ///< ?
		const char GY_KEY_AT = 64; ///< @
		const char GY_KEY_UPPER_A = 65; ///< A
		const char GY_KEY_UPPER_B = 66; ///< B
		const char GY_KEY_UPPER_C = 67; ///< C
		const char GY_KEY_UPPER_D = 68; ///< D
		const char GY_KEY_UPPER_E = 69; ///< E
		const char GY_KEY_UPPER_F = 70; ///< F
		const char GY_KEY_UPPER_G = 71; ///< G
		const char GY_KEY_UPPER_H = 72; ///< H
		const char GY_KEY_UPPER_I = 73; ///< I
		const char GY_KEY_UPPER_J = 74; ///< J
		const char GY_KEY_UPPER_K = 75; ///< K
		const char GY_KEY_UPPER_L = 76; ///< L
		const char GY_KEY_UPPER_M = 77; ///< M
		const char GY_KEY_UPPER_N = 78; ///< N
		const char GY_KEY_UPPER_O = 79; ///< O
		const char GY_KEY_UPPER_P = 80; ///< P
		const char GY_KEY_UPPER_Q = 81; ///< Q
		const char GY_KEY_UPPER_R = 82; ///< R
		const char GY_KEY_UPPER_S = 83; ///< S
		const char GY_KEY_UPPER_T = 84; ///< T
		const char GY_KEY_UPPER_U = 85; ///< U
		const char GY_KEY_UPPER_V = 86; ///< V
		const char GY_KEY_UPPER_W = 87; ///< W
		const char GY_KEY_UPPER_X = 88; ///< X
		const char GY_KEY_UPPER_Y = 89; ///< Y
		const char GY_KEY_UPPER_Z = 90; ///< Z
		const char GY_KEY_SBOPEN = 91; ///< [
		const char GY_KEY_BACKSLASH = 92; ///< \ 
		const char GY_KEY_SBCLOSE = 93; ///< ]
		const char GY_KEY_CARET = 94; ///< ^
		const char GY_KEY_UNDERSCORE = 95; ///< _
		const char GY_KEY_GRAVE = 96; ///< `
		const char GY_KEY_LOWER_A = 97; ///< a
		const char GY_KEY_LOWER_B = 98; ///< b
		const char GY_KEY_LOWER_C = 99; ///< c
		const char GY_KEY_LOWER_D = 100; ///< d
		const char GY_KEY_LOWER_E = 101; ///< e
		const char GY_KEY_LOWER_F = 102; ///< f
		const char GY_KEY_LOWER_G = 103; ///< g
		const char GY_KEY_LOWER_H = 104; ///< h
		const char GY_KEY_LOWER_I = 105; ///< i
		const char GY_KEY_LOWER_J = 106; ///< j
		const char GY_KEY_LOWER_K = 107; ///< k
		const char GY_KEY_LOWER_L = 108; ///< l
		const char GY_KEY_LOWER_M = 109; ///< m
		const char GY_KEY_LOWER_N = 110; ///< n
		const char GY_KEY_LOWER_O = 111; ///< o
		const char GY_KEY_LOWER_P = 112; ///< p
		const char GY_KEY_LOWER_Q = 113; ///< q
		const char GY_KEY_LOWER_R = 114; ///< r
		const char GY_KEY_LOWER_S = 115; ///< s
		const char GY_KEY_LOWER_T = 116; ///< t
		const char GY_KEY_LOWER_U = 117; ///< u
		const char GY_KEY_LOWER_V = 118; ///< v
		const char GY_KEY_LOWER_W = 119; ///< w
		const char GY_KEY_LOWER_X = 120; ///< x
		const char GY_KEY_LOWER_Y = 121; ///< y
		const char GY_KEY_LOWER_Z = 122; ///< z
		const char GY_KEY_BOPEN = 123; ///< {
		const char GY_KEY_VBAR = 124; ///< |
		const char GY_KEY_BCLOSE = 125; ///< }
		const char GY_KEY_TILDE = 126; ///< ~
	}
}

#endif