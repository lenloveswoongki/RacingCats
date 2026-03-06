// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RacerData.h"
#include "UObject/Class.h"
#include "RaceData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct RACINGCATS_API FRaceData
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly, Category="Race")
	TArray<FRacerData> RacersData;
	
	void SetRacers(const TArray<FRacerData>& ParamRacersData) {	RacersData = ParamRacersData; }
};
