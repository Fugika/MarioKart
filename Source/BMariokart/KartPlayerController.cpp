// Fill out your copyright notice in the Description page of Project Settings.


#include "KartPlayerController.h"

#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "KartCountDown.h"
#include "Pawny.h"
#include "KartRaceHUDWidget.h"

void AKartPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// L'UI n'existe que pour le joueur qui joue sur cette machine
	if (!IsLocalController())
	{
		return;
	}
	// On donne au joueur les touches du kart (avant, c'était le kart qui le faisait dans NotifyControllerChanged)
	// Le Mapping Context dit quelle touche déclenche quelle Input Action
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (KartMappingContext != nullptr)
		{
			Subsystem->AddMappingContext(KartMappingContext, 0);
		}
	}
	if (CountDownClass != nullptr)
	{
		CountDown = CreateWidget<UKartCountDown>(this, CountDownClass);
		if (CountDown != nullptr)
		{
			CountDown->AddToViewport();
		}
	}
	if (RaceHUDWidgetClass != nullptr)
	{
		RaceHUDWidget = CreateWidget<UKartRaceHUDWidget>(this, RaceHUDWidgetClass);
		if (RaceHUDWidget != nullptr)
		{
			RaceHUDWidget->AddToViewport();
		}
	}
}

void AKartPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInput == nullptr)
	{
		return;
	}
	
	EnhancedInput->BindAction(ThrottleAction,ETriggerEvent::Triggered, this, &AKartPlayerController::OnThrottle);
	EnhancedInput->BindAction(ThrottleAction,ETriggerEvent::Completed, this, &AKartPlayerController::OnThrottleReleased);
	
	EnhancedInput->BindAction(SteerAction,ETriggerEvent::Triggered, this, &AKartPlayerController::OnSteer);
	EnhancedInput->BindAction(SteerAction,ETriggerEvent::Completed, this, &AKartPlayerController::OnSteerReleased);
	
	EnhancedInput->BindAction(DriftAction,ETriggerEvent::Started, this, &AKartPlayerController::OnDriftPressed);
	EnhancedInput->BindAction(DriftAction,ETriggerEvent::Completed, this, &AKartPlayerController::OnDriftReleased);
	
	EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &AKartPlayerController::OnJumpPressed);
	EnhancedInput->BindAction(UseItemAction,ETriggerEvent::Started, this, &AKartPlayerController::OnUseItemPressed);
	
}

APawny* AKartPlayerController::GetKart() const
{
	return GetPawn<APawny>();
}

void AKartPlayerController::OnThrottle(const FInputActionValue& Value)
{
	if (APawny* Kart = GetKart())
	{
		Kart->SetThrottleInput(Value.Get<float>());
	}
}

void AKartPlayerController::OnThrottleReleased(const FInputActionValue& Value)
{
	if (APawny* Kart = GetKart())
	{
		Kart->SetThrottleInput(0.f);
	}
}

void AKartPlayerController::OnSteer(const FInputActionValue& Value)
{
	if (APawny* Kart = GetKart())
	{
		Kart->SetSteerInput(Value.Get<float>());
	}
}

void AKartPlayerController::OnSteerReleased(const FInputActionValue& Value)
{
	if (APawny* Kart = GetKart())
	{
		Kart->SetSteerInput(0.f);
	}
}

void AKartPlayerController::OnDriftPressed(const FInputActionValue& Value)
{
	if (APawny* Kart = GetKart())
	{
		Kart->StartDrift();
	}
}

void AKartPlayerController::OnDriftReleased(const FInputActionValue& Value)
{
	if (APawny* Kart = GetKart())
	{
		Kart->EndDrift();
	}
}

void AKartPlayerController::OnJumpPressed(const FInputActionValue& Value)
{
	if (APawny* Kart = GetKart())
	{
		Kart->Jump();
	}
}

void AKartPlayerController::OnUseItemPressed(const FInputActionValue& Value)
{
	if (APawny* Kart = GetKart())
	{
		Kart->UseItem();
	}
}