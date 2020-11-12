APP_TITLE = "Résultats de simulation"

WINDOW_RESIZABLE = False

ALGORITHM_PARAMS_PATH = "../../../../data/communication/parametreAlgo.json"
GRID_DATA_PATH = "../../../../data/communication/sortie.json"
ALGORITHM_DETAILS_PATH = "../../../../data/communication/infos.json"
POINTS_DATA_PATH = "../../../../data/communication/points.json"
VERTEX_LIST_PATH = "../../../../data/communication/listeSommets.json"

# will pb. never change
CELLS_SIZE = 25

BURNING_POINT_SIZE = 8

CANVAS_HEIGHT = 600
CANVAS_WIDTH = 1200

DOTS_DETAILS_ZONE_HEIGHT = 100
DOTS_DETAILS_SCROLLABLE = True

GRID_SQUARE_WIDTH = 2

RATE_OF_SPREAD_WIDTH = 2

BURNING_POINTS_ARE_SHOWN = True

# when set to false, you don't see any propagation speed
PROPAGATION_SPEED_IS_SHOWN = True
PROPAGATION_SPEED_WIDTH = 2
ZOOM_PROPAGATION_SPEED = 2.5

INITIAL_OUTLINE_WIDTH = 4
DOT_SIZE = 7

BURNING_DOT_SIZE = 10

# When set at False, the dot animation is negated
DOT_ANIMATION = False
# [0-10] -> 1 the slowest, 0 the fastest (only works with DOT_ANIMATION at True)
DOT_ANIMATION_SPEED = 0

SIZE_HEAD_ARROW = 3

# If the distance btw two points is <=, then the color will be INCORRECT_POINT_COLOR.
DISTANCE_MIN_DOTS = 1.0

# Should be >= 1.
ZOOM_EVENT_NB_SQUARES_AROUND = 3

RELOAD_BUTTON_TEXT = "Relancer"
KEEP_TEST_FOR_NEXT_CHECKBOX_TEXT = "Conserver contour pour prochain test"

GRID_COLOR = "silver"
RATE_OF_SPREAD_COLOR = "silver"
PROPAGATION_SPEED_COLOR = "midnightblue"
PREVIOUS_IGNI_POINTS = "burlywood"
IGNI_POINTS_COLOR = "steelblue"
INCORRECT_POINT_COLOR = "red"
INITIAL_OUTLINE_COLOR = "limegreen"
BURNING_POINT_COLOR = "firebrick"
NOT_BURNING_POINT_COLOR = "black"

# nb of empty squares (axe x,y) around useful squares
NB_SQUARES_SUP = 4
