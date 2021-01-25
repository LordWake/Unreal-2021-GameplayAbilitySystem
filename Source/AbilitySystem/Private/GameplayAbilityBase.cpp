//+---------------------------------------------------------+
//| Project   : Gameplay Ability System Demo	UE 4.25		|
//| Author    : github.com/LordWake					 		|		
//+---------------------------------------------------------+


#include "GameplayAbilityBase.h"
#include "AbilityTypes.h"

FGameplayAbilityInfo UGameplayAbilityBase::GetAbilityInfo()
{
	//Return two gameplay effects
	UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect();
	UGameplayEffect* CostEffect		= GetCostGameplayEffect();

	//If this ability, has a cooldown and it has a cost
	if (CooldownEffect && CostEffect)
	{
		//Because GetStaticMagnitudeIfPossible works by reference, we make this variable.
		float CoolDownDuration = 0;
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(1, CoolDownDuration);

		float Cost = 0;
		EAbilityCostType CostType;

		//If we have more than one modifier in out GameplayEffect... (check the blueprint)
		if (CostEffect->Modifiers.Num() > 0)
		{
			//We take the first modifier, we don't have others either.
			FGameplayModifierInfo EffectInfo = CostEffect->Modifiers[0];
			//We make the same as we did in the cooldown, pass a float by reference
			EffectInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(1, Cost);

			//Here we save what type of attribute this effect will... well, affect. 
			//It will be health, mana, strength?
			FGameplayAttribute CostAttribute = EffectInfo.Attribute;
			
			//Then, we save that attribute name
			FString AttributeName = CostAttribute.AttributeName;

			//And, with that information, now we can check what costType we're taking about.
			if (AttributeName == "Health")
				CostType = EAbilityCostType::Health;
			else if (AttributeName == "Mana")
				CostType = EAbilityCostType::Mana;
			else if (AttributeName == "Strength")
				CostType = EAbilityCostType::Strength;

			//With all the data we need, we made it! We have our FGameplayAbilityInfo already created, 
			//we are so amazing, I'm so proud of you that you're reading this code.
			return FGameplayAbilityInfo(CoolDownDuration, Cost, CostType, UIMaterial, GetClass());
		}
	}	
	
	//Otherwise, if it doesn't have a cooldown and a cost, we return this unseful constructor.
	//I mean, look at this guy, so lonely and without parameters, why'd you need this?
	return FGameplayAbilityInfo();
}
