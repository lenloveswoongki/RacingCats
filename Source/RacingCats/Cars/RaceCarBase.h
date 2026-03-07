

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Wheels")
	USceneComponent* FLWheel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Wheels")
	USceneComponent* FRWheel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Wheels")
	USceneComponent* BLWheel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Wheels")
	USceneComponent* BRWheel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Wheels")
	UStaticMeshComponent* FLWheelMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Wheels")
	UStaticMeshComponent* FRWheelMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Wheels")
	UStaticMeshComponent* BLWheelMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Wheels")
	UStaticMeshComponent* BRWheelMesh;

	/** The speed in which the wheels rotates towards the desire direction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.f, ClampMax = 5.f), Category= "Wheels")
	float WheelsRotationSpeed = 3.f;
	/** The speed in which the wheels realign to the car rotation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.f, ClampMax = 5.f), Category= "Wheels")
	float WheelsRotationNormalizeSpeed = 2.f;
	/* The force that the wheels rotate on the ground**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.f, ClampMax = 20.f), Category= "Wheels")
	float WheelsSpinForce = 20.f;
	/* The speed that the wheels rotate on the ground**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.f, ClampMax = 5.f), Category= "Wheels")
	float WheelsSpinSpeed = 0.5f;

	
	// Suspension
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Wheels")
	float SuspensionForce = 20000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Wheels")
	float SuspensionDamping = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMax = 5.f, ClampMin = 0.f), Category= "Wheels")
	float RotationSpeed = 3.0f;
	UPROPERTY(BlueprintReadOnly, Category="Wheels")
	bool bIsGrounded = false;
	
	
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

	/** Bind this function to Steering Input */
	UFUNCTION(BlueprintCallable, Category="Speed and Accleration")
	void RotateWheels(float Input);

	void NormalizeWheelsRotation();
	void ApplyWheelsSuspension(const USceneComponent* TargetWheel, UStaticMeshComponent* WheelMesh); // Call on Tick()
	void CalculateSpeed();  // Call on Tick()
	void ApplySpeed(USceneComponent* Wheel); // Call on Tick()
	void FixRotations();

	
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
};
