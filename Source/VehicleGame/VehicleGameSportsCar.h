// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VehicleGamePawn.h"
#include "VehicleGameSportsCar.generated.h"

/**
 *  Sports car wheeled vehicle implementation
 */
UCLASS(abstract)
class VEHICLEGAME_API AVehicleGameSportsCar : public AVehicleGamePawn
{
	GENERATED_BODY()
	
public:

	AVehicleGameSportsCar();
};
