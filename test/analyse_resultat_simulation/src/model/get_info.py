from analyse_resultat_simulation.src.view.config import CELLS_SIZE


def get_extreme_points(sorted_test_x_2, sorted_test_y_2):
    minX = sorted_test_x_2[0][0]
    maxX = sorted_test_x_2[-1][0]
    minY = sorted_test_y_2[0][1]
    maxY = sorted_test_y_2[-1][1]
    cells_boundaries = (minX, minY, maxX, maxY)
    return cells_boundaries, minX, minY


def get_useful_information(limites, new_list):
    widthCanvas = limites[1] - limites[0]
    heightCanvas = limites[3] - limites[2]
    if 'orig_cellule_coords' in new_list[0]:
        widthGrid = new_list[-1]['orig_cellule_coords'][0] - \
            new_list[0]['orig_cellule_coords'][0] + CELLS_SIZE
        heightGrid = new_list[-1]['orig_cellule_coords'][1] - \
            new_list[0]['orig_cellule_coords'][1] + CELLS_SIZE
    else:
        widthGrid = new_list[-1]['cellule_coords'][0] - \
            new_list[0]['cellule_coords'][0] + CELLS_SIZE
        heightGrid = new_list[-1]['cellule_coords'][1] - \
            new_list[0]['cellule_coords'][1] + CELLS_SIZE
    factorWidth = widthCanvas / widthGrid
    factorHeight = heightCanvas / heightGrid
    return factorHeight, factorWidth, heightCanvas, widthCanvas
