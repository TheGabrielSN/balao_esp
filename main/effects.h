#ifndef EFFECTS
#define EFFECTS

// Funções de controle de exução
void configPin();
int generateRandomNumber(int, int);
void setRed(int);
void setGreen(int);
void setBlue(int);

// Funções de efeito
void MENU(const int);
void red(const int, const int);                         // 1
void green(const int, const int);                       // 2
void blue(const int, const int);                        // 3
void white(const int, const int);                       // 4
void greenLemon(const int, const int);                  // 5
void pink(const int, const int);                        // 6
void cyan(const int, const int);                        // 7
void purple(const int, const int);                      // 8
void yellow(const int, const int);                      // 9
void orange(const int, const int);                      // 10
void powerOff();                                        // 11
void flashes(int);                                      // 12
void standByPulsate(int);                               // 13
void standByFade(int);                                  // 14
void fire(int);                                         // 15
void rain(int);                                         // 16
void hot(int);                                          // 17
void cold(int);                                         // 18
void randomEffects(int);                                // 19
void sparklingFire(int);                                // 20

#endif