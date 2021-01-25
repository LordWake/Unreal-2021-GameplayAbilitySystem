//+---------------------------------------------------------+
//| Project   : Gameplay Ability System Demo	UE 4.25		|
//| Author    : github.com/LordWake					 		|		
//+---------------------------------------------------------+

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GATargetActorGroundSelect.generated.h"

class UDecalComponent;
class USceneComponent;

UCLASS()
class ABILITYSYSTEM_API AGATargetActorGroundSelect : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:

	AGATargetActorGroundSelect();

	virtual void Tick(float DeltaSeconds) override;

	virtual void StartTargeting(UGameplayAbility* Ability) override;
	
	/*Makes an overlap sphere to check the pawns inside that area*/
	virtual void ConfirmTargetingAndContinue() override;
	
	/*Returns where the player is looking at to use that point in the sphere radius*/
	UFUNCTION(BlueprintCallable, Category = "GroundSelect")
	bool GetPlayerLookingPoint(FVector& OutViewPoint);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "GroundSelect")
	float Radius;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GroundBlast")
	UDecalComponent* Decal;

	USceneComponent*RootComp;

};
