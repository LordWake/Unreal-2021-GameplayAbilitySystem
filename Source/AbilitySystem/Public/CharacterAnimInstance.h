//+---------------------------------------------------------+
//| Project   : Gameplay Ability System Demo	UE 4.25		|
//| Author    : github.com/LordWake					 		|		
//+---------------------------------------------------------+

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

UCLASS()
class ABILITYSYSTEM_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(BlueprintReadWrite)
	FVector PlayerSpaceVelocity;

	UPROPERTY(BlueprintReadWrite)
	bool bPlayerIsFalling;

	UPROPERTY(BlueprintReadWrite)
	bool bShouldDoFullBody;

};
