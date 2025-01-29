#pragma once

enum class Actions {
	ATTACK,
	AOO,
	CHANGE_EQUIP,
	DISENGAGE,
	EVADE,
	EXAMINE,
	FALL_PRONE,
	HELP,
	HIDE,
	JUMP,
	MOVE,
	SHOVE,
	STAND,
	THROW_ITEM
};

enum class Classes {
	ARCANIST,
	ROGUE,
	WARRIOR
};

enum class Menus {
	GO,
	LEVSEL,
	MAIN,
	OPTIONS,
	PAUSE
};

enum class Scenes {
	GAME,
	TITLE
};

enum class Sizes {
	TINY,
	SMALL,
	MED,
	BIG,
	LARGE,
	HUGE,
	MASSIVE,
	COLOSSAL
};

enum class UIElems {
	DEFAULT,
	APPLY,
	BACK,
	FULLSCREEN,
	LEVSEL,
	LEV1,
	LEV2,
	LEV3,
	MAINMENU,
	MUSICV,
	OPTIONS,
	QUIT,
	RESOLUTION,
	RESUME,
	SFXV
};