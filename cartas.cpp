/*	Baralho com cartas de 0 a 10
 *	separar paralho em 2, sendo que:
 *		Baralho1: soma de suas cartas mais proximas possivel de 34
 *		Baralho2: multiplicacao de suas cartas mais proximas possivel de 160
 * */

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cassert>
#include <iostream>

#define REP(i, n) for (int i = 0; i < n; i++)

using namespace std;

const int B1 = 34, B2 = 160;
const int DECK_SIZE = 11;
const int ROLL_SIZE = 10000;
const double PROB_MUT = 0.20, PROB_TRAVEL = 0.75;
const int POP_SIZE = 1000, N_ITER = 100;

int coin_flip (int tam) {
	return rand()%tam;
}

int roulette (double prob) {
	int roll = coin_flip (ROLL_SIZE);

	return (roll < prob * ROLL_SIZE);
}

struct gen {
	bool side[DECK_SIZE];

	gen() {}

	gen (gen &B) {
		memcpy (this->side, B.side, sizeof (this->side));
	}

	void rand_init () {
		REP (i, DECK_SIZE) {
			this->side[i] = rand()%2;
		}
	}

	int eval () {
		int b1 = 0, b2 = 1;
		REP (i, DECK_SIZE) {
			if (side[i] == 0) {
				b1 += i;
			} else {
				b2 *= i;
			}
		}
		
		int fit = abs(B1 - b1) + abs (B2 - b2);
		fit *= -1;
		return fit;
	}

	void cross (gen &A, gen &B) {
		REP (i, DECK_SIZE) {
			if (coin_flip(2) == 0) {
				this->side[i] = A.side[i];
			} else {
				this->side[i] = B.side[i];
			}
		}
	}

	void mutacao () {
		REP (i, DECK_SIZE) {
			if (roulette (PROB_MUT))
				this->side[i] = 1 - this->side[i];
		}
	}

	void print () {
		int b1 = 0, b2 = 1;
		printf ("Baralho 1: { ");
		REP (i, DECK_SIZE) {
			if (this->side[i] == 0) {
				printf ("%d ", i);
				b1 += i;
			}
		}
		printf ("}, soma = %d\n", b1);

		printf ("Baralho 2: { ");
		REP (i, DECK_SIZE) {
			if (this->side[i] == 1) {
				printf ("%d ", i);
				b2 *= i;
			}
		}
		printf ("}, multiplicacao = %d\n", b2);
	}
};

gen v[POP_SIZE], filho[POP_SIZE];
int fit[POP_SIZE];
int ibest;

void init () {
	ibest = 0;
	REP (i, POP_SIZE) {
		v[i].rand_init();

		fit[i] = v[i].eval();

		if (fit[i] > fit[ibest])
			ibest = i;
	}
}

void crossover () {
	REP (i, POP_SIZE) {
		filho[i].cross(v[i], v[ibest]);
		filho[i].mutacao();
	}
}

/*
 * Um individuo estrangeiro (viajante) toma o lugar
 * de um individuo da populacao
 * */
void travel (int itravel) {
	if (itravel == ibest)	return;

	// sorteia a change de um viajante chegar ah populacao
	if (!roulette (PROB_TRAVEL))	return;

	v[itravel].rand_init();
	fit[itravel] = v[itravel].eval();

	if (fit[itravel] > fit[ibest])
		ibest = itravel;
}

void rearranjar () {
	REP (i, POP_SIZE) {
		int fit_filho = filho[i].eval();

		// escolhe um individuo[j] aleatorio da populacao para 
		// comparar o filho[i], se o filho[i] for melhor que o 
		// individuo[j], o filho[i] substitui o individuo[j]
		int j = rand()%POP_SIZE;
		if (fit_filho > fit[j]) {
			v[j] = filho[i];
			fit[j] = fit_filho;

			// o filho[i], novo individuo[j], pode ter se 
			// tornado o novo melhor individuo
			if (fit[j] > fit[ibest])
				ibest = j;
		}
	}

	// Pior individuo sera substituido
	// pelo viajante
	int imin = POP_SIZE - 1;
	REP (i, POP_SIZE) {
		if (fit[i] < fit[imin])
			imin = i;
	}

	travel (imin);
}

int main (int argc, char *argv[]) {
	srand (time(NULL));

	init ();

	REP (TT, N_ITER) {
		crossover();

		rearranjar();

		printf ("Melhor atual: %d\n", fit[ibest]);
	}

	printf ("\n\n");
	printf ("Melhor individuo:\n");
	printf ("\n");
	v[ibest].print();
	printf ("\n");
	printf ("Fitness: %d\n", fit[ibest]);

	return 0;
}
