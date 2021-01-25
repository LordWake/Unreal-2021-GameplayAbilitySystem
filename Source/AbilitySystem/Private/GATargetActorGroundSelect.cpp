//+---------------------------------------------------------+
//| Project   : Gameplay Ability System Demo	UE 4.25		|
//| Author    : github.com/LordWake					 		|		
//+---------------------------------------------------------+

#include "GATargetActorGroundSelect.h"
#include "Abilities/GameplayAbility.h"

#include "GameFrameWork/Pawn.h"
#include "GameFrameWork/PlayerController.h"

#include "Components/DecalComponent.h"
#include "Components/SceneComponent.h"

AGATargetActorGroundSelect::AGATargetActorGroundSelect()
{
	PrimaryActorTick.bCanEverTick = true;

	Decal = CreateDefaultSubobject<UDecalComponent>("Decal");
	RootComp = CreateDefaultSubobject<USceneComponent>("RootComp");
	
	SetRootComponent(RootComp);
	Decal->SetupAttachment(RootComp);
	
	Radius = 200.0f;
	Decal->DecalSize = FVector(Radius);
}

void AGATargetActorGroundSelect::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	FVector LookPoint;
	GetPlayerLookingPoint(LookPoint);

	Decal->SetWorldLocation(LookPoint);
}

void AGATargetActorGroundSelect::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;
	MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
	Decal->DecalSize = FVector(Radius);
}

void AGATargetActorGroundSelect::ConfirmTargetingAndContinue()
{
	FVector ViewLocation; 
	GetPlayerLookingPoint(ViewLocation);

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

	FGameplayAbilityTargetData_LocationInfo* CenterLocation = new FGameplayAbilityTargetData_LocationInfo();	
	if (Decal)
	{
		CenterLocation->TargetLocation.LiteralTransform = Decal->GetComponentTransform();
		CenterLocation->TargetLocation.LocationType		= EGameplayAbilityTargetingLocationType::LiteralTransform;
	}

	//if we have overlaped actors...
	if (OverlapedActors.Num() > 0)
	{		
		FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromActors(OverlapedActors); //Array of TargetData information
		TargetData.Add(CenterLocation); //Return the center location where we have to spawn particles.
		TargetDataReadyDelegate.Broadcast(TargetData); //Calls a parent class delegate passing by reference all the overlapedActors. Called from data blueprint's Pin in GroundAbility.
	}
	else
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle(CenterLocation)); //If it doesn't have actors, returns an empty TargetData with CenterLocation to SpawnParticles anyway	
}

bool AGATargetActorGroundSelect::GetPlayerLookingPoint(FVector& OutViewPoint)
{
	FVector ViewPoint;
	FRotator ViewRotation;

	//Returns player view and rotation point.
	MasterPC->GetPlayerViewPoint(ViewPoint, ViewRotation);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;

	QueryParams.bTraceComplex = true;
	APawn* MasterPawn = MasterPC->GetPawn();

	if (MasterPawn)
		QueryParams.AddIgnoredActor(MasterPawn->GetUniqueID()); //Ignore the pawn that is casting the ability

	bool bTryTrace = GetWorld()->LineTraceSingleByChannel(HitResult, ViewPoint, ViewPoint * ViewRotation.Vector() * 10000.0f, ECC_Visibility, QueryParams);

	if (bTryTrace)
		OutViewPoint = HitResult.ImpactPoint; //Returns where the player is looking
	else
		OutViewPoint = FVector();	

	return bTryTrace;
}
