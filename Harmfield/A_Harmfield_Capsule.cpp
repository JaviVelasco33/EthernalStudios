// All rights reserved to EthernalStudios OÜ


#include "World/Danger/A_Harmfield_Capsule.h"
#include "Components/CapsuleComponent.h"
#include "World/Danger/AC_DangerArea_Capsule.h"
#include "General/Libraries/FL_Errors.h"

AA_Harmfield_Capsule::AA_Harmfield_Capsule(): Super( UCapsuleComponent::StaticClass() )
{
  //Default size values
  HarmfieldParams.SphereOrCapsuleRadius = 1.f;
  HarmfieldParams.CapsuleHalfHeight = 1.f;
}

void AA_Harmfield_Capsule::OnConstruction( const FTransform& Transform )
{
  Super::OnConstruction( Transform );

  TVALIDPERM( HarmfieldParams.CapsuleHalfHeight && HarmfieldParams.SphereOrCapsuleRadius );

  RefreshShape();
}

void AA_Harmfield_Capsule::RefreshShape()
{
  UAC_DangerArea_Capsule* DangerCapsule = Cast<UAC_DangerArea_Capsule>( DangerAreaComponent );
  UCapsuleComponent* CapsuleShape = Cast<UCapsuleComponent>( ShapeComponent );
  TVALID( DangerCapsule );
  TVALID( CapsuleShape );

  DangerCapsule->SetCapsuleSize( HarmfieldParams.SphereOrCapsuleRadius, HarmfieldParams.CapsuleHalfHeight );
  DangerCapsule->SetInfluenceExtent( HarmfieldParams.DangerOverreach );
  CapsuleShape->SetCapsuleSize( HarmfieldParams.SphereOrCapsuleRadius, HarmfieldParams.CapsuleHalfHeight );
}

void AA_Harmfield_Capsule::SetNewSize( float NewHeigth, float NewRadius )
{
  TVALIDPERM( NewHeigth >= 1.f && NewRadius >= 1.f);
  HarmfieldParams.CapsuleHalfHeight = NewHeigth;
  HarmfieldParams.SphereOrCapsuleRadius = NewRadius;

  RefreshShape();
}
