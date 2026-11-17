// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KartPlayerController.generated.h"

class UKartCountDown;
class UKartRaceHUDWidget;
class UInputMappingContext;
class UInputAction;
class APawny;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class BMARIOKART_API AKartPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	
protected:
	
    virtual void BeginPlay() override;
    // Appelée par Unreal pour brancher les touches : c'est ici qu'on relie chaque Input Action à une fonction
    virtual void SetupInputComponent() override;
	
    // Widget du compte à rebours à afficher (mettre WBP_Countdown ici dans BP_KartPlayerController)
    UPROPERTY(EditAnywhere, Category = "Kart|UI")
    TSubclassOf<UKartCountDown> CountDownClass;
	
    // Widget du chrono et des tours (mettre WBP_RaceHud ici dans BP_KartPlayerController)
    UPROPERTY(EditAnywhere, Category = "Kart|UI")
    TSubclassOf<UKartRaceHUDWidget> RaceHUDWidgetClass;
	
    // ---------- Input (avant, ces réglages étaient dans le kart) ----------
    UPROPERTY(EditAnywhere, Category = "Kart|Input")
    TObjectPtr<UInputMappingContext> KartMappingContext;
	
    // Axe 1D : +1 = accélérer, -1 = freiner / reculer
    UPROPERTY(EditAnywhere, Category = "Kart|Input")
	TObjectPtr<UInputAction> ThrottleAction;
	
    // Axe 1D : +1 = droite, -1 = gauche
    UPROPERTY(EditAnywhere, Category = "Kart|Input")
    TObjectPtr<UInputAction> SteerAction;
	
    // Bouton : maintenir en tournant pour drifter
    UPROPERTY(EditAnywhere, Category = "Kart|Input")
    TObjectPtr<UInputAction> DriftAction;
	
    // Bouton : petit saut
    UPROPERTY(EditAnywhere, Category = "Kart|Input")
    TObjectPtr<UInputAction> JumpAction;
	
    // Bouton : utiliser l'objet bonus en main
    UPROPERTY(EditAnywhere, Category = "Kart|Input")
    TObjectPtr<UInputAction> UseItemAction;
	
	
private:
    // ---------- Fonctions appelées par les touches : chacune transmet une commande au kart ----------
    void OnThrottle(const FInputActionValue& Value);
    void OnThrottleReleased(const FInputActionValue& Value);
    void OnSteer(const FInputActionValue& Value);
    void OnSteerReleased(const FInputActionValue& Value);
    void OnDriftPressed(const FInputActionValue& Value);
    void OnDriftReleased(const FInputActionValue& Value);
    void OnJumpPressed(const FInputActionValue& Value);
    void OnUseItemPressed(const FInputActionValue& Value);
    
	// Le kart que ce controller pilote (nullptr s'il ne pilote rien, ou autre chose qu'un kart)
    APawny* GetKart() const;
    
	UPROPERTY()
    TObjectPtr<UKartCountDown> CountDown;
	
    UPROPERTY()
    TObjectPtr<UKartRaceHUDWidget> RaceHUDWidget;
	
};
