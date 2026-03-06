// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "RacecarBase.generated.h"

UCLASS()
class RACINGCATS_API ARacecarBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARacecarBase();

protected:

	// Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* CarMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wheels")
	USceneComponent* FLWheel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wheels")
	USceneComponent* FRWheel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wheels")
	USceneComponent* BLWheel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wheels")
	USceneComponent* BRWheel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Wheels")
	float SuspensionForce = 20000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Wheels")
	float SuspensionDamping = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Speed and Accleration")
	float MaxSpeed = 15000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Speed and Accleration")
	float AccelerationForce = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.f, ClampMax = 2.f), Category= "Speed and Accleration")
	float AccelerationSpeed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.f, ClampMax = 2.f), Category= "Speed and Accleration")
	float DeAccelerationSpeed = 1.f;
	
	UPROPERTY()
	float AccelerationInput;
	
	UPROPERTY()
	float Speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	UInputMappingContext* InputMappingContext;
	
	// functions
	UFUNCTION(BlueprintCallable, Category="Car")
	void SuspensionCast(USceneComponent* TargetWheel);

	UFUNCTION(BlueprintCallable, Category="Car")
	void Accelerate(float Input);

	UFUNCTION(BlueprintCallable, Category="Car")
	void CalculateAcceleration();

	UFUNCTION(BlueprintCallable, Category="Car")
	void ApplyAccelerationToWheels(USceneComponent* Wheel);
	
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void UnPossessed() override;
};
