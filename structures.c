#include "structures.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_HEIGHT 20
#define MAX 100000.0
#define EPSILON 1.0E-7

int min(int i, int j) {
    return i < j ? i : j;
}
Node *new_node(Val val) {
    Node *n = malloc(sizeof(Node));
    n->val = val;
    n->next = NULL;
    return n;
}

void free_list(list lst) {
    list courant = lst;
    while (courant != NULL) {
        free(courant->val);
        list temp = tail(courant);
        free(courant);
        courant = temp;
    }
}


bool is_empty(list lst) {
    return (lst == NULL);
}

Val head(list lst) {
    assert(!is_empty(lst));
    return lst->val;
}

list tail(list lst) {
    assert(!is_empty(lst));
    return lst->next;
}

list cons(Val x, list lst) {
    Node *n = new_node(x);
    n->next = lst;
    return n;
}

//Fonctions pour la file de priorité

int pere(int i) {
    return i / 2;
}

int fils_g(int i) {
    return 2 * i;
}

int fils_d(int i) {
    return 2 * i + 1;
}

void swap(int i, int j, evenement **tab) {
    evenement *temp = tab[i];
    tab[i] = tab[j];
    tab[j] = temp;
}

void resize(ligne *line) {
    if (line->taille == line->capacite - 1) {
        evenement **newdata = malloc(2 * line->capacite * sizeof(evenement *));
        for (int i = 1; i <= line->taille; i++) {
            newdata[i] = line->data[i];
        }
        line->capacite = 2 * line->capacite;
        free(line->data);
        line->data = newdata;
    }
}

void downsize(ligne *line) {
    if (line->taille < line->capacite / 5 && line->capacite > 10) {
        evenement **newdata = malloc(line->capacite / 2 * sizeof(evenement *));
        for (int i = 1; i <= line->taille; i++) {
            newdata[i] = line->data[i];
        }
        line->capacite = line->capacite / 2;
        free(line->data);
        line->data = newdata;
    }
}

ligne *nouvelle_ligne(void) {
    ligne *line = malloc(sizeof(ligne));
    line->taille = 0;
    line->capacite = 10;
    line->data = malloc(10 * sizeof(evenement *));
    return line;
}

bool vide(ligne *line) {
    return line->taille == 0;
}

void libere_ligne(ligne *line) {
    for (int i = 1; i <= line->taille; i++) {
        free(line->data[i]);
    }
    free(line->data);
    free(line);
}

void insere_ligne(ligne *line, evenement *e) {
    if (line->taille == line->capacite - 1) {
        resize(line);
    }
    line->taille++;
    line->data[line->taille] = e;
    int i = line->taille;
    while (i != 1 && line->data[i]->prio < line->data[pere(i)]->prio) {
        swap(i, pere(i), line->data);
        i = pere(i);
    }
}

evenement *peek(ligne *line) {
    assert(!vide(line));
    return line->data[1];
}

int max(int i, int j) {
    return i > j ? i : j;
}

evenement *pop_min(ligne *line) {
    assert(!vide(line));
    swap(1, line->taille, line->data);
    int i = 1;
    line->taille--;
    bool cont = true;
    while (cont) {
        if (i >= line->taille) {
            cont = false;
        } else {
            int mini = i;
            if (fils_g(i) <= line->taille && line->data[fils_g(i)]->prio < line->data[mini]->prio) {
                mini = fils_g(i);
            }
            if (fils_d(i) <= line->taille && line->data[fils_d(i)]->prio < line->data[mini]->prio) {
                mini = fils_d(i);
            }
            if (mini != i) {
                swap(i, mini, line->data);
                i = mini;

            } else {
                cont = false;
            }
        }
    }

    evenement *ans = line->data[line->taille + 1];
    if (line->taille < line->capacite / 5 && line->capacite > 10) {
        downsize(line);
    }
    return (ans);
}


//-----

//Fonctions pour la skiplist

chainon *nouveau_chainon(pointf p) {
    chainon *new = malloc(sizeof(chainon));
    new->p = p;
    new->circle_event = NULL;
    new->prec = NULL;
    new->suiv = NULL;
    new->skip = NULL;
    new->skipped = NULL;
    new->arete = NULL;
    new->sentinelle = false;
    new->ind = -1;
    return new;
}

chainon *new_lst(void) {
    chainon *deb = malloc(sizeof(chainon));
    chainon *fin = malloc(sizeof(chainon));
    deb->skip = malloc(MAX_HEIGHT * sizeof(chainon));
    fin->skip = malloc(MAX_HEIGHT * sizeof(chainon));
    deb->skipped = malloc(MAX_HEIGHT * sizeof(chainon));
    fin->skipped = malloc(MAX_HEIGHT * sizeof(chainon));
    for (int i = 0; i < MAX_HEIGHT; i++) {
        deb->skip[i] = fin;
        fin->skip[i] = NULL;
        deb->skipped[i] = NULL;
        fin->skipped[i] = deb;
    }
    deb->ind = -1;
    fin->ind = -1;
    deb->p.x = -MAX;
    deb->p.y = 0.0;
    fin->p.x = MAX;
    fin->p.y = 0.0;
    deb->suiv = fin;
    fin->prec = deb;
    deb->sentinelle = true;
    fin->sentinelle = true;
    deb->circle_event = NULL;
    fin->circle_event = NULL;
    fin->suiv = NULL;
    deb->prec = NULL;
    deb->arete = NULL;
    fin->arete = NULL;
    //La hauteur de deb correspondra à la hauteur maximale d'un chainon dans la liste
    deb->hauteur = 0;
    fin->hauteur = MAX_HEIGHT;
    return deb;
}

demi_arete *arete(int ind) {
    demi_arete *b = malloc(sizeof(demi_arete));
    b->ind = ind;
    b->debut = (pointf) {-MAX, -MAX};
    b->prev = NULL;
    b->next = NULL;
    b->jumelle = NULL;
    b->complete = false;
    return b;
}

void relie(demi_arete *d1, demi_arete *d2) {
    d1->next = d2;
    d2->prev = d1;
}
//-----

//Fonctions pour la géométrie

bool eg(pointf p1, pointf p2) {
    return fabs(p1.x - p2.x) < 0.0001 && fabs(p1.y - p2.y) < 0.0001;
}

bool lt(pointf p1, pointf p2) {
    return p1.x <= p2.x + 0.0001;
}

//"égalité" entre flottants
bool feg(double x, double y) {
    return fabs(x - y) < EPSILON;
}

double det3x3(double a11, double a12, double a13,
              double a21, double a22, double a23,
              double a31, double a32, double a33) {
    return a11 * (a22 * a33 - a23 * a32) - a12 * (a21 * a33 - a23 * a31) + a13 * (a21 * a32 - a22 * a31);
}
//Retourne l'intersection des arcs générés par p0 et p1 lorsque la traversante est en lgn
pointf intersection(pointf p0, pointf p1, double lgn) {
    pointf res = p0;
    pointf p = p0;
    double r = 0;
    if (feg(p0.y, p1.y)) {
        res.x = (p0.x + p1.x) / 2;
    } else if (feg(p1.y, lgn)) {
        res.x = p1.x;
    } else if (feg(p0.y, lgn)) {
        res.x = p0.x;
        p = p1;
    } else {
        double z0 = 2 * (p0.y - lgn);
        double z1 = 2 * (p1.y - lgn);

        double a = 1 / z0 - 1 / z1;
        double b = -2 * (p0.x / z0 - p1.x / z1);
        double c = (p0.x * p0.x + p0.y * p0.y - lgn * lgn) / z0 - (p1.x * p1.x + p1.y * p1.y - lgn * lgn) / z1;

        r = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
        res.x = r;
    }
    res.y = (p.y * p.y + (p.x - res.x) * (p.x - res.x) - lgn * lgn) / (2 * p.y - 2 * lgn);
    if (res.y > MAX) {
        res.y = MAX;
    }
    if (res.y < -MAX) {
        res.y = -MAX;
    }
    return res;
}

double distance(pointf p, pointf q) {
    return sqrt((p.x - q.x) * (p.x - q.x) + (p.y - q.y) * (p.y - q.y));
}

//renvoie le circle event lié à p0 p1 p2 s'il est après la ligne traversante et que les breakpoints associés convergent bien
evenement *converge(pointf p0, pointf p1, pointf p2, double lgn) {
    if (p0.x == p1.x || p1.x == p2.x || p2.x == p0.x) {
        return NULL;
    }
    //Méthode du déterminant pour le centre d'un cercle circonscrit
    double det1 = det3x3(p0.x * p0.x + p0.y * p0.y, p0.y, 1.0,
                         p1.x * p1.x + p1.y * p1.y, p1.y, 1.0,
                         p2.x * p2.x + p2.y * p2.y, p2.y, 1.0);

    double det2 = det3x3(p0.x, p0.x * p0.x + p0.y * p0.y, 1.0,
                         p1.x, p1.x * p1.x + p1.y * p1.y, 1.0,
                         p2.x, p2.x * p2.x + p2.y * p2.y, 1.0);

    double det3 = det3x3(p0.x, p0.y, 1.0,
                         p1.x, p1.y, 1.0,
                         p2.x, p2.y, 1.0);

    if (fabs(det3) < EPSILON) {
        return NULL;
    }
    double x = det1 / (2 * det3);
    double y = det2 / (2 * det3);
    double prio = (y + sqrt((x - p0.x) * (x - p0.x) + (y - p0.y) * (y - p0.y)));
    pointf p;
    pointf dq1 = intersection(p0, p1, prio);
    p.x = x;
    p.y = y;
    //Le circle event est en dessous de la ligne de rive (avec marge d'erreur) et les breakpoints convergent bien vers le centre du cercle lorsque la ligne avance
    if (prio <= lgn + EPSILON || (distance(dq1, p) > EPSILON)) {
        return NULL;
    }
    evenement *ans = malloc(sizeof(evenement));
    ans->est_cercle = true;
    ans->est_site = false;
    ans->p.x = x;
    ans->prio = prio;
    ans->p.y = y;

    ans->ind = -1;

    return ans;
}

evenement *converge_(pointf p0, pointf p1, pointf p2, double lgn) {
    if (p0.x == p1.x || p1.x == p2.x || p2.x == p0.x) {
        return NULL;
    }
    //Méthode du déterminant pour le centre d'un cercle circonscrit
    double det1 = det3x3(p0.x * p0.x + p0.y * p0.y, p0.y, 1.0,
                         p1.x * p1.x + p1.y * p1.y, p1.y, 1.0,
                         p2.x * p2.x + p2.y * p2.y, p2.y, 1.0);

    double det2 = det3x3(p0.x, p0.x * p0.x + p0.y * p0.y, 1.0,
                         p1.x, p1.x * p1.x + p1.y * p1.y, 1.0,
                         p2.x, p2.x * p2.x + p2.y * p2.y, 1.0);

    double det3 = det3x3(p0.x, p0.y, 1.0,
                         p1.x, p1.y, 1.0,
                         p2.x, p2.y, 1.0);

    if (fabs(det3) < EPSILON) {
        return NULL;
    }
    double x = det1 / (2 * det3);
    double y = det2 / (2 * det3);
    double prio = (y + sqrt((x - p0.x) * (x - p0.x) + (y - p0.y) * (y - p0.y)));
    pointf p;
    pointf dq1 = intersection(p0, p1, prio);
    pointf dq2 = intersection(p1, p2, prio);
    pointf q1 = intersection(p0, p1, lgn);
    pointf q2 = intersection(p1, p2, lgn);
    p.x = x;
    p.y = y;
    //Le circle event est en dessous de la ligne de rive (avec marge d'erreur) et les breakpoints convergent bien vers le centre du cercle lorsque la ligne avance
    if (prio <= lgn + EPSILON || (distance(dq1, dq2) > distance(q1, q2))) {
        return NULL;
    }
    evenement *ans = malloc(sizeof(evenement));
    ans->est_cercle = true;
    ans->est_site = false;
    ans->p.x = x;
    ans->prio = prio;
    ans->p.y = y;

    ans->ind = -1;

    return ans;
}


//-----

//Fonctions pour l'algorithme
void ajoute_cercle(chainon *c, double lgn, ligne *line) {
    if (!c->suiv->sentinelle && !c->suiv->suiv->sentinelle) {
        evenement *cercle = converge(c->p, c->suiv->p, c->suiv->suiv->p, lgn);
        if (cercle) {
            if (!c->suiv->circle_event || (c->suiv->circle_event && !eg(c->suiv->circle_event->p, cercle->p))) {

                if (c->suiv->circle_event) {
                    c->suiv->circle_event->arc = NULL;
                    c->suiv->circle_event->est_cercle = false;
                }
                insere_ligne(line, cercle);
                cercle->arc = c->suiv;
                c->suiv->circle_event = cercle;
            } else {
                free(cercle);
            }
        }
    }
    if (!c->prec->sentinelle && !c->prec->prec->sentinelle) {
        evenement *cercle = converge(c->prec->prec->p, c->prec->p, c->p, lgn);
        if (cercle) {
            if (!c->prec->circle_event || (c->prec->circle_event && !eg(c->prec->circle_event->p, cercle->p))) {

                if (c->prec->circle_event) {
                    c->prec->circle_event->arc = NULL;
                    c->prec->circle_event->est_cercle = false;
                }
                insere_ligne(line, cercle);

                cercle->arc = c->prec;
                c->prec->circle_event = cercle;
            } else {
                free(cercle);
            }
        }
    }
}
//Insertion dans une liste doublement chainée en O(n)
void insere_naif(chainon *lst, pointf p, ligne *line, int ind, list *DCEL, demi_arete **cellules) {
    chainon *courant = lst;
    double lgn = p.y;
    chainon *new = nouveau_chainon(p);
    new->ind = ind;
    if (courant->suiv->sentinelle) {
        //On insere dans une liste vide
        courant->suiv->prec = new;
        new->suiv = courant->suiv;
        courant->suiv = new;
        new->ind = ind;
        new->prec = courant;
    } else {
        courant = courant->suiv;
        while (!courant->suiv->sentinelle && intersection(courant->p, courant->suiv->p, lgn).x < p.x) {
            courant = courant->suiv;
        }
        chainon *duplique = nouveau_chainon(courant->p);
        duplique->ind = courant->ind;
        if (courant->circle_event) {
            courant->circle_event->est_cercle = false;
            courant->circle_event->arc = NULL;
            courant->circle_event = NULL;
        }

        demi_arete *b1 = arete(ind);
        demi_arete *b2 = arete(courant->ind);
        b1->debut = intersection(courant->p, new->p, lgn + 10.0);
        b2->debut = intersection(new->p, duplique->p, lgn + 10.0);
        if (lt(intersection(courant->p, p, lgn), courant->p)) {
            courant->suiv->prec = duplique;
            duplique->suiv = courant->suiv;
            duplique->prec = new;
            new->suiv = duplique;
            new->prec = courant;
            courant->suiv = new;
            duplique->arete = b2;

        } else {
            courant->prec->suiv = duplique;
            duplique->prec = courant->prec;
            duplique->suiv = new;
            new->prec = duplique;
            new->suiv = courant;
            courant->prec = new;
            duplique->arete = courant->arete;
            courant->arete = b2;
        }
        duplique->ind = courant->ind;
        new->ind = ind;
        new->arete = b1;
        b1->jumelle = b2;
        b2->jumelle = b1;
        cellules[ind] = b1;
        if (!cellules[duplique->ind]) {
            cellules[duplique->ind] = b2;
        }
        b1->ind = ind;
        b2->ind = duplique->ind;
        *DCEL = cons((void *) b1, *DCEL);
        *DCEL = cons((void *) b2, *DCEL);
        ajoute_cercle(new, lgn, line);
    }
}

int hauteur(void) {
    int ans = 0;
    while (rand() % 2 && ans < MAX_HEIGHT - 1) {
        ans++;
    }
    return ans;
}
//p doit il être inséré après c ?
bool apres(pointf p, chainon *c, double lgn) {

    if (c->sentinelle) {
        return false;
    }
    if (c->suiv->sentinelle) {
        return false;
    }
    return intersection(c->p, c->suiv->p, lgn).x < p.x;
}
//Pour le débogage
void generer_dot_combine(chainon *liste, ligne *line, list DCEL, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return;

    fprintf(f, "digraph combined {\n");
    fprintf(f, "    rankdir=LR;\n");
    fprintf(f, "    node [shape=record, fontsize=10];\n\n");

    // --- Sous-graphe liste chainée ---
    fprintf(f, "    subgraph cluster_chainon {\n");
    fprintf(f, "        label=\"Liste chainée\";\n");
    fprintf(f, "        color=blue;\n");

    // Parcours liste chainon et création noeuds
    for (chainon *c = liste; c != NULL; c = c->suiv) {
        fprintf(f, "        \"%p\" [label=\"{p=(%.2f, %.2f) | sent=%s | hauteur = %d}\"];\n",
                (void *) c, c->p.x, c->p.y, c->sentinelle ? "true" : "false", c->hauteur);
    }
    for (chainon *c = liste; c != NULL; c = c->suiv) {
        if (c->suiv) {
            if (c->sentinelle || c->suiv->sentinelle) {
                fprintf(f, "        \"%p\" -> \"%p\" [color=black label=\"suiv\"];\n",
                        (void *) c, (void *) c->suiv);
            } else {
                fprintf(f, "        \"%p\" -> \"%p\" [color=black label=\"%1.0f\"];\n",
                        (void *) c, (void *) c->suiv, vide(line) ? 100000.0 : intersection(c->p, c->suiv->p, peek(line)->prio).x);
            }
        }
        if (c->prec) {
            fprintf(f, "        \"%p\" -> \"%p\" [ color=gray label=\"prec\"];\n",
                    (void *) c->prec, (void *) c);
        }
        if (c->circle_event) {
            fprintf(f, "        \"%p\" -> \"%p\" [ color=gray ];\n",
                    (void *) c, (void *) c->circle_event);
        } /*
        if (c->arete) {
            fprintf(f, "        \"%p\" -> \"(%d,%d)\" [ color=gray ];\n",
                    (void *) c, (int) c->arete->debut.x, (int) c->arete->debut.y);
        }*/
        // Arcs skip (tableaux de pointeurs) en rouge, pour chaque niveau

        if (c->skip) {
            for (int h = 0; h < c->hauteur; h++) {
                if (c->skip[h] && !(c->sentinelle && c->skip[h]->sentinelle)) {
                    fprintf(f, "        \"%p\" -> \"%p\" [color=red style=dashed label=\"skip[%d]\"];\n",
                            (void *) c, (void *) c->skip[h], h);
                }
            }
        }
    }


    fprintf(f, "    }\n\n");

    // --- Sous-graphe file de priorité ---
    fprintf(f, "    subgraph cluster_pq {\n");
    fprintf(f, "        label=\"File de priorité (tas min)\";\n");
    fprintf(f, "        color=green;\n");
    fprintf(f, "        rankdir=TB;\n");

    // Noeuds tas min

    for (int i = 1; i <= line->taille; i++) {
        evenement *e = line->data[i];
        fprintf(f, "  \"%p\" [label=\"prio=%1.3f\\n(%d,%d)\", shape=invhouse];\n",
                (void *) e, e->prio, (int) e->p.x, (int) e->p.y);

        if (2 * i <= line->taille)
            fprintf(f, "  \"%p\" -> \"%p\";\n", (void *) e, (void *) line->data[2 * i]);
        if (2 * i + 1 <= line->taille)
            fprintf(f, "  \"%p\" -> \"%p\";\n", (void *) e, (void *) line->data[2 * i + 1]);
    }
    fprintf(f, "    }\n\n");

    // --- Liens arcs dans la file de priorité vers chainon ---
    fprintf(f, "    subgraph cluster_arcs {\n");
    fprintf(f, "        label=\"Liens arcs (chainon) depuis événements\";\n");
    fprintf(f, "        color=purple;\n");
    fprintf(f, "        node [shape=oval];\n");

    // Pour chaque événement dans la file, lien vers arc chainon si présent
    for (int i = 1; i <= line->taille; i++) {
        evenement *e = line->data[i];
        if (e->arc) {
            fprintf(f, "        \"%p\" -> \"%p\" [color=purple style=bold label=\"arc\"];\n",
                    (void *) e, (void *) e->arc);
        }
    }
    fprintf(f, "    }\n");

    // --- Sous-graphe DCEL---
    fprintf(f, "    subgraph cluster_dcel {\n");
    fprintf(f, "        label=\"DCEL\";\n");
    fprintf(f, "        color=purple;\n");
    list parcours = DCEL;
    while (parcours != NULL) {
        demi_arete *b = (demi_arete *) head(parcours);
        if (b->debut.x != -100000) {
            fprintf(f, "  \"(%d,%d)\" [shape=ellipse];\n",
                    (int) b->debut.x, (int) b->debut.y);

            if (b->next) {
                fprintf(f, "        \"(%d,%d)\" -> \"(%d,%d)\" [color=purple style=bold];\n",
                        (int) b->debut.x, (int) b->debut.y, (int) b->next->debut.x, (int) b->next->debut.y);
            }
        }
        parcours = tail(parcours);
    }
    fprintf(f, "    }\n");


    fprintf(f, "}\n");
    fclose(f);
}

void insere_skip(chainon *lst, pointf p, ligne *line, int ind, list *DCEL, demi_arete **cellules) {
    double lgn = p.y;
    chainon *new1 = nouveau_chainon(p);
    new1->ind = ind;
    new1->hauteur = hauteur();
    new1->skip = malloc((new1->hauteur) * sizeof(chainon *));
    new1->skipped = malloc((new1->hauteur) * sizeof(chainon *));
    if (new1->hauteur > lst->hauteur) {
        lst->hauteur = new1->hauteur;
    }
    chainon *courant = lst;
    if (courant->suiv->sentinelle) {
        //On insere dans une liste vide

        for (int i = 0; i < new1->hauteur; i++) {
            courant->skip[i] = new1;
            new1->skip[i] = courant->suiv;
            new1->skipped[i] = courant;
        }
        courant->suiv->prec = new1;
        new1->suiv = courant->suiv;
        courant->suiv = new1;
        new1->ind = ind;
        new1->prec = courant;
    } else {
        chainon *a_modifier1[MAX_HEIGHT];
        chainon *a_modifier2[MAX_HEIGHT];
        chainon *new2 = nouveau_chainon(p);
        //new2 sera inséré juste après new1
        new2->ind = ind;
        new2->hauteur = hauteur();
        if (new2->hauteur > lst->hauteur) {
            lst->hauteur = new2->hauteur;
        }
        new2->skip = malloc(new2->hauteur * sizeof(chainon *));
        new2->skipped = malloc(new2->hauteur * sizeof(chainon *));
        int niveau = lst->hauteur;
        for (int i = 0; i < min(new1->hauteur, new2->hauteur); i++) {
            a_modifier2[i] = new1;
        }
        bool cont = true;
        while (cont) {
            if (niveau < 0) {
                if (courant->sentinelle && courant->suiv) {

                    courant = courant->suiv;
                    for (int i = 0; i < courant->hauteur; i++) {
                        if (i < new1->hauteur) {
                            a_modifier1[i] = courant;
                        } else if (i < new2->hauteur) {
                            a_modifier2[i] = courant;
                        }
                    }
                }
                while (apres(p, courant, lgn)) {
                    courant = courant->suiv;
                }

                cont = false;
            } else {
                if (!apres(p, courant->skip[niveau]->prec, lgn)) {
                    if (niveau < new1->hauteur) {
                        a_modifier1[niveau] = courant;
                    } else if (niveau < new2->hauteur) {
                        a_modifier2[niveau] = courant;
                    }
                    niveau--;
                } else {

                    courant = courant->skip[niveau];
                }
            }
        }
        //On insère nos deux nouveaux noeuds dans la liste
        new1->suiv = new2;
        new2->prec = new1;
        new2->p = courant->p;
        new2->ind = courant->ind;
        new2->suiv = courant->suiv;
        courant->suiv->prec = new2;
        new1->prec = courant;
        courant->suiv = new1;
        for (int i = 0; i < new1->hauteur; i++) {
            new1->skip[i] = a_modifier1[i]->skip[i];
            a_modifier1[i]->skip[i]->skipped[i] = new1;
            new1->skipped[i] = a_modifier1[i];
            a_modifier1[i]->skip[i] = new1;
        }
        for (int i = 0; i < new2->hauteur; i++) {
            new2->skip[i] = a_modifier2[i]->skip[i];
            a_modifier2[i]->skip[i]->skipped[i] = new2;
            new2->skipped[i] = a_modifier2[i];
            a_modifier2[i]->skip[i] = new2;
        }
        //On annule un potentiel circle event
        if (courant->circle_event) {
            courant->circle_event->est_cercle = false;
            courant->circle_event->arc = NULL;
            courant->circle_event = NULL;
        }

        demi_arete *b1 = arete(ind);
        demi_arete *b2 = arete(courant->ind);
        b1->debut = intersection(courant->p, new1->p, lgn + 10.0);
        b2->debut = intersection(new1->p, new2->p, lgn + 10.0);
        new2->arete = b2;
        new1->arete = b1;
        b1->jumelle = b2;
        b2->jumelle = b1;
        cellules[ind] = b1;
        if (!cellules[new2->ind]) {
            cellules[new2->ind] = b2;
        }
        b1->ind = ind;
        b2->ind = new2->ind;
        *DCEL = cons((void *) b1, *DCEL);
        *DCEL = cons((void *) b2, *DCEL);
        ajoute_cercle(new1, lgn, line);
    }
}


void generer_dot(chainon *liste, ligne *line, list DCEL, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return;

    fprintf(f, "digraph combined {\n");
    fprintf(f, "    rankdir=LR;\n");
    fprintf(f, "    node [shape=record, fontsize=10];\n\n");

    // --- Sous-graphe liste chainée ---
    fprintf(f, "    subgraph cluster_chainon {\n");
    fprintf(f, "        label=\"Liste chainée\";\n");
    fprintf(f, "        color=blue;\n");

    // Parcours liste chainon et création noeuds
    for (chainon *c = liste; c != NULL; c = c->suiv) {
        fprintf(f, "        \"%p\" [label=\"{p=(%.2f, %.2f) | sent=%s}\"];\n",
                (void *) c, c->p.x, c->p.y, c->sentinelle ? "true" : "false");
    }
    // Arcs prec et suiv (doublement chaîné)
    for (chainon *c = liste; c != NULL; c = c->suiv) {
        if (c->suiv) {
            fprintf(f, "        \"%p\" -> \"%p\" [color=black label=\"suiv\"];\n",
                    (void *) c, (void *) c->suiv);
        }
        if (c->prec) {
            fprintf(f, "        \"%p\" -> \"%p\" [ color=gray label=\"prec\"];\n",
                    (void *) c->prec, (void *) c);
        }
        if (c->arete) {
            fprintf(f, "        \"%p\" -> \"(%d,%d)\" [ color=gray ];\n",
                    (void *) c, (int) c->arete->debut.x, (int) c->arete->debut.y);
        }
    }

    fprintf(f, "    }\n\n");


    // --- Sous-graphe DCEL---
    fprintf(f, "    subgraph cluster_dcel {\n");
    fprintf(f, "        label=\"DCEL\";\n");
    fprintf(f, "        color=purple;\n");
    list parcours = DCEL;
    while (parcours != NULL) {
        demi_arete *b = (demi_arete *) head(parcours);
        if (b->debut.x != -100000) {
            fprintf(f, "  \"(%d,%d)\" [shape=ellipse];\n",
                    (int) b->debut.x, (int) b->debut.y);

            if (b->next) {
                fprintf(f, "        \"(%d,%d)\" -> \"(%d,%d)\" [color=purple style=bold];\n",
                        (int) b->debut.x, (int) b->debut.y, (int) b->next->debut.x, (int) b->next->debut.y);
            }
        }
        parcours = tail(parcours);
    }
    fprintf(f, "    }\n");


    fprintf(f, "}\n");
    fclose(f);
}


void retire_naif(chainon *c, double lgn, ligne *line, pointf p, list *DCEL) {
    assert(c && !c->sentinelle);
    c->prec->suiv = c->suiv;
    c->suiv->prec = c->prec;
    demi_arete *d1 = c->arete;
    demi_arete *d2 = c->suiv->arete;
    if (!(d1 && d2)) {
        generer_dot(c->prec, line, *DCEL, "rive_.dot");
    }
    assert(d1 && d2);
    d1->debut = p;
    d2->debut = p;
    d1->complete = true;
    d2->complete = true;
    demi_arete *new1 = arete(c->prec->ind);
    demi_arete *new2 = arete(c->suiv->ind);
    new1->jumelle = new2;
    new2->jumelle = new1;
    new1->debut = p;
    new2->debut = intersection(c->p, c->suiv->p, lgn + 10.0);
    relie(d2->jumelle, d1);
    relie(d1->jumelle, new1);
    relie(new2, d2);
    c->suiv->arete = new2;
    *DCEL = cons((void *) new1, *DCEL);
    *DCEL = cons((void *) new2, *DCEL);
    ajoute_cercle(c->prec, lgn, line);
    ajoute_cercle(c->suiv, lgn, line);
    free(c->skip);
    free(c->skipped);
    free(c);
}

void retire_skip(chainon *c, double lgn, ligne *line, pointf p, list *DCEL) {
    assert(c && !c->sentinelle);
    c->prec->suiv = c->suiv;
    c->suiv->prec = c->prec;
    for (int i = 0; i < c->hauteur; i++) {
        c->skipped[i]->skip[i] = c->skip[i];
        c->skip[i]->skipped[i] = c->skipped[i];
    }
    demi_arete *d1 = c->arete;
    demi_arete *d2 = c->suiv->arete;

    d1->debut = p;
    d2->debut = p;
    d1->complete = true;
    d2->complete = true;
    demi_arete *new1 = arete(c->prec->ind);
    demi_arete *new2 = arete(c->suiv->ind);
    new1->jumelle = new2;
    new2->jumelle = new1;
    new1->debut = p;
    new2->debut = intersection(c->prec->p, c->suiv->p, lgn + 10.0);
    relie(d2->jumelle, d1);
    relie(d1->jumelle, new1);
    relie(new2, d2);
    c->suiv->arete = new2;
    *DCEL = cons((void *) new1, *DCEL);
    *DCEL = cons((void *) new2, *DCEL);


    ajoute_cercle(c->prec, lgn, line);
    ajoute_cercle(c->suiv, lgn, line);
    free(c->skip);
    free(c->skipped);
    free(c);
}

void free_lst(chainon *c) {
    chainon *courant = c;
    while (courant != NULL) {
        free(courant->skip);
        free(courant->skipped);
        chainon *temp = courant->suiv;
        free(courant);
        courant = temp;
    }
}
//-----
