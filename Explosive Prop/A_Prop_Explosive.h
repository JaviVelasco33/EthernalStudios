// All rights reserved to EthernalStudios OÜ

#pragma once

#include "CoreMinimal.h"
#include "World/Props/A_Prop.h"
#include "General/Data/Structs/S_ExplosionParams.h"
#include "A_Prop_Explosive.generated.h"


/**
 * 
 */
UCLASS(Abstract)
class ETHERNAL_API AA_Prop_Explosive : public AA_Prop
{
	GENERATED_BODY()
	
	#pragma region CONFIG
	protected:
		// Reference to struct ExplosionParams
		UPROPERTY(EditAnywhere, Category = "Config", meta = (ShowOnlyInnerProperties))
		FExplosionParams ExplosionParams;

private:
	#pragma endregion

	#pragma region DATA
	private:

	
	#pragma endregion

	#pragma region COMPONENTS
	protected:

	#pragma endregion

	#pragma region REFERENCES
	private:


	#pragma endregion

	#pragma region TIMERS
	private:

	#pragma endregion

	/* FUNCTIONALITY */

	#pragma region INIT
	public:
		// Sets default values for this actor's properties
		AA_Prop_Explosive() { return; };
	#pragma endregion

	#pragma region GETTERS
#pragma endregion

	#pragma region SETTERS
#pragma endregion

	#pragma region FUNCTIONS
public:

	virtual void OnDeath( FDamageData Damage ) override;

#pragma endregion

	/* DELEGATES */

	#pragma region ALL
	public:
	
	
	#pragma endregion

	//
	//

};
