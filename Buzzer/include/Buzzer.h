#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer {
        private:
                int pin;
        public:
                Buzzer(int buzzerPin);
                void buzzInterval(int freq1, int freq2);   //buzzInterval1 en 2 en miss set ervoor voor duidelijkheid
};

#endif
