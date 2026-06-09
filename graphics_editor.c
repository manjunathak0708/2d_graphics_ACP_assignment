#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ROWS 30
#define COLS 70
#define MAX_OBJECTS 50

/* ── canvas ── */
char canvas[ROWS][COLS];

/* ── object types ── */
#define OBJ_CIRCLE    1
#define OBJ_RECT      2
#define OBJ_LINE      3
#define OBJ_TRIANGLE  4

typedef struct {
    int  type;
    int  active;
    /* circle */
    int  cx, cy, radius;
    /* rect */
    int  rx, ry, rw, rh;
    /* line */
    int  x1, y1, x2, y2;
    /* triangle */
    int  ax, ay, bx, by, ccx, ccy;
} Object;

Object objects[MAX_OBJECTS];
int    obj_count = 0;

/* ═══════════════════════════════
   Canvas helpers
═══════════════════════════════ */
void init_canvas(void)
{
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            canvas[r][c] = '_';
}

void plot(int r, int c)
{
    if (r >= 0 && r < ROWS && c >= 0 && c < COLS)
        canvas[r][c] = '*';
}

void unplot(int r, int c)
{
    if (r >= 0 && r < ROWS && c >= 0 && c < COLS)
        canvas[r][c] = '_';
}

void display_canvas(void)
{
    printf("\n");
    /* column ruler */
    printf("   ");
    for (int c = 0; c < COLS; c += 10)
        printf("%-10d", c);
    printf("\n");

    for (int r = 0; r < ROWS; r++) {
        printf("%2d |", r);
        for (int c = 0; c < COLS; c++)
            putchar(canvas[r][c]);
        printf("|\n");
    }
    printf("\n");
}

/* ═══════════════════════════════
   Drawing primitives
═══════════════════════════════ */

/* Bresenham line */
void draw_line_coords(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        plot(y1, x1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 <  dx) { err += dx; y1 += sy; }
    }
}

/* Midpoint circle */
void draw_circle_coords(int cx, int cy, int r)
{
    int x = 0, y = r;
    int d = 1 - r;
    while (x <= y) {
        plot(cy + y, cx + x); plot(cy + y, cx - x);
        plot(cy - y, cx + x); plot(cy - y, cx - x);
        plot(cy + x, cx + y); plot(cy + x, cx - y);
        plot(cy - x, cx + y); plot(cy - x, cx - y);
        if (d < 0)      d += 2 * x + 3;
        else          { d += 2 * (x - y) + 5; y--; }
        x++;
    }
}

void draw_rect_coords(int rx, int ry, int rw, int rh)
{
    for (int c = rx; c < rx + rw; c++) {
        plot(ry,          c);
        plot(ry + rh - 1, c);
    }
    for (int r = ry; r < ry + rh; r++) {
        plot(r, rx);
        plot(r, rx + rw - 1);
    }
}

void draw_triangle_coords(int ax, int ay, int bx, int by, int cx, int cy)
{
    draw_line_coords(ax, ay, bx, by);
    draw_line_coords(bx, by, cx, cy);
    draw_line_coords(cx, cy, ax, ay);
}

/* ═══════════════════════════════
   Redraw all active objects
═══════════════════════════════ */
void redraw_all(void)
{
    init_canvas();
    for (int i = 0; i < obj_count; i++) {
        if (!objects[i].active) continue;
        Object *o = &objects[i];
        switch (o->type) {
            case OBJ_CIRCLE:
                draw_circle_coords(o->cx, o->cy, o->radius); break;
            case OBJ_RECT:
                draw_rect_coords(o->rx, o->ry, o->rw, o->rh); break;
            case OBJ_LINE:
                draw_line_coords(o->x1, o->y1, o->x2, o->y2); break;
            case OBJ_TRIANGLE:
                draw_triangle_coords(o->ax, o->ay, o->bx, o->by,
                                     o->ccx, o->ccy); break;
        }
    }
}

/* ═══════════════════════════════
   Add objects
═══════════════════════════════ */
void add_circle(void)
{
    if (obj_count >= MAX_OBJECTS) { puts("Object limit reached."); return; }
    Object o = {0};
    o.type   = OBJ_CIRCLE;
    o.active = 1;
    printf("  Centre col (x, 0-%d): ", COLS-1); scanf("%d", &o.cx);
    printf("  Centre row (y, 0-%d): ", ROWS-1); scanf("%d", &o.cy);
    printf("  Radius: ");                        scanf("%d", &o.radius);
    objects[obj_count++] = o;
    redraw_all();
    printf("Circle added (id %d).\n", obj_count - 1);
}

void add_rectangle(void)
{
    if (obj_count >= MAX_OBJECTS) { puts("Object limit reached."); return; }
    Object o = {0};
    o.type   = OBJ_RECT;
    o.active = 1;
    printf("  Top-left col (x): ");  scanf("%d", &o.rx);
    printf("  Top-left row (y): ");  scanf("%d", &o.ry);
    printf("  Width  (cols): ");     scanf("%d", &o.rw);
    printf("  Height (rows): ");     scanf("%d", &o.rh);
    objects[obj_count++] = o;
    redraw_all();
    printf("Rectangle added (id %d).\n", obj_count - 1);
}

void add_line(void)
{
    if (obj_count >= MAX_OBJECTS) { puts("Object limit reached."); return; }
    Object o = {0};
    o.type   = OBJ_LINE;
    o.active = 1;
    printf("  Start col (x1): "); scanf("%d", &o.x1);
    printf("  Start row (y1): "); scanf("%d", &o.y1);
    printf("  End   col (x2): "); scanf("%d", &o.x2);
    printf("  End   row (y2): "); scanf("%d", &o.y2);
    objects[obj_count++] = o;
    redraw_all();
    printf("Line added (id %d).\n", obj_count - 1);
}

void add_triangle(void)
{
    if (obj_count >= MAX_OBJECTS) { puts("Object limit reached."); return; }
    Object o = {0};
    o.type   = OBJ_TRIANGLE;
    o.active = 1;
    printf("  Vertex A col (ax): "); scanf("%d", &o.ax);
    printf("  Vertex A row (ay): "); scanf("%d", &o.ay);
    printf("  Vertex B col (bx): "); scanf("%d", &o.bx);
    printf("  Vertex B row (by): "); scanf("%d", &o.by);
    printf("  Vertex C col (cx): "); scanf("%d", &o.ccx);
    printf("  Vertex C row (cy): "); scanf("%d", &o.ccy);
    objects[obj_count++] = o;
    redraw_all();
    printf("Triangle added (id %d).\n", obj_count - 1);
}

/* ═══════════════════════════════
   List objects
═══════════════════════════════ */
void list_objects(void)
{
    if (obj_count == 0) { puts("No objects yet."); return; }
    const char *names[] = {"", "Circle", "Rectangle", "Line", "Triangle"};
    printf("\n%-4s %-12s  Details\n", "ID", "Type");
    printf("%-4s %-12s  -------\n", "---", "----------");
    for (int i = 0; i < obj_count; i++) {
        Object *o = &objects[i];
        if (!o->active) { printf("%-4d %-12s  (deleted)\n", i, names[o->type]); continue; }
        printf("%-4d %-12s  ", i, names[o->type]);
        switch (o->type) {
            case OBJ_CIRCLE:
                printf("centre=(%d,%d) r=%d", o->cx, o->cy, o->radius); break;
            case OBJ_RECT:
                printf("top-left=(%d,%d) %dx%d", o->rx, o->ry, o->rw, o->rh); break;
            case OBJ_LINE:
                printf("(%d,%d)->(%d,%d)", o->x1, o->y1, o->x2, o->y2); break;
            case OBJ_TRIANGLE:
                printf("A(%d,%d) B(%d,%d) C(%d,%d)",
                       o->ax, o->ay, o->bx, o->by, o->ccx, o->ccy); break;
        }
        printf("\n");
    }
    printf("\n");
}

/* ═══════════════════════════════
   Delete object
═══════════════════════════════ */
void delete_object(void)
{
    list_objects();
    int id;
    printf("Enter object ID to delete (-1 to cancel): ");
    scanf("%d", &id);
    if (id < 0 || id >= obj_count) { puts("Invalid ID."); return; }
    objects[id].active = 0;
    redraw_all();
    printf("Object %d deleted.\n", id);
}

/* ═══════════════════════════════
   Modify object
═══════════════════════════════ */
void modify_object(void)
{
    list_objects();
    int id;
    printf("Enter object ID to modify (-1 to cancel): ");
    scanf("%d", &id);
    if (id < 0 || id >= obj_count || !objects[id].active) {
        puts("Invalid or deleted ID."); return;
    }
    Object *o = &objects[id];
    printf("Re-enter parameters for object %d:\n", id);
    switch (o->type) {
        case OBJ_CIRCLE:
            printf("  New centre col: "); scanf("%d", &o->cx);
            printf("  New centre row: "); scanf("%d", &o->cy);
            printf("  New radius: ");     scanf("%d", &o->radius);
            break;
        case OBJ_RECT:
            printf("  New top-left col: "); scanf("%d", &o->rx);
            printf("  New top-left row: "); scanf("%d", &o->ry);
            printf("  New width: ");        scanf("%d", &o->rw);
            printf("  New height: ");       scanf("%d", &o->rh);
            break;
        case OBJ_LINE:
            printf("  New x1: "); scanf("%d", &o->x1);
            printf("  New y1: "); scanf("%d", &o->y1);
            printf("  New x2: "); scanf("%d", &o->x2);
            printf("  New y2: "); scanf("%d", &o->y2);
            break;
        case OBJ_TRIANGLE:
            printf("  New ax: "); scanf("%d", &o->ax);
            printf("  New ay: "); scanf("%d", &o->ay);
            printf("  New bx: "); scanf("%d", &o->bx);
            printf("  New by: "); scanf("%d", &o->by);
            printf("  New cx: "); scanf("%d", &o->ccx);
            printf("  New cy: "); scanf("%d", &o->ccy);
            break;
    }
    redraw_all();
    printf("Object %d updated.\n", id);
}

/* ═══════════════════════════════
   Clear canvas
═══════════════════════════════ */
void clear_all(void)
{
    for (int i = 0; i < obj_count; i++) objects[i].active = 0;
    obj_count = 0;
    init_canvas();
    puts("Canvas cleared.");
}

/* ═══════════════════════════════
   Menus
═══════════════════════════════ */
void add_menu(void)
{
    int ch;
    printf("\n--- Add Object ---\n");
    printf("  1. Circle\n");
    printf("  2. Rectangle\n");
    printf("  3. Line\n");
    printf("  4. Triangle\n");
    printf("  0. Back\n");
    printf("Choice: ");
    scanf("%d", &ch);
    switch (ch) {
        case 1: add_circle();    break;
        case 2: add_rectangle(); break;
        case 3: add_line();      break;
        case 4: add_triangle();  break;
        default: break;
    }
}

void main_menu(void)
{
    int choice;
    do {
        printf("\n╔══════════════════════════════╗\n");
        printf("║   2D Graphics Editor (C)     ║\n");
        printf("╠══════════════════════════════╣\n");
        printf("║  1. Display canvas           ║\n");
        printf("║  2. Add object               ║\n");
        printf("║  3. Delete object            ║\n");
        printf("║  4. Modify object            ║\n");
        printf("║  5. List objects             ║\n");
        printf("║  6. Clear canvas             ║\n");
        printf("║  0. Exit                     ║\n");
        printf("╚══════════════════════════════╝\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: display_canvas(); break;
            case 2: add_menu();       break;
            case 3: delete_object();  break;
            case 4: modify_object();  break;
            case 5: list_objects();   break;
            case 6: clear_all();      break;
            case 0: puts("Goodbye!"); break;
            default: puts("Invalid choice, try again.");
        }
    } while (choice != 0);
}

/* ═══════════════════════════════
   Entry point
═══════════════════════════════ */
int main(void)
{
    init_canvas();
    main_menu();
    return 0;
}
