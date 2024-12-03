#include <wiringPi.h> // Include WiringPi library!
#include <math.h>
#include <stdio.h>

#define SIZE 10

#define SWITCH 2
#define BOBOUT 3
#define CASOUT 4
#define ALICEIN 18

int ledPins[10] = {17, 27, 22, 10, 9, 11, 5, 13, 6, 19};
int switchPins[10] = {23, 24, 25, 8, 7, 12, 16, 20, 21, 26}

void
convertToBinary(int val, int array[])
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
    int WorstCasino[SIZE];
    int mostCommonScore = 0;
    int bestBobScore = 0;
    int WorstCasinoScore = 10;
    int bobAnswer[SIZE];
    int bobAnswerB[SIZE];
    while (TRUE)
    {
        if (!digitalRead(SWITCH))
        {
            int scoreBuckets[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            for (Ccount = 0; Ccount < 1024; Ccount++)
            {
                convertToBinary(Ccount, BCcount);
                for (int j = 0; j < 10; j++)
                {
                    digitalWrite(ledPins[j], BCcount[j]);
                }
                for (Bcount = 0; Bcount < 1024; Bcount++)
                {
                    convertToBinary(Bcount, BBcount);
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
                    if (score > bestBobScore)
                    {
                        for (int i = 0; i < 10; i++)
                        {
                            BestAlice[i] = Alice[i];
                            BestBob[i] = BBcount[i];
                        }
                        bestBobScore = score;
                    }
                    printf("%d\n", Bcount * 1024 + Ccount);
                }
                if (bestBobScore < WorstCasino)
                {
                    for (int i = 0; i < 10; i++)
                    {
                        WorstCasino[i] = BestBob[i];
                    }
                    WorstCasinoScore = bestBobScore;
                }
                bestBobScore = 0;
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
            fflush(stdout);
            delay(5000);
            for (int i = 0; i < 10; i++)
            {
                printf("%d", WorstCasino[i]);
            }
            printf("\nBest Bob in the worst casino:\n");
            for (int i = 0; i < 10; i++)
            {
                printf("%d", WorstCasino[i]);
                digitalWrite(ledPins[i], WorstCasino[i]);
            }
            fflush(stdout);
            delay(5000);
            while (!digitalRead(SWITCH))
            {
                for (i = 0; i < 10; i++)
                {
                    digitalWrite(ledPins[i], digitalRead(switchPins[i]));
                    bobAnswer[i] = digitalRead(switchPins[i]);
                }
            }
            convertToBinary(bobAnswer, bobAnswerB);
            for (int i = 0; i < 10; i++)
            {
                Alice[i] = digitalRead(22);
                digitalWrite(17, bobAnswerB[i]);
                digitalWrite(27, WorstCasino[i]);
            }
            score = 0;
            for (int i = 0; i < 10; i++)
            {
                if (Alice[i] == BBcount[i] && BBcount[i] == BCcount[i])
                {
                    score++;
                }
            }
            for (int i = 0; i < 10; i++)
            {
                digitalWrite(ledPins[i], LOW);
            }
            for (int j = 0; j < 10; j++)
            {
                digitalWrite(ledPins[j], Alice[i]);
                fflush(stdout);
                delay(1000);
            }
            fflush(stdout);
            delay(5000);
            for (int i = 0; i < 10; i++)
            {
                digitalWrite(ledPins[i], LOW);
            }
            for (int j = 0; j < score; j++)
            {
                digitalWrite(ledPins[j], HIGH);
                fflush(stdout);
                delay(1000);
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
