//+---------------------------------------------------------+
//| Project   : Gameplay Ability System Demo	UE 4.25		|
//| Author    : github.com/LordWake					 		|		
//+---------------------------------------------------------+

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityTypes.generated.h"

class UGameplayAbilityBase;

UENUM(BlueprintType)
enum class EAbilityCostType : uint8
{
	Health,
	Mana,
	Strength
};

/* 
A simple struct data to save all the different types of data that we need in our abilities or in the UI.
This class it only contains data, just that like the cooldown, cost, the type of ability because
we don't want to use all abilities into the UI, etc. (Come on read the class bro, is not that hard)
*/
USTRUCT(BlueprintType)
struct FGameplayAbilityInfo
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityInfo")
	float CooldownDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityInfo")
	float Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityInfo")
	EAbilityCostType CostType;
	
	/*This is gonna be the clock material that we're gonna use in the UI.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityInfo")
	UMaterialInstance* UIMat;

	/*Here we save the ability type, so like we have the reference right here, we can play that ability from this struct. Pure art.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityInfo")
	TSubclassOf<UGameplayAbilityBase> AbilityClass;
	
	/*Beautiful constructors*/
	FGameplayAbilityInfo();
	FGameplayAbilityInfo(float InCooldownDuration, float InCost, EAbilityCostType InCostType, UMaterialInstance* InUIMat, TSubclassOf<UGameplayAbilityBase> InAbilityClass);
};
