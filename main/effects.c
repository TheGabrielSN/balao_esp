#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_system.h"

#include "effects.h"

#define delay(value) vTaskDelay(value / portTICK_PERIOD_MS)

#define LEDC_TIMER         LEDC_TIMER_0
#define LEDC_MODE          LEDC_LOW_SPEED_MODE
#define LEDC_RESOLUTION    LEDC_TIMER_8_BIT
#define LEDC_FREQUENCY     1000

/*****************************************/
//                                       //
//             Configurações             //
//                                       //
/*****************************************/

void configPin(){
    /* Configuração para utilização dos pinos como PWM
    */
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_RESOLUTION,
        .freq_hz = LEDC_FREQUENCY,
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER
    };
    ledc_timer_config(&ledc_timer);

    // Configura para o pino 12
    ledc_channel_config_t ledc_channel_0 = {
        .gpio_num = GPIO_NUM_12,
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER,
        .duty = 0,
        .hpoint = 0,
    };
    ledc_channel_config(&ledc_channel_0);

    // Configura para o pino 14
    ledc_channel_config_t ledc_channel_1 = {
        .gpio_num = GPIO_NUM_14,
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL_1,
        .timer_sel = LEDC_TIMER,
        .duty = 0,
        .hpoint = 0,
    };
    ledc_channel_config(&ledc_channel_1);

    // Configura para o pino 27
    ledc_channel_config_t ledc_channel_2 = {
        .gpio_num = GPIO_NUM_27,
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL_2,
        .timer_sel = LEDC_TIMER,
        .duty = 0,
        .hpoint = 0,
    };
    ledc_channel_config(&ledc_channel_2);
}

int generateRandomNumber(int min, int max){
    /* Gerador de números aleatorios dentro de um intervalo
    */
    int range = max - min + 1;
    int randomNumber = esp_random() % range;
    int result = min + randomNumber;
    
    return result;
}

/*****************************************/
//                                       //
//                Efeitos                //
//                                       //
/*****************************************/

void setRed(int brightness) {
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_0, brightness);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_0);
}

void setGreen(int brightness) {
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_1, brightness);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_1);
}

void setBlue(int brightness) {
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_2, brightness);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_2);
}

void MENU(const int OPTION){
    int SPEED = 50;
    switch(OPTION) {
      case 1:
        red(255, SPEED);
        break;
      case 2:
        green(255, SPEED);
        break;
      case 3:
        blue(255, SPEED);
        break;
      case 4:
        white(255, SPEED);
        break;
      case 5:
        greenLemon(255, SPEED);
        break;
      case 6:
        pink(255, SPEED);
        break;
      case 7:
        cyan(255, SPEED);
        break;
      case 8:
        purple(255, SPEED);
        break;
      case 9:
        yellow(255, SPEED);
        break;
      case 10:
        orange(255, SPEED);
        break;
      case 11:
        powerOff();
        break;
      case 12:
        flashes(SPEED);
        break;
      case 13:
        standByPulsate(SPEED);
        break;
      case 14:
        standByFade(SPEED);
        break;
      case 15:
        fire(1);
        break;
      case 16:
        rain(SPEED);
        break;
      case 17:
        hot(SPEED);
        break;
      case 18:
        cold(SPEED);
        break;
      case 19:
        randomEffects(SPEED);
        break;
      case 20:
        sparklingFire(SPEED);
        break;
      default:
        powerOff();
        exit(0);
    }
}

void red(const int BRIGHTNESS, const int DELAY) {
  setRed(BRIGHTNESS);
  setGreen(0);
  setBlue(0);
  delay(DELAY);
}

void green(const int BRIGHTNESS, const int DELAY) {
  setRed(0);
  setGreen(BRIGHTNESS);
  setBlue(0);
  delay(DELAY);
}

void blue(const int BRIGHTNESS, const int DELAY) {
  setRed(0);
  setGreen(0);
  setBlue(BRIGHTNESS);
  delay(DELAY);
}

void white(const int BRIGHTNESS, const int DELAY) {
  setRed(BRIGHTNESS);
  setGreen(BRIGHTNESS);
  setBlue(BRIGHTNESS);
  delay(DELAY);
}

void greenLemon(const int BRIGHTNESS, const int DELAY) {
  setRed((int)(BRIGHTNESS * 0.6));
  setGreen(BRIGHTNESS);
  setBlue(0);
  delay(DELAY);
}

void pink(const int BRIGHTNESS, const int DELAY) {
  setRed(BRIGHTNESS);
  setGreen(0);
  setBlue(BRIGHTNESS);
  delay(DELAY);
}

void cyan(const int BRIGHTNESS, const int DELAY) {
  setRed(0);
  setGreen(BRIGHTNESS);
  setBlue(BRIGHTNESS);
  delay(DELAY);
}

void purple(const int BRIGHTNESS, const int DELAY) {
  setRed((int)(BRIGHTNESS * 0.5));
  setGreen(0);
  setBlue(BRIGHTNESS);
  delay(DELAY);
}

void yellow(const int BRIGHTNESS, const int DELAY) {
  setRed(BRIGHTNESS);
  setGreen((int)(BRIGHTNESS * 0.4));
  setBlue(0);
  delay(DELAY);
}

void orange(const int BRIGHTNESS, const int DELAY) {
  setRed(BRIGHTNESS);
  setGreen((int)(BRIGHTNESS * 0.1));
  setBlue(0);
  delay(DELAY);
}

void powerOff() {
  setRed(0);
  setGreen(0);
  setBlue(0);
}

void flashes(int SPEED) {
    SPEED = 50;
    for (int i = 1 ; i <= 10 ; i++) {
        switch (i) {
        case 1:
            red(255, SPEED);
            break;
        case 2:
            green(255, SPEED);
            break;
        case 3:
            blue(255, SPEED);
            break;
        case 4:
            white(255, SPEED);
            break;
        case 5:
            greenLemon(255, SPEED);
            break;
        case 6:
            pink(255, SPEED);
            break;
        case 7:
            cyan(255, SPEED);
            break;
        case 8:
            purple(255, SPEED);
            break;
        case 9:
            yellow(255, SPEED);
            break;
        case 10:
            orange(255, SPEED);
            break;
        }
    }
}

void standByPulsate(int SPEED) {
    SPEED = 20;
    for (int i = 1 ; i <= 10 ; i++) {
        for (int j = 255 ; j >= 0 ; j -= 1) {
            switch (i) {
                case 1:
                    red(j, SPEED);
                    break;
                case 2:
                    green(j, SPEED);
                    break;
                case 3:
                    blue(j, SPEED);
                    break;
                case 4:
                    white(j, SPEED);
                    break;
                case 5:
                    greenLemon(j, SPEED);
                    break;
                case 6:
                    pink(j, SPEED);
                    break;
                case 7:
                    cyan(j, SPEED);
                    break;
                case 8:
                    purple(j, SPEED);
                    break;
                case 9:
                    yellow(j, SPEED);
                    break;
                case 10:
                    orange(j, SPEED);
                    break;
            }
        }
    }
}

void standByFade(int SPEED) {
    SPEED = 20;
    int percentR = 255, percentG = 0, percentB = 0;
    int const NUM_INCREMENT_OR_DECREMENT = 1;
    for (int i = 0 ; i < 765 ; i++) {
        if (i < 255) {
            percentR -=  NUM_INCREMENT_OR_DECREMENT;
            percentG +=  NUM_INCREMENT_OR_DECREMENT;
        } else if (i >= 255 && i < 510) {
            percentG -=  NUM_INCREMENT_OR_DECREMENT;
            percentB +=  NUM_INCREMENT_OR_DECREMENT;
        }
        else if (i >= 510 && i < 765){
            percentB -=  NUM_INCREMENT_OR_DECREMENT;
            percentR +=  NUM_INCREMENT_OR_DECREMENT;
        }
        setRed(percentR);
        setGreen(percentG);
        setBlue(percentB);
        delay(SPEED);
    }
}

void fire(int MODE) {
    int TIME_DELAY;
    switch (MODE) {
        case 1:
            TIME_DELAY = 117;
            break;
        case 2:
            TIME_DELAY = 170;
            break;
        case 3:
            TIME_DELAY = 235;
            break;
        default:
            return;
    }

    int percentG = 50;
    for (int i = 0 ; i <= 255 ; i++) {
        if (i > 0 && i <= 25) {
            percentG -= 1;
        } else if (i >= 231 && i <= 255) {
            percentG += 1;
        }
        setRed(255);
        setGreen(percentG);
        setBlue( 0);
        delay(TIME_DELAY);
        powerOff();
        delay(4);
    }
}

void rain(int DURATION_IN_SECONDS) {
    DURATION_IN_SECONDS = 60;
    if (DURATION_IN_SECONDS <= 0 || DURATION_IN_SECONDS > 60) {
        red(255, 100); powerOff(); delay(200);
        red(255, 100); powerOff(); delay(200);
        red(255, 100); powerOff(); delay(1000);
        return;
    }
    unsigned countTime = 0;
    for (;;) {
        const int TIME_DELAY = generateRandomNumber(0, 50) + 100;
        blue(generateRandomNumber(0, 155)+100, TIME_DELAY);
        countTime += TIME_DELAY;
        if (countTime >= DURATION_IN_SECONDS*1000) return;
    }
}

void hot(int SPEED) {
    SPEED = 20;
    int percentG = 0;
    for (int i = 0 ; i <= 100 ; i++) {
        setRed(255);
        setGreen(percentG);
        setBlue(0);
        delay(SPEED);
        if (i < 50) percentG++;
        else percentG--;
    }
}

void cold(int SPEED) {
    SPEED = 20;
    int percentR = 255, percentG = 255, percentB = 255;
    for(int i = 0 ; i < 765 ; i++) {
        if (i > 0 && i <= 255) {
            percentR--;
        } else if (i > 255 && i <= 510) {
            percentG--;
        } else if (i > 510 && i < 765) {
        percentR++;
        percentG++;
        }
        setRed(percentR);
        setGreen(percentG);
        setBlue(percentB);
        delay(SPEED);
    }
}

void randomEffects(int SPEED){
    SPEED = 20;
    for(int i = 0 ; i < 10 ; i++) {
        const long collor = generateRandomNumber(1, 11);
        switch(collor) {
        case 1:
            red( (int)generateRandomNumber(100, 256), SPEED);
            break;
        case 2:
            green( (int)generateRandomNumber(100, 256), SPEED);
            break;
        case 3:
            blue( (int)generateRandomNumber(100, 256), SPEED);
            break;
        case 4:
            white( (int)generateRandomNumber(100, 256), SPEED);
            break;
        case 5:
            greenLemon( (int)generateRandomNumber(100, 256), SPEED);
            break;
        case 6:
            pink( (int)generateRandomNumber(100, 256), SPEED);
            break;
        case 7:
            cyan( (int)generateRandomNumber(100, 256), SPEED);
            break;
        case 8:
            purple( (int)generateRandomNumber(100, 256), SPEED);
            break;
        case 9:
            yellow( (int)generateRandomNumber(100, 256), SPEED);
            break;
        case 10:
            orange( (int)generateRandomNumber(100, 256), SPEED);
            break;
        }
    }
}

void sparklingFire(int DURATION_IN_SECONDS) {
    DURATION_IN_SECONDS = 60;
    if (DURATION_IN_SECONDS <= 0 || DURATION_IN_SECONDS > 60) {
        red(255, 100); powerOff(); delay(200);
        red(255, 100); powerOff(); delay(200);
        red(255, 100); powerOff(); delay(1000);
        return;
    }
    unsigned countTime = 0;
    for (;;) {
        const int TIME_DELAY = generateRandomNumber(0, 100);
        orange( generateRandomNumber(0, 155)+100, TIME_DELAY);
        countTime += TIME_DELAY;
        if (countTime >= DURATION_IN_SECONDS*1000) return;
    }
}
