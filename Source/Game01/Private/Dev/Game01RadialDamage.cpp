// // © 2024 Igor Vladymyrov. All Rights Reserved.


#include "Dev/Game01RadialDamage.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AGame01RadialDamage::AGame01RadialDamage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Scene Component");
	SetRootComponent(SceneComponent);
}

// Called when the game starts or when spawned
void AGame01RadialDamage::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGame01RadialDamage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 32, SphereColor);
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radius, DamageType, {}, this, nullptr, bDoFullDamage);
}

