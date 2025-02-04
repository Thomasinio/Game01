// // © 2024 Igor Vladymyrov. All Rights Reserved.


#include "Game01GameModeBase.h"
#include "Characters/Game01BaseCharacter.h"
#include "Characters/Game01PlayerController.h"
#include "UI/Game01HUD.h"


AGame01GameModeBase::AGame01GameModeBase()
{
	DefaultPawnClass = AGame01BaseCharacter::StaticClass();
	PlayerControllerClass = AGame01PlayerController::StaticClass();
	HUDClass = AGame01HUD::StaticClass();
}
