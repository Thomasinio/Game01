// // © 2024 Igor Vladymyrov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game01HealthComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float)
DECLARE_MULTICAST_DELEGATE(FOnDeath)


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME01_API UGame01HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGame01HealthComponent();
	
	float GetHealth() const { return Health; }

	UFUNCTION()
	bool IsDead() const { return FMath::IsNearlyZero(Health); }

	FOnHealthChanged OnHealthChanged;
	FOnDeath OnDeath;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Heal")
	bool bAutoHeal = true;
	
	UPROPERTY(EditDefaultsOnly, Category = "Heal", meta = (EditCondition = "bAutoHeal"))
	float HealUpdateTime = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Heal", meta = (EditCondition = "bAutoHeal"))
	float HealDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Heal", meta = (EditCondition = "bAutoHeal"))
	float HealModifier = 1.0f;

private:
	float Health = 0.0f;
	
	FTimerHandle HealTimerHandle;
	
	UFUNCTION()
	void OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	void HealUpdate();
	void SetHealth(float NewHealth);
};
