// All rights reserved to EthernalStudios OÜ

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "A_Barrier.generated.h"

class UTimelineComponent;

UCLASS()
class ETHERNAL_API AA_Barrier : public AActor
{
	GENERATED_BODY()

#pragma region CONFIG
protected:
	//Meshes

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Barrier|Config|Emitter")
  TObjectPtr<UStaticMesh> EmitterMesh1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Barrier|Config|Emitter" )
	TObjectPtr<UStaticMesh> EmitterMesh2;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Barrier|Config|Emitter" )
	TObjectPtr<UStaticMesh> EmitterMesh3;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Barrier|Config|Emitter")
  TObjectPtr<UStaticMesh> EmitterMesh4;

 // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Barrier|Config|Decorator" )
	//TObjectPtr<UStaticMesh> DecoratorMesh1;

	//UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Barrier|Config|Decorator" )
	//TObjectPtr<UStaticMesh> DecoratorMesh2;

	//UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Barrier|Config|Decorator" )
	//TObjectPtr<UStaticMesh> DecoratorMesh3;

	//UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Barrier|Config|Decorator" )
	//TObjectPtr<UStaticMesh> DecoratorMesh4;

	// Other config
  // Barrier Length
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Barrier|Config|Size", meta = (UIMin = 0.0, ClampMin = 0.0) )
	float Length;

  // Barrier Wide
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Barrier|Config|Size", meta = (UIMin = 1.0, ClampMin = 1.0) )
	float Wide;

  // Barrier Thickness
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Barrier|Config|Size", meta = (UIMin = 0.01, ClampMin = 0.01))
  float Thickness;

  // Check if the barrier start active or not
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Barrier|Config|Size")
  bool bStartActive;

  // Anim duration in seconds
  UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Barrier|Config|Size", meta = (UIMin = 0.01, ClampMin = 0.01))
  float Duration;

  // Actors allowed to cross the barrier
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Barrier|Config")
  TArray<TSubclassOf<AActor>> AllowedActorClasses;


	// Material
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Barrier|Config|Material")
  TObjectPtr<UMaterialInstance> BarrierMaterial;

  // Material Brightness
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Barrier|Config|Material")
  float BarrierBrightness;

  // Parameter name from material
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Barrier|Config|Material")
  FName ParameterName;


private:
  UPROPERTY()
  TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial = nullptr;

	// Curve for the timeline
  UPROPERTY()
  TObjectPtr<UCurveFloat> BarrierCurve;

  // bool to save the active state 
  bool bIsActive;
	
#pragma endregion

#pragma region COMPONENTS
	protected:
  UPROPERTY()
	TObjectPtr<USceneComponent> RootSceneComponent;

  UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Barrier|Components" )
  TObjectPtr<UStaticMeshComponent> BarrierMeshComp;

	 UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Barrier|Components|Emitter" )
  TObjectPtr<UStaticMeshComponent> EmitterMeshComp1;

	 UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Barrier|Components|Emitter" )
  TObjectPtr<UStaticMeshComponent> EmitterMeshComp2;

	 UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Barrier|Components|Emitter" )
  TObjectPtr<UStaticMeshComponent> EmitterMeshComp3;

  UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Barrier|Components|Emitter" )
  TObjectPtr<UStaticMeshComponent> EmitterMeshComp4;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Barrier|Components|Decorator" )
	TObjectPtr<UStaticMeshComponent> DecoratorMeshComp1;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Barrier|Components|Decorator" )
	TObjectPtr<UStaticMeshComponent> DecoratorMeshComp2;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Barrier|Components|Decorator" )
	TObjectPtr<UStaticMeshComponent> DecoratorMeshComp3;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Barrier|Components|Decorator" )
  TObjectPtr<UStaticMeshComponent> DecoratorMeshComp4;

  UPROPERTY()
  TObjectPtr<UTimelineComponent> TimelineComponent;


	#pragma endregion

#pragma region INIT

public:	
	// Sets default values for this actor's properties
	AA_Barrier();

  // Called when the actor is created or modified
	virtual void OnConstruction( const FTransform& Transform ) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma endregion

#pragma region GETTERS
public:
	/**
  * @brief Gets the mesh from EmitterMesh1.
  * @return Stored EmitterMesh1 value.
  */
  UFUNCTION(BlueprintPure, Category = "Getters", meta = (Tooltip = "Gets the mesh from EmitterMesh1."))
  FORCEINLINE UStaticMesh* GetEmitterMesh1() const { return EmitterMesh1; };

	/**
  * @brief Gets the mesh from EmitterMesh2.
  * @return Stored EmitterMesh2 value.
  */
  UFUNCTION(BlueprintPure, Category = "Getters", meta = (Tooltip = "Gets the mesh from EmitterMesh2."))
  FORCEINLINE UStaticMesh* GetEmitterMesh2() const { return EmitterMesh2; };

	/**
  * @brief Gets the mesh from EmitterMesh3.
  * @return Stored EmitterMesh3 value.
  */
  UFUNCTION(BlueprintPure, Category = "Getters", meta = (Tooltip = "Gets the mesh from EmitterMesh3."))
  FORCEINLINE UStaticMesh* GetEmitterMesh3() const { return EmitterMesh3; };

	/**
  * @brief Gets the mesh from EmitterMesh4.
  * @return Stored EmitterMesh4 value.
  */
  UFUNCTION(BlueprintPure, Category = "Getters", meta = (Tooltip = "Gets the mesh from EmitterMesh4."))
  FORCEINLINE UStaticMesh* GetEmitterMesh4() const { return EmitterMesh4; };

	///**
 // * @brief Gets the mesh from DecoratorMesh1.
 // * @return Stored DecoratorMesh1 value.
 // */
 // UFUNCTION(BlueprintPure, Category = "Getters", meta = (Tooltip = "Gets the mesh from DecoratorMesh1."))
 // FORCEINLINE UStaticMesh* GetDecoratorMesh1() const { return DecoratorMesh1; };

	///**
 // * @brief Gets the mesh from DecoratorMesh2.
 // * @return Stored DecoratorMesh2 value.
 // */
 // UFUNCTION(BlueprintPure, Category = "Getters", meta = (Tooltip = "Gets the mesh from DecoratorMesh2."))
 // FORCEINLINE UStaticMesh* GetDecoratorMesh2() const { return DecoratorMesh2; };

	///**
 // * @brief Gets the mesh from DecoratorMesh3.
 // * @return Stored DecoratorMesh3 value.
 // */
 // UFUNCTION(BlueprintPure, Category = "Getters", meta = (Tooltip = "Gets the mesh from DecoratorMesh3."))
 // FORCEINLINE UStaticMesh* GetDecoratorMesh3() const { return DecoratorMesh3; };

	///**
 // * @brief Gets the mesh from DecoratorMesh4.
 // * @return Stored DecoratorMesh4 value.
 // */
 // UFUNCTION(BlueprintPure, Category = "Getters", meta = (Tooltip = "Gets the mesh from DecoratorMesh4."))
 // FORCEINLINE UStaticMesh* GetDecoratorMesh4() const { return DecoratorMesh4; };

	/**
  * @brief Gets the length value from barrier.
  * @return Stored Length value.
  */
  UFUNCTION(BlueprintPure, Category = "Getters", meta = (Tooltip = "Gets the length value from barrier."))
  FORCEINLINE float GetLength() const { return Length; };

	/**
  * @brief Gets the wide value from barrier.
  * @return Stored Wide value.
  */
  UFUNCTION(BlueprintPure, Category = "Getters", meta = (Tooltip = "Gets the wide value from barrier."))
  FORCEINLINE float GetWide() const { return Wide; };

	/**
  * @brief Gets the thickness value from barrier.
  * @return Stored Thickness value.
  */
  UFUNCTION(BlueprintPure, Category = "Getters", meta = (Tooltip = "Gets the thickness value from barrier."))
  FORCEINLINE float GetThickness() const { return Thickness; };

	/**
  * @brief Gets whether the barrier should start active.
  * @return Stored bStartActive value.
  */
  UFUNCTION(BlueprintPure, Category = "Getters", meta = (Tooltip = "Gets whether the barrier should start active."))
  FORCEINLINE bool IsStartActive() const { return bStartActive; };

	/**
  * @brief Gets the barrier material.
  * @return Stored BarrierMaterial value.
  */
  UFUNCTION(BlueprintPure, Category = "Getters", meta = (Tooltip = "Gets the barrier material."))
  FORCEINLINE UMaterialInstance* GetBarrierMaterial() const { return BarrierMaterial; };

	/**
  * @brief Gets the value of Brightness from the barrier material.
  * @return Stored BarrierBrightness value.
  */
  UFUNCTION(BlueprintPure, Category = "Getters", meta = (Tooltip = "Gets the value of Brightness from the barrier material."))
  FORCEINLINE float GetBarrierBrightness() const { return BarrierBrightness; };
#pragma endregion

#pragma region SETTERS
public:
  /**
  * @brief Sets the new mesh for EmitterMesh1.
  * @param NewMesh: The new mesh for EmitterMesh1.
  */
  UFUNCTION(BlueprintCallable, Category = "Setters", meta = (Tooltip = "Sets the new mesh for EmitterMesh1."))
  void SetEmitterMesh1( UStaticMesh* NewMesh ){ EmitterMesh1 = NewMesh; };

  /**
  * @brief Sets the new mesh for EmitterMesh2.
  * @param NewMesh: The new mesh for EmitterMesh2.
  */
  UFUNCTION(BlueprintCallable, Category = "Setters", meta = (Tooltip = "Sets the new mesh for EmitterMesh2."))
  void SetEmitterMesh2( UStaticMesh* NewMesh ){ EmitterMesh2 = NewMesh; };

  /**
  * @brief Sets the new mesh for EmitterMesh3.
  * @param NewMesh: The new mesh for EmitterMesh3.
  */
  UFUNCTION(BlueprintCallable, Category = "Setters", meta = (Tooltip = "Sets the new mesh for EmitterMesh3."))
  void SetEmitterMesh3( UStaticMesh* NewMesh ){ EmitterMesh3 = NewMesh; };

  /**
  * @brief Sets the new mesh for EmitterMesh4.
  * @param NewMesh: The new mesh for EmitterMesh4.
  */
  UFUNCTION(BlueprintCallable, Category = "Setters", meta = (Tooltip = "Sets the new mesh for EmitterMesh4."))
  void SetEmitterMesh4( UStaticMesh* NewMesh ){ EmitterMesh4 = NewMesh; };

  ///**
  //* @brief Sets the new mesh for DecoratorMesh1.
  //* @param NewMesh: The new mesh for DecoratorMesh1.
  //*/
  //UFUNCTION(BlueprintCallable, Category = "Setters", meta = (Tooltip = "Sets the new mesh for DecoratorMesh1."))
  //void SetDecoratorMesh1( UStaticMesh* NewMesh ){ DecoratorMesh1 = NewMesh; };

  ///**
  //* @brief Sets the new mesh for DecortatorMesh2.
  //* @param NewMesh: The new mesh for DecoratorMesh2.
  //*/
  //UFUNCTION(BlueprintCallable, Category = "Setters", meta = (Tooltip = "Sets the new mesh for DecortatorMesh2."))
  //void SetDecoratorMesh2( UStaticMesh* NewMesh ){ DecoratorMesh2 = NewMesh; };

  ///**
  //* @brief Sets the new mesh for DecortatorMesh3.
  //* @param NewMesh: The new mesh for DecoratorMesh3.
  //*/
  //UFUNCTION(BlueprintCallable, Category = "Setters", meta = (Tooltip = "Sets the new mesh for DecortatorMesh3."))
  //void SetDecoratorMesh3( UStaticMesh* NewMesh ){ DecoratorMesh3 = NewMesh; };

  ///**
  //* @brief Sets the new mesh for DecortatorMesh4.
  //* @param NewMesh: The new mesh for DecoratorMesh4.
  //*/
  //UFUNCTION(BlueprintCallable, Category = "Setters", meta = (Tooltip = "Sets the new mesh for DecortatorMesh4."))
  //void SetDecoratorMesh4( UStaticMesh* NewMesh ){ DecoratorMesh4 = NewMesh; };

  /**
  * @brief Sets the new value of length.
  * @param NewLength: The new length value.
  */
  UFUNCTION(BlueprintCallable, Category = "Setters", meta = (Tooltip = "Sets the new value of length."))
  void SetLength( float NewLength ){ Length = NewLength; };

  /**
  * @brief Sets the new value of wide.
  * @param NewWide: The new wide value.
  */
  UFUNCTION(BlueprintCallable, Category = "Setters", meta = (Tooltip = "Sets the new value of wide."))
  void SetWide( float NewWide ){ Wide = NewWide; };

  /**
  * @brief Sets the new value of thickness.
  * @param NewThickness: The new thickness for barrier.
  */
  UFUNCTION(BlueprintCallable, Category = "Setters", meta = (Tooltip = "Sets the new value of thickness."))
  void SetThickness( float NewThickness ){ Thickness = NewThickness; };

  /**
  * @brief Sets whether the laser should start active.
  * @param bNewStartActive: The new start active state.
  */
  UFUNCTION(BlueprintCallable, Category = "Setters", meta = (Tooltip = "Sets whether the laser should start active."))
  void SetStartActive( bool bNewStartActive ){ bStartActive = bNewStartActive; };

  /**
  * @brief Sets new material fro barrier.
  * @param NewMaterial: The new material.
  */
  UFUNCTION(BlueprintCallable, Category = "Setters", meta = (Tooltip = "Sets new material fro barrier."))
  void SetBarrierMaterial( UMaterialInstance* NewMaterial ){ BarrierMaterial = NewMaterial; };

  /**
  * @brief Sets new value for material brightness.
  * @param NewBrightness: The new bright value.
  */
  UFUNCTION(BlueprintCallable, Category = "Setters", meta = (Tooltip = "Sets new value for material brightness."))
  void SetBarrierBrightness( float NewBrightness ){ BarrierBrightness = NewBrightness; };
#pragma endregion

#pragma region INTERFACE
public:
    // Activate the barrier
	  UFUNCTION(BlueprintCallable, Category = "Barrier|Interface")
    void Activate(const bool bInstant);

    // Deactivate the barrier
    UFUNCTION(BlueprintCallable, Category = "Barrier|Interface")
    void Deactivate(const bool bInstant);

    // Reset to base position
    UFUNCTION(BlueprintCallable, Category = "Barrier|Interface")
    void Reset();

#pragma endregion

#pragma region FUNCTIONS

  // Call when the timeline end
	UFUNCTION()
	void OnTimelineFinished();

  // Call durign the entire timeline update
	UFUNCTION()
	void UpdateTimeline(float Value);

	UFUNCTION()
	void UpdateMeshes();

  UFUNCTION()
	void UpdateScales(float NewDistance);

#pragma endregion

#pragma region OVERLAP
  UFUNCTION()
  void OnOverlapBegin( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );
#pragma endregion

};
