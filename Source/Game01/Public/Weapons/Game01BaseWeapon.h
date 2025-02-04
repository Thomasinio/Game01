// // © 2024 Igor Vladymyrov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game01BaseWeapon.generated.h"


class USkeletalMeshComponent;

UCLASS()
class GAME01_API AGame01BaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AGame01BaseWeapon();

	virtual void StartFire();
	virtual void StopFire();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	float DamageAmount = 10.0f;

protected:
	virtual void BeginPlay() override;
	
	APlayerController* GetPlayerController() const;
	
	virtual void MakeShot();
	void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;
	void MakeDamage(const FHitResult& HitResult);
	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName MuzzleSocketName = "MuzzleSocket";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	float TraceMaxDistance = 1500.0f;
};
