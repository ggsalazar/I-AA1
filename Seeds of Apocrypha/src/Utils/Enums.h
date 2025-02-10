#pragma once

//Skills & Scores
enum class Ab_Scores {
	STR,
	CON,
	AGI,
	DEX,
	INT,
	WIS,
	CHA
};

enum class A_Skills {
	APPRAISAL,
	ARCANA,
	BEAST_HANDLING,
	BEGUILEMENT,
	CLIMB,
	INSIGHT,
	INTIMIDATION,
	LORE,
	MECHANICS,
	MEDICINE,
	NATURE,
	NAVIGATION,
	PERCEPTION,
	PERSUASION,
	SOH,
	STEALTH,
	SURVIVAL,
	SWIM
};

enum class T_Skills {
	MED_KITS,
	THIEVING_KITS
};

enum class W_Skills {
	ARCHERY,
	FIREARM,
	LARGE,
	MED,
	POLEARM,
	SMALL,
	THROWING,
	UNARMED
};


//Mechanics
enum class Actions {
	ATTACK_M,
	ATTACK_R,
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

enum class Conditions {
	BLIND,
	CHARMED,
	DEAF,
	PRONE,
	SICK,
	STUNNED,
	UNCONSCIOUS
};

enum class Classes {
	NONE,
	ARCANIST,
	ROGUE,
	WARRIOR
};

enum class Genuses {
	BEAST,
	MONSTROSITY,
	OTHER,
	SCORCHED,
	SENTIENT,
	UNDEAD
};

enum class Items {

};

enum class PreGens {
	SORINA, //Female Elf Arcanist
	ESSEK, //Male Kobold Rogue
	DAKN //Male Dwarf Warrior
};

enum class Races {
	AUTOMATON,
	DWARF,
	ELF,
	GNOME,
	HUMAN,
	KOBOLD
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

enum class Terrain {
	NORMAL,
	ROUGH
};

enum class Weathers {
	CLEAR
};


//Game details
enum class Cutscenes {

};

enum class Areas {
	DEFAULT,
	TUTTON
};


//Game Structure
enum class Menus {
	CHARCREA,
	GO,
	INVENTORY,
	LEVELUP,
	LOAD,
	MAIN,
	OPTIONS,
	PAUSE,
	//Sub-Menus
	//CC
	CCAS,
	CCBG,
	CCCLASS,
	CCEQUIP,
	CCRACE,
	CCSKILLS
};

enum class Scenes {
	CUTSCENE,
	AREA,
	TITLE
};

enum class UIElems {
	DEFAULT,
	//Buttons
	APPLY,
	AS,
	BACK,
	BACKGROUND_B,
	CHARCREA,
	CREATE,
	EQUIPMENT_CC,
	FULLSCREEN,
	LOAD,
	MAINMENU,
	OPTIONS,
	QUIT,
	RACE_B,
	RESUME,
	SKILLS,
	TUTORIAL,
	//Pickers
	ASSTR,
	ASCON,
	ASAGI,
	ASDEX,
	ASINT,
	ASWIS,
	ASCHA,
	BACKGROUND_P,
	CLASS,
	RACE_P,
	RESOLUTION,
	SEX,
	SIZE,
	//Sliders
	MUSIC_V,
	SFX_V
};