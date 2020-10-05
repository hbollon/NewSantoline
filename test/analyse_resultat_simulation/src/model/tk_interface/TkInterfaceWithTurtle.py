from __future__ import annotations

import turtle

from analyse_resultat_simulation.src.model.tk_interface.TkInterface import TkInterface


def default_function_onclick(x: int, y: int):
    print("<click> Click :", x, y)


def default_function_on_right_click(x: int, y: int):
    print("<right-click> Click :", x, y)


class TkInterfaceWithTurtle(TkInterface):
    HISTORY_SIZE = 10000

    def __init__(self, title: str = "No title", resizable: bool = False):
        super().__init__(title, resizable)
        self.screen = None
        self.turtle = None

    def add_turtle_to_canvas(self) -> TkInterfaceWithTurtle:
        if self.canvas is None:
            pass
        else:
            self.screen = turtle.TurtleScreen(self.canvas)
            self.turtle = turtle.RawTurtle(self.screen)
            self.init_turtle()
        return self

    def init_turtle(self) -> TkInterfaceWithTurtle:
        if self.canvas is None:
            pass
        else:
            self.turtle.clear()
            self.turtle.reset()
            self.turtle.ht()
            self.turtle.setundobuffer(self.HISTORY_SIZE)
        return self

    def turtle_move_to(self, x: int = 0, y: int = 0) -> TkInterfaceWithTurtle:
        self.turtle.goto(x, y)
        return self

    def turtle_move_to_without_trace(self, x: int = 0, y: int = 0) -> TkInterfaceWithTurtle:
        self.turtle.penup()
        self.turtle_move_to(x, y)
        self.turtle.pendown()
        return self

    def turtle_set_animation(self, activation: bool = True) -> TkInterfaceWithTurtle:
        self.screen.tracer(activation)
        return self

    def turtle_set_speed(self, speed: int = 5) -> TkInterfaceWithTurtle:
        self.turtle.speed(speed)
        return self

    def turtle_set_pen_size(self, pen_size: int = 2) -> TkInterfaceWithTurtle:
        self.turtle.pensize(pen_size)
        return self

    def turtle_set_color(self, color: str = "red") -> TkInterfaceWithTurtle:
        self.turtle.color(color)
        return self

    def turtle_write_dot(self, x: int = None, y: int = None, size: int = None) -> TkInterfaceWithTurtle:
        if x is not None and y is not None:
            self.turtle_move_to_without_trace(x, y)
        self.turtle.dot(size=size)
        return self

    def turtle_undo_last_action(self) -> TkInterfaceWithTurtle:
        self.turtle.undo()
        return self

    def turtle_begin_fill(self, color: str = 'black') -> TkInterfaceWithTurtle:
        self.turtle.fillcolor(color)
        self.turtle.begin_fill()
        return self

    def turtle_end_fill(self) -> TkInterfaceWithTurtle:
        self.turtle.end_fill()
        return self

    def set_on_click_event(self, fun=default_function_onclick) -> TkInterfaceWithTurtle:
        self.screen.onclick(fun, add=True)
        return self

    def set_on_right_click_event(self, fun=default_function_on_right_click) -> TkInterfaceWithTurtle:
        self.screen.onclick(fun, btn=3)
        return self
