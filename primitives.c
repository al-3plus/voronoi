
#include "primitives.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


void SDL_init(SDL_Renderer **renderer, SDL_Window **fenetre) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_ExitError("Initialisaition");
    }
    /// creation de la fenetre et du renderer
    *fenetre = SDL_CreateWindow("Fenêtre", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    *renderer = SDL_CreateRenderer(*fenetre, -1, SDL_RENDERER_SOFTWARE);

    if (*fenetre == NULL) {
        SDL_ExitError("Echec de création de la fenêtre ");
    }

    if (*renderer == NULL) {
        SDL_ExitError("Echec de la creation du renderer");
    }

    printf("Initilisation SDL OK\n");
}

void SDL_exit(SDL_Renderer **renderer, SDL_Window **fenetre, int delay) {
    SDL_Delay(delay * 1000);
    SDL_DestroyRenderer(*renderer);
    SDL_DestroyWindow(*fenetre);
    printf("Sortie SDL OK\n");

    SDL_Quit();
}

void SDL_ExitError(const char *message) {
    SDL_Log("Erreur : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

point vers_grille(int x, int y) {
    point p;
    p.x = x + WIDTH / 2;
    p.y = (-y + HEIGHT / 2);
    return p;
}


void put_pixel(SDL_Window *fenetre, SDL_Renderer *renderer, point p, RGB couleur) {
    int x = p.x;
    int y = p.y;
    if (SDL_SetRenderDrawColor(renderer, couleur.R, couleur.G, couleur.B, SDL_ALPHA_OPAQUE) != 0) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        SDL_ExitError("Echec du changement de la couleur du pinceau");
    }
    if (SDL_RenderDrawPoint(renderer, x, y) != 0) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        SDL_ExitError("Echec du placement d'un point");
    }
}

void tracer_point(SDL_Window *fenetre, SDL_Renderer *renderer, point p, RGB couleur, int taille) {
    int x = p.x;
    int y = p.y;
    for (int i = x - taille * taille / 2; i < x + taille * taille / 2; i++) {
        for (int j = y - taille * taille / 2; j < y + taille * taille / 2; j++) {
            if ((i - x) * (i - x) + (j - y) * (j - y) <= taille * taille) {
                put_pixel(fenetre, renderer, vers_grille(i, j), couleur);
            }
        }
    }
}


///Algorithme de Bresenham
void tracer_ligne(SDL_Window *fenetre, SDL_Renderer *renderer, point p1, point p2, RGB couleur, bool dashed) {
    int x1 = p1.x, y1 = p1.y;
    int x2 = p2.x, y2 = p2.y;

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;
    int i = 0;
    while (!(x1 == x2 && y1 == y2)) {
        if (!(dashed) || (i / 10) % 2 == 0) {
            put_pixel(fenetre, renderer, vers_grille(x1, y1), couleur);
        }
        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
        i++;
    }
}
