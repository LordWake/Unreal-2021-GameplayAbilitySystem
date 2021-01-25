//+---------------------------------------------------------+
//| Project   : Gameplay Ability System Demo	UE 4.25		|
//| Author    : github.com/LordWake					 		|		
//+---------------------------------------------------------+

#include "AttributeSetBase.h"

#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"

#include "CharacterBase.h"


UAttributeSetBase::UAttributeSetBase()
	:Health(100.0f), 
	MaxHealth(100.0f),
	Mana(100.0f),
	MaxMana(100.0f),
	Strength(250.0f),
	MaxStrength(250.0f),
	AttackDamage(20.0f),
	Armor(5.0f)
{	}

void UAttributeSetBase::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	//If we have this value 
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Health)))
	{
		//It will update it
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.f, MaxHealth.GetCurrentValue()));
		Health.SetBaseValue(FMath::Clamp(Health.GetBaseValue(), 0.f, MaxHealth.GetCurrentValue()));
		
		OnHealthChange.Broadcast(Health.GetCurrentValue(), MaxHealth.GetCurrentValue()); //And this is gonna be call from the character to update our beautiful next-gen UI.

		ACharacterBase* CharacterOwner = Cast<ACharacterBase>( GetOwningActor());
		
		//We update this tag so we are not able to play the "Health Regen" ability if our health is full.
		//That doesn't have sense right? Yeah, I think the same.
		if (Health.GetCurrentValue() == MaxHealth.GetCurrentValue())
		{
			if(CharacterOwner)
				CharacterOwner->AddGameplayTag(CharacterOwner->FullHealthTag);
		}
		
		//But, if the health has been updated and we don't have a full health value, we remove that tag
		//So if we want to play the Health Regen Ability we can. This does have sense right? You're so smart.
		else
		{
			if (CharacterOwner)
				CharacterOwner->RemoveGameplayTag(CharacterOwner->FullHealthTag);
		}
	}

	//Then we upadte the mana 
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Mana)))
	{
		Mana.SetCurrentValue(FMath::Clamp(Mana.GetCurrentValue(), 0.f, MaxMana.GetCurrentValue()));
		Mana.SetBaseValue(FMath::Clamp(Mana.GetBaseValue(), 0.f, MaxMana.GetCurrentValue()));
		OnManaChange.Broadcast(Mana.GetCurrentValue(), MaxMana.GetCurrentValue());
	}

	//And the strength of course, we are into fitness life.
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Strength)))
	{
		Strength.SetCurrentValue(FMath::Clamp(Strength.GetCurrentValue(), 0.f, MaxStrength.GetCurrentValue()));
		Strength.SetBaseValue(FMath::Clamp(Strength.GetBaseValue(), 0.f, MaxStrength.GetCurrentValue()));
		OnStrengthChange.Broadcast(Strength.GetCurrentValue(), MaxStrength.GetCurrentValue());
	}
}
