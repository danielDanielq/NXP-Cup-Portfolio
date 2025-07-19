#include <Arduino.h>
#include <LiquidCrystal_I2C.h>


#define ENABLE_SETTINGS_MENU 1


static float lane_width_vector_unit_real = 60.0f;
static float lookahead_min_distance_cm = 16.0f;
static float lookahead_max_distance_cm = 30.0f;
static float emergency_break_distance_cm = 60.0f;
static float min_speed = 96.0f;
static float max_speed = 112.0f;
static float black_color_treshold = 0.2f; // 0=black, 1=white
static float car_length_cm = 17.5f;

#define LANE_WIDTH_VECTOR_UNIT_REAL lane_width_vector_unit_real
#define LOOKAHEAD_MIN_DISTANCE_CM lookahead_min_distance_cm
#define LOOKAHEAD_MAX_DISTANCE_CM lookahead_max_distance_cm
#define CAR_LENGTH_CM car_length_cm
#define BLACK_COLOR_TRESHOLD black_color_treshold // 0=black, 1=white
#define EMERGENCY_BREAK_DISTANCE_CM emergency_break_distance_cm
#define MIN_SPEED min_speed
#define MAX_SPEED max_speed

#define MENU_RIGHT_ARROW_BUTTON_PIN 14
#define MENU_LEFT_ARROW_BUTTON_PIN 15
#define MENU_DECREMENT_BUTTON_PIN 16
#define MENU_INCREMENT_BUTTON_PIN 17

LiquidCrystal_I2C lcd(0x27, 16, 2);




#if ENABLE_SETTINGS_MENU == 1
void settingsMenuRoutine(LiquidCrystal_I2C &lcd_, int left_arrow_btn, int right_arrow_btn, int increment_btn, int decrement_btn) {
  enum LcdMenu {LCDMENU_FIRST_VALUE,
                LCDMENU_MIN_SPEED,
                LCDMENU_MAX_SPEED,
                LCDMENU_LOOKAHEAD_MIN_DISTANCE_CM,
                LCDMENU_LOOKAHEAD_MAX_DISTANCE_CM,
                LCDMENU_EMERGENCY_BREAK_DISTANCE_CM,
                LCDMENU_LANE_WIDTH_VECTOR_UNIT_REAL,
                LCDMENU_BLACK_COLOR_TRESHOLD,
                LCDMENU_LAST_VALUE};
  
  static int lcdMenuIndex = ((int)LCDMENU_FIRST_VALUE) + 1;
  static int leftArrowButtonState=LOW;
  static int rightArrowButtonState=LOW;
  int incrementButton=LOW;
  int decrementButton=LOW;
  int leftArrowButtonPrevState, rightArrowButtonPrevState;

  leftArrowButtonPrevState = leftArrowButtonState;
  rightArrowButtonPrevState = rightArrowButtonState;

  leftArrowButtonState = digitalRead(left_arrow_btn);
  rightArrowButtonState = digitalRead(right_arrow_btn);
  incrementButton = digitalRead(increment_btn);
  decrementButton = digitalRead(decrement_btn);

  if (leftArrowButtonPrevState == HIGH && leftArrowButtonState == HIGH) {
    return;
  }

  if (rightArrowButtonPrevState == HIGH && rightArrowButtonState == HIGH) {
    return;
  }


  if (rightArrowButtonState == HIGH && lcdMenuIndex >= ((int)LCDMENU_LAST_VALUE) - 1) {
    lcdMenuIndex = ((int)LCDMENU_FIRST_VALUE) + 1;
  } else if (rightArrowButtonState == HIGH) {
    (int)lcdMenuIndex++;
  }

  if (leftArrowButtonState == HIGH && lcdMenuIndex <= ((int)LCDMENU_FIRST_VALUE) + 1) {
    lcdMenuIndex = ((int)LCDMENU_LAST_VALUE) - 1;
  } else if (leftArrowButtonState == HIGH) {
    (int)lcdMenuIndex--;
  }

  if (leftArrowButtonState == HIGH || rightArrowButtonState == HIGH || incrementButton == HIGH || decrementButton == HIGH) {
    lcd_.clear();
    delay(100);
    switch (lcdMenuIndex) {
      case LCDMENU_MIN_SPEED:
        if (incrementButton == HIGH) {
          MIN_SPEED += 0.5f;
        } else if (decrementButton == HIGH) {
          MIN_SPEED -= 0.5f;
        }
        lcd_.setCursor(0, 0);
        lcd_.print("MIN_SPEED: ");
        lcd_.setCursor(0, 1);
        lcd_.print(MIN_SPEED);
        break;

      case LCDMENU_MAX_SPEED:
        if (incrementButton == HIGH) {
          MAX_SPEED += 0.5f;
        } else if (decrementButton == HIGH) {
          MAX_SPEED -= 0.5f;
        }
        lcd_.setCursor(0, 0);
        lcd_.print("MAX_SPEED: ");
        lcd_.setCursor(0, 1);
        lcd_.print(MAX_SPEED);
        break;

      case LCDMENU_LOOKAHEAD_MIN_DISTANCE_CM:
        if (incrementButton == HIGH) {
          LOOKAHEAD_MIN_DISTANCE_CM += 0.5f;
        } else if (decrementButton == HIGH) {
          LOOKAHEAD_MIN_DISTANCE_CM -= 0.5f;
        }
        lcd_.setCursor(0, 0);
        lcd_.print("LOOKAHEAD_MIN: ");
        lcd_.setCursor(0, 1);
        lcd_.print(LOOKAHEAD_MIN_DISTANCE_CM);
        break;

      case LCDMENU_LOOKAHEAD_MAX_DISTANCE_CM:
        if (incrementButton == HIGH) {
          LOOKAHEAD_MAX_DISTANCE_CM += 0.5f;
        } else if (decrementButton == HIGH) {
          LOOKAHEAD_MAX_DISTANCE_CM -= 0.5f;
        }
        lcd_.setCursor(0, 0);
        lcd_.print("LOOKAHEAD_MAX: ");
        lcd_.setCursor(0, 1);
        lcd_.print(LOOKAHEAD_MAX_DISTANCE_CM);
        break;
      
      case LCDMENU_EMERGENCY_BREAK_DISTANCE_CM:
        if (incrementButton == HIGH) {
          EMERGENCY_BREAK_DISTANCE_CM += 0.5f;
        } else if (decrementButton == HIGH) {
          EMERGENCY_BREAK_DISTANCE_CM -= 0.5f;
        }
        lcd_.setCursor(0, 0);
        lcd_.print("EMERGENCY_BREAK_DISTANCE_CM: ");
        lcd_.setCursor(0, 1);
        lcd_.print(EMERGENCY_BREAK_DISTANCE_CM);
        break;
      
      case LCDMENU_LANE_WIDTH_VECTOR_UNIT_REAL:
        if (incrementButton == HIGH) {
          LANE_WIDTH_VECTOR_UNIT_REAL += 0.5f;
        } else if (decrementButton == HIGH) {
          LANE_WIDTH_VECTOR_UNIT_REAL -= 0.5f;
        }
        lcd_.setCursor(0, 0);
        lcd_.print("LANE_WIDTH_VECTOR_UNIT_REAL: ");
        lcd_.setCursor(0, 1);
        lcd_.print(LANE_WIDTH_VECTOR_UNIT_REAL);
        break;
      
      case LCDMENU_BLACK_COLOR_TRESHOLD:
        if (incrementButton == HIGH) {
          BLACK_COLOR_TRESHOLD += 0.01f;
        } else if (decrementButton == HIGH) {
          BLACK_COLOR_TRESHOLD -= 0.01f;
        }
        lcd_.setCursor(0, 0);
        lcd_.print("BLACK_COLOR_TRESHOLD: ");
        lcd_.setCursor(0, 1);
        lcd_.print(BLACK_COLOR_TRESHOLD);
        break;
    }
  }
}
#endif


void setup() {
  Serial.begin(9600);

  #if ENABLE_SETTINGS_MENU == 1
    lcd.init();  //display initialization
    lcd.backlight();  // activate the backlight
    pinMode(MENU_LEFT_ARROW_BUTTON_PIN, INPUT);
    pinMode(MENU_RIGHT_ARROW_BUTTON_PIN, INPUT);
    pinMode(MENU_INCREMENT_BUTTON_PIN, INPUT);
    pinMode(MENU_DECREMENT_BUTTON_PIN, INPUT);
  #endif

  // initialize the pushbutton pin as an input:
}
void loop() {
    #if ENABLE_SETTINGS_MENU == 1
      settingsMenuRoutine(lcd, MENU_LEFT_ARROW_BUTTON_PIN, MENU_RIGHT_ARROW_BUTTON_PIN, MENU_INCREMENT_BUTTON_PIN, MENU_DECREMENT_BUTTON_PIN);
    #endif
}