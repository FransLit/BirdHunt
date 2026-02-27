#include "Player/BHPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Player/UI/PlayerHUD.h"
#include "InputAction.h"

void ABHPlayerController::ShowStats(const FInputActionInstance& Instance)
{
	ETriggerEvent EventType = Instance.GetTriggerEvent();
	if (EventType == ETriggerEvent::Started)
	{
		HUDWidget->ShowStats(ESlateVisibility::Visible);
	}
	else if (EventType == ETriggerEvent::Completed)
	{
		HUDWidget->ShowStats(ESlateVisibility::Hidden);
	}
}

void ABHPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//bShowMouseCursor = true;

	//if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	//{
	//	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
	//		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	//	{
	//		if (UIInputMappingContext)
	//		{
	//			Subsystem->AddMappingContext(UIInputMappingContext, 1);
	//		}
	//	}
	//}

	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UPlayerHUD>(this, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
			HUDWidget->ShowStats(ESlateVisibility::Hidden);
		}
	}
}

void ABHPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	//{

	//}
}


