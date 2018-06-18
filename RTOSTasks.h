// RTOS Tasks for various modes and periphials



String currentModeToString(int myCurrentMode)
{
  switch (myCurrentMode)
  {


    case BC24VARIETYOFDISPLAY:
      return "Display Demo";
      break;
    case BC24DOWJONES:
      return "Dow Jones";
      break;
    case BC24CIRCLEDISPLAY:
      return "Circle LED";
      break;
    case BC24CLOCK:
      return "Clock";
      break;


    case BC24DISPLAYCURRENTMODE:
      return "Display CM";
      break;

    default:
      return "Undefined Mode";
      break;

  }

}

//
//
// Debounced button task
//
//


#define DEBOUNCETIME 30

volatile int numberOfButtonInterrupts = 0;
volatile bool lastState;
volatile uint32_t debounceTimeout = 0;

// For setting up critical sections (enableinterrupts and disableinterrupts not available)
// used to disable and interrupt interrupts

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;


// Interrupt Service Routine - Keep it short!
void IRAM_ATTR handleButtonInterrupt() {
  portENTER_CRITICAL_ISR(&mux);
  numberOfButtonInterrupts++;
  lastState = digitalRead(BUTTONPIN);
  debounceTimeout = xTaskGetTickCount(); //version of millis() that works from interrupt
  portEXIT_CRITICAL_ISR(&mux);
}

//
// RTOS Task for reading button pushes (debounced)
//

void taskButtonRead( void * parameter)
{
  String taskMessage = "Debounced ButtonRead Task running on core ";
  taskMessage = taskMessage + xPortGetCoreID();
  Serial.println(taskMessage);

  // set up button Pin
  pinMode (BUTTONPIN, INPUT);
  pinMode(BUTTONPIN, INPUT_PULLUP);  // Pull up to 3.3V on input - some buttons already have this done

  attachInterrupt(digitalPinToInterrupt(BUTTONPIN), handleButtonInterrupt, FALLING);

  uint32_t saveDebounceTimeout;
  bool saveLastState;
  int save;

  // Enter RTOS Task Loop
  for (;;)  {

    portENTER_CRITICAL_ISR(&mux); // so that value of numberOfButtonInterrupts,l astState are atomic - Critical Section
    save  = numberOfButtonInterrupts;
    saveDebounceTimeout = debounceTimeout;
    saveLastState  = lastState;
    portEXIT_CRITICAL_ISR(&mux); // end of Critical Section

    bool currentState = digitalRead(BUTTONPIN);

    // This is the critical IF statement
    // if Interrupt Has triggered AND Button Pin is in same state AND the debounce time has expired THEN you have the button push!
    //
    if ((save != 0) //interrupt has triggered
        && (currentState == saveLastState) // pin is still in the same state as when intr triggered
        && (millis() - saveDebounceTimeout > DEBOUNCETIME ))
    { // and it has been low for at least DEBOUNCETIME, then valid keypress

#ifdef BC24DEBUG
      if (currentState == LOW)
      {
        Serial.printf("Button is pressed and debounced, current tick = % d\n", millis());
      }
      else
      {
        Serial.printf("Button is released and debounced, current tick = % d\n", millis());
      }


      Serial.printf("Button Interrupt Triggered % d times, current State = % u, time since last trigger % dms\n", save, currentState, millis() - saveDebounceTimeout);
#endif

      portENTER_CRITICAL_ISR(&mux); // can't change it unless, atomic - Critical section
      numberOfButtonInterrupts = 0; // acknowledge keypress and reset interrupt counter
      portEXIT_CRITICAL_ISR(&mux);

      currentMode = (currentMode + 1) % BC24NUMBEROFMODES;
      writePreferences();


      //sendCurrentMode();

      Serial.print("currentMode =");
      Serial.println(currentModeToString(currentMode));

      vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}


void taskSingleBlink( void * parameter)
{
  // Enter RTOS Task Loop
  for (;;)  {

    if (uxSemaphoreGetCount( xSemaphoreSingleBlink ) > 0)
    {

      BC24OneBlink(Blue, 300);
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);

  }

}

