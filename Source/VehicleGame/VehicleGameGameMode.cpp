// Copyright Epic Games, Inc. All Rights Reserved.

#include "VehicleGameGameMode.h"
#include "VehicleGamePlayerController.h"

AVehicleGameGameMode::AVehicleGameGameMode()
{
	PlayerControllerClass = AVehicleGamePlayerController::StaticClass();
}
