// All rights reserved to EthernalStudios OÜ

#pragma once

#include "CoreMinimal.h"
#include "World/Danger/A_Harmfield.h"
#include "A_Harmfield_Box.generated.h"

/**
 * 
 */
UCLASS()
class ETHERNAL_API AA_Harmfield_Box : public AA_Harmfield
{
	GENERATED_BODY()

#pragma region CONFIG
protected:

#pragma endregion

#pragma region DATA
#pragma endregion

#pragma region COMPONENTS
#pragma endregion

#pragma region REFERENCES
#pragma endregion

#pragma region TIMERS
#pragma endregion

#pragma region INIT
	public:
    AA_Harmfield_Box();

    virtual void OnConstruction( const FTransform& Transform ) override;

    virtual void RefreshShape() override;
#pragma endregion

#pragma region GETTERS
#pragma endregion

#pragma region SETTERS
public:
    void SetNewBoxExtent( FVector BoxExtent );
#pragma endregion

#pragma region FUNCTIONS
#pragma endregion

#pragma region DELEGATES
#pragma endregion
};
