// All rights reserved to EthernalStudios OÜ


#include "World/Danger/A_Harmfield_Box.h"
#include "Components/BoxComponent.h"
#include "General/Libraries/FL_Errors.h"
#include "World/Danger/AC_DangerArea_Box.h"

AA_Harmfield_Box::AA_Harmfield_Box(): Super( UBoxComponent::StaticClass() )
{
  // Default Extent values
  HarmfieldParams.BoxExtentX = 1.f;
  HarmfieldParams.BoxExtentY = 1.f;
  HarmfieldParams.BoxExtentZ = 1.f;
}

void AA_Harmfield_Box::OnConstruction( const FTransform& Transform )
{
  Super::OnConstruction( Transform );

  TVALIDPERM( HarmfieldParams.BoxExtentX && HarmfieldParams.BoxExtentY && HarmfieldParams.BoxExtentZ);

  RefreshShape();
}

void AA_Harmfield_Box::RefreshShape()
{
  UAC_DangerArea_Box* DangerBox = Cast<UAC_DangerArea_Box>( DangerAreaComponent );
  UBoxComponent* BoxShape = Cast<UBoxComponent>( ShapeComponent );
  TVALID( DangerBox );
  TVALID( BoxShape );

  DangerBox->SetBoxExtent( HarmfieldParams.BoxExtentX, HarmfieldParams.BoxExtentY, HarmfieldParams.BoxExtentZ );
  DangerBox->SetInfluenceExtent( HarmfieldParams.DangerOverreach );
  FVector BoxExtent( HarmfieldParams.BoxExtentX, HarmfieldParams.BoxExtentY, HarmfieldParams.BoxExtentZ );
  BoxShape->SetBoxExtent( BoxExtent );
}

void AA_Harmfield_Box::SetNewBoxExtent( FVector BoxExtent )
{
  TVALIDPERM( BoxExtent.X >= 1.f && BoxExtent.Y >= 1.f && BoxExtent.Z >= 1.f);
  HarmfieldParams.BoxExtentX = BoxExtent.X;
  HarmfieldParams.BoxExtentY = BoxExtent.Y;
  HarmfieldParams.BoxExtentZ = BoxExtent.Z;

  RefreshShape();
}
