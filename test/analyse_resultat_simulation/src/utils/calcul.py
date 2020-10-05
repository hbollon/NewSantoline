import math


def distance(dot_1, dot_2):
    diff_x = (dot_2["x"] - dot_1["x"])
    diff_y = (dot_2["y"] - dot_1["y"])
    return math.sqrt(diff_x * diff_x + diff_y * diff_y)