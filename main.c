#include "primitives.h"
#include "structures.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define N 50

RGB RGB_white = {255, 255, 255};
RGB RGB_black = {0, 0, 0};
RGB RGB_gray = {25, 25, 25};
RGB RGB_red = {255, 0, 0};

pointf *genere_points(int n) {
    pointf *tab = malloc(n * sizeof(pointf));
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        double x = -N + rand() % (2 * N) + ((double) (rand() % 1000)) / 1000;
        double y = -N + rand() % (2 * N) + ((double) (rand() % 1000)) / 1000;
        ;
        tab[i] = (pointf) {x, y};
    }
    return tab;
}

pointf *genere_vitesses(int n) {
    pointf *tab = (malloc(n * sizeof(pointf)));
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        double x = -10 / 2 + rand() % (10) + ((double) (rand() % 10000)) / 1000;
        double y = -10 / 4 + rand() % (10 / 2) + ((double) (rand() % 10000)) / 1000;
        ;
        tab[i] = (pointf) {x, y};
    }
    return tab;
}

void maj(pointf *points, pointf *vitesses, double dt, int n) {
    for (int i = 0; i < n; i++) {
        points[i].x += vitesses[i].x * dt;
        if (fabs(points[i].x) > N) {
            vitesses[i].x = -vitesses[i].x;
        }
        points[i].y += vitesses[i].y * dt;
        if (fabs(points[i].y) > N) {
            vitesses[i].y = -vitesses[i].y;
        }
    }
}

point pointf_to_point(pointf p) {
    point p1;
    p1.x = (int) ((p.x) * HEIGHT * (1) / (2 * N));
    p1.y = (int) ((p.y) * HEIGHT * (1) / (2 * N));
    return p1;
}
void gestion_cercle1(evenement *e, ligne *line, list *DCEL) {
    retire_skip(e->arc, e->prio, line, e->p, DCEL);
    //printf("(%1.2f,%1.2f),\n", e->p.x, e->p.y);
}

void gestion_site1(chainon *lst, pointf p, ligne *line, int ind, list *DCEL, demi_arete **cellules) {
    insere_skip(lst, p, line, ind, DCEL, cellules);
}

void gestion_cercle2(evenement *e, ligne *line, list *DCEL) {
    retire_naif(e->arc, e->prio, line, e->p, DCEL);
}

void gestion_site2(chainon *lst, pointf p, ligne *line, int ind, list *DCEL, demi_arete **cellules) {
    insere_naif(lst, p, line, ind, DCEL, cellules);
}


list diagramme(pointf *pointfs, int n, bool skip) {
    list DCEL = NULL;
    demi_arete **cellules = malloc(n * sizeof(demi_arete *));
    for (int i = 0; i < n; i++) {
        cellules[i] = NULL;
    }
    ligne *sweep_line = nouvelle_ligne();
    for (int i = 0; i < n; i++) {
        evenement *evt = malloc(sizeof(evenement));
        evt->est_cercle = false;
        evt->est_site = true;
        evt->p = pointfs[i];
        evt->prio = evt->p.y;
        evt->arc = NULL;
        evt->ind = i;
        insere_ligne(sweep_line, evt);
    }
    chainon *ligne_rive = new_lst();
    int cpt = 0;


    while (!vide(sweep_line)) {
        evenement *e = pop_min(sweep_line);

        if (e->est_site) {
            if (skip) {
                gestion_site1(ligne_rive, e->p, sweep_line, e->ind, &DCEL, cellules);
            } else {
                gestion_site2(ligne_rive, e->p, sweep_line, e->ind, &DCEL, cellules);
            }
        } else if (e->est_cercle) {
            if (skip) {
                gestion_cercle1(e, sweep_line, &DCEL);
            } else {
                gestion_cercle2(e, sweep_line, &DCEL);
            }
        }
        free(e);
        char filename[64];
        sprintf(filename, "rives_%03d.dot", cpt++);
        //printf("%d \n", cpt);
        //printf("%d: ", cpt);
        //generer_dot_combine(ligne_rive, sweep_line, DCEL, filename);
    }


    libere_ligne(sweep_line);
    free_lst(ligne_rive);
    free(cellules);
    return DCEL;
}

void trace_diagramme(pointf *points, int n, SDL_Renderer *renderer, SDL_Window *fenetre) {
    list DCEL = diagramme(points, n, true);
    list courant = DCEL;
    for (int i = 0; i < n; i++) {
        tracer_point(fenetre, renderer, pointf_to_point(points[i]), RGB_black, 2);
    }
    while (courant) {
        demi_arete *b = (demi_arete *) head(courant);
        if (b->next && b->debut.x != -100000.0) {
            point p1 = pointf_to_point(b->debut);
            point p2 = pointf_to_point(b->next->debut);
            tracer_ligne(fenetre, renderer, p1, p2, RGB_gray, !b->complete);
        }


        courant = tail(courant);
    }
    SDL_RenderPresent(renderer);
    free_list(DCEL);
}

void genere_csv(int max_points, int pas, int nb_tests) {
    FILE *f = fopen("data.csv", "w");
    fprintf(f, "N,Time,Time/N\n");
    for (int i = 10; i < max_points; i += pas) {

        double s = 0;
        for (int j = 0; j < nb_tests; j++) {
            clock_t start = clock();
            pointf *sites = genere_points(i);
            list ans = diagramme(sites, i, false);
            clock_t end = clock();
            free_list(ans);
            s += (double) (end - start) / CLOCKS_PER_SEC;
            free(sites);
        }
        printf("%d\n", i);
        s /= 10;
        fprintf(f, "%d;%f;%f\n", i, s, s / i);
    }
    fclose(f);
}


int main(int argc, char **argv) {
    /*

    */
    /*
    pointf sites[20] = {
            {9, 150},
            {488, 89},
            {240, 20},
            {98, 256},
            {446, 103},
            {110, 184},
            {65, 292},
            {290, 41},
            {230, 341},
            {31, 476},
            {300, 201},
            {63, 371},
            {250, 188},
            {426, 493},
            {44, 235},
            {330, 9},
            {480, 387},
            {144, 47},
            {451, 268},
            {58, 291}};*/

    pointf *sites2 = genere_points(N);
    pointf *vitesses = genere_vitesses(N);


    SDL_Window *fenetre = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_init(&renderer, &fenetre);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    trace_diagramme(sites2, N, renderer, fenetre);
    SDL_Event event;

    bool quit = false;
    while (!quit) {
        SDL_PollEvent(&event);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        maj(sites2, vitesses, 0.1, N);
        trace_diagramme(sites2, N, renderer, fenetre);
        SDL_Delay(16);

        switch (event.type) {
            case SDL_QUIT:
                quit = true;
        }
    }
    free(vitesses);
    free(sites2);


    return 0;
}