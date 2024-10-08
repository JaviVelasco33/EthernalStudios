// All rights reserved to EthernalStudios OÜ

#include "World/Doors/A_Door.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Characters/Player/Main/CH_Player.h"
#include "Characters/Enemies/Main/CH_Enemy_Base.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "General/Libraries/FL_Errors.h"
#include "General/Libraries/FL_General.h"

// Sets default values
AA_Door::AA_Door() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize components
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>( TEXT( "RootSceneComponent" ) );
	RootComponent = RootSceneComponent;

	DoorMesh = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT( "DoorMesh" ) );
	DoorMesh->SetupAttachment( RootSceneComponent );
    DoorMesh->SetCollisionProfileName("BlockAll", 0);
    DoorMesh->CanCharacterStepUpOn = ECB_No;
    DoorMesh->SetGenerateOverlapEvents(1);
    DoorMesh->SetNotifyRigidBodyCollision(1);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>( TEXT( "TriggerBox" ) );
	TriggerBox->SetupAttachment( RootSceneComponent );
	TriggerBox->OnComponentBeginOverlap.AddUniqueDynamic( this, &AA_Door::OnOverlapBegin );
	TriggerBox->OnComponentEndOverlap.AddUniqueDynamic( this, &AA_Door::OnOverlapEnd );

	// Initialize variables
	AllowedActorClasses = { ACH_Player::StaticClass(), ACH_Enemy_Base::StaticClass() };
	AutoCloseDelay = 0.0f;
	LightMaterialParameterName = "Color";
	bStartPoweredOn = true;
	bStartBlocked = false;
	bBlocked = false;
    StartState = 0;
    bIsOpen = 0;
    bInTransition = 0;
	bIsPoweredOn = true;
	bAutomaticDoor = false;
	UnlockedSound = nullptr;

    DoorMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
}

void AA_Door::OnConstruction(const FTransform& Transform) {
    Super::OnConstruction(Transform);
    SetDoorPowered(bStartPoweredOn);
    SetDoorBlocked(bStartBlocked);
}

// Called when the game starts or when spawned
void AA_Door::BeginPlay() {
  switch (StartState) {
    case 0: {
        CloseDoor(1);
        break;
    }
    case 1: {
        OpenDoor(1);
        if (bAutomaticDoor) { AutoClose(); }
        break;
    }
    case 2: {
        TVALIDPERM_NRET_CODE(break, bIsPoweredOn);
        CloseDoor(0);
        break;
    }
    case 3: {
        TVALIDPERM_NRET_CODE(break, bIsPoweredOn && !bBlocked);
        OpenDoor(0);
        if (bAutomaticDoor) { AutoClose(); }
        break;
    }
  }


  Super::BeginPlay();

    
}

// Called every frame
void AA_Door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (!bIsPoweredOn) { return; }

    if (bInTransition) {

        if (IsDoorOpened()) { CheckAutoClose(); OnDoorOpened.Broadcast(this); bInTransition = 0; TWARNING("DOOR OPENED!", 1); }
        
        else if (IsDoorClosed()) { OnDoorClosed.Broadcast(this); bInTransition = 0; TWARNING("DOOR CLOSED!", 1); }
    }

    return;
}

FLinearColor AA_Door::GetLightColor() const {
    if (bIsPoweredOn) { return bBlocked ? FLinearColor::Red : FLinearColor::Green; }
    return FLinearColor::Black;
}

bool AA_Door::IsDoorOpening() {
    TVALID_RET(0, DoorMesh);
    
    UAnimSingleNodeInstance* SingleNodeInstance = DoorMesh->GetSingleNodeInstance();
    TVALID_RET(0, SingleNodeInstance);

    return bIsOpen && SingleNodeInstance->GetAnimationAsset() && SingleNodeInstance->IsPlaying();
}

bool AA_Door::IsDoorOpened() {
    TVALID_RET(0, DoorMesh);
    
    UAnimSingleNodeInstance* SingleNodeInstance = DoorMesh->GetSingleNodeInstance();
    TVALID_RET(0, SingleNodeInstance);

    return bIsOpen && SingleNodeInstance->GetAnimationAsset() && !SingleNodeInstance->IsPlaying();
}

bool AA_Door::IsDoorClosing() {
    TVALID_RET(0, DoorMesh);
    
    UAnimSingleNodeInstance* SingleNodeInstance = DoorMesh->GetSingleNodeInstance();
    TVALID_RET(0, SingleNodeInstance);

    return !bIsOpen && SingleNodeInstance->GetAnimationAsset() && SingleNodeInstance->IsPlaying();
}

bool AA_Door::IsDoorClosed() {
    TVALID_RET(0, DoorMesh);
    
    UAnimSingleNodeInstance* SingleNodeInstance = DoorMesh->GetSingleNodeInstance();
    TVALID_RET(0, SingleNodeInstance);

    return !bIsOpen && SingleNodeInstance->GetAnimationAsset() && !SingleNodeInstance->IsPlaying();
}

void AA_Door::SetDoorBlocked(const bool bBlock) {
    bBlocked = bBlock;

    if (bIsPoweredOn && HasActorBegunPlay())
    {
        UGameplayStatics::PlaySoundAtLocation(this, bBlocked ? BlockedSound : UnlockedSound, GetActorLocation());
        if (bBlocked && (IsDoorOpened() || IsDoorOpening())) { CloseDoor(0); }
    }

    UpdateLights();
	return;
}

void AA_Door::SetDoorPowered(const bool bOn) {
    bIsPoweredOn = bOn;

    DoorMesh->bPauseAnims = !bOn;

    UpdateLights();

    if (bBlocked && (IsDoorOpened() || IsDoorOpening())) { CloseDoor(0); }

	else { CheckAutoClose(); }
}

void AA_Door::SetDoorAutomatic(const bool bAuto) {
	if (bAutomaticDoor == bAuto) { return; }
	bAutomaticDoor = bAuto;
	if (!bAuto) { GetWorldTimerManager().ClearTimer(AutoCloseTimerHandle); }
	else { OnOverlapEnd(nullptr, nullptr, nullptr, -1); }
	return;
}

void AA_Door::CloseDoor(const bool bInstant)
{
    if (IsDoorClosed() || IsDoorClosing()) { TWARNING("Tried to close a door that is closed or closing!", 0); return; }
    
    else if (!bInstant && !bIsPoweredOn) { TWARNING("Tried to close a door but it is powered off!", 0); return; }

    float DoorPosition = CloseAnimation->GetPlayLength();
    if (!bInstant) { DoorPosition = IsDoorOpening() ? OpenAnimation->GetPlayLength() - DoorMesh->GetPosition() : 0.f; }
            
    DoorMesh->PlayAnimation(CloseAnimation, false);
    DoorMesh->SetPosition(DoorPosition);

    bIsOpen = 0;
    bInTransition = !bInstant;

    if (HasActorBegunPlay()) {
        if (bInstant) { OnDoorClosed.Broadcast(this); }
        else { OnDoorClosing.Broadcast(this); }
    }

    GetWorldTimerManager().ClearTimer(AutoCloseTimerHandle);
    
    if (CloseSound && !bInstant && HasActorBegunPlay()) { UGameplayStatics::PlaySoundAtLocation(this, CloseSound, GetActorLocation()); }

    return;
}

void AA_Door::OpenDoor(const bool bInstant)
{
    if (IsDoorOpened() || IsDoorOpening()) { TWARNING("Tried to open a door that is opened or opening!", 0); return; }

    else if (!bInstant && !bIsPoweredOn) { TWARNING("Tried to open a door but it is powered off!", 0); return; }

    else if (bBlocked) {
        UGameplayStatics::PlaySoundAtLocation(this, BlockedSound, GetActorLocation());
        TWARNING("Tried to open a door but it is blocked!", 0);
        return;
    }

    float DoorPosition = OpenAnimation->GetPlayLength();
    if (!bInstant) { DoorPosition = IsDoorClosing() ? CloseAnimation->GetPlayLength() - DoorMesh->GetPosition() : 0.f; }

    DoorMesh->PlayAnimation(OpenAnimation, false);
    DoorMesh->SetPosition(DoorPosition);
    
    bIsOpen = 1;
    bInTransition = !bInstant;

    if (HasActorBegunPlay()) {
        if (bInstant) { OnDoorOpened.Broadcast(this); }
        else { OnDoorOpening.Broadcast(this); }
    }

    if (OpenSound && !bInstant && HasActorBegunPlay()) { UGameplayStatics::PlaySoundAtLocation(this, OpenSound, GetActorLocation()); }

    return;
}

void AA_Door::UpdateLights()
{
    TVALID(DoorMesh);

    // Get mesh materials
    TArray<UMaterialInterface*> Materials = DoorMesh->GetMaterials();

    // .
    FLinearColor LightColor = GetLightColor();

    // Runs all indexes in LightMaterialIndexes
    for (uint8 Index : LightMaterialIndexes) {
        TVALID_NRET_CODE(continue, Materials.IsValidIndex(Index) && Materials[Index]);
        UMaterialInterface* Material = Materials[Index];

        // Converts the material to DynamicInstance
        UMaterialInstanceDynamic* DynamicMaterial = DoorMesh->CreateDynamicMaterialInstance(Index, Materials[Index]);
        TVALID_NRET_CODE(continue, DynamicMaterial);
        
        DynamicMaterial->SetVectorParameterValue(LightMaterialParameterName, LightColor);
    }
}

void AA_Door::AutoClose()
{
  if (AutoCloseDelay > 0.f)
  {
      FTimerDelegate CloseTimer = FTimerDelegate::CreateUObject(this, &AA_Door::CloseDoor, false);
    GetWorldTimerManager().SetTimer( AutoCloseTimerHandle, CloseTimer, AutoCloseDelay, false );
  }

  else { CloseDoor(0); }
}

void AA_Door::OnOverlapBegin( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
  if(bBlocked || !bIsPoweredOn || !bAutomaticDoor) return;

  TVALIDPERM(OtherActor);

  TSubclassOf<AActor> ActorClass = OtherActor->GetClass();
  TVALID(ActorClass);

  if (UFL_General::IsInActorClassArray(ActorClass, AllowedActorClasses, 1) && !UFL_General::IsInActorClassArray(ActorClass, ForbiddenActorClasses, 0))
  {
    if(!IsDoorOpened() && !IsDoorOpening())
    {
      OpenDoor(0);
    }
    GetWorldTimerManager().ClearTimer( AutoCloseTimerHandle );
  }
}

void AA_Door::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	CheckAutoClose();
    return;
}

void AA_Door::CheckAutoClose() {
    if (!bAutomaticDoor || (!IsDoorOpened() && !IsDoorOpening())) { return; }

    TArray<AActor*> OverlappingActors;
    GetOverlappingActors(OverlappingActors);

    TArray<TSubclassOf<AActor>> OverlappingActorClasses;
    for (AActor* Actor : OverlappingActors) {
        TVALIDPERM_NRET_CODE(continue, Actor);
        
        TSubclassOf<AActor> AClass = Actor->GetClass();
        TVALID_NRET_CODE(continue, AClass);

        OverlappingActorClasses.AddUnique(AClass);
    }

    UFL_General::FilterActorClassArrayByRef(AllowedActorClasses, ForbiddenActorClasses, OverlappingActorClasses, 1, 0);
    
    if (OverlappingActorClasses.IsEmpty()) { AutoClose(); }
	
	return;
}