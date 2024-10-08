// All rights reserved to EthernalStudios OÜ


#include "World/Danger/A_Harmfield.h"
#include "../../../Public/General/Libraries/FL_Errors.h"
#include "../../../Public/World/Danger/AC_DangerArea_Sphere.h"
#include "../../../Public/World/Danger/AC_DangerArea_Box.h"
#include "../../../Public/World/Danger/AC_DangerArea_Capsule.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "../../../Public/General/Libraries/FL_General.h"
#include "../../../Public/General/Libraries/FL_Damage.h"

/* INIT */
  AA_Harmfield::AA_Harmfield( TSubclassOf<UShapeComponent> ShapeClass )
  {
		// Validate Shape class
		TVALID( ShapeClass );

		// Create and validate influence area component.
		ShapeComponent = Cast<UShapeComponent>( CreateDefaultSubobject( "Harmfield_ShapeComponent", ShapeClass, ShapeClass, 1, 0 ) );
		TVALID( ShapeComponent );

		// Create and validate danger area component.
		if (ShapeClass == USphereComponent::StaticClass())
		{
			DangerAreaComponent = CreateDefaultSubobject<UAC_DangerArea_Sphere>(TEXT("Harmfield_DangerAreaComponent"));
		}
		else if (ShapeClass == UBoxComponent::StaticClass())
		{
			DangerAreaComponent = CreateDefaultSubobject<UAC_DangerArea_Box>( TEXT( "Harmfield_DangerAreaComponent" ) );
		}
		else if(ShapeClass == UCapsuleComponent::StaticClass())
		{
			DangerAreaComponent = CreateDefaultSubobject<UAC_DangerArea_Capsule>( TEXT( "Harmfield_DangerAreaComponent" ) );
		}
		TVALID( DangerAreaComponent );

		// Set collisions of ShapeComponent
		ShapeComponent->SetCollisionResponseToAllChannels( ECR_Overlap );
		ShapeComponent->SetCollisionEnabled( ECollisionEnabled::NoCollision);
		ShapeComponent->SetGenerateOverlapEvents( true );

  }

  void AA_Harmfield::BeginPlay()
  {
		Super::BeginPlay();

		if (HarmfieldParams.bStartActive)
		{
			ToggleField( true );
		}
  }

  void AA_Harmfield::OnConstruction( const FTransform& Transform )
  {
		Super::OnConstruction( Transform );

		TVALID( DangerAreaComponent );

		//TVALIDPERM( HarmfieldParams.AreParamsValid() );

		RefreshShape();
  }

  void AA_Harmfield::RefreshShape()
  {

  }

  bool AA_Harmfield::IsFieldActive()
  {
		if (ShapeComponent->GetCollisionEnabled() == ECollisionEnabled::NoCollision)
		{
      return false;
		}
		else
		{
			return true;
		}
  }

  void AA_Harmfield::TriggerFieldDamage()
  {
		TVALID( IsFieldActive() );
		TArray<AActor*> OverlappingActors;
		ShapeComponent->GetOverlappingActors( OverlappingActors );

		for (AActor* Actor : OverlappingActors)
		{
      if(UFL_General::IsInActorClassArray( Actor->GetClass(), HarmfieldParams.AllowedTargets, 1 ) && !UFL_General::IsInActorClassArray( Actor->GetClass(), HarmfieldParams.ForbiddenTargets, 1 ))
			{
				UFL_Damage::DealDamage( HarmfieldParams.DamageData, Actor );
			}
		}
  }

  void AA_Harmfield::ToggleField( bool bActivate )
  {
		if(bActivate)
		{
			if (IsFieldActive()){ TWARNING( "Field is Active", 1 ); return;	}
			ShapeComponent->SetCollisionEnabled( ECollisionEnabled::QueryOnly );
			GetWorldTimerManager().ClearTimer( LifeTimer );
			GetWorldTimerManager().SetTimer( LifeTimer, this, &AA_Harmfield::OnLifeTimeEnd, HarmfieldParams.FieldLifeTime, false );
			GetWorldTimerManager().SetTimer( DamageTimer, this, &AA_Harmfield::TriggerFieldDamage, HarmfieldParams.DamageTick, true );
			if (HarmfieldParams.bFirstDamageInstant)	{	TriggerFieldDamage();	}
			DangerAreaComponent->Activate( true );
			VFXRef = UNiagaraFunctionLibrary::SpawnSystemAtLocation( GetWorld(), HarmfieldParams.FieldNiagara, GetActorLocation(), GetActorRotation() );
			TVALID(VFXRef);
			VFXRef->SetNiagaraVariableFloat( "Lifetime", HarmfieldParams.FieldLifeTime );
			SoundRef = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HarmfieldParams.FieldSound, GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, true );
			TVALID(SoundRef);
		}

		else
		{
			if(!IsFieldActive()){ TWARNING( "Field is not Active", 1); return ; }
			ShapeComponent->SetCollisionEnabled( ECollisionEnabled::NoCollision );
			LifeTimer.Invalidate();
			DamageTimer.Invalidate();
			DangerAreaComponent->Activate( false );
			VFXRef->Deactivate();
			VFXRef->SetAsset( nullptr );
			SoundRef->FadeOut( HarmfieldParams.SoundFadeOutTime, 1.f);
			SoundRef->SetSound( nullptr );
		}
  }

  void AA_Harmfield::OnLifeTimeEnd()
  {
		if (HarmfieldParams.bDieAtEnd)
		{
			Destroy();
		}
		else
		{
			ToggleField( false );
		}
  }

  void AA_Harmfield::SetFieldParams( FHarmfieldParams& InParams )
  {
		TVALIDPERM( InParams.AreParamsValid() );
		HarmfieldParams = InParams;
		RefreshShape();
  }
