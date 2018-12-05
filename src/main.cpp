#include <Arduino.h>
#include <stdio.h>

// 20 Hours
#define UNLOCK_TIME 72000

#define PAD_POWER_PIN 2
#define BUTTON_UNLOCK_PIN 3
#define BUTTON_LOCK_PIN 4

#define SOLENOID_PIN 5
#define GREEN_LIGHT_PIN 6
#define RED_LIGHT_PIN 7

bool locked;
bool lockButtonWasPressed;
bool unlockWasButtonPressed;
bool canUnlock;
long elapsedSeconds;
long elapsedMillis;
long startMillis; 
long previousMillis;
long greenLightTimer;
bool greenLightOn;

void setup(){
  pinMode(PAD_POWER_PIN, OUTPUT);
  pinMode(BUTTON_UNLOCK_PIN, INPUT);
  pinMode(BUTTON_LOCK_PIN, INPUT);
  pinMode(SOLENOID_PIN, OUTPUT);
  pinMode(RED_LIGHT_PIN, OUTPUT);
  pinMode(GREEN_LIGHT_PIN, OUTPUT);

  lockButtonWasPressed = false;
  unlockWasButtonPressed = false;
  locked = true;
  canUnlock = false;
  elapsedSeconds = 0;
  elapsedMillis  = 0;
  previousMillis = 0;
  greenLightTimer = 0;
  greenLightOn = false;
}

void loop(){

  long now = millis();
  elapsedMillis += now - previousMillis;
  previousMillis = now;

  if(elapsedMillis >= 1000){
    elapsedSeconds += 1;
    elapsedMillis  = 0;
  }

  if(elapsedSeconds >= UNLOCK_TIME){
    elapsedSeconds = 0;
    canUnlock = true;
  }

  digitalWrite(PAD_POWER_PIN, 1);
  digitalWrite(SOLENOID_PIN, !locked);
  digitalWrite(RED_LIGHT_PIN, locked && !canUnlock);
  digitalWrite(GREEN_LIGHT_PIN, canUnlock && locked);
  
  if(!locked){
    greenLightTimer += millis() - previousMillis;
    if(greenLightTimer >= 200){
      greenLightTimer = 0;
      greenLightOn = !greenLightOn;
    }
    digitalWrite(GREEN_LIGHT_PIN, greenLightOn);
  }

  int lockButtonState = digitalRead(BUTTON_LOCK_PIN);
  int unlockButtonState = digitalRead(BUTTON_UNLOCK_PIN);

  if(!unlockButtonState && unlockWasButtonPressed && canUnlock){
    locked = false;
    canUnlock = false;
  }else if(!lockButtonState && lockButtonWasPressed){
    locked = true;
    elapsedSeconds = 0;
    canUnlock = false;
  }

  lockButtonWasPressed = lockButtonState;
  unlockWasButtonPressed = unlockButtonState;
}
