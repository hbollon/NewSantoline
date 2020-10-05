def sort_cells_by_x_then_y(arr):
    return sorted(arr, key=lambda k: (k['cellule_coords'][0], k['cellule_coords'][1]))


def sort_points_by_x_then_y(arr):
    return sorted(arr, key=lambda k: (k[0], k[1]))
