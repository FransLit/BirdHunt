#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "BHPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UPlayerHUD;

UCLASS()
class BIRDHUNT_API ABHPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void ShowStats(const FInputActionInstance& Instance);
	UPROPERTY()
	UPlayerHUD* HUDWidget;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* UIInputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UPlayerHUD> HUDWidgetClass;


};
