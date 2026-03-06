// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RacerData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct RACINGCATS_API FRacerData 
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category="GameMode")
	APawn* Pawn;

	UPROPERTY(BlueprintReadOnly, Category="GameMode")
	int32 RacePosition;
	
};
