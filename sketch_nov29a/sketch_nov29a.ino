#define BUTTON_PIN 2
#define LED_PIN 13
#define PRESSED HIGH
#define BOUNCE_TIME 50

int prev=0;
long int press_time = 0;
int led_state=LOW;
bool pressed_candidate=false;
bool button_pressed=false;
int lastState = HIGH;
void setup(){
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, led_state);
  Serial.begin(9600);
}

void loop (){
  int val = digitalRead(BUTTON_PIN);
  if(pressed_candidate == false and
     val == PRESSED and 
     prev != PRESSED) {
  

    press_time = millis();
    pressed_candidate = true;
  }
  if(pressed_candidate == true and 
     millis() - press_time > BOUNCE_TIME) {
    if(digitalRead(BUTTON_PIN) == PRESSED){
      button_pressed = true;
    } else {
      pressed_candidate = false;
    }
  }
  // released event
  if(button_pressed == true and
     prev == PRESSED and
     val != PRESSED) {
    led_state=!led_state;
    digitalWrite(LED_PIN, led_state);
    button_pressed = false;
    pressed_candidate = false;
  }
  prev = val;
  
  if (lastState != val) {
    lastState = val;
    if (val == HIGH) {
      Serial.println(" released");
    }
    if (val == LOW) {
      Serial.println(" pressed");
    }
}
}

  
