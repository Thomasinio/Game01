// // © 2024 Igor Vladymyrov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Game01BaseWeapon.h"
#include "Game01PistolWeapon.generated.h"

/**
 * 
 */
UCLASS()
class GAME01_API AGame01PistolWeapon : public AGame01BaseWeapon
{
	GENERATED_BODY()

public:
	virtual void StartFire() override;
	virtual void StopFire() override;

protected:
	virtual void MakeShot() override;
};
