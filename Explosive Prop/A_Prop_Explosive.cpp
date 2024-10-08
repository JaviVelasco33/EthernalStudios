// All rights reserved to EthernalStudios OÜ


#include "World/Props/A_Prop_Explosive.h"
#include "World/Explosions/A_Explosion.h"
#include "General/Data/Structs/S_DamageData.h"
#include <math.h>

/* HEALTH */

void AA_Prop_Explosive::OnDeath(FDamageData Damage) {
	// .
	Damage.Force = ExplosionParams.ExplosionArea / (ExplosionParams.ExplosionExpandTime > 0.f ? ExplosionParams.ExplosionExpandTime : HUGE);
	Damage.HitData.Location = GetActorLocation();

	// .
	Super::OnDeath(Damage);

	FActorSpawnParameters SpawnParams;
	FTransform Transform;
	Transform.SetLocation(GetActorLocation());

  AA_Explosion* Explosion = GetWorld()->SpawnActor<AA_Explosion>( AA_Explosion::StaticClass(), Transform, SpawnParams );

  TVALID( Explosion );

  Explosion->SetExplosionFromParams( ExplosionParams );

  TArray<AActor*> ToIgnore;
  Explosion->TriggerExplosion( true, ToIgnore);
}
