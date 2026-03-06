
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
	GENERATED_BODY();

public:

	/** This is a list of all the cars in the Race, the order represents their position in the race*/
	UPROPERTY(BlueprintReadOnly, Category="Race")
	TArray<ARaceCarBase*> Racers;
	
	void SetRacers(const TArray<ARaceCarBase*>& ParamRacers) {	Racers = ParamRacers; }
};
