// // © 2024 Igor Vladymyrov. All Rights Reserved.


#include "Components/Game01HealthComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"


DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);


UGame01HealthComponent::UGame01HealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGame01HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHealth(MaxHealth);

	AActor* ComponentOwner = GetOwner();
	check(ComponentOwner);
	ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnTakeAnyDamageHandle);
}

void UGame01HealthComponent::OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || IsDead() || !GetWorld()) return;

	SetHealth(Health - Damage);

	// Clear the heal timer if damage is taken
	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
	else if (bAutoHeal)
	{
		// Start the heal timer if auto-heal is enabled
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &ThisClass::HealUpdate, HealUpdateTime, true, HealDelay);
	}
}

void UGame01HealthComponent::HealUpdate()
{
	SetHealth(Health + HealModifier);

	// Stop healing if health is nearly equal to max health
	if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void UGame01HealthComponent::SetHealth(const float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
}
