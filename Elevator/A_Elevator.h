// All rights reserved to EthernalStudios OÜ

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineMeshComponent.h"
#include "General/Signatures/SIG_Common.h"

#include "A_Elevator.generated.h"

class UTimelineComponent;

UCLASS()
class ETHERNAL_API AA_Elevator : public AActor
{
	GENERATED_BODY()
	
	/* PROPERTIES */

	#pragma region CONFIG
	protected:
		// Mesh to use as Top platform
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elevator|Config")
    TObjectPtr<UStaticMesh> TopPlatformMesh;

		// Mesh to use as base
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elevator|Config" )
		TObjectPtr<UStaticMesh> BottomBaseMesh;

		// Mesh to use as cage
		UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Elevator|Config" )
		TObjectPtr<UStaticMesh> CageMesh;

		// Mesh to use as Mechanism
		UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Elevator|Config" )
		TObjectPtr<UStaticMesh> ElevatorMechanismMesh;

		// Seconds to complete the timeline when is rising, is 1s by default
		UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Elevator|Config", meta = (UIMin = 0.1, ClampMin = 0.1) )
		float RiseTime;

		// Seconds to complete the timeline when is descending, is 1s by default
		UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Elevator|Config", meta = (UIMin = 0.1, ClampMin = 0.1) )
		float DescendTime;

		// Percentage
		UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Elevator|Config", meta = (UIMin = 0.0, ClampMin = 0.0, UIMax = 1.0, ClampMax = 1.0))
    float StartAtPercentage;

		// Spline max length
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elevator|Config", meta = (UIMin = 1.0, ClampMin = 1.0))
    float MaxLength;

	private:

		// Curve for the timeline
    UPROPERTY()
    TObjectPtr<UCurveFloat> ElevatorCurve;
	
	#pragma endregion

	#pragma region DATA
	private:	
	#pragma endregion

	#pragma region COMPONENTS
	protected:
	// Root Component
  UPROPERTY()
	TObjectPtr<USceneComponent> RootSceneComponent;

	// Top platform mesh component
  UPROPERTY()
  TObjectPtr<UStaticMeshComponent> TopPlatform;

	// Base mesh component
  UPROPERTY()
  TObjectPtr<UStaticMeshComponent> BottomBase;

	// Cage mesh component
  UPROPERTY()
  TObjectPtr<UStaticMeshComponent> Cage;

	// Mechanism mesh component
  UPROPERTY()
  TObjectPtr<UStaticMeshComponent> ElevatorMechanism;

	// Timeline component
  UPROPERTY()
  TObjectPtr<UTimelineComponent> TimelineComponent;


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
		AA_Elevator();

	virtual void OnConstruction( const FTransform& Transform ) override;

	virtual void BeginPlay() override;

	virtual void Tick( float DeltaTime ) override;


	#pragma endregion

#pragma region GETTERS
#pragma endregion

#pragma region SETTERS
#pragma endregion

#pragma region INTERFACE
public:
		// Rise the elevator
	  UFUNCTION(BlueprintCallable, Category = "Elevator|Interface")
    void Rise();

		// Descend the elevator
    UFUNCTION(BlueprintCallable, Category = "Elevator|Interface")
    void Descend();

		// Reverse the current movement 
    UFUNCTION(BlueprintCallable, Category = "Elevator|Interface")
    void Reverse();

		// Stop the movement
    UFUNCTION(BlueprintCallable, Category = "Elevator|Interface")
    void Stop();
#pragma endregion

#pragma region FUNCTIONS

	// Call when the timeline end
	UFUNCTION()
	void OnTimelineFinished();

	// Call durign the entire timeline update
	UFUNCTION()
	void UpdateTimeline(float Value);

#pragma endregion

#pragma region DELEGATES
	UPROPERTY(BlueprintAssignable, Category = "Elevator|Delegates")
  FSimpleSignature OnElevatorReachedTop;

  UPROPERTY(BlueprintAssignable, Category = "Elevator|Delegates")
  FSimpleSignature OnElevatorReachedBottom;

  UPROPERTY(BlueprintAssignable, Category = "Elevator|Delegates")
  FSimpleSignature OnElevatorStopped;
#pragma endregion

	#pragma region ALL
	public:
	
	#pragma endregion


};
