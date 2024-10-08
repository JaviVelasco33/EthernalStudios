// All rights reserved to EthernalStudios OÜ

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../General/Data/Structs/S_DamageData.h"
#include "General/Data/Structs/S_HarmfieldParams.h"
#include "A_Harmfield.generated.h"

/* FORWARDS */
class UAC_DangerArea;
class UNiagaraSystem;
class UNiagaraComponent;
class UAudioComponent;

UCLASS(Abstract)
class ETHERNAL_API AA_Harmfield : public AActor {

	GENERATED_BODY()
	
	/* PROPERTIES */

	#pragma region CONFIG
	protected:
		// Reference to struct HarmfieldParams
		UPROPERTY(EditAnywhere, Category = "Config", meta = (ShowOnlyInnerProperties))
		FHarmfieldParams HarmfieldParams;

private:
	UNiagaraComponent* VFXRef;

	UAudioComponent* SoundRef;
	
	#pragma endregion

	#pragma region DATA
	private:

	
	#pragma endregion

	#pragma region COMPONENTS
	protected:
		// Shape Component from Actor
		UPROPERTY()
		UShapeComponent* ShapeComponent;

		// Danger Arena from Actor
		UPROPERTY()
		UAC_DangerArea* DangerAreaComponent;


	#pragma endregion

	#pragma region REFERENCES
	private:


	#pragma endregion

	#pragma region TIMERS
	private:
		FTimerHandle LifeTimer;

		FTimerHandle DamageTimer;

	#pragma endregion

	/* FUNCTIONALITY */

	#pragma region INIT
	public:

	// Sets default values for this actor's properties
		AA_Harmfield() { return; };

		AA_Harmfield( TSubclassOf<UShapeComponent> ShapeClass );

		virtual void BeginPlay() override;

		virtual void OnConstruction( const FTransform& Transform ) override;

		virtual void RefreshShape();
	#pragma endregion

#pragma region GETTERS
	UFUNCTION(BlueprintCallable)
	bool IsFieldActive();
#pragma endregion

#pragma region SETTERS
#pragma endregion

#pragma region FUNCTIONS
public:
	UFUNCTION()
	void TriggerFieldDamage();

	UFUNCTION(BlueprintCallable)
	void ToggleField(bool bActivate);
	
	UFUNCTION()
	void OnLifeTimeEnd();

	UFUNCTION()
	void SetFieldParams( FHarmfieldParams& InParams);

#pragma endregion

	/* DELEGATES */

	#pragma region ALL
	public:
	
	
	#pragma endregion

	//
	//
};
