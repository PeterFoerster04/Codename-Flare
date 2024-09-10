// Copyright Epic Games, Inc. All Rights Reserved.


#include "VehicleGameSportsCar.h"
#include "VehicleGameSportsWheelFront.h"
#include "VehicleGameSportsWheelRear.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/RectLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/PostProcessComponent.h"


AVehicleGameSportsCar::AVehicleGameSportsCar()
{
	// Note: for faster iteration times, the vehicle setup can be tweaked in the Blueprint instead

	// Set up the chassis
	GetChaosVehicleMovement()->ChassisHeight = 144.0f;
	GetChaosVehicleMovement()->DragCoefficient = 0.31f;

	// Set up the wheels
	GetChaosVehicleMovement()->bLegacyWheelFrictionPosition = true;
	GetChaosVehicleMovement()->WheelSetups.SetNum(4);

	GetChaosVehicleMovement()->WheelSetups[0].WheelClass = UVehicleGameSportsWheelFront::StaticClass();
	GetChaosVehicleMovement()->WheelSetups[0].BoneName = FName("Phys_Wheel_FL");
	GetChaosVehicleMovement()->WheelSetups[0].AdditionalOffset = FVector(0.0f, 0.0f, 0.0f);

	GetChaosVehicleMovement()->WheelSetups[1].WheelClass = UVehicleGameSportsWheelFront::StaticClass();
	GetChaosVehicleMovement()->WheelSetups[1].BoneName = FName("Phys_Wheel_FR");
	GetChaosVehicleMovement()->WheelSetups[1].AdditionalOffset = FVector(0.0f, 0.0f, 0.0f);

	GetChaosVehicleMovement()->WheelSetups[2].WheelClass = UVehicleGameSportsWheelRear::StaticClass();
	GetChaosVehicleMovement()->WheelSetups[2].BoneName = FName("Phys_Wheel_BL");
	GetChaosVehicleMovement()->WheelSetups[2].AdditionalOffset = FVector(0.0f, 0.0f, 0.0f);

	GetChaosVehicleMovement()->WheelSetups[3].WheelClass = UVehicleGameSportsWheelRear::StaticClass();
	GetChaosVehicleMovement()->WheelSetups[3].BoneName = FName("Phys_Wheel_BR");
	GetChaosVehicleMovement()->WheelSetups[3].AdditionalOffset = FVector(0.0f, 0.0f, 0.0f);

	// Set up the engine
	// NOTE: Check the Blueprint asset for the Torque Curve
	GetChaosVehicleMovement()->EngineSetup.MaxTorque = 750.0f;
	GetChaosVehicleMovement()->EngineSetup.MaxRPM = 7000.0f;
	GetChaosVehicleMovement()->EngineSetup.EngineIdleRPM = 900.0f;
	GetChaosVehicleMovement()->EngineSetup.EngineBrakeEffect = 0.2f;
	GetChaosVehicleMovement()->EngineSetup.EngineRevUpMOI = 5.0f;
	GetChaosVehicleMovement()->EngineSetup.EngineRevDownRate = 600.0f;

	// Set up the transmission
	GetChaosVehicleMovement()->TransmissionSetup.bUseAutomaticGears = true;
	GetChaosVehicleMovement()->TransmissionSetup.bUseAutoReverse = true;
	GetChaosVehicleMovement()->TransmissionSetup.FinalRatio = 2.81f;
	GetChaosVehicleMovement()->TransmissionSetup.ChangeUpRPM = 6000.0f;
	GetChaosVehicleMovement()->TransmissionSetup.ChangeDownRPM = 2000.0f;
	GetChaosVehicleMovement()->TransmissionSetup.GearChangeTime = 0.2f;
	GetChaosVehicleMovement()->TransmissionSetup.TransmissionEfficiency = 0.9f;

	GetChaosVehicleMovement()->TransmissionSetup.ForwardGearRatios.SetNum(5);
	GetChaosVehicleMovement()->TransmissionSetup.ForwardGearRatios[0] = 4.25f;
	GetChaosVehicleMovement()->TransmissionSetup.ForwardGearRatios[1] = 2.52f;
	GetChaosVehicleMovement()->TransmissionSetup.ForwardGearRatios[2] = 1.66f;
	GetChaosVehicleMovement()->TransmissionSetup.ForwardGearRatios[3] = 1.22f;
	GetChaosVehicleMovement()->TransmissionSetup.ForwardGearRatios[4] = 1.0f;

	GetChaosVehicleMovement()->TransmissionSetup.ReverseGearRatios.SetNum(1);
	GetChaosVehicleMovement()->TransmissionSetup.ReverseGearRatios[0] = 4.04f;

	// Set up the steering
	// NOTE: Check the Blueprint asset for the Steering Curve
	GetChaosVehicleMovement()->SteeringSetup.SteeringType = ESteeringType::Ackermann;
	GetChaosVehicleMovement()->SteeringSetup.AngleRatio = 0.7f;



	HeadlightLeft = CreateDefaultSubobject<USpotLightComponent>("LeftHeadLight");
	HeadlightLeft->SetupAttachment(GetMesh());

	HeadlightRight = CreateDefaultSubobject<USpotLightComponent>("RightHeadLight");
	HeadlightRight->SetupAttachment(GetMesh());

	UnderCarIllumination = CreateDefaultSubobject<URectLightComponent>("UnderCarIllumination");
	UnderCarIllumination->SetupAttachment(GetMesh());

	LeftBooster = CreateDefaultSubobject<UNiagaraComponent>("BoosterLeft");
	LeftBooster->SetupAttachment(GetMesh());

	RightBooster = CreateDefaultSubobject<UNiagaraComponent>("BoosterRight");
	RightBooster->SetupAttachment(GetMesh());

	
}

void AVehicleGameSportsCar::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ChangeBackSpringArmLength(DeltaSeconds);
	ChangeCameraEffects(DeltaSeconds);
	ChangeFOV(DeltaSeconds);
	UpdateKMDriven(DeltaSeconds);

}

void AVehicleGameSportsCar::BeginPlay()
{
	Super::BeginPlay();
	GetBackCamera()->PostProcessSettings.bOverride_SceneFringeIntensity = true;
	GetBackCamera()->PostProcessSettings.bOverride_VignetteIntensity = true;
	GetFollowCamera()->PostProcessSettings.bOverride_SceneFringeIntensity = true;
	GetFollowCamera()->PostProcessSettings.bOverride_VignetteIntensity = true;

	LastPosition = GetMesh()->GetComponentLocation();


}

void AVehicleGameSportsCar::ChangeBackSpringArmLength(float Delta)
{
	if(IsBoosting)
	{
		GetBackSpringArm()->TargetArmLength = FMath::Lerp(GetBackSpringArm()->TargetArmLength, BoostSpringArmLength, SpringArmLerpSpeed * Delta);
	}
	else
	{
		GetBackSpringArm()->TargetArmLength = FMath::Lerp(GetBackSpringArm()->TargetArmLength, DefaultSpringArmLength, SpringArmLerpSpeed * Delta);
	}
}

void AVehicleGameSportsCar::ChangeFOV(float Delta)
{
	if (IsBoosting)
	{
		GetBackCamera()->FieldOfView = FMath::Lerp(GetBackCamera()->FieldOfView, BoostFOV, FOVLerpSpeed * Delta);
	}
	else
	{
		GetBackCamera()->FieldOfView = FMath::Lerp(GetBackCamera()->FieldOfView, DefaultFOV, FOVLerpSpeed * Delta);
	}
}

void AVehicleGameSportsCar::ChangeCameraEffects(float Delta)
{
	if(IsBoosting)
	{
		GetBackCamera()->PostProcessSettings.VignetteIntensity = FMath::Lerp(GetBackCamera()->PostProcessSettings.VignetteIntensity, BoostVignette, EffectLerpSpeed * Delta);
		GetBackCamera()->PostProcessSettings.SceneFringeIntensity = FMath::Lerp(GetBackCamera()->PostProcessSettings.SceneFringeIntensity, BoostFringe, EffectLerpSpeed * Delta);
		GetFollowCamera()->PostProcessSettings.VignetteIntensity = FMath::Lerp(GetBackCamera()->PostProcessSettings.VignetteIntensity, BoostVignette, EffectLerpSpeed * Delta);
		GetFollowCamera()->PostProcessSettings.SceneFringeIntensity = FMath::Lerp(GetBackCamera()->PostProcessSettings.SceneFringeIntensity, BoostFringe, EffectLerpSpeed * Delta);
	}
	else
	{
		GetBackCamera()->PostProcessSettings.VignetteIntensity = FMath::Lerp(GetBackCamera()->PostProcessSettings.VignetteIntensity, DefaultVignette, EffectLerpSpeed * Delta);
		GetBackCamera()->PostProcessSettings.SceneFringeIntensity = FMath::Lerp(GetBackCamera()->PostProcessSettings.SceneFringeIntensity, DefaultFringe, EffectLerpSpeed * Delta);
		GetFollowCamera()->PostProcessSettings.VignetteIntensity = FMath::Lerp(GetBackCamera()->PostProcessSettings.VignetteIntensity, DefaultVignette, EffectLerpSpeed * Delta);
		GetFollowCamera()->PostProcessSettings.SceneFringeIntensity = FMath::Lerp(GetBackCamera()->PostProcessSettings.SceneFringeIntensity, DefaultFringe, EffectLerpSpeed * Delta);
	}
}

void AVehicleGameSportsCar::ToggleBooster(bool Activate)
{
	if(Activate)
	{
		LeftBooster->Activate();
		RightBooster->Activate();
	}
	else
	{
		LeftBooster->Deactivate();
		RightBooster->Deactivate();
	}
	
	
}

void AVehicleGameSportsCar::UpdateKMDriven(float Delta)
{
	if (GetMesh()->GetPhysicsLinearVelocity().Length() <= 0.1f) return;
	//convert from m to km
	DrivenKM += (FVector::Distance(GetMesh()->GetComponentLocation(), LastPosition) * Delta)*0.001f;
	LastPosition = GetMesh()->GetComponentLocation();



}

void AVehicleGameSportsCar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(BoostAction, ETriggerEvent::Triggered, this, &AVehicleGameSportsCar::Boost);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component."), *GetNameSafe(this));
	}
	


}

void AVehicleGameSportsCar::Boost(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("%i"), Value.Get<bool>())

	bool BoostInput = Value.Get<bool>();
	if(BoostInput)
	{
		IsBoosting = true;
		GetMesh()->AddForce(GetMesh()->GetForwardVector()*BoostAmount,NAME_None,true);
	}
	else
	{
		IsBoosting = false;
	}
	ToggleBooster(BoostInput);
}
