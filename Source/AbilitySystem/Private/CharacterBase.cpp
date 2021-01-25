//+---------------------------------------------------------+
//| Project   : Gameplay Ability System Demo	UE 4.25		|
//| Author    : github.com/LordWake					 		|		
//+---------------------------------------------------------+

#include "CharacterBase.h"

#include "AIController.h"
#include "BrainComponent.h"

#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/KismetSystemLibrary.h"

#include "GameplayAbilityBase.h"
#include "PlayerControllerBase.h"
#include "AttributeSetBase.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CameraSpring = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring"));
	CameraSpring->SetupAttachment(RootComponent);
	CameraSpring->TargetArmLength = 300.0f;
	CameraSpring->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->SetupAttachment(CameraSpring, USpringArmComponent::SocketName);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->JumpZVelocity		= 820.0f;
	GetCharacterMovement()->RotationRate.Pitch	= 0.0f;
	GetCharacterMovement()->RotationRate.Roll	= 0.0f;
	GetCharacterMovement()->RotationRate.Yaw	= 720.0f;

	AbilitySystemComponent		= CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	BaseAttributeSetComponent	= CreateDefaultSubobject<UAttributeSetBase>("AttributeSetComponent");

	bIsDead = false;

	//TeamID is to be able to differentiate between the enemies and the main character because they both inherit from this class.
	TeamID = 255;

	ComboCount = 1;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	AquireAllAbilities();

	BaseAttributeSetComponent->OnHealthChange.AddDynamic(this,		&ACharacterBase::OnHealthChanged);
	BaseAttributeSetComponent->OnManaChange.AddDynamic(this,		&ACharacterBase::OnManaChanged);
	BaseAttributeSetComponent->OnStrengthChange.AddDynamic(this,	&ACharacterBase::OnStrengthChanged);

	AutoSelectIDByControllerType();
	AddGameplayTag(FullHealthTag);
}

void ACharacterBase::MeleeAttack()
{
	if (bIsTargeting)
		AbilitySystemComponent->TargetConfirm(); //Used to confirm target data on GroundBlast Ability

	else
		AbilitySystemComponent->TryActivateAbilityByClass(AllAbilities[(int32)EAbility::Melee], true);
}

void ACharacterBase::DeterminateCombo()
{
	MeleeMontage = MeleeCombos[ComboCount - 1];
	switch (ComboCount)
	{
		case 1:
		ComboCount++;
		MeleeComboDamageLevel = 1;
		break;

		case 2:
		ComboCount++;
		MeleeComboDamageLevel = 2;
		break;

		case 3:
		ComboCount = 1;
		MeleeComboDamageLevel = 3;
		break;
	}
}

void ACharacterBase::ClearCombo()
{
	ComboCount = 1;
	MeleeComboDamageLevel = 1;
}

void ACharacterBase::CancelAbility()
{
	AbilitySystemComponent->TargetCancel();
}

void ACharacterBase::AquireAbility(TSubclassOf<UGameplayAbility> AbilityToAquire)
{
	if (AbilitySystemComponent)
	{
		if (HasAuthority() && AbilityToAquire)
		{
			FGameplayAbilitySpecDef SpecDef = FGameplayAbilitySpecDef();
			SpecDef.Ability = AbilityToAquire;
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(SpecDef, 1);

			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}

		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void ACharacterBase::AquireAllAbilities()
{
	for (TSubclassOf<UGameplayAbility> AbilityItem : AllAbilities)
	{
		AquireAbility(AbilityItem);
		//If it is a child from GameplayAbilityBase, it means this ability needs to be added to the UI.
		if (AbilityItem->IsChildOf(UGameplayAbilityBase::StaticClass()))
		{
			TSubclassOf<UGameplayAbilityBase> AbilityBaseClass = *AbilityItem;
			if (AbilityBaseClass)
			{
				//We add this ability to the UI.
				AddAbilityToUI(AbilityBaseClass);
			}
		}
	}
}

void ACharacterBase::AddAbilityToUI(TSubclassOf<UGameplayAbilityBase> AbilityToAdd)
{
	APlayerControllerBase* PCBase = Cast<APlayerControllerBase>(GetController());
	if (PCBase)
	{
		UGameplayAbilityBase* AbilityInstance = AbilityToAdd.Get()->GetDefaultObject<UGameplayAbilityBase>();
		if (AbilityInstance)
		{
			FGameplayAbilityInfo AbilityInfo = AbilityInstance->GetAbilityInfo();
			PCBase->AddAbilityToUI(AbilityInfo);
		}
	}
}

void ACharacterBase::OnHealthChanged(float Health, float MaxHealth)
{
	if(bIsDead)
		return;

	if (Health <= 0.0f)
	{
		bIsDead = true;
		float AnimMontageReturnValue = PlayAnimMontage(DeathMontage, 1.0f, NAME_None);
		
		DisableInputs();

		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = FName("PlayerDeath");
		LatentInfo.Linkage = 0;
		LatentInfo.UUID = 1;

		UKismetSystemLibrary::Delay(GetWorld(), AnimMontageReturnValue, LatentInfo);
	}

	BP_OnHealthChanged(Health, MaxHealth);
}

void ACharacterBase::OnManaChanged(float Mana, float MaxMana)
{
	BP_OnManaChanged(Mana, MaxMana);
}

void ACharacterBase::OnStrengthChanged(float Strength, float MaxStrength)
{
	BP_OnStrengthChanged(Strength, MaxStrength);
}

void ACharacterBase::AutoSelectIDByControllerType()
{
	if (GetController() && GetController()->IsPlayerController())
		TeamID = 0;	
}

void ACharacterBase::DisableInputs()
{
	APlayerController* PC	= Cast<APlayerController>(GetController());
	AAIController* AIC		= Cast<AAIController>(GetController());

	if (PC)
		PC->DisableInput(PC);

	if (AIC)
		AIC->GetBrainComponent()->StopLogic("Dead");
}

void ACharacterBase::EnableInputs()
{
	if(bIsDead)
		return;

	APlayerController* PC	= Cast<APlayerController>(GetController());
	AAIController* AIC		= Cast<AAIController>(GetController());
	
	if (PC)
		PC->EnableInput(PC);

	if (AIC)
		AIC->GetBrainComponent()->RestartLogic();
}

void ACharacterBase::PlayerDeath()
{
	Destroy(true);
}

void ACharacterBase::AddGameplayTag(FGameplayTag& TagToAdd)
{
	GetAbilitySystemComponent()->AddLooseGameplayTag(TagToAdd);
	GetAbilitySystemComponent()->SetTagMapCount(TagToAdd, 1);
}

void ACharacterBase::RemoveGameplayTag(FGameplayTag& TagToRemove)
{
	GetAbilitySystemComponent()->RemoveLooseGameplayTag(TagToRemove);
}

void ACharacterBase::SetNewCollision(ECollisionResponse NewResponse)
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, NewResponse);
}

void ACharacterBase::PushCharacter(FVector ImpulseDirection, float PushForce, float StunDuration, UAnimMontage* PushAnimation)
{
	ClearCombo();
	SetNewCollision(ECollisionResponse::ECR_Ignore);
	PlayAnimMontage(PushAnimation, 1.f, NAME_None);

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	FVector ImpulseVector = ImpulseDirection * PushForce;
	
	OriginalGroundFriction = MoveComp->GroundFriction;
	
	//Set the ground friction to zero so we can push the character
	MoveComp->GroundFriction = 0.0f;
	MoveComp->AddImpulse(ImpulseVector, true);

	if (StunDuration > 0)
		HitStun(StunDuration);

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("RestoreGroundFriction");
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = 2;

	UKismetSystemLibrary::Delay(GetWorld(), 0.3f, LatentInfo);
}

void ACharacterBase::RestoreGroundFriction()
{
	GetCharacterMovement()->GroundFriction = OriginalGroundFriction;
	SetNewCollision(ECollisionResponse::ECR_Block);
}

void ACharacterBase::HitStun(float StunDuration)
{
	DisableInputs();
	GetWorldTimerManager().SetTimer(StunTimeHandle, this, &ACharacterBase::EnableInputs, StunDuration, false);
}

void ACharacterBase::MakeCameraControlRotation(const bool bShouldControlRotation)
{
	if (bShouldControlRotation)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

bool ACharacterBase::IsOtherHostile(ACharacterBase* Other)
{
	return TeamID != Other->TeamID;
}

bool ACharacterBase::EnemyIsAlreadyExcluded(AActor* CharacterToCheck)
{
	return OverlapExclude.Contains(CharacterToCheck);
}