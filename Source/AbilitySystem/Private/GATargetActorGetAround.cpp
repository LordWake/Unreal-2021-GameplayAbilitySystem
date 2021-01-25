//+---------------------------------------------------------+
//| Project   : Gameplay Ability System Demo	UE 4.25		|
//| Author    : github.com/LordWake					 		|		
//+---------------------------------------------------------+

#include "GATargetActorGetAround.h"

#include "Abilities/GameplayAbility.h"

#include "GameFrameWork/PlayerController.h"
#include "GameFrameWork/Pawn.h"

void AGATargetActorGetAround::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;
	MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
}

void AGATargetActorGetAround::ConfirmTargetingAndContinue()
{ 
	APawn* OwningPawn = MasterPC->GetPawn();

	if (!OwningPawn)
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
		return;
	}

	FVector ViewLocation = OwningPawn->GetActorLocation();

	TArray<FOverlapResult> Overlaps;
	TArray<TWeakObjectPtr<AActor>> OverlapedActors;

	bool bTraceComplexData = false;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bTraceComplex = bTraceComplexData;
	CollisionQueryParams.bReturnPhysicalMaterial = false;

	APawn* MasterPawn = MasterPC->GetPawn();
	if (MasterPawn)
		CollisionQueryParams.AddIgnoredActor(MasterPawn->GetUniqueID());

	//Makes an overlap Sphere to check what pawns we are overlapping
	bool bTryOverlap = GetWorld()->OverlapMultiByObjectType(Overlaps, ViewLocation, FQuat::Identity,
		FCollisionObjectQueryParams(ECC_Pawn), FCollisionShape::MakeSphere(Radius), CollisionQueryParams);

	if (bTryOverlap)
	{
		for (int32 i = 0; i < Overlaps.Num(); ++i)
		{
			APawn* OverlapedPawn = Cast<APawn>(Overlaps[i].GetActor());

			if (OverlapedPawn && !OverlapedActors.Contains(OverlapedPawn))
				OverlapedActors.Add(OverlapedPawn);
		}
	}

	//if we have overlaped actors...
	if (OverlapedActors.Num() > 0)
	{
		FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromActors(OverlapedActors); //Array of TargetData information
		TargetDataReadyDelegate.Broadcast(TargetData); //Calls a parent class delegate passing by reference all the overlapedActors. Called from data blueprint's Pin in GroundAbility.
	}
}
