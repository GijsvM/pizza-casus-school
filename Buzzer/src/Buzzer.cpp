#include "Buzzer.h"

Buzzer::Buzzer(int buzzerPin){
    pin = buzzerPin;
}


void Buzzer::buzzInterval(int freq1, int freq2) {
    unsigned char i;

    while(1){
        for(i=0;i<freq1;i++){
            digitalWrite(pin,HIGH);
            delay(1);
            digitalWrite(pin,LOW);
            delay(1);
        }

        for(i=0;i<freq2;i++){
            digitalWrite(pin,HIGH);
            delay(2);
            digitalWrite(pin,LOW);
            delay(2);
        }
    }
}