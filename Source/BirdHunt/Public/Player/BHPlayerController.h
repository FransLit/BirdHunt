#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "BHPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class BIRDHUNT_API ABHPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void Navigate(const FInputActionValue& Value);
	void Confirm();
	void Back();

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* UIInputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* NavigateAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ConfirmAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* BackAction;
};
