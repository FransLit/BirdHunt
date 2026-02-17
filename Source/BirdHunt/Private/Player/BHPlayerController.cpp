#include "Player/BHPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

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
}

void ABHPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (NavigateAction)
			EnhancedInput->BindAction(NavigateAction, ETriggerEvent::Triggered, this, &ABHPlayerController::Navigate);

		if (ConfirmAction)
			EnhancedInput->BindAction(ConfirmAction, ETriggerEvent::Started, this, &ABHPlayerController::Confirm);

		if (BackAction)
			EnhancedInput->BindAction(BackAction, ETriggerEvent::Started, this, &ABHPlayerController::Back);
	}
}

void ABHPlayerController::Navigate(const FInputActionValue& Value)
{
	FVector2D NavVector = Value.Get<FVector2D>();
	UE_LOG(LogTemp, Warning, TEXT("UI Navigate: %s"), *NavVector.ToString());
}

void ABHPlayerController::Confirm()
{
	UE_LOG(LogTemp, Warning, TEXT("UI Confirm Pressed"));
}

void ABHPlayerController::Back()
{
	UE_LOG(LogTemp, Warning, TEXT("UI Back Pressed"));
}
