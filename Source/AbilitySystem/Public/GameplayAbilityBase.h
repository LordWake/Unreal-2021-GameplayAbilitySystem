//+---------------------------------------------------------+
//| Project   : Gameplay Ability System Demo	UE 4.25		|
//| Author    : github.com/LordWake					 		|		
//+---------------------------------------------------------+

#pragma once

#include "CoreMinimal.h"
#include "AbilityTypes.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbilityBase.generated.h"

/*This will be the master GameplayAbilityClass. All the other abilities are going to inherit from here, 
so you can have access to some awesome stuff I coded in this class.*/

UCLASS()
class ABILITYSYSTEM_API UGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:

	/*Like all the abilities are going to inherit from here, they'll have access to this
	UMaterial to edit from UE4. So when the abilities use the FGameplayAbilityInfo (see AbilityTypes.h)
	and that struct ask for a UMaterial in their constructor, we are gonna pass this shit.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityBase")
	UMaterialInstance* UIMaterial;

	/*This will create an AbilityInfo from the constructor and return it */
	UFUNCTION(BlueprintCallable, Category = "AbilityBase")
	FGameplayAbilityInfo GetAbilityInfo();

};
