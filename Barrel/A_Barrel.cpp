// All rights reserved to EthernalStudios OÜ

#include "World/Props/A_Barrel.h"
#include "World/Danger/A_Harmfield.h"

/* INIT */

AA_Barrel::AA_Barrel() {
	// Set default config.
	HarmfieldClass = nullptr;
	bUseHarmfieldClassParams = 0;
	HarmfieldParams = FHarmfieldParams();

	return;
}

/* DEATH */

void AA_Barrel::OnDeath(FDamageData Damage) {
	// Call super.
	Super::OnDeath(Damage);

	// Create harmfield spawn parameters.
	FActorSpawnParameters SpawnParams;
	FTransform Transform;
	Transform.SetLocation(GetActorLocation());

	// Spawn and validate harmfield.
	AA_Harmfield* Harmfield = GetWorld()->SpawnActor<AA_Harmfield>(HarmfieldClass, Transform, SpawnParams);
	TVALID(Harmfield);

	// If desired, set custom field parameters.
	if (!bUseHarmfieldClassParams) { Harmfield->SetFieldParams(HarmfieldParams); }

	// Activate field.
	Harmfield->ToggleField(1);

	return;
}
