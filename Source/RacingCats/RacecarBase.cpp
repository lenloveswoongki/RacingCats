// Fill out your copyright notice in the Description page of Project Settings.

#include "RacecarBase.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ARacecarBase::ARacecarBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create default components
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxComponent->SetSimulatePhysics(true);
	BoxComponent->SetLinearDamping(SuspensionDamping);
	SetRootComponent(BoxComponent);
	
	CarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Car Mesh"));
	CarMesh->SetupAttachment(BoxComponent);
	
	CarMesh->bEditableWhenInherited = true;
	BoxComponent->bEditableWhenInherited = true;

	FLWheel = CreateDefaultSubobject<USceneComponent>(TEXT("Forward Left Wheel"));
	FLWheel->SetupAttachment(BoxComponent);
	FRWheel = CreateDefaultSubobject<USceneComponent>(TEXT("Forward Right Wheel"));
	FRWheel->SetupAttachment(BoxComponent);
	BLWheel = CreateDefaultSubobject<USceneComponent>(TEXT("Back Left Wheel"));
	BLWheel->SetupAttachment(BoxComponent);
	BRWheel = CreateDefaultSubobject<USceneComponent>(TEXT("Back Right Wheel"));
	BRWheel->SetupAttachment(BoxComponent);
	
	FLWheel->bEditableWhenInherited = true;
	BLWheel->bEditableWhenInherited = true;
	FRWheel->bEditableWhenInherited = true;
	BRWheel->bEditableWhenInherited = true;
	
}

void ARacecarBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (PlayerController->IsLocalController())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}

void ARacecarBase::UnPossessed()
{
	Super::UnPossessed();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (PlayerController->IsLocalController())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(InputMappingContext);
			}
		}
	}
}


void ARacecarBase::SuspensionCast(USceneComponent* TargetWheel)
{
	FVector TraceStartPos = TargetWheel->GetComponentLocation();
	FVector TraceEndPos = TargetWheel->GetComponentLocation() + FVector(0, 0, -60);
	FHitResult Hit;
	
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStartPos, TraceEndPos, ECC_Visibility);
	DrawDebugLine(GetWorld(), TraceStartPos, TraceEndPos, FColor::Red);

	if (!Hit.bBlockingHit)
	{
		return;
	}
	float OutputFloat = 1.f - FMath::GetRangePct(0.f, 60.f, Hit.Distance);
	FVector ForceDirection = FVector(Hit.TraceStart - Hit.TraceEnd).GetSafeNormal() * OutputFloat;
	
	BoxComponent->AddForceAtLocation(ForceDirection * SuspensionForce, TargetWheel->GetComponentLocation());
}

void ARacecarBase::Accelerate(float Input)
{
	AccelerationInput = FMath::FInterpTo(AccelerationInput, 1.f, GetWorld()->GetDeltaSeconds(), AccelerationSpeed);
}

void ARacecarBase::CalculateAcceleration()
{
	Speed = FMath::Lerp(0.f, MaxSpeed, AccelerationInput);
	AccelerationInput = FMath::FInterpTo(AccelerationInput, 0.f, GetWorld()->GetDeltaSeconds(), DeAccelerationSpeed);
}

void ARacecarBase::ApplyAccelerationToWheels(USceneComponent* Wheel)
{
	const FVector AccelerationForceVector = BoxComponent->GetForwardVector() * Speed;
	BoxComponent->AddForceAtLocation(AccelerationForceVector, Wheel->GetComponentLocation());
}


void ARacecarBase::Tick(float DeltaTime)
{
	SuspensionCast(FRWheel);
	SuspensionCast(BLWheel);
	SuspensionCast(FLWheel);
	SuspensionCast(BRWheel);

	CalculateAcceleration();
	ApplyAccelerationToWheels(FRWheel);
	ApplyAccelerationToWheels(BLWheel);
	ApplyAccelerationToWheels(FLWheel);
	ApplyAccelerationToWheels(BRWheel);
}





