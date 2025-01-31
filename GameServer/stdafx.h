// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

class Bulb;

enum TStates { On, Off, Broken };

//class Bulb;
typedef void(Bulb::* CBP)(); //call-back
typedef bool(Bulb::* GP)();  // 


/*
class Weapon;

typedef void (Weapon::*CBP)();
typedef bool (Weapon::*GP)();

enum TStates {Empty, Reload, Loaded, Firing};
*/