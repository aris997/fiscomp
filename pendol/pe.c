#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323

//Rodari Riva
//begin 5th october 2017	- v.0.3.2

struct vector{
	double a;
	double va;
	double t;
};

struct vector cromer(struct vector, double, double);

double acceleration(struct vector, double, double, double, double, double);
double energy(struct vector, double);

//void period(struct vector, struct vector, double);

int main(int argc, char *argv[]) {

	int k;
	double x0, v0, omega2, gamma, f0, omegaf, dt, tmax, e, e0, phi, Ta;
	long int i, j, steps, n;
	//char filename[20];
	double tstar[1000] = {0};

	struct vector state;
	struct vector last;

	FILE *input;
	//FILE *output;

	input = fopen("input.dat", "r");
	printf("Cromer\n");

	for (k=0; k<1; k++) {

	  fscanf(input, "%lf %lf %lf %lf %lf %lf %lf %lf", &x0, &v0, &omega2, &dt, &tmax, &gamma, &f0, &omegaf);

	  steps = (long int)(tmax/dt);
	  Ta = 2.*PI/sqrt(omega2);
	  // sprintf(filename, "p_cromer%d.dat", k+1);
	  // output = fopen(filename, "w");
	  
	  // fprintf(output, "#t, \ta_i, \tva_i, \tE(t)/E0 - 1\n");
	  // fprintf(output, "%lf %lf %lf %lf\n", 0., state.a, state.va, 0.);

	  for(j=0; j<3000; j++){
	  	
	  	state.a = x0*j;
	  	state.va = v0;
	  	n = 0;
	  	e0 = energy(state, omega2);

	  	for (i=0; i<steps; i++) {

	  		last.a = state.a;
	  		last.va = state.va;
	  		last.t = dt * i;

	  		phi = acceleration(state, omega2, gamma, f0, omegaf, dt * (i+1));
	  		state = cromer(state, dt, phi);

			if (last.a > 0 && state.a < 0) {
	  		
		  		tstar[n] = last.t + last.a * (last.t - dt * (i + 1))/(state.a - last.a);
	  			printf("%lf %lf %lf %lf %.10lf\n", last.a, state.a, last.t, dt * (i+1), tstar[n]);
	  			n++;
	  		}

		  	//e = energy(state, omega2);
		  	
		  	//fprintf(output, "%lf %lf %lf %lf\n", dt*(i+1), state.a, state.va, e/e0 - 1.);
		  	
		}

		if ((tstar[1] - tstar[0])/Ta - 1 > 0.1 ) {
	  		printf("STOP: errore del periodo sopra il 10%% %.10lf per x0 = %lf\n", (tstar[1] - tstar[0])/Ta - 1, x0);
	  		exit(0);
	  	}

		x0 += 0.001;		

		}

	  //fclose(output);
	}
	fclose(input);

	exit(0);
}

struct vector cromer(struct vector old, double dt, double phi) {
	
	struct vector new;
	
	new.va = old.va + phi * dt;
	new.a = old.a + new.va * dt;

	return new;
  
}


double energy(struct vector state, double omega2) {

	return 0.5 * state.va * state.va + omega2 * (1 - cos(state.a));
}


double acceleration(struct vector state, double omega2, double gamma, double f0, double omegaf, double t) {

  return - omega2 * sin(state.a) - gamma * state.va + f0 * cos(omegaf * t);
  
}


// void period(struct vector old, struct vector new, double omega2) {

// 	double T, Ta;

// 	Ta = 2*PI/sqrt(omega2);

// 	if (old.a + new.a < 0 ) {
  		
//   		T = old.t + old.a * (old.t - new.t) / (new.a - old.a);

//   		printf("%lf %lf %lf %lf %.10lf %.10lf\n", old.a, new.a, new.t, old.t, T, T/Ta - 1);
  		
//   		if (T/Ta - 1 > 0.1 ) {

//   			printf("STOP: errore del periodo sopra il 10%% %lf \n", T/Ta - 1);
//   			exit(0);

//   		}
//   	}
// }