/*
 * Implementation of the Barned-Hut algorithm
 */

#include <iostream>
#include <cstdlib>
#include <random>
#include <ctime>

#include "nbody.h"

using namespace std;

// Helpers

void init_node(node *n){
    
    n->part_id = -1;
    n->is_internal = 0;
    n->next = NULL;
    n->m = 0;
}


// Put a particle in given node
void put_particle(part_t *p ,node *n){
    
    n->part_id = p->part_id;
    n->is_internal = 0; // This is still external until it is subdivided

    n->cx = p->sx;
    n->m = p->m;
    n->cy = p->sy;
}


void subdivide_node(node *n){
    
    n->part_id = -1;
    n->is_internal = 1;
    
    n->next = (node *)malloc(4 * sizeof(node));

    n->next[0].xmin = n->xmin;
    n->next[0].xmax = n->xmin + (n->xmax - n->xmin) / 2.0;
    n->next[0].ymin = n->ymin;
    n->next[0].ymax = n->ymin + (n->ymax - n->ymin) / 2.0;
    n->next[0].next = NULL;

    n->next[1].xmin = n->xmin + (n->xmax - n->xmin) / 2.0;
    n->next[1].xmax = n->xmax;
    n->next[1].ymin = n->ymin;
    n->next[1].ymax = n->ymin + (n->ymax - n->ymin) / 2.0;
    n->next[1].next = NULL;

    n->next[2].xmin = n->xmin;
    n->next[2].xmax = n->xmin + (n->xmax - n->xmin) / 2.0;;
    n->next[2].ymin = n->ymin + (n->ymax - n->ymin) / 2.0;;
    n->next[2].ymax = n->ymax;
    n->next[2].next = NULL;

    n->next[3].xmin = n->xmin + (n->xmax - n->xmin) / 2.0;;
    n->next[3].xmax = n->xmax;
    n->next[3].ymin = n->ymin + (n->ymax - n->ymin) / 2.0;;
    n->next[3].ymax = n->ymax;
    n->next[3].next = NULL;
    
    int i;
    for(i = 0; i < 4; i++){
        n->next[i].is_internal = 0;
        n->next[i].part_id = -1;
    }
}


void insert_particle(part_t* p, node *root){
    
    if( (!root->is_internal) && (root->part_id == -1) ){// This node does not contain a particle either in it or in any of it's chidlren
        // cout << "Putting particle " << endl;
        put_particle(p, root);
    }
    else if (root->is_internal){ // If root is non-leaf (and thus doesn't have a particle)
        // update com and mass of current node  
        // cout << "Non-leaf internal node" << endl;
        root->cx = (root->m * root->cx + p->m * p->sx) / (root->m + p->m);
        root->cy = (root->m * root->cy + p->m * p->sy) / (root->m + p->m);
        root->m += p->m;
        
        // Locate the correct child and place...
        int i;
        for(i = 0; i < 4; i++){
            if( (root->next[i].xmax > p->sx) && (root->next[i].xmin <= p->sx) )
            if( (root->next[i].ymax > p->sy) && (root->next[i].ymin <= p->sy) ){
                insert_particle(p, &root->next[i]);
                break;
            } 
        }
    }
    else if ( (root->part_id != -1) && (!root->is_internal) ){ // If this node is a leaf and already contains a particle then divide!
        // cout << "Need to subdivide" << endl;    
        // Subdivide node
        int old_part = root->part_id; // Save the particle already in root to re-insert
        subdivide_node(root);

        int i;
        for(i = 0; i < 4; i++){
            if( (root->next[i].xmax > p->sx) && (root->next[i].xmin <= p->sx) )
            if( (root->next[i].ymax > p->sy) && (root->next[i].ymin <= p->sy) ){
                // cout << "Inserting new" << endl;
                insert_particle(p, &root->next[i]);
                break;
            }   
        }

        for(i = 0; i < 4; i++){
            if( (root->next[i].xmax > particles[old_part].sx) && (root->next[i].xmin <= particles[old_part].sx) )
            if( (root->next[i].ymax > particles[old_part].sy) && (root->next[i].ymin <= particles[old_part].sy) ){
                // cout << "Inserting old" << endl;
                insert_particle(&particles[old_part], &root->next[i]);
                break;
            }   
        } // second for ends
        
        // Update COM of root
        root->cx = (particles[old_part].m * particles[old_part].sx + p->m * p->sx) / (particles[old_part].m + p->m);
        root->cy = (particles[old_part].m * particles[old_part].sy + p->m * p->sy) / (particles[old_part].m + p->m);
        root->m = particles[old_part].m + p->m;
    } // else if ends
    else{
        cout << "NOTA?" << endl;
    }
}

void free_tree(node *root){
    int i;
    if(root != NULL){
        // if(root->next != NULL)
        for(i = 0; i < 4; i++)
            free_tree(root[i].next);
        free(root);
    }
}

/* Helper function for barnes algorithm. Updates the force on particle p by adding that by quad tree node n. */
void update_particle_force(part_t *p, node *n){
    
    double gm1, gmmr, r;
    
    r = sqrt(pow(p->sx - n->cx, 2.0) + pow(p->sy - n->cy, 2.0));
    gmmr = G * p->m * n->m / pow(r, 3.0);
    
    p->fx += gmmr * (p->sx - n->cx);
    p->fy += gmmr * (p->sy - n->cy);
}


/* Computes the ratio s/d */
double get_theta_(part_t *p, node *n){
    
    double s, d;
    
    // s is the width of the region represented by node n
    s = n->xmax - n->xmin;

    // d is the distance between the particle p and the COM of node n
    d = sqrt(pow(p->sx - n->cx, 2.0) + pow(p->sy - n->cy, 2.0));

    return (double)s / d;
}



/* Compute net force on particle p */
void barnes_force(part_t *p, node *root){
    
    if((root->part_id != -1) && (root->part_id != p->part_id) && (!root->is_internal) ){ // Current node is external and does not house particle p
       // Compute the force by this particle on p and update net force on p 
       update_particle_force(p, root);
    }
    else if(root->is_internal){
        double sd = get_theta_(p, root);

        if(sd < theta){
            // Compute force by this node on p and update p's net forcei
            update_particle_force(p ,root);
        }
        else{
            int i;
            // cout << "after this?" << endl;
            if(root->next)
            for(i = 0; i < 4; i++) barnes_force(p, &root->next[i]);
        }
    }
}

