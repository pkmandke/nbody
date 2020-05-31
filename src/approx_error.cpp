#include <iostream>
#include <cstdlib>
#include <cmath>

#include "nbody.h"

using namespace std;

double G;
part_t *particles;
double theta;

int main(int argc, char *argv[]){

    int n = atoi(argv[1]);
    double delta_t = atof(argv[2]);
    int runs = atoi(argv[3]);
    theta = atof(argv[4]);

    G = 6.67 * pow(10.0, -11);
    
    part_t *p = (part_t *)malloc(n * sizeof(part_t));
    init_pos_and_mass(p, n);
    
    int i, j;
    particles = (part_t *)malloc(n * sizeof(part_t));
    for(j = 0; j < n; j++){
       particles[j].part_id = j;
       particles[j].vx = 0.0;
       particles[j].vy = 0.0;
       particles[j].fx = 0.0;
       particles[j].fy = 0.0;
       particles[j].sx = p[j].sx;
       particles[j].sy = p[j].sy;
       particles[j].m = p[j].m;
    }
    
    node *root;
    double mae;
    mae = 0.0;
    for(i = 0; i < runs; i++){
        
	    root = (node *)malloc(sizeof(node));
        init_node(root);
        root->xmin = 0.0;
        root->xmax = 10000.0;
        root->ymin = 0.0;
        root->ymax = 10000.0;
        subdivide_node(root);
        root->cx = 0.0;
        root->cy = 0.0;

        int k;
        for(k = 0; k < n; k++){
            particles[k].fx = 0.0;
            particles[k].fy = 0.0;
            insert_particle(&particles[k], root);
        }
	    /* Update forces and position using Barnes Hut */
        for(k = 0;k < n; k++) barnes_force(&particles[k], root);
        update_a(particles, n);
        update_v(particles, n, delta_t);
        update_pos(particles, n, delta_t);

        /* Compute forces and poisitions using the vanilla algorithm */
	    compute_force(p, n);
        update_a(p, n);
        update_v(p, n, delta_t);
        update_pos(p, n, delta_t);
    
        /* Compute Mean absolute error */
        for(k = 0; k < n; k++){
            mae += ((abs(particles[k].sx - p[k].sx) + abs(particles[k].sy - p[k].sy)) / 2.0);
        }
        // cout << "Mean abosulute error at the " << i + 1 << "th iteration is " << mae / n << endl;
        free_tree(root->next);
        free(root);
    }
    cout << "The mean absolute error = " << (double)mae / (n * runs) << endl;

    free(p);
    free(particles);
    return 0;
}
