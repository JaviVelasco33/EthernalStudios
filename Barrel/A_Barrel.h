// All rights reserved to EthernalStudios OÜ

#pragma once

#include "CoreMinimal.h"
#include "World/Props/A_Prop_Explosive.h"
#include "General/Data/Structs/S_HarmfieldParams.h"
#include "A_Barrel.generated.h"

// Forward declares.
class AA_Harmfield;

/**
 * 
 */
UCLASS(Abstract)
class ETHERNAL_API AA_Barrel : public AA_Prop_Explosive {
	GENERATED_BODY()
	
	#pragma region CONFIG
	protected:

	// .
	UPROPERTY(EditAnywhere, Category = "Config", meta = (ShowOnlyInnerProperties))
	TSubclassOf<AA_Harmfield> HarmfieldClass;

	// .
	UPROPERTY(EditAnywhere, Category = "Config", meta = (ShowOnlyInnerProperties))
	bool bUseHarmfieldClassParams;

	// .
	UPROPERTY(EditAnywhere, Category = "Config", meta = (ShowOnlyInnerProperties))
	FHarmfieldParams HarmfieldParams;
	#pragma endregion

	/* FUNCTIONALITY */

	#pragma region INIT
	public:

	/*
	* @brief .
	*/
	AA_Barrel();
	#pragma endregion

	#pragma region DEATH
	protected:

	/*
	* @brief .
	* @param Damage: .
	*/
	virtual void OnDeath(FDamageData Damage) override;
	#pragma endregion

};
