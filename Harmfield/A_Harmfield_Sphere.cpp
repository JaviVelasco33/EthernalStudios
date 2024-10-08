// All rights reserved to EthernalStudios OÜ


#include "World/Danger/A_Harmfield_Sphere.h"
#include "Components/SphereComponent.h"
#include "World/Danger/AC_DangerArea_Sphere.h"
#include "General/Libraries/FL_Errors.h"

AA_Harmfield_Sphere::AA_Harmfield_Sphere(): Super( USphereComponent::StaticClass())
{
  // Default Radius value
  HarmfieldParams.SphereOrCapsuleRadius = 1.f;
}

void AA_Harmfield_Sphere::OnConstruction( const FTransform& Transform )
{
  Super::OnConstruction( Transform );

  TVALIDPERM( HarmfieldParams.SphereOrCapsuleRadius );

  RefreshShape();
}

void AA_Harmfield_Sphere::RefreshShape()
{
  UAC_DangerArea_Sphere* DangerSphere = Cast<UAC_DangerArea_Sphere>( DangerAreaComponent );
  USphereComponent* SphereShape = Cast<USphereComponent>( ShapeComponent );
  TVALID( DangerSphere );
  TVALID( SphereShape );

  DangerSphere->SetSphereRadius( HarmfieldParams.SphereOrCapsuleRadius );
  DangerSphere->SetInfluenceExtent( HarmfieldParams.DangerOverreach );
  SphereShape->SetSphereRadius( HarmfieldParams.SphereOrCapsuleRadius );
}

void AA_Harmfield_Sphere::SetNewRadius( float NewRadius )
{
  TVALIDPERM( NewRadius >= 1.f );
  HarmfieldParams.SphereOrCapsuleRadius = NewRadius;

  RefreshShape();
}
