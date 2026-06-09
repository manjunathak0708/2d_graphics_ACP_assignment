# 2D Graphics Editor in C

A menu-driven 2D graphics editor that uses a 2D character array as a drawing canvas.

## Canvas
- Size: 30 rows × 70 columns
- Background fill character: `_` (underscore)
- Drawing character: `*` (asterisk)

## Features

### Shapes supported
| Shape     | Parameters |
|-----------|-----------|
| Circle    | Centre (col, row) and radius |
| Rectangle | Top-left corner, width, height |
| Line      | Two endpoints (x1,y1) → (x2,y2) |
| Triangle  | Three vertices A, B, C |

### Operations
- **Add** – draw a new shape onto the canvas
- **Delete** – remove a shape by its ID
- **Modify** – change the parameters of an existing shape
- **List** – view all object IDs, types, and parameters
- **Clear** – wipe the canvas and remove all objects
- **Display** – print the current canvas to the terminal

## Build & Run

```bash
# Compile
make

# Run
./graphics_editor

# Or in one step
make run
```

Requires GCC and the standard math library (`-lm`).

## Algorithms used
- **Circle** – Midpoint circle algorithm
- **Line** – Bresenham's line algorithm
- **Rectangle** – Direct boundary plotting
- **Triangle** – Three Bresenham lines connecting the vertices

## Example session
```
Choice: 2          <- Add object
Choice: 1          <- Circle
  Centre col: 35
  Centre row: 15
  Radius: 10
Choice: 1          <- Display canvas
```
