#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//RODARI RIVA
//5 ottobre 2017   -   v.0.1.0

struct vector{
  double a;
  double va;
};

struct vector euler(struct vector, double, double);
struct vector cromer(struct vector, double, double);

double acceleration(struct vector, double, double, double, double, double);
double energy(struct vector, double);               

int main(int argc, char *argv[]){
  
  int choice = 2, j;
  long int i, steps;                                             
  double omega2, gamma, f0, omegaf, dt, tmax, e, e0, phi;
  
  struct vector state;
  
  FILE *input;
  FILE *output;

  input = fopen("input.dat", "r");
  
  char filename[20];
  for (j=0; j<2; j++) {

	  fscanf(input, "%lf %lf %lf %lf %lf %lf %lf %lf", &state.a, &state.va, &omega2, &dt, &tmax, &gamma, &f0, &omegaf);

	  e0 = energy(state, omega2);
	  
	  steps = (long int)(tmax/dt);   
	  
	  while(choice != 0 && choice != 1){           
	    printf("Insert (0) for Euler, (1) for Euler-Cromer: ");
	    scanf("%d", &choice);
	  }

	  if(choice == 0) {
	  	sprintf(filename, "p_euler%d.dat", j+1);
	    output = fopen(filename, "w");          

	    fprintf(output, "# Structure: t,\t a_calc,\t va_calc,\t E(t)/E0 - 1\n");
	    fprintf(output, "\t\t%lf\t %lf\t %lf\t %lf\n", 0., state.a, state.va, 0.);    
	 
	    for (i=0; i<steps; i++){
	      phi = acceleration(state, omega2, gamma, f0, omegaf, dt * (i+1));
	      state = euler(state, dt, phi);
	      e = energy(state, omega2);
	      fprintf(output, "\t\t%lf\t %lf\t %lf\t %lf\n", dt*(i+1), state.a, state.va, e/e0 - 1.);
	    }
	    fclose(output);
	  }


	  else{
	  	sprintf(filename, "p_cromer%d.dat", j+1);
	    output = fopen(filename, "w");

	    fprintf(output, "#t,\t a_calc,\t va_calc,\t E(t)/E0 - 1\n");
	    fprintf(output, "%lf %lf %lf %lf\n", 0., state.a, state.va, 0.);    
	 
	    for (i=0; i<steps; i++){
	      phi = acceleration(state, omega2, gamma, f0, omegaf, dt * (i+1));
	      state = cromer(state, dt, phi);
	      e = energy(state, omega2);
	      fprintf(output, "%lf %lf %lf\t %lf\n", dt*(i+1), state.a, state.va, e/e0 - 1.);
	    }
	  }

	  fclose(output);
	}

  exit(0);
}


struct vector euler(struct vector old, double dt, double phi){    //Eulero
  
  struct vector new;
  
  new.a = old.a + old.va * dt;
  new.va = old.va + phi * dt;

  return new;
  
}

struct vector cromer(struct vector old, double dt, double phi){    //Eulero-Cromer

  struct vector new;
  
  new.va = old.va + phi * dt;
  new.a = old.a + new.va * dt;

  return new;
  
}

double acceleration(struct vector state, double omega2, double gamma, double f0, double omegaf, double t) {

  return - omega2 * sin(state.a) - gamma * state.va + f0 * cos(omegaf * t);
  
}

double energy(struct vector state, double omega2) {

  return 0.5 * state.va * state.va + omega2 * (1 - cos(state.a));
}
