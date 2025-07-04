#ifndef PRIMITIVES
#define PRIMITIVES
#include <SDL.h>
#include <stdbool.h>
#define HEIGHT 1000
#define WIDTH 1000

void SDL_ExitError(const char *);

void SDL_init(SDL_Renderer **, SDL_Window **);

void SDL_exit(SDL_Renderer **, SDL_Window **, int);

struct RGB {
    int R;
    int G;
    int B;
};
typedef struct RGB RGB;

struct point {
    int x;
    int y;
};

typedef struct point point;

point vers_grille(int, int);

void put_pixel(SDL_Window *, SDL_Renderer *, point, RGB);


void tracer_point(SDL_Window *, SDL_Renderer *, point, RGB, int);

void tracer_ligne(SDL_Window *fenetre, SDL_Renderer *renderer, point deb, point fin, RGB couleur, bool dashed);


#endif