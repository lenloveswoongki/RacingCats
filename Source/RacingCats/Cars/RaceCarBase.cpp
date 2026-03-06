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
	ApplyWheelsSuspension(FRWheel);
	ApplyWheelsSuspension(BLWheel);
	ApplyWheelsSuspension(FLWheel);
	ApplyWheelsSuspension(BRWheel);

	CalculateSpeed();
	ApplySpeed(FRWheel);
	ApplySpeed(BLWheel);
	ApplySpeed(FLWheel);
	ApplySpeed(BRWheel);
}

#pragma region Wheels
void ARaceCarBase::ApplyWheelsSuspension(USceneComponent* TargetWheel)
{
	// Cast The trace to detect the floor
	const FVector TraceStartPos = TargetWheel->GetComponentLocation();
	const FVector TraceEndPos = TargetWheel->GetComponentLocation() + FVector(0, 0, -60);
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStartPos, TraceEndPos, ECC_Visibility);
	DrawDebugLine(GetWorld(), TraceStartPos, TraceEndPos, FColor::Red);
	if (!Hit.bBlockingHit)
		return;

	// Calculate and apply the force
	float OutputFloat = 1.f - FMath::GetRangePct(0.f, 60.f, Hit.Distance);
	const FVector ForceDirection = FVector(Hit.TraceStart - Hit.TraceEnd).GetSafeNormal() * OutputFloat;
	CarBody->AddForceAtLocation(ForceDirection * SuspensionForce, TargetWheel->GetComponentLocation());
}

#pragma endregion

#pragma region Movement
void ARaceCarBase::Accelerate(float Input)
{
	// Calculate the acceleration input
	AccelerationInput = FMath::FInterpTo(AccelerationInput, Input, GetWorld()->GetDeltaSeconds(), AccelerationSpeed);
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

void ARaceCarBase::ApplySpeed(const USceneComponent* Wheel)
{
	const FVector AccelerationForceVector = CarBody->GetForwardVector() * Speed; 	// Calculate acceleration the force
	CarBody->AddForceAtLocation(AccelerationForceVector, Wheel->GetComponentLocation());  //  apply the force
	CarBody->SetCenterOfMass(FVector(CenterOfMassOffsetWhenAccelerating, 0.f, 0.f) * AccelerationInput);  // Apply the car tilting.
}
#pragma endregion







