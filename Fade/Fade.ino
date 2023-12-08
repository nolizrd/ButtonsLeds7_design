#define BUTTON_PIN 2
#define LED_PIN 9
#define SEGMENT_DISPLAY_PIN 10
#define PRESSED HIGH
#define BOUNCE_TIME 50
#define latchPin 5
#define clockPin 3
#define dataPin 7

int press_count = 0;
int previous = 0;
long int hold_time = 0;
long int press_time = 0;
bool pressed_candidate = false;
bool button_pressed = false;
int ledBrightness = 0;
int ledIncrement = 5;
bool ledState = LOW;

byte digits[10] = {
  B01111110,  // 0
  B00110000,  // 1
  B01101101,  // 2
  B01111001,  // 3
  B00110011,  // 4
  B01011011,  // 5
  B01011111,  // 6
  B01110000,  // 7
  B01111111,  // 8
  B01111011   // 9
};

unsigned long previousUpdateTime = 0;
unsigned long updateInterval = 10;  // Интервал обновления в миллисекундах

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SEGMENT_DISPLAY_PIN, OUTPUT);
  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButtonInterrupt, FALLING);
}

void loop() {
  int val = digitalRead(BUTTON_PIN);
  unsigned long currentTime = millis();

  if (pressed_candidate == false && val == PRESSED && previous != PRESSED) {
    press_count++;
    Serial.print("Button is pressed ");
    Serial.println(press_count);
    hold_time = 0;
    press_time = currentTime;
    pressed_candidate = true;
  }

  if (pressed_candidate == true && currentTime - press_time > BOUNCE_TIME) {
    if (digitalRead(BUTTON_PIN) == PRESSED) {
      button_pressed = true;
    } else {
      pressed_candidate = false;
    }
  }

  if (button_pressed == true) {
    hold_time = currentTime - press_time;
    Serial.print("Button is hold ");
    Serial.println(hold_time);

    if (ledState == LOW) {
      // Включаем светодиод с минимальной яркостью
      ledState = HIGH;
      analogWrite(LED_PIN, ledBrightness);
    }

    // Регулировка яркости при удержании
    if (ledBrightness < 255) {
      ledBrightness += ledIncrement;
    } else {
      ledBrightness = 0;
    }
  }

  if (button_pressed == true && previous == PRESSED && val != PRESSED) {
    Serial.print("Button is released ");
    button_pressed = false;
    pressed_candidate = false;

    // Выключаем светодиод
    analogWrite(LED_PIN, 0);
    ledState = LOW;
  }

  if (currentTime - previousUpdateTime >= updateInterval) {
    // Обновляем каждые 10 миллисекунд
    previousUpdateTime = currentTime;

    // Отображение времени удержания на семисегментном индикаторе
    if (button_pressed) {
      showNumber(min(9, hold_time / 3000));
      digitalWrite(SEGMENT_DISPLAY_PIN, HIGH);
    } else {
      digitalWrite(SEGMENT_DISPLAY_PIN, LOW);
    }
  }

  previous = val;
}

void showNumber(int digit) {
  digitalWrite(latchPin, LOW);
  if (digit < 0 || digit > 9) {
    return;
  }
  shiftAndSet(digits[digit]);
  digitalWrite(latchPin, HIGH);
}

void shiftAndSet(byte val) {
  shiftOut(dataPin, clockPin, MSBFIRST, val);
}

void handleButtonInterrupt() {
  if (pressed_candidate == false) {
    press_time = millis();
    pressed_candidate = true;
    press_count++;
    hold_time = 0;
  }
}
