/*
 * Implementation of the Barned-Hut algorithm
 */

#include <iostream>
#include <cstdlib>
#include <random>
#include <ctime>

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
    
    // Initialize global variables
    G = 6.67 * pow(10.0, -11);

    particles = (part_t *)malloc(n * sizeof(part_t));
    if(particles == NULL){
        cout << "Particles is none." << endl;
        exit(1);
    }
    
    // Initialize the particles 
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

    // For step # if times, simualate the particles
    for(j = 0; j < steps; j++){
        // Initialize the root of the quad tree and insert particles in the tree
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
        int i;
        /*
        cout << "root dissection: " << endl;
        if(root != NULL)
            if(root->next != NULL){
                for(i = 0; i < 4; i++) cout << "is_internal = " << root->next[i].is_internal << ", part_id = " << root->next[i].part_id << endl;
            }
        
        for(i = 0; i < n; i++){
            cout << "fx = " << particles[i].fx << ", fy = " << particles[i].fy << endl;
            cout << "ax = " << particles[i].ax << ", ay = " << particles[i].ay << endl;
            cout << "vx = " << particles[i].vx << ", vy = " << particles[i].vy << endl;
            cout << "sx = " << particles[i].sx << ", sy = " << particles[i].sy << endl;
        }*/
        
        for(i = 0; i < n; i++){
            // cout << "Inserting " << i << "..." << endl;
            insert_particle(&particles[i], root);
            // cout << endl;
        }
        
        for(i = 0; i < n; i++) // Compute force on each particle
            barnes_force(&particles[i], root); 
        update_a(particles, n);
        update_v(particles, n, delta_t);
        update_pos(particles, n, delta_t);
        free_tree(root->next);
        free(root);
        cout << root->next[0].xmin << endl;
        cout << "Force is " << endl;
        cout << particles[0].fx << ", " << particles[0].fy << endl;
        // For debugging
        cout << "Particle distances:" << endl; 
        // int i;
        for(i = 0; i < n; i++){
            cout << particles[i].sx << ", " << particles[i].sy << endl;
        }
    }
        
    // cout << "In free node ?" << endl;
    free(particles);
    return 0;
}
