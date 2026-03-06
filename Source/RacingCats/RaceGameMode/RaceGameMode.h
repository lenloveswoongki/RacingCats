#pragma once
#include "RaceData.h"
#include "GameFramework/GameMode.h"
#include "RaceGameMode.generated.h"

/**
 *
 * This game mode shall be used on the race mode. It will handle the global game during all the race
 */
UCLASS()
class ARaceGameMode : public AGameMode
{
	GENERATED_BODY()
	
protected:

	//           ------------- Properties --------------       //
			// ------------------------------------- //

	/** A list of all the cars in the race */
	UPROPERTY(BlueprintReadOnly, Category="GameMode")
	TArray<FRacerData> Racers;

	/** A list of data of all the races in the current Game */
	UPROPERTY(BlueprintReadOnly, Category="GameMode")
	TArray<FRaceData> RacesData;

	/** The data of the current Race */
	UPROPERTY(BlueprintReadOnly, Category="GameMode")
	FRacerData CurrentRaceData;

	/** The spawn points in the current race */
	UPROPERTY(BlueprintReadOnly, Category="GameMode")
	TArray<FVector> SpawnPoints;
	
public:

	UFUNCTION(BlueprintCallable, Category="GameMode")
	inline void AddRacer(FRacerData Racer) { Racers.Add(Racer); }

	
};
