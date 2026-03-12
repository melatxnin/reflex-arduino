/*
  REFLEX GAME

  Controls:
  Press the button matching the displayed colour:
  Red button     ->  red
  Green button   ->  green
  Blue button    ->  blue
  Yellow button  ->  yellow

  Author:
  Nicolas GOSSARD
*/

#include <LiquidCrystal.h>

// pins
constexpr int redButtonPin = 5;
constexpr int greenButtonPin = 4;
constexpr int blueButtonPin = 3;
constexpr int yellowButtonPin = 2;
constexpr int redLedPin = A3;
constexpr int greenLedPin = A4;
constexpr int blueLedPin = A5;

// settings
constexpr int resultDelay = 2500;

// game states
enum GameState
{
  WAITING_START,
  WAITING_INPUT,
  SHOW_RESULT
};

GameState gameState = WAITING_START;

// state variables
int targetColour = 0;
unsigned long waitDuration = 0;
unsigned long stateStartTime = 0;
unsigned long reactionStartTime = 0;
unsigned long reactionTime = 0;
unsigned long bestTime = 0;

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

void setup()
{
  randomSeed(analogRead(A0));

  pinMode(redButtonPin, INPUT_PULLUP);
  pinMode(greenButtonPin, INPUT_PULLUP);
  pinMode(blueButtonPin, INPUT_PULLUP);
  pinMode(yellowButtonPin, INPUT_PULLUP);

  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);

  lcd.begin(16, 2);

  startNewRound();
}

void loop()
{
  unsigned long currentTime = millis();

  switch (gameState)
  {
    case WAITING_START:
    {
      if (currentTime - stateStartTime >= waitDuration)
      {
        showTargetColour(targetColour);
        reactionStartTime = millis();
        gameState = WAITING_INPUT;

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("GO!");
      }
      break;
    }

    case WAITING_INPUT:
    {
      int pressedButton = readPressedButton();

      if (pressedButton == -1)
      {
        break;
      }

      if (pressedButton == targetColour)
      {
        reactionTime = millis() - reactionStartTime;
        showSuccessResult();
      }
      else
      {
        showErrorResult();
      }

      break;
    }

    case SHOW_RESULT:
    {
      if (currentTime - stateStartTime >= resultDelay)
      {
        startNewRound();
      }
      break;
    }
  }
}

void setColour(bool redOn, bool greenOn, bool blueOn)
{
  digitalWrite(redLedPin, redOn ? HIGH : LOW);
  digitalWrite(greenLedPin, greenOn ? HIGH : LOW);
  digitalWrite(blueLedPin, blueOn ? HIGH : LOW);
}

void showTargetColour(int colour)
{
  switch (colour)
  {
    case 0:
      setColour(true, false, false);
      break;

    case 1:
      setColour(false, true, false);
      break;

    case 2:
      setColour(false, false, true);
      break;

    case 3:
      setColour(true, true, false);
      break;
  }
}

int readPressedButton()
{
  if (digitalRead(redButtonPin) == LOW)
  {
    return 0;
  }

  if (digitalRead(greenButtonPin) == LOW)
  {
    return 1;
  }

  if (digitalRead(blueButtonPin) == LOW)
  {
    return 2;
  }

  if (digitalRead(yellowButtonPin) == LOW)
  {
    return 3;
  }

  return -1;
}

void startNewRound()
{
  setColour(false, false, false);

  waitDuration = random(2000, 5001);

  targetColour = random(0, 4);

  stateStartTime = millis();

  gameState = WAITING_START;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Wait...");
}

void showErrorResult()
{
  stateStartTime = millis();
  gameState = SHOW_RESULT;

  setColour(false, false, false);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Wrong button");
}

void showSuccessResult()
{
  stateStartTime = millis();
  gameState = SHOW_RESULT;

  setColour(false, false, false);

  if (bestTime == 0 || reactionTime < bestTime)
  {
    bestTime = reactionTime;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time:");
  lcd.print(reactionTime);
  lcd.print("ms");

  lcd.setCursor(0, 1);
  lcd.print("Record:");
  lcd.print(bestTime);
  lcd.print("ms");
}