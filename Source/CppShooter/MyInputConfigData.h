#pragma once
 
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "MyInputConfigData.generated.h"
 
/**
 * Store pointer to Input Action for native binding
*/
UCLASS()
class CPPSHOOTER_API UMyInputConfigData : public UDataAsset
{
    GENERATED_BODY()
 
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputMove;
 
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputLook;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputJump;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputShoot;
};