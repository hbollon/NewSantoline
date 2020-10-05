from analyse_resultat_simulation.src.utils.drawing import draw_arrow, draw_square, draw_triangle
from analyse_resultat_simulation.src.view.config import SIZE_HEAD_ARROW, CELLS_SIZE, ZOOM_PROPAGATION_SPEED, GRID_COLOR, \
    BURNING_POINT_COLOR, BURNING_POINT_SIZE


def draw_grid_square_rate_of_spread(axis, axis_dir, coords, factor_height, factor_width, window):
    pos = {"x": coords[0] + (CELLS_SIZE * factor_width) / 2, "y": coords[1] + (CELLS_SIZE * factor_height) / 2}
    size_tail = {"x": axis_dir[0] * axis, "y": axis_dir[1] * axis}
    size_head = SIZE_HEAD_ARROW

    draw_arrow(pos, size_head, size_tail, window)


def draw_point_propagation_speed(coords, vecteur_vitesse, factor_width, factor_height, window):
    pos = {"x": coords[0], "y": coords[1]}
    size_tail = {"x": vecteur_vitesse[0] * ZOOM_PROPAGATION_SPEED * factor_width,
                 "y": vecteur_vitesse[1] * ZOOM_PROPAGATION_SPEED * factor_height}
    size_head = SIZE_HEAD_ARROW

    draw_arrow(pos, size_head, size_tail, window)


def draw_grid_square(coords, factor_height, factor_width, window):
    window.turtle_set_color(color=GRID_COLOR)
    pos = {"x": coords[0], "y": coords[1]}
    size = {"x": CELLS_SIZE * factor_width, "y": CELLS_SIZE * factor_height}
    draw_square(pos=pos, size=size, window=window)


def draw_burning_point(coords, factor_height, factor_width, window):
    window.turtle_set_color(color=BURNING_POINT_COLOR)
    pos = {"x": coords[0], "y": coords[1]}
    size = {"x": BURNING_POINT_SIZE * factor_width, "y": BURNING_POINT_SIZE * factor_height}
    draw_triangle(
        cx=pos["x"],
        cy=pos["y"],
        size=size,
        window=window
    )
