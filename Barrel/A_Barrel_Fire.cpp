// All rights reserved to EthernalStudios OÜ


#include "World/Props/A_Barrel_Fire.h"
#include "World/Danger/A_Harmfield_Sphere.h"
#include "General/Data/Structs/S_HarmfieldPresets.h"
#include "General/Data/Structs/S_DestructiblePiece.h"
#include "General/Data/Enums/E_DestructibleBreakImpulseModes.h"
#include "General/Data/HardData/HD_Paths.h"
#include "NiagaraSystem.h"

AA_Barrel_Fire::AA_Barrel_Fire() {
	// Set default config.
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
	ExplosionParams = FExplosionParams(
		500.f,
		750.f,
		0.25f,
		{FDamageData()},
		{ AActor::StaticClass() },
		{},
		LOAD_OBJECT(UNiagaraSystem, UHD_Paths::NS_Explosion_Fire_Medium),
		LOAD_OBJECT(USoundBase, UHD_Paths::Sound_Explosion_Fire_Medium)
	);
	DestructiblePieces = {
		FDestructiblePiece(LOAD_OBJECT(UStaticMesh, UHD_Paths::M_Barrel_Fire_Piece_1), 20.f, 20.f, FTransform::Identity),
		FDestructiblePiece(LOAD_OBJECT(UStaticMesh, UHD_Paths::M_Barrel_Fire_Piece_2), 20.f, 20.f, FTransform::Identity),
		FDestructiblePiece(LOAD_OBJECT(UStaticMesh, UHD_Paths::M_Barrel_Fire_Piece_3), 20.f, 20.f, FTransform::Identity),
		FDestructiblePiece(LOAD_OBJECT(UStaticMesh, UHD_Paths::M_Barrel_Fire_Piece_4), 20.f, 20.f, FTransform::Identity),
		FDestructiblePiece(LOAD_OBJECT(UStaticMesh, UHD_Paths::M_Barrel_Fire_Piece_5), 20.f, 20.f, FTransform::Identity),
	};
	HarmfieldClass = AA_Harmfield_Sphere::StaticClass();
	HarmfieldParams = FHarmfield_Molotov_Medium;
	HarmfieldParams.DamageData.Instigator = this;
	HarmfieldParams.FieldNiagara = LOAD_OBJECT(UNiagaraSystem, UHD_Paths::NS_Fire_Pool_Medium);
	HarmfieldParams.FieldSound = LOAD_OBJECT(USoundBase, UHD_Paths::Sound_Danger_Fire);
	bUseHarmfieldClassParams = 0;

	return;
}
