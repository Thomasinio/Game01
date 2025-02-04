// // © 2024 Igor Vladymyrov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Game01BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UGame01HealthComponent;
class UTextRenderComponent;
class UGame01WeaponComponent;


UCLASS()
class GAME01_API AGame01BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGame01BaseCharacter();

	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovementDirection() const;

	UFUNCTION()
	void SetMovementRestriction(bool bRestricted);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCharacterMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UGame01HealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTextRenderComponent* HealthTextComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UGame01WeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* InputLook;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* InputMove;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* InputJump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* InputSprint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* InputAim;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* InputFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* InputNextWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	bool bIsSprinting = false;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	bool bIsAiming = false;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	bool bCanShoot = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float IdleSpeed = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float WalkSpeed = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float RunSpeed = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SprintSpeed = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float NormalArmLength = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float AimArmLength = 150.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float SprintArmLength = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float TargetArmLength = NormalArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ZoomInterpSpeed = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FVector AimSocketOffset = FVector(0.0f, 75.0f, 50.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float AimFOV = 70.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FVector NormalSocketOffset = FVector(0.0f, 50.0f, 95.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float NormalFOV = 90.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FVector SprintSocketOffset = FVector(0.0f, 50.0f, 95.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float SprintFOV = 110.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FVector TargetSocketOffset = NormalSocketOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float TargetFOV = NormalFOV;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandedDamageVelocity = FVector2D(900, 1200); 

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandedDamage = FVector2D(10, 100);

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathAnimMontage;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void Move(const FInputActionValue& Value);
	void MoveForwardBackward(const float Value);
	void MoveLeftRight(const float Value);
	void Look(const FInputActionValue& Value);
	void StartSprint();
	void StopSprint();
	void StartAim();
	void StopAim();
	
	void OnHealthChanged(const float Health) const;
	void OnDeath();

private:
	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);

	void UpdateCameraSettings(float DeltaTime);
	void AimStateMovement(const FInputActionValue& Value);

	bool bMovementRestricted = false;
};
