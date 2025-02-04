// // © 2024 Igor Vladymyrov. All Rights Reserved.


#include "Weapons/Game01PistolWeapon.h"


DEFINE_LOG_CATEGORY_STATIC(PistolWeaponLog, All, All);


void AGame01PistolWeapon::StartFire()
{
	MakeShot();
}

void AGame01PistolWeapon::StopFire()
{
	
}

void AGame01PistolWeapon::MakeShot()
{
	check(GetWorld());

	UE_LOG(PistolWeaponLog, Warning, TEXT("Pistol FIRE!"));
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
		UE_LOG(PistolWeaponLog, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString());
		MakeDamage(HitResult);
	}
}
