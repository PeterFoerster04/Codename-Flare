// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VehicleGamePawn.h"
#include "VehicleGameSportsCar.generated.h"

class UPostProcessComponent;
class URectLightComponent;
class USpotLightComponent;
class UNiagaraComponent;


/**
 *  Sports car wheeled vehicle implementation
 */
UCLASS(abstract)
class VEHICLEGAME_API AVehicleGameSportsCar : public AVehicleGamePawn
{
	GENERATED_BODY()
	
public:

	AVehicleGameSportsCar();

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

protected:

	//components
	UPROPERTY(EditAnywhere,Category = Lights, meta = (AllowPrivateAccess))
	USpotLightComponent* HeadlightLeft;

	UPROPERTY(EditAnywhere, Category = Lights, meta = (AllowPrivateAccess))
	USpotLightComponent* HeadlightRight;

	UPROPERTY(EditAnywhere, Category = Lights, meta = (AllowPrivateAccess))
	URectLightComponent* UnderCarIllumination;


	UPROPERTY(EditAnywhere, Category = Lights, meta = (AllowPrivateAccess))
	UNiagaraComponent* LeftBooster;
	UPROPERTY(EditAnywhere, Category = Lights, meta = (AllowPrivateAccess))
	UNiagaraComponent* RightBooster;

	//members

	UPROPERTY(EditAnywhere, Category = Booster, meta = (AllowPrivateAccess))
	float DefaultSpringArmLength = 650.0f;
	UPROPERTY(EditAnywhere, Category = Booster, meta = (AllowPrivateAccess))
	float BoostSpringArmLength = 850.0f;
	UPROPERTY(EditAnywhere, Category = Booster, meta = (AllowPrivateAccess))
	float SpringArmLerpSpeed = 2.0f;


	UPROPERTY(EditAnywhere, Category = Booster, meta = (AllowPrivateAccess))
	float DefaultFOV = 90.0f;
	UPROPERTY(EditAnywhere, Category = Booster, meta = (AllowPrivateAccess))
	float BoostFOV = 105.0f;
	UPROPERTY(EditAnywhere, Category = Booster, meta = (AllowPrivateAccess))
	float FOVLerpSpeed = 2.0f;



	UPROPERTY(EditAnywhere, Category = Booster, meta = (AllowPrivateAccess))
	bool IsBoosting;
	UPROPERTY(EditAnywhere, Category = Booster, meta = (AllowPrivateAccess))
	float BoostAmount = 500.0f;

	UPROPERTY(EditAnywhere, Category = CameraEffect, meta = (AllowPrivateAccess))
	float DefaultFringe = 0.0f;
	UPROPERTY(EditAnywhere, Category = CameraEffect, meta = (AllowPrivateAccess))
	float DefaultVignette = 0.0f;


	UPROPERTY(EditAnywhere, Category = CameraEffect, meta = (AllowPrivateAccess))
	float BoostFringe = 5.0f;
	UPROPERTY(EditAnywhere, Category = CameraEffect, meta = (AllowPrivateAccess))
	float BoostVignette = 10.0f;
	UPROPERTY(EditAnywhere, Category = CameraEffect, meta = (AllowPrivateAccess))
	float EffectLerpSpeed = 2.0f;


	//functions

	void ChangeBackSpringArmLength(float Delta);
	void ChangeFOV(float Delta);
	void ChangeCameraEffects(float Delta);
	void ToggleBooster(bool Activate);



	//input
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* BoostAction;






	void Boost(const FInputActionValue& Value);
};
