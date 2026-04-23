// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"
#include "Blueprint/UserWidget.h"

void APlayerCharacterController::BeginPlay()
{
	if (InventoryWidgetClass) {
		InventoryWidget = CreateWidget<UUserWidget>(this, InventoryWidgetClass);
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		InventoryWidget->AddToViewport();
	}
}

void APlayerCharacterController::ShowInventory()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, FString::Printf(TEXT("Controller ShowInventory")));
	//if (bIsInventoryOpen) {
	//	//close
	//	bShowMouseCursor = false;
	//	SetInputMode(FInputModeGameOnly());
	//	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	//}
	//else {
	//	//open
	//	bShowMouseCursor = true;
	//	SetInputMode(FInputModeGameAndUI());
	//	InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	//}
	bIsInventoryOpen = !bIsInventoryOpen;
}
