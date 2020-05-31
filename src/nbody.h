/*
 *
 * Header file for N-body problem implementation.
 * Author: Prathamesh Mandke
 */

#ifndef NBODY_H_
#define NBODY_H_

// struct particle;
// typedef struct particle part_t;

struct particle {
    
    int part_id;    
    double ax, ay;
    double vx, vy;
    double sx, sy; // position

    double fx, fy; // Force at time t
    double m; // mass
};
typedef struct particle part_t;


void init_pos_and_mass(part_t *, int );
void compute_force(part_t *, int );
void update_a(part_t *, int );
void update_v(part_t *, int , double );
double adjust_pos(double , double );
void update_pos(part_t *, int , double);


/* Types for Barnes Hut */

struct qtree_node {
    
    int part_id; // Particle id for this node, -1 if no particle here
    int is_internal; // 1 if internal node (having 4 children)
    
    double xmin, xmax, ymin, ymax, m, cx, cy;
    struct qtree_node *next;
};

typedef struct qtree_node node;


/* Functions for Barnes Hut algorithm */

extern part_t *particles;
extern double theta, G;
void init_node(node *);
void put_particle(part_t *p ,node *n);
void subdivide_node(node *n);
void insert_particle(part_t* p, node *root);
void free_tree(node *root);
void update_particle_force(part_t *p, node *n);
double get_theta_(part_t *p, node *n);
void barnes_force(part_t *p, node *root);

/*struct part {
    
    double ax, ay;
    double vx, vy;
    double sx, sy; // position

    double fx, fy; // Force at time t
    double m; // mass
};*/

#endif
