#ifndef HEADER_FILE
#define HEADER_FILE

//definition
#define BUTTON_DELAY     20 // at 1ms interrupt service rate, 20 results in 20ms (50hz)
#define BUTTON_FILTER    5  // "filter" constant
#define BUTTON_THRESHOLD 80 // capacitive touch threshold
#define LOOP_DELAY       30 // at 1ms interrupt service rate, 30 results in 30ms (33.333hz)

//button value (1 or 0)
int nButton1 = 0; // left
int nButton2 = 0; // right
int nButton3 = 0; // cross
int nButton4 = 0; // check

// timer interrupt
hw_timer_t * timerInterruptService = NULL; // interrupt service timer hook
volatile SemaphoreHandle_t timerLoopSemaphore; // main loop semaphore from interrupt service
unsigned long nMilliseconds = 0; // milliseconds timer

enum button_t{
  NOTHING,
  LEFT,
  RIGHT,
  CROSS,
  CHECK,
  SETTING,
  MANUAL
};

void IRAM_ATTR InterruptService();
button_t WhichButtonTouched();
void touch_setup();

#endif
