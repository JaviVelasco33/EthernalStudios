// All rights reserved to EthernalStudios OÜ

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "General/Signatures/SIG_Common.h"
#include "A_Door.generated.h"

class UBoxComponent;

UCLASS()
class ETHERNAL_API AA_Door : public AActor {

	GENERATED_BODY()
	
	/* PROPERTIES */

	#pragma region CONFIG
	protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    UAnimSequence* OpenAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    UAnimSequence* CloseAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    USoundBase* OpenSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    USoundBase* CloseSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    USoundBase* BlockedSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    USoundBase* UnlockedSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    float AutoCloseDelay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    TArray<TSubclassOf<AActor>> AllowedActorClasses;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    TArray<TSubclassOf<AActor>> ForbiddenActorClasses;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    TArray<uint8> LightMaterialIndexes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    FName LightMaterialParameterName = "Color";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    bool bStartPoweredOn;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    bool bStartBlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    bool bAutomaticDoor;

	// 0 = Closed, 1 = Opened, 2 = Closing, 3 = Opening.
	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta = (Category = "Config", UIMax = "3", ClampMax = "3"))
	uint8 StartState;
	#pragma endregion

	#pragma region COMPONENTS
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USceneComponent> RootSceneComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USkeletalMeshComponent> DoorMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UBoxComponent> TriggerBox;
#pragma endregion

	#pragma region VARIABLES
	private:

		
			bool bIsOpen;

			bool bInTransition;

			bool bBlocked;

			bool bIsPoweredOn;

			FTimerHandle AutoCloseTimerHandle;

	#pragma endregion

	/* FUNCTIONALITY */

	#pragma region INIT
	public:

	/**
	 * @brief: .
	*/
	AA_Door();

	protected:
	
	virtual void OnConstruction(const FTransform& Transform) override;
	#pragma endregion

	#pragma region GETTERS
	public:

	/**
	 * @brief: .
	*/
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ".", Category = "Getters"))
	bool IsDoorOpening();

	/**
	 * @brief: .
	*/
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ".", Category = "Getters"))
	bool IsDoorOpened();

	/**
	 * @brief: .
	*/
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ".", Category = "Getters"))
	bool IsDoorClosing();

	/**
	 * @brief: .
	*/
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ".", Category = "Getters"))
	bool IsDoorClosed();

	/**
	 * @brief: .
	*/
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ".", Category = "Getters"))
	FORCEINLINE bool IsDoorUnlocked() { return !bBlocked; }

	UFUNCTION(BlueprintCallable, meta = (Tooltip = ".", Category = "Getters"))
	FLinearColor GetLightColor() const;
	#pragma endregion

	#pragma region ALL
	public:

	UFUNCTION(BlueprintCallable, meta = (Tooltip = "Open door event.", Category = "Main"))
	void OpenDoor(const bool bInstant);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = "Close door event.", Category = "Main"))
	void CloseDoor(const bool bInstant);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = "Block door event.", Category = "Main"))
	void SetDoorBlocked(const bool bBlock);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = "Block door event.", Category = "Main"))
	void SetDoorPowered(const bool bOn);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = "Block door event.", Category = "Main"))
	void SetDoorAutomatic(const bool bAuto);

	private:

	void UpdateLights();

	void AutoClose();

	protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	public:	
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	#pragma endregion

	#pragma region OVERLAP
		UFUNCTION()
		void OnOverlapBegin( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

		UFUNCTION()
		void OnOverlapEnd( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex );
	#pragma endregion

	#pragma region INTERNAL
	private:

	void CheckAutoClose();
	#pragma endregion

	/* DELEGATES */

	#pragma region ALL
	public:

	// .
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (Tooltip = ".", Category = "Delegates"))
	FActorSignature OnDoorOpened;

	// .
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (Tooltip = ".", Category = "Delegates"))
	FActorSignature OnDoorClosed;

	// .
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (Tooltip = ".", Category = "Delegates"))
	FActorSignature OnDoorOpening;

	// .
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (Tooltip = ".", Category = "Delegates"))
	FActorSignature OnDoorClosing;
	#pragma endregion

};
