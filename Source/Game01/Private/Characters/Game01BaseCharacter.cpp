// // © 2024 Igor Vladymyrov. All Rights Reserved.


#include "Characters/Game01BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Game01HealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/DamageEvents.h"
#include "Components/Game01WeaponComponent.h"
#include "Components/CapsuleComponent.h"


DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);


AGame01BaseCharacter::AGame01BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = NormalArmLength; // Adjust distance
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 20.0f;
	SpringArmComponent->SocketOffset = NormalSocketOffset; 
	SpringArmComponent->bInheritPitch = true;
	SpringArmComponent->bInheritYaw = true;
	SpringArmComponent->bInheritRoll = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->FieldOfView = NormalFOV;
	
	MovementComponent = CastChecked<UCharacterMovementComponent>(GetCharacterMovement());
	MovementComponent->MaxWalkSpeed = RunSpeed;

	HealthComponent = CreateDefaultSubobject<UGame01HealthComponent>("HealthComponent");

	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());

	WeaponComponent = CreateDefaultSubobject<UGame01WeaponComponent>("WeaponComponent");
}

void AGame01BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	check(SpringArmComponent);
	check(CameraComponent);
	check(HealthComponent);
	check(HealthTextComponent);
	check(WeaponComponent);

	OnHealthChanged(HealthComponent->GetHealth()); // Needed to have max health on the beginning instead of default placeholder
	HealthComponent->OnHealthChanged.AddUObject(this, &ThisClass::OnHealthChanged);
	HealthComponent->OnDeath.AddUObject(this, &ThisClass::OnDeath);

	LandedDelegate.AddDynamic(this, &ThisClass::OnGroundLanded);
}

void AGame01BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateCameraSettings(DeltaTime);
}

void AGame01BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	const APlayerController* MyPlayerController = CastChecked<APlayerController>(GetController());

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(MyPlayerController->GetLocalPlayer());
	check(Subsystem);
	
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);
	
	UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
 
	Input->BindAction(InputMove, ETriggerEvent::Triggered, this, &ThisClass::Move);
	Input->BindAction(InputLook, ETriggerEvent::Triggered, this, &ThisClass::Look);
	Input->BindAction(InputJump, ETriggerEvent::Started, this, &ThisClass::Jump);
	Input->BindAction(InputSprint, ETriggerEvent::Started, this, &ThisClass::StartSprint);
	Input->BindAction(InputSprint, ETriggerEvent::Completed, this, &ThisClass::StopSprint);
	Input->BindAction(InputAim, ETriggerEvent::Started, this, &ThisClass::StartAim);
	Input->BindAction(InputAim, ETriggerEvent::Completed, this, &ThisClass::StopAim);
	Input->BindAction(InputFire, ETriggerEvent::Started, WeaponComponent, &UGame01WeaponComponent::StartFire);
	Input->BindAction(InputFire, ETriggerEvent::Completed, WeaponComponent, &UGame01WeaponComponent::StopFire);
	Input->BindAction(InputNextWeapon, ETriggerEvent::Started, WeaponComponent, &UGame01WeaponComponent::NextWeapon);
}

float AGame01BaseCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero()) return 0.0f;
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void AGame01BaseCharacter::SetMovementRestriction(bool bRestricted)
{
	if (bRestricted)
	{
		bMovementRestricted = true;
		MovementComponent->MaxWalkSpeed = WalkSpeed;
		StopSprint(); // Ensure sprinting is stopped
	}
	else
	{
		bMovementRestricted = false;
		MovementComponent->MaxWalkSpeed = RunSpeed;
	}
}

void AGame01BaseCharacter::Move(const FInputActionValue& Value)
{
	if (bIsAiming)
	{
		AimStateMovement(Value);
	}
	else
	{
		const FVector2D MovementVector = Value.Get<FVector2D>();
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0, ControlRotation.Yaw, 0);

		// Get forward and right vector based on camera rotation
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Move the character
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		// Set rotation to movement direction
		MovementComponent->bOrientRotationToMovement = !MovementVector.IsZero();
	}
}

void AGame01BaseCharacter::MoveForwardBackward(const float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void AGame01BaseCharacter::MoveLeftRight(const float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void AGame01BaseCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(-LookAxisVector.Y);
}


void AGame01BaseCharacter::StartSprint()
{
	if (!bMovementRestricted)
	{
		bIsSprinting = true;
		UE_LOG(BaseCharacterLog, Display, TEXT("Speed: %f"), MovementComponent->MaxWalkSpeed);
		MovementComponent->MaxWalkSpeed = SprintSpeed;
		TargetArmLength = SprintArmLength;
		TargetSocketOffset = SprintSocketOffset;
		TargetFOV = SprintFOV;
	}
}

void AGame01BaseCharacter::StopSprint()
{
	if (!bMovementRestricted)
	{
		bIsSprinting = false;
		UE_LOG(BaseCharacterLog, Display, TEXT("Speed: %f"), MovementComponent->MaxWalkSpeed);
		MovementComponent->MaxWalkSpeed = RunSpeed;
		TargetArmLength = NormalArmLength;
		TargetSocketOffset = NormalSocketOffset;
		TargetFOV = NormalFOV;
	}
}

void AGame01BaseCharacter::StartAim()
{
	bIsAiming = true;
	bCanShoot = true;
	TargetArmLength = AimArmLength;
	TargetSocketOffset = AimSocketOffset;
	TargetFOV = AimFOV;
}

void AGame01BaseCharacter::StopAim()
{
	bIsAiming = false;
	bCanShoot = false;
	TargetArmLength = NormalArmLength;
	TargetSocketOffset = NormalSocketOffset;
	TargetFOV = NormalFOV;
}


void AGame01BaseCharacter::UpdateCameraSettings(float DeltaTime)
{
	if (!FMath::IsNearlyEqual(SpringArmComponent->TargetArmLength, TargetArmLength, 0.1f))
	{
		SpringArmComponent->TargetArmLength = FMath::FInterpTo(SpringArmComponent->TargetArmLength, TargetArmLength, DeltaTime, ZoomInterpSpeed);
	}

	if (!FMath::IsNearlyEqual(CameraComponent->FieldOfView, TargetFOV, 0.1f))
	{
		CameraComponent->FieldOfView = FMath::FInterpTo(CameraComponent->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);
	}

	if (!SpringArmComponent->SocketOffset.Equals(TargetSocketOffset, 0.1f))
	{
		SpringArmComponent->SocketOffset = FMath::VInterpTo(SpringArmComponent->SocketOffset, TargetSocketOffset, DeltaTime, ZoomInterpSpeed);
	}
}

void AGame01BaseCharacter::AimStateMovement(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	// Move the character
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);

	// Disable character rotation to movement direction
	MovementComponent->bOrientRotationToMovement = false;
}

void AGame01BaseCharacter::OnHealthChanged(const float Health) const
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void AGame01BaseCharacter::OnDeath()
{
	UE_LOG(BaseCharacterLog, Error, TEXT("Player %s is DEAD!"), *GetName());
	PlayAnimMontage(DeathAnimMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.0f);
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void AGame01BaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
	float const FallVelocity = -GetVelocity().Z;
	UE_LOG(BaseCharacterLog, Display, TEXT("Fall velocity is %f"), FallVelocity);

	if (FallVelocity < LandedDamageVelocity.X) return;
	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocity);
	UE_LOG(BaseCharacterLog, Display, TEXT("Final damage is %f"), FinalDamage);
	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}


