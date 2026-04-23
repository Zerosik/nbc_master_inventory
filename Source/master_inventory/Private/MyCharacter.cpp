// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCharacterController.h"
#include "InventoryWidget.h"
#include "ItemDatabaseComponent.h"
#include "AchievementComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	//스프링암을 루트컴포넌트에 부착하고
	SpringArmComp->SetupAttachment(RootComponent);
	//캐릭터와 거리를 300
	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//카메라를 스프링암에 부착, 다만 소캣에 부착하는데 이는 스프링암에 끝부분에 부착하는걸 의미함.
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;



	ItemDatabaseComp = CreateDefaultSubobject<UItemDatabaseComponent>(TEXT("ItemDatabaseComponent"));
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	AchievementComponent = CreateDefaultSubobject<UAchievementComponent>(TEXT("AchievementComponent"));
}
bool AMyCharacter::AddItemToInventory(int32 ItemID, int32 AMount)
{
	InventoryComp->AddItem(ItemID, AMount);
	AchievementComponent->OnItemAcquired(ItemID);
	return true;
}
bool AMyCharacter::RemoveItemFromInventory(int32 ItemID)
{

	return true;
}
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (OpenInventoryAction) {
			EnhancedInput->BindAction(OpenInventoryAction, ETriggerEvent::Triggered, this, &AMyCharacter::OpenInventory);
		}
		if (LookAction) {
			EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
		}
		if (MoveAction) {
			EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		}
		if (UseAllItemAction) {
			EnhancedInput->BindAction(UseAllItemAction, ETriggerEvent::Triggered, this, &AMyCharacter::UseAllItem);
		}
	}
}

void AMyCharacter::Look(const FInputActionValue& value)
{
	const FVector2d lookInput = value.Get<FVector2d>();
	AddControllerYawInput(lookInput.X);
	AddControllerPitchInput(-lookInput.Y);
}

void AMyCharacter::OpenInventory(const FInputActionValue& value)
{
	InventoryComp->PrintInventory();
	//APlayerCharacterController* controller = Cast< APlayerCharacterController>(GetController());
	//controller->ShowInventory();
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (bIsInventoryOpen) {
		//close
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		//open
		PC->bShowMouseCursor = true;
		PC->SetInputMode(FInputModeGameAndUI());
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	}
	bIsInventoryOpen = !bIsInventoryOpen;
}

void AMyCharacter::Move(const FInputActionValue& value)
{
	if (!Controller)return;
	const FVector2D moveInput = value.Get<FVector2D>();
	if (!FMath::IsNearlyZero(moveInput.X)) {
		AddMovementInput(GetActorForwardVector(), moveInput.X);
	}
	if (!FMath::IsNearlyZero(moveInput.Y)) {
		AddMovementInput(GetActorRightVector(), moveInput.Y);
	}
}

void AMyCharacter::UseAllItem(const FInputActionValue& value)
{
	InventoryComp->UseAllItem();
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (DefaultInputMappingContext)
				{
					Subsystem->AddMappingContext(DefaultInputMappingContext, 0);
				}
			}
		}
	}

	if (InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
		if (InventoryWidget)
		{
			InventoryWidget->AddToViewport();
			InventoryWidget->InitializeInventoryWidget(InventoryComp);
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

