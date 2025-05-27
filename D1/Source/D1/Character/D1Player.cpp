// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/D1Player.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameData/D1CharacterStat.h"
#include "CharacterStat/D1CharacterStatComponent.h"
#include "UI/D1PlayerHUDWidget.h"
#include "GameFramework/GameModeBase.h"
#include  "Interface/D1GameInterface.h"

AD1Player::AD1Player()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 700.0f;
	SpringArm->SetRelativeRotation(FRotator(-30.0f , 0.0f , 0.0f));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	static ConstructorHelpers::FClassFinder<UD1PlayerHUDWidget> PlayerHUDWidgetRef(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_PlayerHUD.WBP_PlayerHUD_C'"));
	if (PlayerHUDWidgetRef.Succeeded())
	{
		PlayerHUDWidgetClass = PlayerHUDWidgetRef.Class;
	}


#pragma region InputSystem
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCDefaultRef(
		TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));
	if (IMCDefaultRef.Succeeded())
	{
		InputMappingContext = IMCDefaultRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Attack.IA_Attack'"));
	if (AttackActionRef.Succeeded())
	{
		AttackAction = AttackActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Move.IA_Move'"));
	if (MoveActionRef.Succeeded())
	{
		MoveAction = MoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TurnActionRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Turn.IA_Turn'"));
	if (TurnActionRef.Succeeded())
	{
		TurnAction = TurnActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Jump.IA_Jump'"));
	if (JumpActionRef.Succeeded())
	{
		JumpAction = JumpActionRef.Object;
	}
#pragma endregion
}

void AD1Player::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputMappingContext , 0);
		}

		EnableInput(PlayerController);
	}

	PlayerHUDWidget = CreateWidget<UD1PlayerHUDWidget>(PlayerController , PlayerHUDWidgetClass);
	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->AddToViewport();
	}

	if (PlayerHUDWidget && StatComponent)
	{
		PlayerHUDWidget->UpdateStat(StatComponent->GetBaseStat() , StatComponent->GetModifierStat());
		PlayerHUDWidget->UpdateHp(StatComponent->GetCurrentHp());

		StatComponent->OnHpChanged.AddUObject(PlayerHUDWidget , &UD1PlayerHUDWidget::UpdateHp);
		StatComponent->OnStatChanged.AddUObject(PlayerHUDWidget , &UD1PlayerHUDWidget::UpdateStat);
	}
}

void AD1Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AD1Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(AttackAction , ETriggerEvent::Triggered , this , &AD1Player::Input_Attack);
		EnhancedInputComponent->BindAction(MoveAction , ETriggerEvent::Triggered , this , &AD1Player::Input_Move);
		EnhancedInputComponent->BindAction(TurnAction , ETriggerEvent::Triggered , this , &AD1Player::Input_Turn);
		EnhancedInputComponent->BindAction(JumpAction , ETriggerEvent::Started , this , &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction , ETriggerEvent::Completed , this , &ACharacter::StopJumping);
	}
}

void AD1Player::SetDead()
{
	Super::SetDead();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	DisableInput(PlayerController);

	ID1GameInterface* D1GameMode = Cast<ID1GameInterface>(GetWorld()->GetAuthGameMode());
	if (D1GameMode)
	{
		D1GameMode->OnPlayerDead();
	}
}

void AD1Player::Input_Attack(const FInputActionValue& InputValue)
{
	ProcessComboAttack();
}

void AD1Player::Input_Move(const FInputActionValue& InputValue)
{
	FVector2D MovementVector = InputValue.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.0f , Rotation.Yaw , 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection , MovementVector.X);
	AddMovementInput(RightDirection , MovementVector.Y);
}

void AD1Player::Input_Turn(const FInputActionValue& InputValue)
{
	float XValue = InputValue.Get<float>();
	AddControllerYawInput(XValue);
}
