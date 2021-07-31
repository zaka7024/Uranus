#pragma once

// Todo:: Use enum insted of macros

#define UR_KEY_SPACE              32
#define UR_KEY_APOSTROPHE         39  /* ' */
#define UR_KEY_COMMA              44  /* , */
#define UR_KEY_MINUS              45  /* - */
#define UR_KEY_PERIOD             46  /* . */
#define UR_KEY_SLASH              47  /* / */
#define UR_KEY_0                  48
#define UR_KEY_1                  49
#define UR_KEY_2                  50
#define UR_KEY_3                  51
#define UR_KEY_4                  52
#define UR_KEY_5                  53
#define UR_KEY_6                  54
#define UR_KEY_7                  55
#define UR_KEY_8                  56
#define UR_KEY_9                  57
#define UR_KEY_SEMICOLON          59  /* ; */
#define UR_KEY_EQUAL              61  /* = */
#define UR_KEY_A                  65
#define UR_KEY_B                  66
#define UR_KEY_C                  67
#define UR_KEY_D                  68
#define UR_KEY_E                  69
#define UR_KEY_F                  70
#define UR_KEY_G                  71
#define UR_KEY_H                  72
#define UR_KEY_I                  73
#define UR_KEY_J                  74
#define UR_KEY_K                  75
#define UR_KEY_L                  76
#define UR_KEY_M                  77
#define UR_KEY_N                  78
#define UR_KEY_O                  79
#define UR_KEY_P                  80
#define UR_KEY_Q                  81
#define UR_KEY_R                  82
#define UR_KEY_S                  83
#define UR_KEY_T                  84
#define UR_KEY_U                  85
#define UR_KEY_V                  86
#define UR_KEY_W                  87
#define UR_KEY_X                  88
#define UR_KEY_Y                  89
#define UR_KEY_Z                  90
#define UR_KEY_LEFT_BRACKET       91  /* [ */
#define UR_KEY_BACKSLASH          92  /* \ */
#define UR_KEY_RIGHT_BRACKET      93  /* ] */
#define UR_KEY_GRAVE_ACCENT       96  /* ` */
#define UR_KEY_WORLD_1            161 /* non-US #1 */
#define UR_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define UR_KEY_ESCAPE             256
#define UR_KEY_ENTER              257
#define UR_KEY_TAB                258
#define UR_KEY_BACKSPACE          259
#define UR_KEY_INSERT             260
#define UR_KEY_DELETE             261
#define UR_KEY_RIGHT              262
#define UR_KEY_LEFT               263
#define UR_KEY_DOWN               264
#define UR_KEY_UP                 265
#define UR_KEY_PAGE_UP            266
#define UR_KEY_PAGE_DOWN          267
#define UR_KEY_HOME               268
#define UR_KEY_END                269
#define UR_KEY_CAPS_LOCK          280
#define UR_KEY_SCROLL_LOCK        281
#define UR_KEY_NUM_LOCK           282
#define UR_KEY_PRINT_SCREEN       283
#define UR_KEY_PAUSE              284
#define UR_KEY_F1                 290
#define UR_KEY_F2                 291
#define UR_KEY_F3                 292
#define UR_KEY_F4                 293
#define UR_KEY_F5                 294
#define UR_KEY_F6                 295
#define UR_KEY_F7                 296
#define UR_KEY_F8                 297
#define UR_KEY_F9                 298
#define UR_KEY_F10                299
#define UR_KEY_F11                300
#define UR_KEY_F12                301
#define UR_KEY_F13                302
#define UR_KEY_F14                303
#define UR_KEY_F15                304
#define UR_KEY_F16                305
#define UR_KEY_F17                306
#define UR_KEY_F18                307
#define UR_KEY_F19                308
#define UR_KEY_F20                309
#define UR_KEY_F21                310
#define UR_KEY_F22                311
#define UR_KEY_F23                312
#define UR_KEY_F24                313
#define UR_KEY_F25                314
#define UR_KEY_KP_0               320
#define UR_KEY_KP_1               321
#define UR_KEY_KP_2               322
#define UR_KEY_KP_3               323
#define UR_KEY_KP_4               324
#define UR_KEY_KP_5               325
#define UR_KEY_KP_6               326
#define UR_KEY_KP_7               327
#define UR_KEY_KP_8               328
#define UR_KEY_KP_9               329
#define UR_KEY_KP_DECIMAL         330
#define UR_KEY_KP_DIVIDE          331
#define UR_KEY_KP_MULTIPLY        332
#define UR_KEY_KP_SUBTRACT        333
#define UR_KEY_KP_ADD             334
#define UR_KEY_KP_ENTER           335
#define UR_KEY_KP_EQUAL           336
#define UR_KEY_LEFT_SHIFT         340
#define UR_KEY_LEFT_CONTROL       341
#define UR_KEY_LEFT_ALT           342
#define UR_KEY_LEFT_SUPER         343
#define UR_KEY_RIGHT_SHIFT        344
#define UR_KEY_RIGHT_CONTROL      345
#define UR_KEY_RIGHT_ALT          346
#define UR_KEY_RIGHT_SUPER        347
#define UR_KEY_MENU               348

enum class KeyCode {
	SPACE				= 32,
	APOSTROPHE			= 39,  /* ' */
	COMMA				= 44,  /* , */
	MINUS				= 45,  /* - */
	PERIOD				= 46,  /* . */
	SLASH				= 47,  /* / */
	KEY_0				= 48,
	KEY_1				= 49,
	KEY_2				= 50,
	KEY_3				= 51,
	KEY_4				= 52,
	KEY_5				= 53,
	KEY_6				= 54,
	KEY_7				= 55,
	KEY_8				= 56,
	KEY_9				= 57,
	SEMICOLON			= 59,  /* ; */
	EQUAL				= 61,  /* = */
	A					= 65,
	B					= 66,
	C					= 67,
	D					= 68,
	E					= 69,
	F					= 70,
	G					= 71,
	H					= 72,
	I					= 73,
	J					= 74,
	K					= 75,
	L					= 76,
	M					= 77,
	N					= 78,
	O					= 79,
	P					= 80,
	Q					= 81,
	R					= 82,
	S					= 83,
	T					= 84,
	U					= 85,
	V					= 86,
	W					= 87,
	X					= 88,
	Y					= 89,
	Z					= 90,
	LEFT_BRACKET		= 91,  /* [ */
	BACKSLASH			= 92,  /* \ */
	RIGHT_BRACKET		= 93,  /* ] */
	GRAVE_ACCENT		= 96,  /* ` */
	WORLD_1				= 161, /* non-US #1 */
	WORLD_2				= 162,/* non-US #2 */

	Escape				= 256,
	Enter				= 257,
	Tab					= 258,
	Backspace			= 259,
	Insert				= 260,
	Delete				= 261,
	Right				= 262,
	Left				= 263,
	Down				= 264,
	Up					= 265,
	PAGE_UP				= 266,
	PAGE_DOWN			= 267,
	HOME				= 268,
	END					= 269,
	CAPS_LOCK			= 280,
	SCROLL_LOCK			= 281,
	NUM_LOCK			= 282,
	PRINT_SCREEN		= 283,
	PAUSE				= 284,
	F1					= 290,
	F2					= 291,
	F3					= 292,
	F4					= 293,
	F5					= 294,
	F6					= 295,
	F7					= 296,
	F8					= 297,
	F9					= 298,
	F10					= 299,
	F11					= 300,
	F12					= 301,
	F13					= 302,
	F14					= 303,
	F15					= 304,
	F16					= 305,
	F17					= 306,
	F18					= 307,
	F19					= 308,
	F20					= 309,
	F21					= 310,
	F22					= 311,
	F23					= 312,
	F24					= 313,
	F25					= 314,
	KP_0				= 320,
	KP_1				= 321,
	KP_2				= 322,
	KP_3				= 323,
	KP_4				= 324,
	KP_5				= 325,
	KP_6				= 326,
	KP_7				= 327,
	KP_8				= 328,
	KP_9				= 329,
	KP_DECIMAL			= 330,
	KP_DIVIDE			= 331,
	KP_MULTIPLY			= 332,
	KP_SUBTRACT			= 333,
	KP_ADD				= 334,
	KP_ENTER			= 335,
	KP_EQUAL			= 336,
	LeftShift			= 340,
	LEFT_CONTROL		= 341,
	LeftAlt				= 342,
	LEFT_SUPER			= 343,
	RIGHT_SHIFT			= 344,
	RIGHT_CONTROL		= 345,
	RIGHT_ALT			= 346,
	RIGHT_SUPER			= 347,
	MENU				= 348,
};
