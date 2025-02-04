// // © 2024 Igor Vladymyrov. All Rights Reserved.


#include "Weapons/Game01RifleWeapon.h"
#include "Engine/World.h"
#include "TimerManager.h"



DEFINE_LOG_CATEGORY_STATIC(RifleWeaponLog, All, All);


void AGame01RifleWeapon::StartFire()
{
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ThisClass::MakeShot, TimeBetweenShots, true);
}

void AGame01RifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void AGame01RifleWeapon::MakeShot()
{
	check(GetWorld());

	UE_LOG(RifleWeaponLog, Warning, TEXT("Rifle FIRE!"));
	const FTransform MuzzleSocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);

	// Shoot from the muzzle
	const FVector MTraceStart = MuzzleSocketTransform.GetLocation();
	const FVector MShootDirection = MuzzleSocketTransform.GetRotation().GetForwardVector();
	const FVector MTraceEnd = MTraceStart + MShootDirection * TraceMaxDistance;
	DrawDebugLine(GetWorld(), MTraceStart, MTraceEnd, FColor::Red, false, 5.0f, 0, 5.0f);

	FHitResult MHitResult;
	GetWorld()->LineTraceSingleByChannel(MHitResult, MTraceStart, MTraceEnd, ECollisionChannel::ECC_Visibility);

	if (MHitResult.bBlockingHit)
	{
		DrawDebugSphere(GetWorld(), MHitResult.ImpactPoint, 10.0f, 8, FColor::Red, false, 5.0f);
	}

	// Shoot from the camera
	FVector TraceStart, TraceEnd;
	check(GetTraceData(TraceStart, TraceEnd));

	DrawDebugLine(GetWorld(), MuzzleSocketTransform.GetLocation(), TraceEnd, FColor::Green, false, 5.0f, 0, 5.0f);

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	if (HitResult.bBlockingHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 8, FColor::Green, false, 5.0f);
		UE_LOG(RifleWeaponLog, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString());
		MakeDamage(HitResult);
	}
}

bool AGame01RifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd)
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	const auto ConeHalfAngleRad = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), ConeHalfAngleRad);
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}
