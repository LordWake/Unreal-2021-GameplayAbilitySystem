//+---------------------------------------------------------+
//| Project   : Gameplay Ability System Demo	UE 4.25		|
//| Author    : github.com/LordWake					 		|		
//+---------------------------------------------------------+

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AbilityTypes.h"
#include "PlayerControllerBase.generated.h"

class ACharacter;
class ACharacterBase;

UCLASS()
class ABILITYSYSTEM_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UUserWidget> wPrimaryUI;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* UIReference;

	/*Called from the CharacterBase.cpp. When the Character gets all the abilities in the BeginPlay, it will take the FGameplayAbilityInfo from each one 
	(from the GetAbilityInfo function inherited from GameplayAbilitySystemBase), and then it will pass that value to this function executed in blueprints. 
	This function basically calls WBP_PrimaryUI's AddAbilityToSlots function and passes that information. Kind of works like a bridge */
	UFUNCTION(BlueprintImplementableEvent, Category = "PlayerControllerBase")
	void AddAbilityToUI(FGameplayAbilityInfo AbilityInfo);

	/* Invokes a function made in Blueprints to execute the first ability */
	UFUNCTION(BlueprintImplementableEvent, Category = "AbilityUI")
	void SlotOneAbilityUI();
	
	/* Invokes a function made in Blueprints to execute the second ability */
	UFUNCTION(BlueprintImplementableEvent, Category = "AbilityUI")
	void SlotTwoAbilityUI();

	/* Invokes a function made in Blueprints to execute the third ability */
	UFUNCTION(BlueprintImplementableEvent, Category = "AbilityUI")
	void SlotThreeAbilityUI();

	/* Invokes a function made in Blueprints to execute the fourth ability */
	UFUNCTION(BlueprintImplementableEvent, Category = "AbilityUI")
	void SlotFourAbilityUI();

	/* Invokes a function made in Blueprints to execute the fifth ability */
	UFUNCTION(BlueprintImplementableEvent, Category = "AbilityUI")
	void SlotFiveAbilityUI();

private:

	ACharacter* PlayerCharacter;
	ACharacterBase* PlayerBase;

	/*Calls directly the player melee attack in this case.*/
	void PlayerPrimaryAction();
	void PlayerJump();
	void CancelTargetAbility();
	void PlayerMoveFoward(float Val);
	void PlayerMoveRight(float Val);

	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
};
