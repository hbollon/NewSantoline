from analyse_resultat_simulation.src.view.config import CELLS_SIZE


def coords_in_range(cell, rad_x, rad_y, coords: (float, float)) -> bool:
    c_coords = cell['cellule_coords']
    return c_coords[0] - rad_x <= coords[0] <= c_coords[0] + rad_x \
        and c_coords[1] - rad_y <= coords[1] <= c_coords[1] + rad_y


class SquaresContainer:
    def __init__(self, squares=None, factor=None):
        if squares is None:
            self.squares = []
            self.factor = (0, 0)
            self.boundaries = (0.0, 0.0, 0.0, 0.0)
        else:
            self.squares = squares
            self.factor = factor
            self.boundaries = (squares[0]['cellule_coords'][0],
                               squares[0]['cellule_coords'][1],
                               squares[-1]['cellule_coords'][0],
                               squares[-1]['cellule_coords'][1])

    def set(self, squares=None, factor=None):
        if squares is None:
            self.squares = []
        else:
            self.squares = squares
            self.factor = factor
            self.boundaries = (squares[0]['cellule_coords'][0],
                               squares[0]['cellule_coords'][1],
                               squares[-1]['cellule_coords'][0],
                               squares[-1]['cellule_coords'][1])

    def get_squares(self):
        return self.squares

    def get_squares_around(self, x: float, y: float, nb_squares: int):
        rad = (nb_squares * CELLS_SIZE *
               self.factor[0], nb_squares * CELLS_SIZE * self.factor[1])
        squares = []
        for square in self.squares:
            if coords_in_range(square, rad[0], rad[1], (x, y)):
                squares.append(square)
        return SquaresContainer(squares=squares, factor=self.factor)

    def is_in_boundaries(self, coords: (float, float)) -> bool:
        return (self.boundaries[0] <= coords[0] <= self.boundaries[2]
                and self.boundaries[1] <= coords[1] <= self.boundaries[3])
