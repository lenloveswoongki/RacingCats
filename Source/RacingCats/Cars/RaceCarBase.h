

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "RaceCarBase.generated.h"


/**
 *
 * This is the base class for all cars in the game. The functions must be binded to the controller on blueprints
 */
UCLASS()
class RACINGCATS_API ARaceCarBase : public APawn
{
	GENERATED_BODY()

public:
	ARaceCarBase();

protected:

	//           ------------- Properties --------------       //
			// ------------------------------------- //

	/** This is the body of the Car, the physics are applied to this object*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UBoxComponent* CarBody;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* CarMesh;

	// Wheels
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wheels")
	USceneComponent* FLWheel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wheels")
	USceneComponent* FRWheel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wheels")
	USceneComponent* BLWheel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wheels")
	USceneComponent* BRWheel;

	// Suspension
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Wheels")
	float SuspensionForce = 20000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Wheels")
	float SuspensionDamping = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Speed and Accleration")
	float MaxSpeed = 15000.f;

	/** The speed in which the car will get closer to max speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.f, ClampMax = 2.f), Category= "Speed and Accleration")
	float AccelerationSpeed = 1.f;

	/** The speed in which the car will slow down to zero speed*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.f, ClampMax = 2.f), Category= "Speed and Accleration")
	float DeAccelerationSpeed = 1.f;

	/** When accelerating the car will tilt forward or backwards depending on direction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.f, ClampMax = 100.f), Category= "Speed and Accleration")
	float CenterOfMassOffsetWhenAccelerating = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Speed and Accleration")
	float SteeringForce = 1000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	UInputMappingContext* InputMappingContext;
	
	UPROPERTY()
	float AccelerationInput;
	UPROPERTY()
	float Speed;

	
	//           ------------- Functions --------------       //
			// ------------------------------------- //

	/** Bind this function to Acceleration input */
	UFUNCTION(BlueprintCallable, Category="Speed and Accleration")
	void Accelerate(float Input);

	/** Bind this function to Steering Input */
	UFUNCTION(BlueprintCallable, Category="Speed and Accleration")
	void Steer(float Input);
	
	void ApplyWheelsSuspension(USceneComponent* TargetWheel); // Call on Tick()
	void CalculateSpeed();  // Call on Tick()
	void ApplySpeed(const USceneComponent* Wheel); // Call on Tick()
	
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
};
