// // © 2024 Igor Vladymyrov. All Rights Reserved.


#include "Components/Game01WeaponComponent.h"

#include "Weapons/Game01BaseWeapon.h"
#include "Gameframework/Character.h"


UGame01WeaponComponent::UGame01WeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGame01WeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponClasses.Num() > 0)
	{
		SpawnWeapons();
     	EquipWeapon(CurrentWeaponIndex);
	}
	else
	{
		bIsUnarmed = true;
	}
}

void UGame01WeaponComponent::SpawnWeapons()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	check(Character && GetWorld());

	for (auto WeaponClass : WeaponClasses)
	{
		AGame01BaseWeapon* Weapon = GetWorld()->SpawnActor<AGame01BaseWeapon>(WeaponClass);
		check(Weapon);

		Weapon->SetOwner(Character);
		Weapons.Add(Weapon);

		AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
	}
}

void UGame01WeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	check(Character);

	if (CurrentWeapon)
	{ 
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
	}
	if (Weapons.IsValidIndex(WeaponIndex))
	{
		CurrentWeapon = Weapons[WeaponIndex];
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
		bIsUnarmed = false;
	}
	else
	{
		CurrentWeapon = nullptr;
		bIsUnarmed = true;
	}
}

void UGame01WeaponComponent::NextWeapon()
{
	if (Weapons.Num() == 0)
	{
		CurrentWeapon = nullptr;
		bIsUnarmed = true;
		return;
	}

	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % (Weapons.Num() + 1); // Add 1 for the unarmed state

	if (CurrentWeaponIndex >= Weapons.Num())
	{
		CurrentWeaponIndex = -1; // Set to unarmed state
		CurrentWeapon = nullptr;
		bIsUnarmed = true;
	}
	else
	{
		EquipWeapon(CurrentWeaponIndex);
	}
	// if (Weapons.Num() > 0)
	// {
	// 	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	// 	EquipWeapon(CurrentWeaponIndex);	
	// }
}

void UGame01WeaponComponent::AttachWeaponToSocket(AGame01BaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
	check(Weapon && SceneComponent);

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void UGame01WeaponComponent::StartFire() 
{
	if (bIsUnarmed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot fire while unarmed"));
		return;
	}
	
	check(CurrentWeapon);
	CurrentWeapon->StartFire();
}

void UGame01WeaponComponent::StopFire() 
{
	if (bIsUnarmed)
	{
		return;
	}
	
	check(CurrentWeapon);
	CurrentWeapon->StopFire();
}
