#include <stdbool.h>
#include <stdio.h>
#ifndef STRUCTURES
#define STRUCTURES


typedef struct pointf pointf;

struct pointf {
    double x;
    double y;
};
//Liste doublement chainée
struct demi_arete {
    int ind;
    struct demi_arete *jumelle;
    pointf debut;
    struct demi_arete *prev;
    struct demi_arete *next;
    bool complete;
};

typedef struct demi_arete demi_arete;

typedef void *Val;

struct Node {
    Val val;
    struct Node *next;
};

typedef struct Node Node;

typedef Node *list;


Node *new_node(Val);
void free_list(list);
bool is_empty(list);
Val head(list);
list tail(list);
list cons(Val, list);

typedef struct ligne ligne;

typedef struct evenement evenement;

typedef struct rive rive;

struct ligne {
    int taille;
    int capacite;
    evenement **data;
};

typedef struct chainon chainon;

struct evenement {
    bool est_cercle;
    bool est_site;
    double prio;
    int ind;
    pointf p;
    chainon *arc;
};


typedef struct Voronoi Voronoi;
struct Voronoi {
    demi_arete **cellules;
    list *sommets;
};


struct chainon {
    evenement *circle_event;
    int ind;
    demi_arete *arete;
    chainon *suiv;
    chainon *prec;
    chainon **skip;
    chainon **skipped;
    int hauteur;
    pointf p;
    bool sentinelle;
};

void libere_rive(rive *);

evenement *converge(pointf p, pointf pg, pointf pd, double l);

//File de priorité : ici tasmin avec tableau dynamique, la priorité étant l'ordonnée de l'évènement considéré


ligne *nouvelle_ligne(void);
void libere_ligne(ligne *);
bool vide(ligne *);
evenement *peek(ligne *);
evenement *pop_min(ligne *);
void insere_ligne(ligne *, evenement *);
pointf intersection(pointf p0, pointf p1, double l);
bool eg(pointf p, pointf q);
chainon *new_lst(void);
void insere_naif(chainon *lst, pointf p, ligne *line, int ind, list *DCEL, demi_arete **cellules);
void insere_skip(chainon *lst, pointf p, ligne *line, int ind, list *DCEL, demi_arete **cellules);
void retire_naif(chainon *c, double lgn, ligne *line, pointf p, list *DCEL);
void retire_skip(chainon *c, double lgn, ligne *line, pointf p, list *DCEL);
void free_lst(chainon *c);
void generer_dot_combine(chainon *liste, ligne *pq, list DCEL, const char *filename);

#endif