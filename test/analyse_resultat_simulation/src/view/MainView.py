from tkinter import constants

from analyse_resultat_simulation.src.model.tk_interface.TkInterfaceWithTurtle import TkInterfaceWithTurtle
from ..model.color import set_correspondent_point_color, set_correspondent_burning_point_color
from ..model.conversion import adapt_data_to_canvas, convert_to_displayable_data, \
    adapt_points_with_details_coords_to_canvas, convert_burning_p_to_displayable_data
from ..model.creation import create_everything
from ..model.data_container.PointContainer import PointContainer
from ..model.data_container.SquaresContainer import SquaresContainer
from ..model.drawing import draw_grid_square_rate_of_spread, draw_grid_square, draw_point_propagation_speed
from ..model.get_info import get_extreme_points, get_useful_information
from ..model.optim import remove_unused_cells_from_grid
from ..model.sort import sort_cells_by_x_then_y, sort_points_by_x_then_y
from .config import *
import json

from ..utils.drawing import draw_triangle


class MainView:
    ID_TEXT_ZONE = "1"
    ID_LABEL = "1"
    ID_SAVED_LABEL = "2"
    ID_CHECKBOX = "1"
    DEFAULT_TEXT_LABEL = ""

    def __init__(self):
        self.detail_algorithm = None
        self.initial_outline = None
        self.converted_grid = SquaresContainer()
        self.saved_detail_algorithm = None
        self.saved_detailed_points = PointContainer()
        self.vertex_list = None
        self.points = None
        self.converted_points = None
        self.converted_detailed_points = PointContainer()
        self.factor_width = 0
        self.factor_height = 0
        self.limites = None
        self.window = None
        self.zoom_grid = None
        self.zoom_nb_squares = ZOOM_EVENT_NB_SQUARES_AROUND

        self.construct_and_display_interface()

    def construct_and_display_interface(self):
        self.window = TkInterfaceWithTurtle(
            APP_TITLE, resizable=WINDOW_RESIZABLE)
        self.window \
            .add_canvas(width=CANVAS_WIDTH, height=CANVAS_HEIGHT, position=constants.TOP) \
            .add_turtle_to_canvas() \
            .add_label(ident=self.ID_LABEL, position=constants.BOTTOM, text=self.DEFAULT_TEXT_LABEL) \
            .add_label(ident=self.ID_SAVED_LABEL, position=constants.BOTTOM, text=self.DEFAULT_TEXT_LABEL) \
            .add_text_zone(ident=self.ID_TEXT_ZONE, height=DOTS_DETAILS_ZONE_HEIGHT,
                           scrollable=DOTS_DETAILS_SCROLLABLE, position=constants.BOTTOM) \
            .add_button(text=RELOAD_BUTTON_TEXT, command=self.display_main_window, position=constants.LEFT) \
            .add_checkbox(ident=self.ID_CHECKBOX, text=KEEP_TEST_FOR_NEXT_CHECKBOX_TEXT, position=constants.LEFT)
        self.display_main_window()
        self.window.start()

    def display_main_window(self):
        self.save_actual_state()
        self.window.init_turtle()
        self.limites = self.window.get_extreme_coordinates_canvas_content()
        initial_outline, sorted_test_x_2, sorted_test_x_y, sorted_test_y_2, points, points_details, algorithm_details, \
            vertex_list = create_everything()
        self.vertex_list = vertex_list
        points = sort_points_by_x_then_y(points)
        self.detail_algorithm = algorithm_details
        cells_boundaries, minX, minY = get_extreme_points(
            sorted_test_x_2, sorted_test_y_2)
        newList = remove_unused_cells_from_grid(
            cells_boundaries, sorted_test_x_y)
        newList = sort_cells_by_x_then_y(newList)
        factorHeight, factorWidth, heightCanvas, widthCanvas = get_useful_information(
            self.limites, newList)
        self.set_factors(factorHeight, factorWidth)
        convertedGrid, convertedPoints, converted_detailed_points, converted_old_detailed_points, \
            convertedInitial_outline, converted_burning_points = adapt_data_to_canvas(
                self.factor_height, self.factor_width, heightCanvas, widthCanvas,
                newList, points, self.saved_detailed_points, points_details, initial_outline
            )
        self.points = points
        self.converted_points = convertedPoints
        self.saved_detailed_points.set(converted_old_detailed_points)
        self.initial_outline = convertedInitial_outline
        self.converted_grid.set(squares=convertedGrid,
                                factor=(factorWidth, factorHeight))
        self.converted_detailed_points.set(points=converted_detailed_points)
        self.window \
            .set_on_click_event(lambda x, y: self.on_click_event(window=self.window,
                                                                 points=self.converted_detailed_points,
                                                                 saved_points=self.saved_detailed_points,
                                                                 x=x, y=y)) \
            .set_on_right_click_event(self.on_right_click_event)
        self.display_everything_in_canvas(self.converted_grid.squares, self.initial_outline,
                                          convertedPoints)

    def display_zoom_window(self):
        newW = TkInterfaceWithTurtle(APP_TITLE) \
            .add_canvas(width=CANVAS_WIDTH, height=CANVAS_HEIGHT, position=constants.TOP) \
            .add_turtle_to_canvas() \
            .add_text_zone(ident=self.ID_TEXT_ZONE, height=DOTS_DETAILS_ZONE_HEIGHT,
                           scrollable=DOTS_DETAILS_SCROLLABLE, position=constants.BOTTOM) \
            .init_turtle()
        factorHeight, factorWidth, heightCanvas, widthCanvas = get_useful_information(self.limites,
                                                                                      self.zoom_grid.squares)
        zoom_points = []
        for converted, point in zip(self.converted_points, self.points):
            if self.zoom_grid.is_in_boundaries(converted):
                zoom_points.append(point)
        d = []
        # pb: ordre des points
        c_d_p_copy = json.loads(json.dumps(
            self.converted_detailed_points.points))
        for point in c_d_p_copy:
            if self.zoom_grid.is_in_boundaries(point["coordonnees"]):
                d.append(point)
        convertedGrid, convertedPoints, cv_d_points, converted_old_detailed_points, \
            convertedInitial_outline, converted_burning_points = adapt_data_to_canvas(
                factorHeight, factorWidth, heightCanvas, widthCanvas,
                self.zoom_grid.squares, zoom_points, burning_points=self.vertex_list
            )
        converted_d = adapt_points_with_details_coords_to_canvas(
            factor_height=factorHeight, factor_width=factorWidth, height_canvas=heightCanvas,
            new_list=self.zoom_grid.squares, points_details=d,
            width_canvas=widthCanvas)
        self.display_grid(newW, convertedGrid, factorWidth, factorHeight)

        if BURNING_POINTS_ARE_SHOWN:
            burning_points = []
            for b_point in converted_burning_points:
                if (-widthCanvas/2 - CELLS_SIZE <= b_point["coordonnees"][0] <= widthCanvas/2
                        and -heightCanvas/2 - CELLS_SIZE <= b_point["coordonnees"][1] <= heightCanvas/2):
                    burning_points.append(b_point)
        self.display_burning_points(window=newW, burning_points=burning_points,
                                    factor_width=factorWidth, factor_height=factorHeight)

        self.display_igni_points(
            newW, convertedPoints, factorWidth, factorHeight)

        if PROPAGATION_SPEED_IS_SHOWN:
            self.display_propagation_speed(points=converted_d, window=newW,
                                           factor_width=factorWidth, factor_height=factorHeight)

        self.display_text_zone_text(window=newW,
                                    actual_points_to_display=convert_to_displayable_data(
                                        converted_d),
                                    old_points_to_display=convert_burning_p_to_displayable_data(burning_points))
        newW \
            .turtle_set_animation(activation=True) \
            .set_on_click_event(lambda x, y: self.zoom_on_click_event(window=newW,
                                                                      points=PointContainer(
                                                                          converted_d),
                                                                      x=x, y=y,
                                                                      burning_points=PointContainer(burning_points))) \
            .start()
        self.zoom_grid = None

    def save_actual_state(self):
        if self.window.is_checkbox_checked(self.ID_CHECKBOX):
            self.save_detailed_points()
        else:
            self.saved_detailed_points.clear()

    def set_factors(self, factorHeight, factorWidth):
        self.factor_width = factorWidth
        self.factor_height = factorHeight

    def save_detailed_points(self):
        self.saved_detailed_points.set(self.converted_detailed_points.points)
        self.saved_detail_algorithm = self.detail_algorithm

    def is_set_saved_detailed_points(self):
        return not self.saved_detailed_points.is_empty()

    def on_click_event(self, window, points, x, y, saved_points=None):
        actual_points = points.get(x=x, y=y, radius=DOT_SIZE)
        actual_points_to_display = convert_to_displayable_data(actual_points)
        old_points_to_display = None
        if saved_points is not None:
            old_points = saved_points.get(x=x, y=y, radius=DOT_SIZE)
            old_points_to_display = convert_to_displayable_data(old_points)
        self.display_text_zone_text(window=window,
                                    actual_points_to_display=actual_points_to_display,
                                    old_points_to_display=old_points_to_display)

    def zoom_on_click_event(self, window, points, x, y, burning_points=None):
        actual_points = points.get(x=x, y=y, radius=DOT_SIZE)
        actual_points_to_display = convert_to_displayable_data(actual_points)
        b_points_to_display = None
        if burning_points is not None:
            b_points = burning_points.get(x=x, y=y, radius=BURNING_DOT_SIZE)
            b_points_to_display = convert_burning_p_to_displayable_data(
                b_points)
        self.display_text_zone_text(window=window,
                                    actual_points_to_display=actual_points_to_display,
                                    old_points_to_display=b_points_to_display)

    def on_right_click_event(self, x, y):
        self.zoom_grid = self.converted_grid.get_squares_around(
            x, y, self.zoom_nb_squares)
        self.display_zoom_window()

    def display_text_zone_text(self, window, actual_points_to_display,
                               old_points_to_display=None):
        old_p = ''
        if old_points_to_display is not None:
            old_p = convert_to_displayable_str(old_points_to_display)
        window.text_zone_change_text(self.ID_TEXT_ZONE,
                                     convert_to_displayable_str(
                                         actual_points_to_display)
                                     + "\n---------------------------\n" + old_p)

    def display_everything_in_canvas(self, converted_grid, converted_initial_outline, converted_points):
        self.display_infos()
        self.display_grid(self.window, converted_grid,
                          self.factor_width, self.factor_height)
        self.display_init_outline(converted_initial_outline)

        if self.is_set_saved_detailed_points():
            self.display_saved_igni_points()
        self.display_igni_points(
            self.window, converted_points, self.factor_width, self.factor_height)

        if PROPAGATION_SPEED_IS_SHOWN:
            self.display_propagation_speed(points=self.converted_detailed_points.points, window=self.window,
                                           factor_width=self.factor_width, factor_height=self.factor_height)

    def display_grid(self, window, converted_grid, factor_width, factor_height):
        window.turtle_set_animation(activation=False)
        self.draw_grid_squares(window, converted_grid,
                               factor_width, factor_height)
        self.draw_rate_of_spread(
            window, converted_grid, factor_width, factor_height)
        window.turtle_set_animation(activation=True)

    def draw_rate_of_spread(self, window, converted_grid, factor_width, factor_height):
        window \
            .turtle_set_color(color=RATE_OF_SPREAD_COLOR) \
            .turtle_set_pen_size(pen_size=RATE_OF_SPREAD_WIDTH)
        for grid_square in converted_grid:
            draw_grid_square_rate_of_spread(
                grid_square['great_axis'], grid_square['great_axis_dir'], grid_square['cellule_coords'],
                factor_height, factor_width, window
            )

    def display_infos(self):
        self.window.label_set_text(
            ident=self.ID_LABEL, text=json.dumps(self.detail_algorithm))
        if self.window.is_checkbox_checked(self.ID_CHECKBOX):
            self.window.label_set_text(
                ident=self.ID_SAVED_LABEL, text=json.dumps(self.saved_detail_algorithm))

    def draw_grid_squares(self, window, converted_grid, factor_width, factor_height):
        window \
            .turtle_set_color(color=GRID_COLOR) \
            .turtle_set_pen_size(pen_size=GRID_SQUARE_WIDTH)
        for grid_square in converted_grid:
            draw_grid_square(
                grid_square['cellule_coords'],
                factor_height, factor_width, window
            )

    def display_init_outline(self, converted_initial_outline):
        self.window.turtle_set_animation(activation=False)
        self.window \
            .turtle_set_color(INITIAL_OUTLINE_COLOR) \
            .turtle_set_speed(speed=DOT_ANIMATION_SPEED) \
            .turtle_set_pen_size(pen_size=INITIAL_OUTLINE_WIDTH)

        initial_point = {
            "x": converted_initial_outline["contourInitial"][0][0],
            "y": converted_initial_outline["contourInitial"][0][1]
        }

        self.window.turtle_move_to_without_trace(
            x=initial_point["x"], y=initial_point["y"]
        )
        for i in range(len(converted_initial_outline["contourInitial"])):
            self.window.turtle_move_to(
                x=converted_initial_outline["contourInitial"][i][0], y=converted_initial_outline["contourInitial"][i][1]
            )
        self.window.turtle_move_to(
            x=initial_point["x"], y=initial_point["y"]
        )
        self.window.turtle_set_animation(activation=True)

    def display_saved_igni_points(self):
        points = self.saved_detailed_points.points

        if points is not None:
            self.window.turtle_set_animation(activation=False) \
                .turtle_set_speed(speed=DOT_ANIMATION_SPEED) \
                .turtle_set_color(color=PREVIOUS_IGNI_POINTS)

            for i in range(len(points)):
                self.window.turtle_write_dot(
                    x=points[i]['coordonnees'][0],
                    y=points[i]['coordonnees'][1],
                    size=DOT_SIZE
                )

            self.window.turtle_set_animation(activation=False)

    def display_igni_points(self, window, converted_points, factor_width, factor_height):
        window.turtle_set_animation(activation=DOT_ANIMATION)
        if DOT_ANIMATION:
            window.turtle_set_speed(speed=DOT_ANIMATION_SPEED)

        c_p_size = len(converted_points)
        for i in range(c_p_size):
            previous_coords = converted_points[(i - 1) % c_p_size]
            next_coords = converted_points[(i + 1) % c_p_size]

            set_correspondent_point_color(converted_points[i], previous_coords, next_coords,
                                          factor_width, factor_height, window)
            window.turtle_write_dot(
                x=converted_points[i][0], y=converted_points[i][1],
                size=DOT_SIZE
            )

    def display_propagation_speed(self, points, window, factor_width, factor_height):
        window \
            .turtle_set_color(PROPAGATION_SPEED_COLOR) \
            .turtle_set_pen_size(PROPAGATION_SPEED_WIDTH) \
            .turtle_set_animation(activation=False)
        for point in points:
            draw_point_propagation_speed(
                coords=point["coordonnees"], vecteur_vitesse=point["vitesse"],
                factor_width=factor_width, factor_height=factor_height, window=window
            )
        window.turtle_set_animation(activation=True)

    def display_burning_points(self, window, burning_points, factor_width, factor_height):
        window.turtle_set_animation(activation=DOT_ANIMATION)
        if DOT_ANIMATION:
            window.turtle_set_speed(speed=DOT_ANIMATION_SPEED)
        for i in range(len(burning_points)):
            set_correspondent_burning_point_color(
                burning_points[i], self.detail_algorithm[0]["temps"], window)
            window.turtle_write_dot(
                x=burning_points[i]['coordonnees'][0], y=burning_points[i]['coordonnees'][1],
                size=BURNING_DOT_SIZE
            )
        window.turtle_set_animation(activation=False)


def convert_to_displayable_str(actual_points_to_display):
    return json.dumps(actual_points_to_display).replace("},", "},\n")
