// // © 2024 Igor Vladymyrov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "MovementRestrictionVolume.generated.h"


UCLASS()
class GAME01_API AMovementRestrictionVolume : public AVolume
{
	GENERATED_BODY()

public:
	AMovementRestrictionVolume();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	void OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

private:
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDrawDebug = true;

	UPROPERTY(EditAnywhere, Category = "Debug")
	FColor DebugColor = FColor::Green;

	UPROPERTY(EditAnywhere, Category = "Debug")
	float DebugLifetime = -1.0f; // -1 means infinite duration
};
