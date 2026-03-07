// Fill out your copyright notice in the Description page of Project Settings.

#include "RaceCarBase.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ARaceCarBase::ARaceCarBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set up Car Body
	CarBody = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	CarBody->SetSimulatePhysics(true);
	CarBody->SetLinearDamping(SuspensionDamping);
	SetRootComponent(CarBody);
	CarBody->bEditableWhenInherited = true;

	// Set up Car Mesh
	CarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Car Mesh"));
	CarMesh->SetupAttachment(CarBody);
	CarMesh->bEditableWhenInherited = true;

	// Set up wheels
	FLWheel = CreateDefaultSubobject<USceneComponent>(TEXT("Forward Left Wheel"));
	FLWheel->SetupAttachment(CarBody);
	FRWheel = CreateDefaultSubobject<USceneComponent>(TEXT("Forward Right Wheel"));
	FRWheel->SetupAttachment(CarBody);
	BLWheel = CreateDefaultSubobject<USceneComponent>(TEXT("Back Left Wheel"));
	BLWheel->SetupAttachment(CarBody);
	BRWheel = CreateDefaultSubobject<USceneComponent>(TEXT("Back Right Wheel"));
	BRWheel->SetupAttachment(CarBody);
	FLWheel->bEditableWhenInherited = true;
	BLWheel->bEditableWhenInherited = true;
	FRWheel->bEditableWhenInherited = true;
	BRWheel->bEditableWhenInherited = true;

	FLWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Forward Left Wheel Mesh"));
	FLWheelMesh->SetupAttachment(FLWheel);
	FRWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Forward Right Wheel Mesh"));
	FRWheelMesh->SetupAttachment(FRWheel);
	BLWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Back Left Wheel Mesh"));
	BLWheelMesh->SetupAttachment(BLWheel);
	BRWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Back Right Wheel Mesh"));
	BRWheelMesh->SetupAttachment(BRWheel);
	FLWheelMesh->bEditableWhenInherited = true;
	BLWheelMesh->bEditableWhenInherited = true;
	FRWheelMesh->bEditableWhenInherited = true;
	BRWheelMesh->bEditableWhenInherited = true;
}

#pragma region Controllers
void ARaceCarBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) // If the car is a player we assign the mapping context
	{
		if (PlayerController->IsLocalController())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
	else if (AAIController* AIController = Cast<AAIController>(Controller)) // If the car is AI we run the Behaviour Tree
	{
		if (AIController->IsLocalController())
		{
			UE_LOG(LogTemp, Warning, TEXT("AIController is local controller"));
		}
	}
}

void ARaceCarBase::UnPossessed() // clear the controllers once we finish
{
	Super::UnPossessed();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (PlayerController->IsLocalController())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(InputMappingContext);
			}
		}
	}
	else if (AAIController* AIController = Cast<AAIController>(Controller)) // If the car is AI we run the Behaviour Tree
	{
		if (AIController->IsLocalController())
		{
			UE_LOG(LogTemp, Warning, TEXT("AIController is local controller"));
		}
	}
}
#pragma endregion

void ARaceCarBase::Tick(float DeltaTime)
{
	ApplyWheelsSuspension(FRWheel, FRWheelMesh);
	ApplyWheelsSuspension(BLWheel, BLWheelMesh);
	ApplyWheelsSuspension(FLWheel, FLWheelMesh);
	ApplyWheelsSuspension(BRWheel, BRWheelMesh);
	
	CalculateSpeed();
	ApplySpeed(FRWheel);
	ApplySpeed(BLWheel);
	ApplySpeed(FLWheel);
	ApplySpeed(BRWheel);
	FixRotations();
	CarBody->SetCenterOfMass(FVector(CenterOfMassOffsetWhenAccelerating, 0.f, 0.f) * AccelerationInput);  // Apply car tilting.
	// NormalizeWheelsRotation();
}

void ARaceCarBase::RotateWheels(float Input)
{
	float TargetYawFL = FMath::FInterpTo(FLWheel->GetRelativeRotation().Yaw, 30.f * -AccelerationInput, GetWorld()->GetDeltaSeconds(), WheelsRotationSpeed);
	float TargetYawFR = FMath::FInterpTo(FRWheel->GetRelativeRotation().Yaw, 30.f * -AccelerationInput, GetWorld()->GetDeltaSeconds(), WheelsRotationSpeed);

	UE_LOG(LogTemp, Warning, TEXT("Yaw: %f"), TargetYawFL);
	
	FLWheel->SetRelativeRotation(FRotator(FLWheel->GetRelativeRotation().Pitch, TargetYawFL, FLWheel->GetRelativeRotation().Roll));
	FRWheel->SetRelativeRotation(FRotator(FRWheel->GetRelativeRotation().Pitch, TargetYawFR, FRWheel->GetRelativeRotation().Roll));
}

void ARaceCarBase::NormalizeWheelsRotation()
{
	const float TargetYawFL = FMath::FInterpTo(FLWheel->GetRelativeRotation().Yaw, 0.f, GetWorld()->GetDeltaSeconds(), WheelsRotationNormalizeSpeed);
	const float TargetYawFR = FMath::FInterpTo(FRWheel->GetRelativeRotation().Yaw, 0.f, GetWorld()->GetDeltaSeconds(), WheelsRotationNormalizeSpeed);
	FLWheel->SetRelativeRotation(FRotator(FLWheel->GetRelativeRotation().Pitch, TargetYawFL, FLWheel->GetRelativeRotation().Roll));
	FRWheel->SetRelativeRotation(FRotator(FRWheel->GetRelativeRotation().Pitch, TargetYawFR, FRWheel->GetRelativeRotation().Roll));
}

#pragma region Wheels
void ARaceCarBase::ApplyWheelsSuspension(const USceneComponent* TargetWheel, UStaticMeshComponent* WheelMesh)
{
	// Cast The trace to detect the floor
	const FVector TraceStartPos = TargetWheel->GetComponentLocation();
	const FVector TraceEndPos = TargetWheel->GetComponentLocation() + (CarBody->GetUpVector() * -60.f);

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStartPos, TraceEndPos, ECC_Visibility);
	DrawDebugLine(GetWorld(), TraceStartPos, TraceEndPos, FColor::Red);
	if (!Hit.bBlockingHit)
	{
		bIsGrounded = false;
		return;
	}
	else
	{
		bIsGrounded = true;
	}
	// Calculate and apply the force
	float OutputFloat = 1.f - FMath::GetRangePct(0.f, 60.f, Hit.Distance);
	const FVector ForceDirection = FVector(Hit.TraceStart - Hit.TraceEnd).GetSafeNormal() * OutputFloat;
	CarBody->AddForceAtLocation(ForceDirection * SuspensionForce, TargetWheel->GetComponentLocation());

	// Calculate the wheel mesh position and apply it
	const float TargetMeshZLocation = FMath::FInterpTo(WheelMesh->GetComponentLocation().Z,
												Hit.Distance * -1 + WheelMesh->GetStaticMesh()->GetBounds().BoxExtent.Z * 2,
												GetWorld()->GetDeltaSeconds(),
												RotationSpeed);
	WheelMesh->SetWorldLocation(FVector(TargetWheel->GetComponentLocation().X, TargetWheel->GetComponentLocation().Y, TargetMeshZLocation));
}

#pragma endregion

#pragma region Movement
void ARaceCarBase::Accelerate(float Input)
{
	if (bIsGrounded)
	{
		// Calculate the acceleration input
		AccelerationInput = FMath::FInterpTo(AccelerationInput, Input, GetWorld()->GetDeltaSeconds(), AccelerationSpeed);
	}
}
void ARaceCarBase::Steer(float Input)
{
	const FVector TargetRotation = FVector(0.f, 0.f, Input * SteeringForce * AccelerationInput); // calculate the rotation
	CarBody->AddTorqueInRadians(TargetRotation); // apply rotation
}
void ARaceCarBase::CalculateSpeed()
{
	Speed = FMath::Lerp(0.f, MaxSpeed, AccelerationInput); // calculate the current speed
	AccelerationInput = FMath::FInterpTo(AccelerationInput, 0.f, GetWorld()->GetDeltaSeconds(), DeAccelerationSpeed); // slow down the acceleration
}

void ARaceCarBase::ApplySpeed(USceneComponent* Wheel)
{
	const FVector AccelerationForceVector = CarBody->GetForwardVector() * Speed; 	// Calculate acceleration the force
	CarBody->AddForceAtLocation(AccelerationForceVector, Wheel->GetComponentLocation());  //  apply the force

	// Apply wheel spin
	const float TargetRotationPitch = FMath::FInterpTo(Wheel->GetRelativeRotation().Pitch,
											Wheel->GetRelativeRotation().Pitch + WheelsSpinForce * -AccelerationInput,
												GetWorld()->GetDeltaSeconds(),
												WheelsSpinSpeed);
	Wheel->SetRelativeRotation(FRotator(TargetRotationPitch, Wheel->GetRelativeRotation().Yaw, Wheel->GetRelativeRotation().Roll));
}

void ARaceCarBase::FixRotations()
{
	// Fix Car Body Rotation
	const float TargetCarBodyRotationRoll = FMath::Clamp(CarBody->GetComponentRotation().Roll, -90.f, 90.f);
	CarBody->SetRelativeRotation(FRotator(CarBody->GetComponentRotation().Pitch, CarBody->GetComponentRotation().Yaw, TargetCarBodyRotationRoll));

	//Fix Wheels Rotation
	const float TargetFLWheelRotationYaw = FMath::Clamp(FLWheel->GetRelativeRotation().Yaw, -30.f, 30.f);
	const float TargetFRWheelRotationYaw = FMath::Clamp(FRWheel->GetRelativeRotation().Yaw, -30.f, 30.f);
	FLWheel->SetRelativeRotation(FRotator(FLWheel->GetRelativeRotation().Pitch, TargetFLWheelRotationYaw, FLWheel->GetRelativeRotation().Roll));
	FRWheel->SetRelativeRotation(FRotator(FRWheel->GetRelativeRotation().Pitch, TargetFRWheelRotationYaw, FRWheel->GetRelativeRotation().Roll));
}
#pragma endregion







