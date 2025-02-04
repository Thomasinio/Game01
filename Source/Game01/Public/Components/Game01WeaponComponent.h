// // © 2024 Igor Vladymyrov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game01WeaponComponent.generated.h"


class AGame01BaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAME01_API UGame01WeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGame01WeaponComponent();
	
	void StartFire();
	void StopFire();
	void NextWeapon();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bIsUnarmed = true;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<TSubclassOf<AGame01BaseWeapon>> WeaponClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponEquipSocketName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponArmorySocketName = "ArmorySocket";

private:
	UPROPERTY()
	AGame01BaseWeapon* CurrentWeapon = nullptr;

	UPROPERTY()
	TArray<AGame01BaseWeapon*> Weapons;

	int32 CurrentWeaponIndex = 0;
	
	void SpawnWeapons();
	static void AttachWeaponToSocket(AGame01BaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
	void EquipWeapon(int32 WeaponIndex);
};
