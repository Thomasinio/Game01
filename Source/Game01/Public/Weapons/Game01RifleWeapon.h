// // © 2024 Igor Vladymyrov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Game01BaseWeapon.h"
#include "Game01RifleWeapon.generated.h"


UCLASS()
class GAME01_API AGame01RifleWeapon : public AGame01BaseWeapon
{
	GENERATED_BODY()

public:
	virtual void StartFire() override;
	virtual void StopFire() override;

protected:
	virtual void MakeShot() override;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	float TimeBetweenShots = 0.1f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	float BulletSpread = 1.5f;

private:
	FTimerHandle ShotTimerHandle;
};
