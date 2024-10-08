// All rights reserved to EthernalStudios OÜ


#include "World/Props/A_Prop_Bottle_Fire.h"
#include "General/Data/Enums/E_DestructibleBreakImpulseModes.h"

AA_Prop_Bottle_Fire::AA_Prop_Bottle_Fire()
{
	// Set default config.
	//Destructible
	MaxHealth = 100;
	bBreakAtHit = 1;
	BreakHitForce = 1000.f;
	ImpulseStrengthVariation = {0.25f, 1.f};
	BrokenSound = nullptr;
	BrokenVFX = nullptr;
	BrokenVFXTransform = FTransform();
	BreakMode = EDestructibleBreakImpulseModes::RadialImpulse;
	BreakRadius = 10.f;
	bStartWithPhysics = 0;
	AllowedPowers = {};
	ExplosionParams;
}

