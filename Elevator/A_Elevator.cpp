// All rights reserved to EthernalStudios OÜ


#include "World/Elevator/A_Elevator.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "General/Libraries/FL_Errors.h"

// Sets default values
AA_Elevator::AA_Elevator() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and attach the root component
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>( TEXT( "RootSceneComponent" ) );
	TVALID(RootSceneComponent);
	RootComponent = RootSceneComponent;

  // Create and attach the top platform
  TopPlatform = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "TopPlatform" ) );
  TVALID( TopPlatform );
  TopPlatform->SetupAttachment( RootSceneComponent );

  // Create and attach the bottom base
  BottomBase = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "BottomBase" ) );
  TVALID( BottomBase );
  BottomBase->SetupAttachment( RootSceneComponent );

  // Create and attach the cage
  Cage = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Cage" ) );
  TVALID( Cage );
  Cage->SetupAttachment( RootSceneComponent );

  // Create and attach the elevator mechanism
  ElevatorMechanism = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "ElevatorMechanism" ) );
  TVALID( ElevatorMechanism );
  ElevatorMechanism->SetupAttachment( RootSceneComponent );

  // Create the timeline component
  TimelineComponent = CreateDefaultSubobject<UTimelineComponent>( TEXT( "TimelineComponent" ) );
  TVALID( TimelineComponent );

  // Set default values for configurable properties
  RiseTime = 1.0f;
  DescendTime = 1.0f;
  StartAtPercentage = 0.0f;
  MaxLength = 100.0f;

  // Create Curve
  ElevatorCurve = CreateDefaultSubobject<UCurveFloat>( "Curve" );
  ElevatorCurve->FloatCurve.AddKey( 0.f, 0.f );
  ElevatorCurve->FloatCurve.AddKey( 1.f, 1.f );

  // Bind timeline finished event to custom handler
  FOnTimelineEvent ElevatorTLFinish;
  ElevatorTLFinish.BindUFunction( this, FName( "OnTimelineFinished" ) );
  TimelineComponent->SetTimelineFinishedFunc( ElevatorTLFinish );

  FOnTimelineFloat ElevatorTLUpdate;
  ElevatorTLUpdate.BindUFunction( this, FName( "UpdateTimeline" ) );
  TimelineComponent->AddInterpFloat( ElevatorCurve, ElevatorTLUpdate );
}

void AA_Elevator::OnConstruction( const FTransform& Transform )
{
  Super::OnConstruction( Transform );

#if WITH_EDITOR
  // Get world.
  UWorld* World = GetWorld();

  // If world is invalid or not a game world, abort.
  if(!World) { return; }
#endif

  // Update the static meshes based on the properties set in the editor
  TVALID( TopPlatform && BottomBase && Cage && ElevatorMechanism && TimelineComponent);
  if (TopPlatformMesh)
  {
    TopPlatform->SetStaticMesh( TopPlatformMesh );
  }

  if (BottomBaseMesh)
  {
    BottomBase->SetStaticMesh( BottomBaseMesh );
  }  

  if (CageMesh)
  {
    Cage->SetStaticMesh( CageMesh );
  }

  if (ElevatorMechanismMesh)
  {
    ElevatorMechanism->SetStaticMesh( ElevatorMechanismMesh );
  }

  // Calculate the initial position of the elevator based on StartAtPercentage and MaxLength
  float InitialHeight = FMath::Lerp( 30.f, MaxLength, StartAtPercentage );

  // Set the position of the top platform and bottom base
  FVector TopPlatformLocation = FVector( 0.f, 0.f, InitialHeight - ElevatorMechanismMesh->GetBounds().BoxExtent.Z * 2.f);
  TopPlatform->SetRelativeLocation( TopPlatformLocation );

  FVector BottomBaseLocation = FVector( 0.f, 0.f, 0.f );
  BottomBase->SetRelativeLocation( BottomBaseLocation );

  DrawDebugSphere( World, GetActorLocation() + FVector( 0.f, 0.f, InitialHeight ), 50.f, 25, FColor::Red, false, 5.f );

  // Update the ElevatorMechanism mesh scale to match the height
  if(ElevatorMechanismMesh) {
    // Calculate the scale factor for the Z-axis.
    float ScaleFactor = InitialHeight / (ElevatorMechanismMesh->GetBounds().BoxExtent.Z * 2.f);
    DEBUG_FLOAT( ElevatorMechanismMesh->GetBounds().BoxExtent.Z * 2.f, 1.f );
    DEBUG_FLOAT( InitialHeight, 1.f );
    // Create the new scale vector.
    FVector MeshScale = FVector( 1.0f, 1.0f, ScaleFactor );

    // Apply the new scale to the ElevatorMechanism.
    ElevatorMechanism->SetWorldScale3D( MeshScale );
  }

  // Initialize the timeline based on StartAtPercentage
  TimelineComponent->SetNewTime( StartAtPercentage * TimelineComponent->GetTimelineLength() );
}

void AA_Elevator::BeginPlay()
{
  Super::BeginPlay();
}

void AA_Elevator::Tick( float DeltaTime )
{
  Super::Tick( DeltaTime );
}

void AA_Elevator::Rise()
{
  TVALID( TimelineComponent );
  // Set the timeline play rate to RiseTime and play from start
  TimelineComponent->SetPlayRate( 1 / RiseTime );
  TimelineComponent->Play();
}

void AA_Elevator::Descend()
{
  TVALID( TimelineComponent );
  // Set the timeline play rate to DescendTime and play in reverse from end
  TimelineComponent->SetPlayRate( 1 / DescendTime );
  TimelineComponent->Reverse();
}

void AA_Elevator::Reverse()
{
  // Validate TimelineComponent
  TVALID( TimelineComponent );

  // Check if the timeline is reversing, and play the correct animation
  if(TimelineComponent->IsReversing())
  {
    TimelineComponent->Play();
  }
  else
  {
    TimelineComponent->Reverse();
  }
}

void AA_Elevator::Stop()
{
  TVALID( TimelineComponent );

  // Stop the timeline
  TimelineComponent->Stop();
  // Trigger the OnElevatorStopped delegate
  OnElevatorStopped.Broadcast();
}

void AA_Elevator::OnTimelineFinished()
{
  // Validate TimelineComponent
  TVALID( TimelineComponent );

  // Create variables for PlaybackPosition and TimelineLength
  float CurrentPlaybackPosition = TimelineComponent->GetPlaybackPosition();
  float TimelineLength = TimelineComponent->GetTimelineLength();

  // When PlaybackPoisition reach 0, call OnElevatorReachedBotton,
  if(CurrentPlaybackPosition == 0.0f)
  {
    OnElevatorReachedBottom.Broadcast();
  }
  // if is greater or equal than TimelineLength call OnElevatorReachedTop
  else if(CurrentPlaybackPosition >= TimelineLength)
  {
    OnElevatorReachedTop.Broadcast();
  }
}

void AA_Elevator::UpdateTimeline(float Value) {
#if WITH_EDITOR
	// Get world.
	UWorld* World = GetWorld();

	// If world is invalid or not a game world, abort.
	if (!World || !World->IsGameWorld()) { return; }
#endif

  TVALID( ElevatorMechanism && ElevatorMechanismMesh);
  // Calculate the new height based on the timeline value and Length
  float NewHeight = FMath::Lerp( 30.f, MaxLength, Value );

  // Update the ElevatorMechanism spline mesh
  FVector TopPlatformLocation = FVector( 0.0f, 0.0f, NewHeight - ElevatorMechanismMesh->GetBounds().BoxExtent.Z * 2.f );
  TopPlatform->SetRelativeLocation( TopPlatformLocation );

  // Update the ElevatorMechanism mesh scale to match the height.
  if(ElevatorMechanismMesh) {
    // Calculate the scale factor for the Z-axis.
    float ScaleFactor = NewHeight / (ElevatorMechanismMesh->GetBounds().BoxExtent.Z * 2.f);

    // Create the new scale vector.
    FVector MeshScale = FVector( 1.0f, 1.0f, ScaleFactor );

    // Apply the new scale to the ElevatorMechanism.
    ElevatorMechanism->SetRelativeScale3D( MeshScale );
  }
}
