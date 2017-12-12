#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>

#define REP(i, n) for (int i = 0; i < n; i++)

using namespace std;

const int RANGE = 100000;
const int POP_SIZE = 100, N_ITER = 10000, N_MUT = 3;
const double tx_mut[N_MUT] = {-0.02 , 0.0, 0.02};
double v[POP_SIZE], filho[POP_SIZE];
double fit[POP_SIZE];
int ibest;

/* 
  y ^
    | *
    |* *
    *---*--> x
   *|    *		
   				*/
double fun (double val) {
	double x = val - 10.0;
	double y = x;
	if (y > 0.0)	y *= -1.0;
	y += 10.0;

	return y;
}

int coin_flip (int tam) {
	return rand()%tam;
}

void init () {
	REP (i, POP_SIZE) {
		v[i] = (rand()%RANGE) - RANGE/2;
	}
	ibest = 0;
}

double eval (double val) {
	double fitness = fun(val);

	return fitness;
}

void eval_all () {
	REP (i, POP_SIZE) {
		fit[i] = eval(v[i]);

		if (fit[i] > fit[ibest])
			ibest = i;
	}
}

void crossover () {
	REP (i, POP_SIZE) {
		filho[i] = (v[i] + v[ibest])/2.0;
	}
}

void mutacao () {
	REP (i, POP_SIZE) {
		filho[i] += filho[i] * tx_mut[coin_flip(N_MUT)];
	}
}

void rearranjar () {
	REP (i, POP_SIZE) {
		double fit_filho = eval(filho[i]);

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
}

int main (int argc, char *argv[]) {
	srand (time(NULL));

	init ();
	eval_all();

	REP (t, N_ITER) {
		crossover();

		mutacao();

		rearranjar();

		printf ("%.6lf\n", fit[ibest]);
	}

	printf ("Melhor individuo: %.12lf, Fitness: %.12lf\n", v[ibest], fit[ibest]);

	return 0;
}
