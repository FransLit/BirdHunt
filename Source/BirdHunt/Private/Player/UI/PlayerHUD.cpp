#include "Player/UI/PlayerHUD.h"
#include "Components/TextBlock.h"


void UPlayerHUD::UpdateScore(int32 NewScore)
{
    if (ScoreText)
    {
        ScoreText->SetText(FText::AsNumber(NewScore));
    }
}

void UPlayerHUD::ShowStats(ESlateVisibility state)
{
    if(ScoreText)
    {
        ScoreText->SetVisibility(state);
    }
}
