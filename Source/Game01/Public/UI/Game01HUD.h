// // © 2024 Igor Vladymyrov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Game01HUD.generated.h"

/**
 * 
 */
UCLASS()
class GAME01_API AGame01HUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void DrawHUD() override;

private:
	void DrawCrosshair();
};
