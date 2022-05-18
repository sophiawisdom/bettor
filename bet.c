#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define NUM_RUNS 100000

int simulate(int iterations, int bankroll, int bet, int increment) {
    unsigned int rand;

    unsigned int big_inc = increment * 31;
    unsigned int big_bet = bet * 31;

    for (int i = 0; i < iterations/31; i++) {
        rand = random();

        // For 1,000 iterations, this is a 50% improvement in simulations/s
        // for 1,000,000 iterations this is a 200% improvement in simulations/s
        if (bankroll > big_bet) {
            bankroll += (31-(__builtin_popcount(rand)*2))*bet + big_inc;
            continue;
        }

        for (int j = 0; j < 31; j++) {
            if (rand&1) {
                bankroll += bet;
            } else {
                bankroll -= bet;
            }
            bankroll += increment;
            if (bankroll <= bet) {
                return false;
            }
            rand >>= 1;
        }
    }

    return bankroll;
}

int main(int argc, char **argv) {
    int increment = 1;
    int iterations = 1000;
    int starting_bankroll = 10000;
    int bet = 500;
    
    if (argc >= 5) {
        increment = atoi(argv[4]);
    }
    if (argc >= 4) {
        bet = atoi(argv[3]);
    }
    if (argc >= 3) {
        starting_bankroll = atoi(argv[2]);
    }
    if (argc >= 2) {
        iterations = atoi(argv[1]);
    }
    int val = 0;
    srandom(&val);

    printf("Running %d simulations of a game where you bet $%d on a coinflip up to %d times but get an extra $%d every time starting with a bankroll of $%d\n", NUM_RUNS, bet, iterations, increment, starting_bankroll);
    unsigned long long start = clock();

    unsigned long long total_winnings = 0;
    unsigned int failures = 0;
    unsigned int above_init = 0;
    for (int i = 0; i < NUM_RUNS; i++) {
        int result = simulate(iterations, starting_bankroll, bet, increment);
        if (result) {
            if (result >= starting_bankroll) {
                above_init++;
            }
            total_winnings += result;
        } else {
            failures++;
        }
    }
    unsigned long long end = clock();
    unsigned long long diff = end-start;
    printf("Took %llu us\n", diff);
    printf("Lost all money: %d. Lost some money: %d. Successes: %d. average winnings: %llu\n", failures, (NUM_RUNS - failures - above_init), above_init, total_winnings/NUM_RUNS);

    return 0;
}