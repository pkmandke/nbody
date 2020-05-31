/*
 * Implementation of the Barned-Hut algorithm
 */

#include <iostream>
#include <cstdlib>
#include <random>
#include <ctime>
#include <omp.h>

#include "nbody.h"

using namespace std;

// Global vars
part_t *particles;
double theta;
double G;

int main(int argc, char *argv[]){

    int n = atoi(argv[1]);
    double delta_t = atof(argv[2]);
    int steps = atoi(argv[3]);
    theta = atof(argv[4]); // The distance threshold
    int threads = atoi(argv[5]);
    int chunks = atoi(argv[6]);

    // Initialize global variables
    G = 6.67 * pow(10.0, -11);

    particles = (part_t *)malloc(n * sizeof(part_t));
    if(particles == NULL){
        cout << "Particles is none." << endl;
        exit(1);
    }

    /* Initialize the particles */
    init_pos_and_mass(particles, n);

    int j;
    for(j = 0; j < n; j++){
        particles[j].part_id = j;
        particles[j].vx = 0.0;
        particles[j].vy = 0.0;
        particles[j].fx = 0.0;
        particles[j].fy = 0.0;
        // cout << "Mass = " << particles[j].m << endl;
    }
    node *root;
    
    omp_set_num_threads(threads);
    /* For step # if times, simualate the particles */
    #pragma omp parallel private (j) shared(root, particles, n, delta_t, chunks, steps)
    {
        if(omp_get_thread_num() == 0) cout << "Number of threads at runtime = " << omp_get_num_threads() << endl;
        for(j = 0; j < steps; j++){
            /* Initialize the root of the quad tree and insert particles in the tree */
            if(omp_get_thread_num() == 0){
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
                }
            }
            
            int i;

            if(omp_get_thread_num() == 0){
                for(i = 0; i < n; i++){
                    // cout << "Inserting " << i << "..." << endl;
                    insert_particle(&particles[i], root);
                    // cout << endl;
                }
            

                for(i = 0; i < n; i++) // Compute force on each particle
                    barnes_force(&particles[i], root);
            }

            #pragma omp barrier

            #pragma omp for schedule(static, chunks)
            for(i = 0; i < n; i++){
                 
                // Update acceleration
                particles[i].ax = (double)particles[i].fx / particles[i].m;
                particles[i].ay = (double)particles[i].fy / particles[i].m;

                // Update Velocity
                particles[i].vx = particles[i].vx + delta_t * particles[i].ax;
                particles[i].vy = particles[i].vy + delta_t * particles[i].ay;

                // Update position
                double updt = particles[i].vx * delta_t;
                particles[i].sx = adjust_pos(particles[i].sx, updt);
                updt = particles[i].vy * delta_t;
                particles[i].sy = adjust_pos(particles[i].sy, updt);

                // update_a(particles, n);
                // update_v(particles, n, delta_t);
                // update_pos(particles, n, delta_t);
            }

            if(omp_get_thread_num() == 0){
                free_tree(root->next);
                free(root);
                
                // cout << root->next[0].xmin << endl;
                // cout << "Force is " << endl;
                // cout << particles[0].fx << ", " << particles[0].fy << endl;
                // For debugging
                // cout << "Particle distances:" << endl;
                // int i;
                // for(i = 0; i < n; i++)
                    // cout << particles[i].ax << ", " << particles[i].ay << endl;
            }
        }
    } // end omp parallel

    free(particles);
    return 0;
}
       
