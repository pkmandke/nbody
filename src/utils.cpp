/*
 * Common utilities
 * Author: Prathamesh Mandke
 */

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <random>

#include "nbody.h"


using namespace std;


void init_pos_and_mass(part_t *p, int n){
    
    /*
    std::default_random_engine gen_x;
    std::uniform_real_distribution<double> dist_x(0.0,1000.0);
    
    std::default_random_engine gen_y;
    std::uniform_real_distribution<double> dist_y(0.0,1000.0);
    */

    std::random_device rd_x;
    std::mt19937 gen_x(rd_x());
    std::uniform_real_distribution<double> dist_x(0.0,10000.0);

    std::random_device rd_y;
    std::mt19937 gen_y(rd_y());
    std::uniform_real_distribution<double> dist_y(0.0,10000.0);
    
    int i;
    for(i = 0; i < n; i++){
        // cout << i << endl;
        
        p[i].sx = dist_x(gen_x);
        p[i].sy = dist_y(gen_y);
        p[i].m = (3.0 + (7.0 * (pow(p[i].sx, 2.0) + pow(p[i].sy, 2.0)) / pow(10.0, 8)));
        p[i].m = p[i].m * pow(10.0, 22);
        // cout << "Mass = " << p[i].m << endl;
    }
}

void compute_force(part_t *p, int n){
    
    int i, j;
    double gm1, gmmr, r;

    for(i = 0; i < n; i++){
        p[i].fx = 0.0;
        p[i].fy = 0.0;
        gm1 = G * p[i].m;

        for(j = 0; j < n; j++)
        if(i != j){
            r = (double)sqrt(pow(p[i].sx - p[j].sy, 2.0) + pow(p[i].sy - p[j].sy, 2.0));
            gmmr = gm1 * p[j].m / pow(r, 3.0);
            p[i].fx += gmmr * (p[i].sx - p[j].sx);
            p[i].fy += gmmr * (p[i].sy - p[j].sy);
        } // if cond ends
    } // i loop ends
}


void update_a(part_t *p, int n){
    int i;

    for(i = 0; i < n; i++){
        p[i].ax = (double)p[i].fx / p[i].m;
        // cout << "Acc = " << p[i].fx << " / " << p[i].m << " = " << p[i].ax << endl;
        p[i].ay = (double)p[i].fy / p[i].m;
    }
}

void update_v(part_t *p, int n, double delta_t){
    
    int i;

    for(i = 0; i < n; i++){
       p[i].vx = p[i].vx + delta_t * p[i].ax;
       p[i].vy = p[i].vy + delta_t * p[i].ay;
    }
}

/* Rebound particle if position crosses bounds of grid.*/
double adjust_pos(double pos, double delta){
    
    /*
    double d_ = fmod(abs(delta), 10000.0);
    if(delta < 0) delta = -1.0 * d_;
    else delta = d_;

    if((pos + delta) < 0) return abs(delta) - pos;

    if((pos + delta) > 10000.0) return (10000.0 - (delta - (10000.0 - pos)));
    */
    double pd = pos + delta;
    // cout << "pd = " << pd << endl;
    if(pd >= 10000.0){
        // cout << "Input = " << delta - (10000.0 - pos) << ", mod  = " << fmod(delta - (10000.0 - pos), 20000.0) << endl;
        pd = fmod(delta - (10000.0 - pos), 20000.0);
        if((pd / 10000.0) > 1.0) return (double)fmod(pd, 10000.0);
        else return (10000.0 - (double)fmod(pd, 10000.0));
    }
    else if(pd < 0.0){
        delta = (double)abs(delta);
        // cout << "Input = " << delta - pos << ", mod  = " << fmod(delta - pos, 20000.0) << endl;
        pd = fmod(delta - pos, 20000.0);
        if((pd / 10000.0) < 1.0) return (double)fmod(pd, 10000.0);
        else return (10000.0 - (double)fmod(pd, 10000.0));
    }
    else return pd;
}


void update_pos(part_t *p, int n, double delta_t){
    
    int i;
    double updt;

    for(i = 0; i < n; i++){
        
        updt = p[i].vx * delta_t;
        p[i].sx = adjust_pos(p[i].sx, updt);
        // cout << "Update is: " << updt << ", pos is: " << p[i].sx << endl;
        updt = p[i].vy * delta_t;
        p[i].sy = adjust_pos(p[i].sy, updt);
        
    }
}


/*
int main(int argc, char *argv[]){
    
    int n = atoi(argv[1]);
    double delta_t = atof(argv[2]);

    part_t *p = (part_t *)malloc(n * sizeof(part_t));
    // allocate_n_particles(p, n);
    // if(p == NULL) cout << "Yeah" << endl;
    // cout << p[0].sx << endl;
    init_pos_and_mass(p, n);
    compute_force(p, n);
    update_a(p, n);
    update_v(p, n, delta_t);
    update_pos(p, n, delta_t);
    free(p);
    return 0;

}*/

