#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KartGameMode.generated.h"

// Un "delegate" est un event : le GameMode le déclenche (Broadcast) et tous ceux qui s'y sont abonnés sont prévenus
// Le nom de la macro se lit morceau par morceau :
// - DECLARE   = "déclarer" : la macro écrit pour nous le code d'un nouveau TYPE d'event (comme on déclare une struct)
//               Ce n'est pas encore l'event lui-même, juste son modèle : le nom du type + les paramètres qu'il envoie
//               L'event réel est la variable créée avec ce type plus bas (FOnCountdownTick OnCountdownTick;)
// - DYNAMIC   = utilisable en Blueprint (on peut le voir et s'y abonner dans un Widget ou un BP)
// - MULTICAST = plusieurs abonnés en même temps (le widget, un son, un BP de décor...)
// - DELEGATE  = c'est un delegate (un event)
// - _OneParam = l'event envoie 1 paramètre (il existe aussi _TwoParams, _ThreeParams...), rien = aucun paramètre
// Par convention le nom du type commence par F (FOnCountdownTick), comme les autres types Unreal (FVector, FString...)

// Event avec un paramètre : on donne le type puis le nom du paramètre (Number = 3, 2, 1 puis 0 pour le GO)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCountdownTick, int32, Number);

// Event sans paramètre : il dit juste "c'est parti"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRaceStarted);

// Event envoyé quand un kart a fini tous ses tours (Kart = celui qui vient de finir)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKartFinished, APawny*, Kart);

// Règles de la course : gère le compte à rebours "3, 2, 1, GO", débloque les karts au GO,
// lance le chrono et connaît le nombre de tours à faire
// L'UI (widget) n'a qu'à écouter OnCountdownTick et OnRaceStarted pour s'afficher
UCLASS()
class BMARIOKART_API AKartGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AKartGameMode();
    
    virtual void Tick(float DeltaTime) override;

    // Appelé à chaque chiffre puis au GO : à brancher dans le widget du compte à rebours
    UPROPERTY(BlueprintAssignable, Category = "Kart|Countdown")
    FOnCountdownTick OnCountdownTick;

    // Appelé au GO, quand les karts peuvent bouger
    UPROPERTY(BlueprintAssignable, Category = "Kart|Countdown")
    FOnRaceStarted OnRaceStarted;

    // Appelé quand un kart passe la ligne d'arrivée de son dernier tour
    UPROPERTY(BlueprintAssignable, Category = "Kart|Race")
    FOnKartFinished OnKartFinished;

    // Vrai une fois le GO passé (ou tout de suite si le compte à rebours est désactivé)
    // "const" après les parenthèses = la fonction promet de ne rien modifier dans le GameMode, elle ne fait que lire
    // (le compilateur refuse si on essaie de changer une variable dedans, et on peut l'appeler sur un GameMode const)
    UFUNCTION(BlueprintPure, Category = "Kart|Countdown")
    bool IsRaceStarted() const { return bRaceStarted || !bUseCountdown; }

    // Chiffre affiché en ce moment (0 = pas encore commencé ou course lancée)
    UFUNCTION(BlueprintPure, Category = "Kart|Countdown")
    int32 GetCountdownNumber() const { return CountdownNumber; }

    // Relance le compte à rebours (bloque les karts), par exemple pour recommencer une course
    UFUNCTION(BlueprintCallable, Category = "Kart|Countdown")
    void StartCountdown();

    // ---------- Course : tours et chrono ----------

    // Nombre de tours à faire pour finir la course
    UFUNCTION(BlueprintPure, Category = "Kart|Race")
    int32 GetLapCount() const { return LapCount; }

    // Nombre de checkpoints posés sur le circuit (les ALapZone avec un ZoneIndex de 1 ou plus)
    UFUNCTION(BlueprintPure, Category = "Kart|Race")
    int32 GetCheckpointCount() const { return CheckpointCount; }

    // Temps écoulé depuis le GO, en secondes (0 avant le GO)
    UFUNCTION(BlueprintPure, Category = "Kart|Race")
    float GetRaceTime() const;

    // Appelée par un kart quand il passe la ligne d'arrivée de son dernier tour
    void NotifyKartFinished(APawny* Kart);

    // Vrai si la case bShowDebugText est cochée (le kart s'en sert pour afficher aussi son debug)
    UFUNCTION(BlueprintPure, Category = "Kart|Countdown|Debug")
    bool IsDebugTextEnabled() const { return bShowDebugText; }

    // Transforme un temps en secondes en texte "minutes:secondes.millisecondes" (ex : 83.5 -> "01:23.500")
    // "static" = la fonction n'a pas besoin d'un GameMode précis pour marcher, on l'appelle avec le nom de la classe :
    // AKartGameMode::FormatRaceTime(Temps) (par exemple depuis le widget du chrono)
    UFUNCTION(BlueprintPure, Category = "Kart|Race")
    static FText FormatRaceTime(float Seconds);

protected:
    virtual void BeginPlay() override;

    // ---------- Réglages ----------

    // Nombre de tours demandés pour finir la course
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kart|Race", meta = (ClampMin = "1"))
    int32 LapCount = 3;

    // Décoché = les karts peuvent rouler tout de suite
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kart|Countdown")
    bool bUseCountdown = true;

    // Nombre de secondes avant le GO (3 = "3, 2, 1")
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kart|Countdown", meta = (ClampMin = "1"))
    int32 CountdownSeconds = 3;

    // Attente avant le "3" : laisse le temps au niveau de charger et au widget de s'abonner aux events
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kart|Countdown", meta = (ClampMin = "0"))
    float StartDelay = 1.f;

    // Affiche le compte à rebours, le chrono et les tours en texte de debug (à décocher quand les widgets seront prêts)
    UPROPERTY(EditAnywhere, Category = "Kart|Countdown|Debug")
    bool bShowDebugText = true;

    UPROPERTY(EditAnywhere, Category = "Kart|Countdown|Debug")
    float DebugTextScale = 6.f;

    // Temps pendant lequel "GO!" reste affiché
    UPROPERTY(EditAnywhere, Category = "Kart|Countdown|Debug")
    float GoMessageDuration = 1.f;

    // Taille du texte de debug du chrono et des tours
    UPROPERTY(EditAnywhere, Category = "Kart|Countdown|Debug")
    float RaceDebugTextScale = 2.f;

private:
    // Appelée par le timer : affiche le premier chiffre
    void BeginCountdown();

    // Appelée par le timer toutes les secondes : passe au chiffre suivant, puis au GO
    void CountdownStep();

    void StartRace();

    // Bloque ou débloque tous les karts du niveau
    void SetAllKartsCanMove(bool bCanMove) const;

    // Compte les checkpoints posés dans le niveau (= le plus grand ZoneIndex des ALapZone)
    void CountCheckpoints();

    // Affiche "Tour 1/3   00:12.345" pour le kart du joueur (seulement avec bShowDebugText)
    void ShowRaceDebugText() const;

    // "const FString&" = on passe le texte sans le copier (&), et la fonction n'a pas le droit de le modifier (const)
    void ShowDebugText(const FString& Text, const FColor& Color, float Duration) const;

    FTimerHandle CountdownTimer;

    int32 CountdownNumber = 0;

    bool bRaceStarted = false;

    // Moment du GO (en secondes depuis le lancement du niveau) : le chrono = maintenant - RaceStartTime
    float RaceStartTime = 0.f;

    int32 CheckpointCount = 0;
};
 
