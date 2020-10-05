def draw_square(pos, size, window):
    window.turtle_move_to_without_trace(
        x=pos["x"],
        y=pos["y"]
    ).turtle_move_to(
        x=pos["x"] + size["x"],
        y=pos["y"]
    ).turtle_move_to(
        x=pos["x"] + size["x"],
        y=pos["y"] + size["y"]
    ).turtle_move_to(
        x=pos["x"],
        y=pos["y"] + size["y"]
    ).turtle_move_to(
        x=pos["x"],
        y=pos["y"]
    )


def draw_triangle(cx, cy, size, window):
    window.turtle_move_to_without_trace(
        x=cx - size,
        y=cy - size
    ).turtle_begin_fill(

    ).turtle_move_to(
        x=cx - size,
        y=cy + size
    ).turtle_move_to(
        x=cx + size,
        y=cy
    ).turtle_move_to(
        x=cx - size,
        y=cy - size
    ).turtle_end_fill()


def draw_arrow(pos, size_head, size_tail, window):
    window \
        .turtle_move_to_without_trace(x=pos["x"], y=pos["y"]) \
        .turtle_move_to(x=pos["x"] + size_tail["x"], y=pos["y"] + size_tail["y"])
    draw_triangle(
        cx=pos["x"] + size_tail["x"],
        cy=pos["y"] + size_tail["y"],
        size=size_head,
        window=window
    )
