//+---------------------------------------------------------+
//| Project   : Gameplay Ability System Demo	UE 4.25		|
//| Author    : github.com/LordWake					 		|		
//+---------------------------------------------------------+

#include "PlayerControllerBase.h"

#include "GameFramework/Character.h"
#include "CharacterBase.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacter = CastChecked<ACharacter>(GetPawn());
	PlayerBase		= CastChecked<ACharacterBase>(PlayerCharacter);

	if (wPrimaryUI)
	{
		UIReference = CreateWidget<UUserWidget>(this, wPrimaryUI);

		if (UIReference)
			UIReference->AddToViewport();
	}
}

void APlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Jump",			IE_Pressed, this,	&APlayerControllerBase::PlayerJump);
	InputComponent->BindAction("PrimaryAction", IE_Pressed, this,	&APlayerControllerBase::PlayerPrimaryAction);
	
	InputComponent->BindAction("SlotOneAbility",	IE_Pressed, this,	&APlayerControllerBase::SlotOneAbilityUI);
	InputComponent->BindAction("SlotTwoAbility",	IE_Pressed, this,	&APlayerControllerBase::SlotTwoAbilityUI);
	InputComponent->BindAction("SlotThreeAbility",	IE_Pressed, this,	&APlayerControllerBase::SlotThreeAbilityUI);
	InputComponent->BindAction("SlotFourAbility",	IE_Pressed, this,	&APlayerControllerBase::SlotFourAbilityUI);
	InputComponent->BindAction("SlotFiveAbility",	IE_Pressed, this,	&APlayerControllerBase::SlotFiveAbilityUI);
	InputComponent->BindAction("Cancel",			IE_Pressed, this,	&APlayerControllerBase::CancelTargetAbility);

	InputComponent->BindAxis("MoveFoward",	this,	&APlayerControllerBase::PlayerMoveFoward);
	InputComponent->BindAxis("MoveRight",	this,	&APlayerControllerBase::PlayerMoveRight);

	InputComponent->BindAxis("LookingUp",	this,	&APlayerControllerBase::AddPitchInput);
	InputComponent->BindAxis("LookingRight",this,	&APlayerControllerBase::AddYawInput);
}

void APlayerControllerBase::PlayerJump()
{
	PlayerCharacter->Jump();	
}

void APlayerControllerBase::CancelTargetAbility()
{
	PlayerBase->CancelAbility();
}

void APlayerControllerBase::PlayerPrimaryAction()
{
	PlayerBase->MeleeAttack();
}

void APlayerControllerBase::PlayerMoveFoward(float Val)
{
	const FRotator CameraRotation		= UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraRotation();
	const FVector CameraFowardVector	= UKismetMathLibrary::GetForwardVector(CameraRotation);

	GetPawn()->AddMovementInput(CameraFowardVector, Val);
}

void APlayerControllerBase::PlayerMoveRight(float Val)
{
	const FRotator CameraRotation		= UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraRotation();
	const FVector CameraFowardVector	= UKismetMathLibrary::GetRightVector(CameraRotation);

	GetPawn()->AddMovementInput(CameraFowardVector, Val);
}
