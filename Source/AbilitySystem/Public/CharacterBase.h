//+---------------------------------------------------------+
//| Project   : Gameplay Ability System Demo	UE 4.25		|
//| Author    : github.com/LordWake					 		|		
//+---------------------------------------------------------+//+---------------------------------------------------------+
//| Project   : Gameplay Ability System Demo	UE 4.25		|
//| Author    : github.com/LordWake					 		|		
//+---------------------------------------------------------+

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"

#include "CharacterBase.generated.h"


class UAttributeSetBase;
class UGameplayAbilityBase;

UENUM(BlueprintType)
enum class EAbility : uint8
{
	Melee,
	HealthRegen,
	Dash
};

UCLASS()
class ABILITYSYSTEM_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraSpring;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:

	ACharacterBase();

	//========================================================================
	//======================= ABILITY SYSTEM =================================
	//========================================================================
	
	/*Public reference to our AbilitySystemComponent. It's created on the constructor.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
	UAbilitySystemComponent* AbilitySystemComponent;

	/*Public reference to our BaseAttributeSetComponent. It's created on the constructor.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
	UAttributeSetBase* BaseAttributeSetComponent;

	/*This GameplayTag will helps us to block the HealthRegen if we have full health. Is called from AttributeSetBase.cpp*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AbilitySystem")
	FGameplayTag FullHealthTag;

	/*Called from AI_BP too. It just triggers the MeleeAttack.*/
	UFUNCTION(BlueprintCallable, Category = "Ability|Attack") 
	void MeleeAttack();

	/*Cancel Targeting Ground Blast*/
	UFUNCTION(BlueprintCallable, Category = "Ability|cANCEL")
	void CancelAbility();

	/*Adds GameplayTag on the BeginPlay.*/
	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void AddGameplayTag(FGameplayTag& TagToAdd);
	
	/*Removes GameplayTag from AttributeSetBase.*/
	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void RemoveGameplayTag(FGameplayTag& TagToRemove);

	/*Used in Dash Ability, to overlap with other enemies. */
	UFUNCTION(BlueprintCallable, Category = "GameplayCollision")
	void SetNewCollision(ECollisionResponse NewResponse);

	/*Used in Dash Ability and Melee too, to add a little impulse to the actor in some direction.*/
	UFUNCTION(BlueprintCallable, Category = "GameplayCollision")
	void PushCharacter(FVector ImpulseDirection, float PushForce, float StunDuration, UAnimMontage* PushAnimation);

	/*Used to control the character rotation when we are using the lazer Ability so we can rotate with the camera*/
	UFUNCTION(BlueprintCallable, Category = "Ability|Lazer")
	void MakeCameraControlRotation(const bool bShouldControlRotation);
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

protected:
	
	uint8 TeamID;

	int32 ComboCount;
	
	UPROPERTY(BlueprintReadOnly)
	int32 MeleeComboDamageLevel;

	/*Called from PlayerController to check if we have to activate GroundBlast ability or the default Melee Attack*/
	UPROPERTY(BlueprintReadWrite)
	bool bIsTargeting;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* DeathMontage;

	UPROPERTY(BlueprintReadWrite, Category = "Animations")
	UAnimMontage* MeleeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	TArray<UAnimMontage*> MeleeCombos;

	/*Here we set all the abilities for this character from the UE4 Editor.*/
	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem")
	TArray<TSubclassOf<UGameplayAbility>> AllAbilities;

	/*We save here what actors we have already overlapped, so we don't make multiple damage to the same enemy in the dash movement*/
	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> OverlapExclude;

	/*Called from AttributeSetBase when Health Changes. If the value is zero, it will handle all the death events stuffs,
	if it isn't it will call the BP_OnHealthChanged*/
	UFUNCTION()
	void OnHealthChanged(float Health, float MaxHealth);

	/*Calls the BP_OnManaChanged*/
	UFUNCTION()
	void OnManaChanged(float Mana, float MaxMana);

	/*BP_OnStrengthChanged*/
	UFUNCTION()
	void OnStrengthChanged(float Strength, float MaxStrength);

	/*Updates the Health UI Value*/
	UFUNCTION(BlueprintImplementableEvent, Category = "AbilitySystem", meta = (DisplayName = "OnHealthChanged"))
	void BP_OnHealthChanged(float Health, float MaxHealth);

	/*Updates the Mana UI Value*/
	UFUNCTION(BlueprintImplementableEvent, Category = "AbilitySystem", meta = (DisplayName = "OnManaChanged"))
	void BP_OnManaChanged(float Mana, float MaxMana);

	/*Updates the Strength UI Value*/
	UFUNCTION(BlueprintImplementableEvent, Category = "AbilitySystem", meta = (DisplayName = "OnStrengthChanged"))
	void BP_OnStrengthChanged(float Strength, float MaxStrength);
	
	/*Determinates the Melee Combo*/
	UFUNCTION(BlueprintCallable, Category = "Melee Combo")
	void DeterminateCombo();

	/*Determinates the Melee Combo*/
	UFUNCTION(BlueprintCallable, Category = "Melee Combo")
	void ClearCombo();

	/*Checks if is other hostile based on TeamID.*/
	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	bool IsOtherHostile(ACharacterBase* Other);
	
	/*Checks if the enemy we are overlapping is already included in the OverlapExclude TArray*/
	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	bool EnemyIsAlreadyExcluded(AActor* CharacterToCheck);

private:
	
	FTimerHandle StunTimeHandle;

	float OriginalGroundFriction;

	bool bIsDead;

	/*Destroys character.*/
	UFUNCTION()
	void PlayerDeath();
	
	/*Called from PushCharacter after X seconds using a Delay.*/
	UFUNCTION()
	void RestoreGroundFriction();
	
	/*Sets the TeamID to zero if is controlled by a PlayerController. It means, it is not an AI*/
	void AutoSelectIDByControllerType();
	/*It will loop through all the abilities that we set in the AllAbilities TArray*/
	void AquireAllAbilities();
	/*It will disable inputs and AI Brain Logic on Death, or it will disable the same on stun effect.*/
	void DisableInputs();
	/*It will enable inputs and AI Brain Logic after the stun effect.*/
	void EnableInputs();

	/*Calls the DisableInputs function and after a timer, it will call the EnableInputs function*/
	void HitStun(float StunDuration);

	/*It takes an ability from AquireAllAbilities, and add it to the AbilitySystemComponent.*/
	void AquireAbility(TSubclassOf<UGameplayAbility> AbilityToAquire);
	/*Gives the FGameplayAbilityInfo to the PlayerController so it can add this ability to the UI*/
	void AddAbilityToUI(TSubclassOf<UGameplayAbilityBase> AbilityToAdd);

	virtual void BeginPlay() override;
};
