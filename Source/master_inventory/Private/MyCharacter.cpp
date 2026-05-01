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
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

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
		if (FireAction) {
			EnhancedInput->BindAction(FireAction, ETriggerEvent::Triggered, this, &AMyCharacter::FireShotgun);
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

void AMyCharacter::FireShotgun(const FInputActionValue& value)
{
	/*
	float ShotgunMinSpread = 5.f;
	float ShotgunMaxSpread = 30.f;
	float ShotgunRange = 2000.f;
	int32 SHotgunPelletCount = 8;
	*/
	
	FVector Start = GetActorLocation() + FVector{ 0, 0, 80 };//총구지점같은게 있으면 좋을듯
	FVector ForwardVector = GetActorForwardVector();//조준방향
	ForwardVector = (GetCameraLookAt() - GetActorLocation()).GetSafeNormal();;//카메라가 바라보는 지점 방향
	float Range = ShotgunRange; // 샷건 사거리
	int32 PelletCount = SHotgunPelletCount; // 샷건 산탄 수
	float SpreadRadius = ShotgunCurrentSpread; // 산탄 퍼짐 정도
	ShotgunCurrentSpread = FMath::Clamp(ShotgunCurrentSpread + 10.f, ShotgunMinSpread, ShotgunMaxSpread);//산탄도 증가(다음 사격에 적용
	//자신 제외
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	for (int32 i = 0; i < PelletCount; ++i)
	{
		// 최대 SpreadRadius각도로 ForwardVector를 회전한 값 생성(산탄 퍼짐)
		float RandomYaw = FMath::FRandRange(-SpreadRadius, SpreadRadius);
		float RandomPitch = FMath::FRandRange(-SpreadRadius, SpreadRadius);

		FRotator RandomYawPitch = FRotator(RandomPitch, RandomYaw, 0);
		FVector PelletDirection = RandomYawPitch.RotateVector(ForwardVector);

		// 최종 사격지점
		FVector End = Start + (PelletDirection * Range);

		FHitResult HitResult;
		TArray<FHitResult> HitResults;
		UKismetSystemLibrary::LineTraceMulti(
			GetWorld(),
			Start,
			End,
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			HitResults,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			1.f
		);
	}
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
	ShotgunCurrentSpread = FMath::Clamp(ShotgunCurrentSpread - (recoilRecoverySpeed * DeltaTime), ShotgunMinSpread, ShotgunMaxSpread);

	GEngine->AddOnScreenDebugMessage(0, 1, FColor::White, FString::Printf(TEXT("Current Recoil Angle : %02f"), ShotgunCurrentSpread));
}

FVector AMyCharacter::GetCameraLookAt()
{
	APlayerCameraManager* CamManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	FHitResult HitResult;
	float TraceDistance = 10000.f;
	FVector Start = CamManager->GetCameraLocation();
	FVector End = Start + (CamManager->GetCameraRotation().Vector() * TraceDistance); // 최대 거리

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // 나(캐릭터)는 제외

	FVector ActualLookAtPoint;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		// 충돌한 지점 (카메라가 정확히 조준하고 있는 실제 위치)
		ActualLookAtPoint = HitResult.ImpactPoint;
	}
	else
	{
		// 충돌하지 않았으면 최대 거리 지점 반환
		ActualLookAtPoint = End;
	}
	return ActualLookAtPoint;
}


