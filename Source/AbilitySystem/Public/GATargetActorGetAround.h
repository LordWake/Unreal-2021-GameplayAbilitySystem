//+---------------------------------------------------------+
//| Project   : Gameplay Ability System Demo	UE 4.25		|
//| Author    : github.com/LordWake					 		|		
//+---------------------------------------------------------+

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GATargetActorGetAround.generated.h"


UCLASS()
class ABILITYSYSTEM_API AGATargetActorGetAround : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:

	void StartTargeting(UGameplayAbility* Ability) override;
	void ConfirmTargetingAndContinue() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "FindAround")
	float Radius;

};
