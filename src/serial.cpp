/*
 * Serial implementation of the vanilla algorithm for N-body problem.
 *
 * Steps:
 *
 * 1. Initialize the positions of n-particles randomly and simultaneously compute their masses.
 * Update the acc and vel values for all particles based on the initial formation.
 * Select a delta_t value and some N based on cmd arguments.
 *
 * 2. For N steps do:
 * 
 * First update the acceleration and velocity of all particles followed by the position.
 * Repeat!
 */


#include <iostream>
#include <cstdlib>
#include <cmath>

#include "nbody.h"

using namespace std;

/*
struct particle {

    double ax, ay;
    double vx, vy;
    double sx, sy; // position

    double fx, fy; // Force at time t
    double m; // mass
};
typedef struct particle part_t;
*/
double G;

int main(int argc, char *argv[]){
    
    int n = atoi(argv[1]);
    double delta_t = atof(argv[2]);
    int runs = atoi(argv[3]);
    
    G = 6.67 * pow(10.0, -11);

    part_t *p = (part_t *)malloc(n * sizeof(part_t));
    init_pos_and_mass(p, n);
    
    int i;
    for(i = 0; i < runs; i++){
        compute_force(p, n);
        update_a(p, n);
        update_v(p, n, delta_t);
        update_pos(p, n, delta_t);
        int k;
        // for(k = 0; k < n; k++) cout << "sx = " << p[k].sx << ", sy = " << p[k].sy << endl;
    }
    free(p);
    return 0;

}
