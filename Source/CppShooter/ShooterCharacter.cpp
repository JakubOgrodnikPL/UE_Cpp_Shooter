// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "MyInputConfigData.h"
#include "InputActionValue.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "CppShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

    Health = MaxHealth;

    Gun = GetWorld()->SpawnActor<AGun>(GunClass);
    GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
    Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
    Gun->SetOwner(this);
	
}

bool AShooterCharacter::ISDead() const
{
    return Health <= 0;
}

float AShooterCharacter::GetHealthPercent() const
{
    return Health / MaxHealth;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    // Get the player controller
    APlayerController* PC = Cast<APlayerController>(GetController());
 
    // Get the local player subsystem
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
    // Clear out existing mapping, and add our mapping
    Subsystem->ClearAllMappings();
    Subsystem->AddMappingContext(InputMapping, 0);

    // Get the EnhancedInputComponent
    UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    // Bind the actions
    PEI->BindAction(InputActions->InputMove, ETriggerEvent::Triggered, this, &AShooterCharacter::Move);
    PEI->BindAction(InputActions->InputLook, ETriggerEvent::Triggered, this, &AShooterCharacter::Look);
    PEI->BindAction(InputActions->InputJump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
    PEI->BindAction(InputActions->InputShoot, ETriggerEvent::Triggered, this, &AShooterCharacter::Shoot);

}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
    float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    DamageToApply = FMath::Min(Health, DamageToApply);
    Health -= DamageToApply;

    UE_LOG(LogTemp, Warning, TEXT("Health left %f"), Health);

    if(ISDead())
    {
        ACppShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ACppShooterGameModeBase>();
        if (GameMode != nullptr)
        {
            GameMode->PawnKilled(this);
        }
        DetachFromControllerPendingDestroy();
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);


    }

    return DamageToApply;


}

void AShooterCharacter::Move(const FInputActionValue& Value)
{
    if (Controller != nullptr)
    {
        const FVector2D MoveValue = Value.Get<FVector2D>();
        const FRotator MovementRotation(0, Controller->GetControlRotation().Yaw, 0);
 
        // Forward/Backward direction
        if (MoveValue.Y != 0.f)
        {
            // Get forward vector
            const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);
 
            AddMovementInput(Direction, MoveValue.Y);
        }
 
        // Right/Left direction
        if (MoveValue.X != 0.f)
        {
            // Get right vector
            const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);
 
            AddMovementInput(Direction, MoveValue.X);
        }
    }
}
 
void AShooterCharacter::Look(const FInputActionValue& Value)
{
    if (Controller != nullptr)
    {
        const FVector2D LookValue = Value.Get<FVector2D>();
 
        if (LookValue.X != 0.f)
        {
            AddControllerYawInput(LookValue.X);
        }
 
        if (LookValue.Y != 0.f)
        {
            AddControllerPitchInput(LookValue.Y);
        }
    }
}

void AShooterCharacter::Shoot()
{
    Gun->PullTrigger();
}
