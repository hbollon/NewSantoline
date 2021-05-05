from analyse_resultat_simulation.src.utils.calcul import distance
from analyse_resultat_simulation.src.view.config import DISTANCE_MIN_DOTS, INCORRECT_POINT_COLOR, IGNI_POINTS_COLOR, \
    BURNING_POINT_COLOR, NOT_BURNING_POINT_COLOR


def set_correspondent_burning_point_color(burning_point, time_limit, window):
    if burning_point["t_ignition"] > time_limit:
        color = NOT_BURNING_POINT_COLOR
    else:
        color = BURNING_POINT_COLOR
    window.turtle_set_color(color=color)


def set_correspondent_point_color(converted_points, previous_coords, next_coords, factor_width, factor_height,
                                  window):
    if len(previous_coords) > 0 \
            and point_is_incorrect(converted_points, factor_height, factor_width, previous_coords, next_coords):
        window.turtle_set_color(color=INCORRECT_POINT_COLOR)
    else:
        window.turtle_set_color(color=IGNI_POINTS_COLOR)


def point_is_incorrect(converted_points, factor_height, factor_width, previous_coords, next_coords):
    prev_dot = {"x": previous_coords[0] / factor_width,
                "y": previous_coords[1] / factor_height}
    actual_dot = {
        "x": converted_points[0] / factor_width, "y": converted_points[1] / factor_height}
    next_dot = {"x": next_coords[0] / factor_width,
                "y": next_coords[1] / factor_height}
    return distance(prev_dot, actual_dot) <= DISTANCE_MIN_DOTS \
        or distance(next_dot, actual_dot) <= DISTANCE_MIN_DOTS
