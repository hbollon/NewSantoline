def adapt_coords_to_canvas(factor, min_val, point, size_canvas):
    return (
        (point["x"] - min_val["x"]) * factor["x"] - size_canvas["x"] / 2,
        (point["y"] - min_val["y"]) * factor["y"] - size_canvas["y"] / 2
    )
