#include <Arduino.h>

#define HEADLIGHTS 5  
#define BRAKELIGHTS 6  
#define RIGHTTURN 7  
#define LEFTTURN 8  

#define BT_HL A0  
#define BT_RT A1  
#define BT_LT A2  
#define BT_BL A3  
#define BT_RL A4  

bool frontState = false;
bool rightState = false;
bool leftState = false;
bool hazardState = false;

unsigned long lastBlinkTime = 0;
bool blinkState = false;

void setup() {
    pinMode(HEADLIGHTS, OUTPUT);
    pinMode(BRAKELIGHTS, OUTPUT);
    pinMode(RIGHTTURN, OUTPUT);
    pinMode(LEFTTURN, OUTPUT);

    pinMode(BT_HL, INPUT_PULLUP);
    pinMode(BT_RT, INPUT_PULLUP);
    pinMode(BT_LT, INPUT_PULLUP);
    pinMode(BT_BL, INPUT_PULLUP);
    pinMode(BT_RL, INPUT_PULLUP);

    digitalWrite(HEADLIGHTS, LOW);
    digitalWrite(BRAKELIGHTS, LOW);
    digitalWrite(RIGHTTURN, LOW);
    digitalWrite(LEFTTURN, LOW);
}

void loop() {
    if (pressed(BT_HL)) {
        hazardState = !hazardState;
        if (!hazardState) {  
            digitalWrite(RIGHTTURN, LOW);
            digitalWrite(LEFTTURN, LOW);
        }
    }

    if (!hazardState) {  
        if (pressed(BT_RT)) {
            rightState = !rightState;
            if (!rightState) digitalWrite(RIGHTTURN, LOW);
        }

        if (pressed(BT_LT)) {
            leftState = !leftState;
            if (!leftState) digitalWrite(LEFTTURN, LOW);
        }
    }

    if (pressed(BT_RL)) {
        frontState = !frontState;
        digitalWrite(HEADLIGHTS, frontState ? HIGH : LOW);
    }

    digitalWrite(BRAKELIGHTS, digitalRead(BT_BL) == LOW ? HIGH : LOW);

    if (hazardState) {
        hazardBlink();
    } else {
        if (rightState) blink(RIGHTTURN);
        if (leftState) blink(LEFTTURN);
    }
}

void blink(int relay) {
    static bool relayState = false;

    if (millis() - lastBlinkTime >= 500) {  
        lastBlinkTime = millis();
        relayState = !relayState;
        digitalWrite(relay, relayState ? HIGH : LOW);
    }
}

void hazardBlink() {
    static bool relayState = false;

    if (millis() - lastBlinkTime >= 500) {
        lastBlinkTime = millis();
        relayState = !relayState;
        digitalWrite(RIGHTTURN, relayState ? HIGH : LOW);
        digitalWrite(LEFTTURN, relayState ? HIGH : LOW);
    }
}

bool pressed(int bt) {
    static bool lastState = HIGH;
    bool currentState = digitalRead(bt);

    if (lastState == HIGH && currentState == LOW) {  
        delay(50);  
        while (digitalRead(bt) == LOW);  
        lastState = currentState;
        return true;
    }
    lastState = currentState;
    return false;
}
