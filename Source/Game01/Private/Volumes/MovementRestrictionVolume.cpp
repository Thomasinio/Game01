// // © 2024 Igor Vladymyrov. All Rights Reserved.


#include "Volumes/MovementRestrictionVolume.h"
#include "Characters/Game01BaseCharacter.h"

AMovementRestrictionVolume::AMovementRestrictionVolume()
{
	PrimaryActorTick.bCanEverTick = true;
	OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);
}

void AMovementRestrictionVolume::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	AGame01BaseCharacter* Character = Cast<AGame01BaseCharacter>(OtherActor);
	if (Character)
	{
		Character->SetMovementRestriction(true);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("AMovementRestrictionVolume::OnBeginOverlap"));
}

void AMovementRestrictionVolume::OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	AGame01BaseCharacter* Character = Cast<AGame01BaseCharacter>(OtherActor);
	if (Character)
	{
		Character->SetMovementRestriction(false); // Re-enable normal movement when leaving the volume
	}
	UE_LOG(LogTemp, Warning, TEXT("AMovementRestrictionVolume::OnEndOverlap"));

}



void AMovementRestrictionVolume::BeginPlay()
{
	Super::BeginPlay();

	if (bDrawDebug)
	{
		DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FQuat::Identity, DebugColor, true, DebugLifetime, 0, 5);
	}
}

void AMovementRestrictionVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDrawDebug)
	{
		DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FQuat::Identity, DebugColor, false, -1.0f, 0, 5);
	}
}
