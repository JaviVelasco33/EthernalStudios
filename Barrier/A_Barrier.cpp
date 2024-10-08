// All rights reserved to EthernalStudios OÜ


#include "World/Lasers/A_Barrier.h"
#include "General/Libraries/FL_Errors.h"
#include "Components/TimelineComponent.h"
#include "General/Data/HardData/HD_Paths.h"
#include "Math/NumericLimits.h"
#include "Combat/Ammo/A_Ammo.h"
#include "General/Libraries/FL_General.h"

// Sets default values
AA_Barrier::AA_Barrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  // Create and attach the root component
  RootSceneComponent = CreateDefaultSubobject<USceneComponent>( TEXT( "RootSceneComponent" ) );
  TVALID( RootSceneComponent );
  RootComponent = RootSceneComponent;

  // Create and attach the top platform
  BarrierMeshComp = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "BarrierMeshComp" ) );
  TVALID( BarrierMeshComp );
  BarrierMeshComp->SetupAttachment( RootSceneComponent );
  BarrierMeshComp->SetStaticMesh(LOAD_OBJECT( UStaticMesh, UHD_Paths::M_Cube_1x1 )) ;
  BarrierMeshComp->OnComponentBeginOverlap.AddDynamic( this, &AA_Barrier::OnOverlapBegin);
  BarrierMeshComp->SetCollisionResponseToChannel( ECC_Pawn, ECR_Overlap );

  // Create and attach the emitter 1
  EmitterMeshComp1 = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "EmitterMeshComp1" ) );
  TVALID( EmitterMeshComp1 );
  EmitterMeshComp1->SetupAttachment( RootSceneComponent );

  // Create and attach the emitter 2
  EmitterMeshComp2 = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "EmitterMeshComp2" ) );
  TVALID( EmitterMeshComp2 );
  EmitterMeshComp2->SetupAttachment( RootSceneComponent );

  // Create and attach the emitter 3
  EmitterMeshComp3 = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "EmitterMeshComp3" ) );
  TVALID( EmitterMeshComp3 );
  EmitterMeshComp3->SetupAttachment( RootSceneComponent );

  // Create and attach the emitter 4
  EmitterMeshComp4 = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "EmitterMeshComp4" ) );
  TVALID( EmitterMeshComp4 );
  EmitterMeshComp4->SetupAttachment( RootSceneComponent );

  // Create the timeline component
  TimelineComponent = CreateDefaultSubobject<UTimelineComponent>( TEXT( "TimelineComponent" ) );
  TVALID( TimelineComponent );

  // Set BarrierMaterial
  BarrierMaterial = LOAD_OBJECT( UMaterialInstance, UHD_Paths::Mat_FieldPulserOrangeSmall );

  // Set default values for configurable properties
  Length = 100.f;
  Wide = 100.f;
  Thickness = 1.f;
  bStartActive = false;
  Duration = 1;
  bIsActive = false;

  // Create Curve
  BarrierCurve = CreateDefaultSubobject<UCurveFloat>( "Curve" );
  BarrierCurve->FloatCurve.AddKey( 0.f, 0.f );
  BarrierCurve->FloatCurve.AddKey( Duration, 1.f );

  // Bind timeline finished events to custom handler
  FOnTimelineEvent BarrierTLFinish;
  BarrierTLFinish.BindUFunction( this, FName( "OnTimelineFinished" ) );
  TimelineComponent->SetTimelineFinishedFunc( BarrierTLFinish );

  // Bind timeline update events to custom handler
  FOnTimelineFloat BarrierTLUpdate;
  BarrierTLUpdate.BindUFunction( this, FName( "UpdateTimeline" ) );
  TimelineComponent->AddInterpFloat( BarrierCurve, BarrierTLUpdate );

}

void AA_Barrier::OnConstruction( const FTransform& Transform )
{
  Super::OnConstruction( Transform );

  // Update meshes
  UpdateMeshes();

  // Check if BarrierMeshComp and BarrierMaterial is valid, and create a new DynamicMaterial
  TVALID( BarrierMeshComp && BarrierMaterial );
  DynamicMaterial = UMaterialInstanceDynamic::Create( BarrierMaterial, this );

  // Check if DynamicMaterial is valid and set as material from BarrierMeshComp
  TVALID( DynamicMaterial );
  BarrierMeshComp->SetMaterial( 0, DynamicMaterial );

  // Update the scale using var Length
  UpdateScales( Length );

  // Update the visibility and collision according to bStartActive
  BarrierMeshComp->SetHiddenInGame(!bStartActive);
  BarrierMeshComp->SetVisibility(bStartActive);
  BarrierMeshComp->SetCollisionEnabled(bStartActive ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AA_Barrier::BeginPlay()
{
	Super::BeginPlay();
  
  // Update the active state according to bStartActive
  bIsActive = bStartActive;
}

// Called every frame
void AA_Barrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AA_Barrier::Activate( const bool bInstant )
{
  TVALID( TimelineComponent );

  // If the barrier is already activated, it doesn't perform the animation
  if (!bIsActive)
  {
    // Sets the barrier visible before animation
    BarrierMeshComp->SetHiddenInGame( false );
    BarrierMeshComp->SetVisibility( true );
    BarrierMeshComp->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics );

    // if bInstant is true, it directly updates the scale using Length, else plays the timeline
    if (bInstant)
    {
      UpdateScales( Length );
    }
    else
    {
      TimelineComponent->Play();
    }
  }
  else
  {
    // Set timeline at the end
    TimelineComponent->SetNewTime( TimelineComponent->GetTimelineLength() );
  }

  bIsActive = true;
}

void AA_Barrier::Deactivate( const bool bInstant )
{
  TVALID( TimelineComponent );

  // If the barrier is already deactivated, it doesn't perform the animation
  if (bIsActive)
  {
    // if bInstant is true, it directly updates the scale using Length, else plays the timeline
    if(bInstant)
    {
      // Set the scale to the minimum float
      UpdateScales( TNumericLimits<float>().Min());
    }
    else
    {
      // Reverse the timeline
      TimelineComponent->Reverse();
    }
  }

  bIsActive = false;
}

void AA_Barrier::Reset()
{
  UpdateScales( 100 );
}

void AA_Barrier::OnTimelineFinished()
{
  // Shows or hides the barrier at the end of the animation according to bIsActive
  BarrierMeshComp->SetHiddenInGame( !bIsActive );
  BarrierMeshComp->SetVisibility( bIsActive );
  BarrierMeshComp->SetCollisionEnabled( bIsActive ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision );
}

void AA_Barrier::UpdateTimeline( float Value )
{
#if WITH_EDITOR
  // Get world.
  UWorld* World = GetWorld();

  // If world is invalid or not a game world, abort.
  if(!World || !World->IsGameWorld()) { return; }
#endif

  // Check if DynamicMaterial is valid and set a new ScalarParameterValue
  TVALID( DynamicMaterial );
  float NewBright = FMath::Lerp( 0, BarrierBrightness, Value );
  DynamicMaterial->SetScalarParameterValue( ParameterName, NewBright );

  //TVALID( BarrierMeshComp && DecoratorMeshComp1 && DecoratorMeshComp2 && DecoratorMeshComp3 && DecoratorMeshComp4 );
  // Calculate the new distance based on the timeline value and BarrierDistance
  float NewDistance = FMath::Lerp( TNumericLimits<float>().Min(), Length, Value);

  // Adjust BarrierMeshComp length
  UpdateScales( NewDistance );
}

void AA_Barrier::UpdateMeshes()
{
  // Check if the components are valid
  TVALID( EmitterMeshComp1 && EmitterMeshComp2 && EmitterMeshComp3 && EmitterMeshComp4 && BarrierMeshComp );

  // Assign static meshes to EmitterMesh components
  EmitterMeshComp1->SetStaticMesh( EmitterMesh1 );
  EmitterMeshComp2->SetStaticMesh( EmitterMesh2 );
  EmitterMeshComp3->SetStaticMesh( EmitterMesh3 );
  EmitterMeshComp4->SetStaticMesh( EmitterMesh4 );

  // Set relative locations for EmitterMesh components
  EmitterMeshComp1->SetRelativeLocation( FVector( -9, (Wide * -100) - 9, (Thickness * 50) - 13 ) );
  EmitterMeshComp2->SetRelativeLocation( FVector( Length + 9, (Wide * -100) - 9, (Thickness * 50) + 13 ) );
  EmitterMeshComp3->SetRelativeLocation( FVector( Length + 9, 9, (Thickness * 50) - 13 ) );
  EmitterMeshComp4->SetRelativeLocation( FVector( -9, 9, (Thickness * 50) + 13 ) );

  // Set relative rotations for EmitterMesh components
  EmitterMeshComp1->SetRelativeRotation( FRotator( 90, -90, 0 ) );
  EmitterMeshComp2->SetRelativeRotation( FRotator( -90, 90, 0 ) );
  EmitterMeshComp3->SetRelativeRotation( FRotator( 90, 90, 0 ) );
  EmitterMeshComp4->SetRelativeRotation( FRotator( -90, -90, 0 ) );

}

void AA_Barrier::UpdateScales( float NewDistance )
{
  // Adjust BarrierMeshComp length
  BarrierMeshComp->SetRelativeLocation( FVector( 0, 0, 0 ) );
  FVector BarrierScale = BarrierMeshComp->GetComponentScale();
  BarrierScale.X = NewDistance / 100;
  BarrierScale.Y = Wide / 1;
  BarrierScale.Z = Thickness / 1;
  BarrierMeshComp->SetRelativeScale3D( BarrierScale );
}

void AA_Barrier::OnOverlapBegin( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
  if(OtherActor)
  {
    // Check if OtherActor is a Ammo type
    AA_Ammo* Ammo = Cast<AA_Ammo>( OtherActor );
    if(Ammo)
    {
      // Get the instigator from Ammo
      TSubclassOf<AActor> InstigatorActor = Ammo->GetInstigator()->GetClass();

      // Check if the instigator is an allowed class      
      bool bIsAllowed = UFL_General::IsInActorClassArray(InstigatorActor, AllowedActorClasses, true);

      if(bIsAllowed)
      {
        // The instigator is allowed class
        UE_LOG( LogTemp, Log, TEXT( "Pass Ammo" ) );
      }
      else
      {
        // The instigator is not allowed class, destroy the ammo
        UE_LOG( LogTemp, Log, TEXT( "Destroy Ammo" ) );
        Ammo->Destroy();
      }
    }
  }
}

