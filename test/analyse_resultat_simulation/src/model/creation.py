from analyse_resultat_simulation.src.model.sort import sort_cells_by_x_then_y
from analyse_resultat_simulation.src.utils.files import import_json_file
from analyse_resultat_simulation.src.view.config import GRID_DATA_PATH, POINTS_DATA_PATH, ALGORITHM_PARAMS_PATH, \
    ALGORITHM_DETAILS_PATH, VERTEX_LIST_PATH


def create_everything():
    details_algorithm = create_algo_details()
    sorted_test_x_y = create_grid_cells_list()
    initial_outline = create_initial_outline()
    sorted_test_x_2, sorted_test_y_2, test_2, points_with_details = create_points_lists()
    vertex_list = create_vertex_list()
    return initial_outline, sorted_test_x_2, sorted_test_x_y, sorted_test_y_2, test_2, points_with_details,\
        details_algorithm, vertex_list


def create_grid_cells_list():
    test = import_json_file(GRID_DATA_PATH)
    sorted_test_x_y = sort_cells_by_x_then_y(test)
    return sorted_test_x_y


def create_points_lists():
    points = import_json_file(POINTS_DATA_PATH)

    # only keep speed coordinates
    points_with_details = create_list_detailed_points(points)
    test_2 = list(map(lambda x: x["vitesses"][0]["coordonnees"], points))

    sorted_test_x_2 = sorted(test_2, key=lambda k: k[0])
    sorted_test_y_2 = sorted(test_2, key=lambda k: k[1])
    return sorted_test_x_2, sorted_test_y_2, test_2, points_with_details


def create_list_detailed_points(points):
    points_with_details = list(map(lambda x: {
        "id": x["id"],
        "indice": x["indice"],
        "ancetre": x["ancetre"],
        "precedent": x["precedent"],
        "suivant": x["suivant"],
        "temps": x["temps"],
        "creation": x["creation"],
        "coordonnees_non_converties": x["vitesses"][0]["coordonnees"],
        "coordonnees": x["vitesses"][0]["coordonnees"],
        "coordonnees_calcul": x["vitesses"][0]["coordonnees_calcul"],
        "vitesse": x["vitesses"][0]["vecteur"]
    }, points))
    return points_with_details


def create_initial_outline():
    initial_outline = import_json_file(ALGORITHM_PARAMS_PATH)
    return initial_outline


def create_algo_details():
    details_algorithm = import_json_file(ALGORITHM_DETAILS_PATH)
    return details_algorithm


def create_vertex_list():
    file_content = import_json_file(VERTEX_LIST_PATH)
    vertex_list = list(map(lambda x: {
        "coordonnees_calcul": x["coordonnees_calcul"],
        "coordonnees_non_converties": x["coordonnees"],
        "coordonnees": x["coordonnees"],
        "indice_allume": x["indice_allume"],
        "t_ignition": x["t_ignition"]
    }, file_content))
    return vertex_list
