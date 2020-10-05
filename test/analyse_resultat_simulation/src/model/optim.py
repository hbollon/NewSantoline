from analyse_resultat_simulation.src.view.config import CELLS_SIZE, NB_SQUARES_SUP


def cell_is_relevant(cell, cells_boundaries):
    squares_around = CELLS_SIZE + NB_SQUARES_SUP * CELLS_SIZE
    return (cells_boundaries[0] - squares_around < cell[0] < cells_boundaries[2] + squares_around
            and cells_boundaries[1] - squares_around < cell[1] < cells_boundaries[3] + squares_around)


def remove_unused_cells_from_grid(cells_boundaries, sorted_test_x_y):
    newList = []
    for i in range(len(sorted_test_x_y)):
        cellActual = sorted_test_x_y[i]
        inter = cellActual['cellule_coords']
        if cell_is_relevant(inter, cells_boundaries):
            newList.append(cellActual)
    return newList
