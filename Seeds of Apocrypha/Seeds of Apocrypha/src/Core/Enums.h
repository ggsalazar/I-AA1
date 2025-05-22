#pragma once
#include <string>

constexpr float sqrt2 = 1.414f;

//Skills & Scores
enum class Ab_Score {
	STR,
	CON,
	AGI,
	DEX,
	INT,
	WIS,
	CHA
};

enum class A_Skill {
	Appraisal,
	Arcana,
	Beast_Handling,
	Beguilement,
	Climb,
	Insight,
	Intimidation,
	Lore,
	Mechanics,
	Medicine,
	Nature,
	Navigation,
	Perception,
	Persuasion,
	SOH,
	Stealth,
	Survival,
	Swim
};

enum class T_Skill {
	Med_Kits,
	Theiving_Kits
};

enum class W_Skill {
	Archery,
	Firearm,
	Large,
	Med,
	Polearm,
	Small,
	Throwing,
	Unarmed
};


//Mechanics
enum class Action {
	DEFAULT,
	NOACTION, //Slashed circle
	Attack_M, //Sword
	Attack_R, //Gun? Bow? Both?
	AOO,
	Change_Equip,
	Close,	  //Closed door/container?
	Disengage,
	Evade,
	Examine, //Eye
	Fall_Prone,
	Help,
	Hide,
	Jump,
	Loot, //Treasure Chest
	Move, //Boot
	Open, //Open door
	Pickup, //Hand
	Shove,
	Stand,
	Talk, //Mouth? Or speech bubble?
	Throw_Item
};

enum class Condition {
	Blind,
	Charmed,
	Deaf,
	Prone,
	Sick,
	Stunned,
	Unconscious
};

enum class Class {
	None,
	Arcanist,
	Rogue,
	Warrior
};
/*inline Class StringToClass(std::string& s) {
	if (s == "None") return Class::None;
	else if (s == "Arcanist") return Class::Arcanist;
	else if (s == "Rogue") return Class::Rogue;
	else if (s == "Warrior") return Class::Warrior;
}*/

enum class Genus {
	Beast,
	Monstrosity,
	Other,
	Scorched,
	Sentient,
	Undead
};
/*inline Genus StringToGenus(std::string& s) {
	if (s == "Beast") return Genus::Beast;
	else if (s == "Monstrosity") return Genus::Monstrosity;
	else if (s == "Other") return Genus::Other;
	else if (s == "Scorched") return Genus::Scorched;
	else if (s == "Sentient") return Genus::Sentient;
	else if (s == "Undead") return Genus::Undead;
}*/

enum class Race {
	Automaton,
	Dwarf,
	Elf,
	Gnome,
	Human,
	Kobold
};
/*inline Race StringToRace(std::string& s) {
	if (s == "Automaton") return Race::Automaton;
	else if (s == "Dwarf") return Race::Dwarf;
	else if (s == "Elf") return Race::Elf;
	else if (s == "Gnome") return Race::Gnome;
	else if (s == "Human") return Race::Human;
	else if (s == "Kobold") return Race::Kobold;
}*/

enum class Size {
	Tiny,
	Small,
	Med,
	Big,
	Large,
	Huge,
	Massive,
	Colossal
};
/*inline Size StringToSize(std::string& s) {
	if (s == "Tiny") return Size::Tiny;
	else if (s == "Small") return Size::Small;
	else if (s == "Med") return Size::Med;
	else if (s == "Big") return Size::Big;
	else if (s == "Large") return Size::Large;
	else if (s == "Huge") return Size::Huge;
	else if (s == "Massive") return Size::Massive;
	else if (s == "Colossal") return Size::Colossal;
}*/

enum class PreGens {
	Spark, //Automaton Arcanist
	Essek, //Female Kobold Rogue
	Dakn //Male Dwarf Warrior
};

enum class Item {

};

enum class Terrain {
	Normal,
	Rough,
	Water
};

enum class Weather {
	Clear
};


//Game details
enum class Area {
	NONE,
	Debug,
	Tutton
};

enum class Cutscenes {

};

enum class Interface {
	NONE,
	CharSheet, //C
	Inv,	   //I
	Journal,   //J
	Map_Area,  //M
	Map_World, 
	Options    //O
};



//Engine
enum class Scenes {
	CUTSCENE,
	AREA,
	TITLE
};

enum class Menus {
	CharCrea,
	GO,
	Inv,
	LVLUp,
	Load,
	Main,
	Options,
	Options_G,
	Pause,
	//Sub-Menus
	//CC
	CCAS,
	CCBG,
	CCClass,
	CCEquip,
	CCRace,
	CCSkills
};

enum class UIElem {
	DEFAULT,
	Confirm, //For Confirm Boxes
	//Buttons
	Apply,
	AS,
	Back,
	BG_B,
	CharCrea,
	Class_B,
	Close,
	Create,
	Debug_Room,
	Equipment_CC,
	Fullscreen,
	Load,
	No,
	Options,
	Quit,
	Race_B,
	Resume,
	Skills,
	Title,
	Tutorial,
	Yes,
	//Pickers
	ASSTR,
	ASCON,
	ASAGI,
	ASDEX,
	ASINT,
	ASWIS,
	ASCHA,
	BG_P,
	Class_P,
	Race_P,
	Resolution,
	Sex,
	Size,
	//Sliders
	CAMSPD,
	Music_V,
	SFX_V
};