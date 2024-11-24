#include <wiringPi.h> // Include WiringPi library!
#include <math.h>
#include <stdio.h>

#define SIZE 10

#define SWITCH 2
#define BOBOUT 3
#define CASOUT 4
#define ALICEIN 18

#define LED1 17
#define LED2 27
#define LED3 22
#define LED4 10
#define LED5 9
#define LED6 11
#define LED7 5
#define LED8 6
#define LED9 13
#define LED10 26

int ledPins[10] = {17, 27, 22, 10, 9, 11, 5, 13, 6, 19};

void convertToBinary(int val, int array[])
{
    for (int i = 0; i < SIZE; i++)
    {
        array[i] = val % 2;
        val /= 2;
    }
}

int main(void)
{

    // uses BCM numbering of the GPIOs and directly accesses the GPIO registers.
    wiringPiSetupGpio();

    // pin mode ..(INPUT, OUTPUT, PWM_OUTPUT, GPIO_CLOCK)
    // set pin 17 to input
    pinMode(CASOUT, OUTPUT); // Casino
    pinMode(BOBOUT, OUTPUT); // Bob
    pinMode(ALICEIN, INPUT); // Alice

    // pull up/down mode (PUD_OFF, PUD_UP, PUD_DOWN) => down
    pullUpDnControl(ALICEIN, PUD_DOWN);
    pullUpDnControl(SWITCH, PUD_DOWN);
    for (int j = 0; j < 10; j++)
    {
        pinMode(ledPins[j], OUTPUT);
        digitalWrite(ledPins[j], LOW);
    }
    int Bcount;
    int Ccount;
    int BBcount[SIZE];
    int BCcount[SIZE];
    int Alice[SIZE];
    int score;
    int BestScore = 0;
    int BestAlice[SIZE];
    int BestBob[SIZE];
    int mostCommonScore = 0;
    while (TRUE)
    {
        if (!digitalRead(SWITCH))
        {
            int scoreBuckets[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            for (Bcount = 0; Bcount < 1024; Bcount++)
            {
                convertToBinary(Bcount, BBcount);
                for (int j = 0; j < 10; j++)
                {
                    digitalWrite(ledPins[j], BBcount[j]);
                }
                for (Ccount = 0; Ccount < 1024; Ccount++)
                {
                    convertToBinary(Ccount, BCcount);
                    for (int i = 0; i < 10; i++)
                    {
                        Alice[i] = digitalRead(22);
                        digitalWrite(17, BCcount[i]);
                        digitalWrite(27, BBcount[i]);
                    }
                    score = 0;
                    for (int i = 0; i < 10; i++)
                    {
                        if (Alice[i] == BBcount[i] && BBcount[i] == BCcount[i])
                        {
                            score++;
                        }
                    }
                    for (int j; j < 11; j++)
                    {
                        if (score = j)
                        {
                            scoreBuckets[j]++;
                        }
                    }
                    if (score > BestScore)
                    {
                        for (int i = 0; i < 10; i++)
                        {
                            BestAlice[i] = Alice[i];
                            BestBob[i] = BBcount[i];
                        }
                        BestScore = score;
                    }
                    printf("%d\n", Bcount * 1024 + Ccount);
                }
            }

            for (int j; j < 11; j++)
            {
                if (j > 0)
                {
                    if (scoreBuckets[j] > scoreBuckets[j - 1])
                    {
                        mostCommonScore = j;
                    }
                }
            }
            printf("Best score: %d \n", +BestScore);
            printf("Best Alice:\n");
            for (int i = 0; i < 10; i++)
            {
                digitalWrite(ledPins[i], LOW);
            }
            for (int j = 0; j < BestScore; j++)
            {
                digitalWrite(ledPins[j], HIGH);
            }

            fflush(stdout);
            delay(5000);
            for (int i = 0; i < 10; i++)
            {
                digitalWrite(ledPins[i], LOW);
            }
            for (int j = 0; j < mostCommonScore; j++)
            {
                digitalWrite(ledPins[j], HIGH);
            }
            fflush(stdout);
            delay(5000);
            for (int i = 0; i < 10; i++)
            {
                printf("%d", BestAlice[i]);
            }
            printf("\nBest Bob:\n");
            for (int i = 0; i < 10; i++)
            {
                printf("%d", BestBob[i]);
                digitalWrite(ledPins[i], BestBob[i]);
            }
            fflush(stdout);
            delay(5000);
        }
        if (digitalRead(SWITCH))
        {
            for (int i = 0; i < 10; i++)
            {
                digitalWrite(ledPins[i], HIGH);
                fflush(stdout);
                delay(100);
                if (i > 0)
                {
                    digitalWrite(ledPins[i - 1], LOW);
                }
                else if (i == 9)
                {
                    digitalWrite(ledPins[i], LOW);
                }
                fflush(stdout);
                delay(100);
            }
        }
    }

    return 1;
}
