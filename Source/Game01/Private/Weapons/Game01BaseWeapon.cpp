// // © 2024 Igor Vladymyrov. All Rights Reserved.


#include "Weapons/Game01BaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Gameframework/Character.h"
#include "Gameframework/Controller.h"
#include "Engine/DamageEvents.h"
#include "Engine/World.h"


DEFINE_LOG_CATEGORY_STATIC(BaseWeaponLog, All, All);


AGame01BaseWeapon::AGame01BaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}



void AGame01BaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

APlayerController* AGame01BaseWeapon::GetPlayerController() const
{
	const auto Player = CastChecked<ACharacter>(GetOwner());
	const auto Controller = Player->GetController<APlayerController>();
	check(Controller);
	return Controller;
}

void AGame01BaseWeapon::StartFire()
{
	// Implement fire start logic
}

void AGame01BaseWeapon::StopFire()
{
	// Implement fire stop logic
}

void AGame01BaseWeapon::MakeShot()
{
	// Implement shot logic
}

void AGame01BaseWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamagedActor = HitResult.GetActor();
	if (!DamagedActor) return;

	DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}

bool AGame01BaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	APlayerController* Controller = GetPlayerController();
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	return true;
}

bool AGame01BaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd)
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	const FVector ShootDirection = ViewRotation.Vector();
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void AGame01BaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const
{
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}