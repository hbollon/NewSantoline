from __future__ import annotations

from tkinter import *


def default_function():
    print("Hello World!")


class TkInterface:
    CANVAS_PADDING = 30

    def __init__(self, title: str = "No title", resizable: bool = False) -> None:
        self.window = Tk(className=title)
        self.canvas = None
        self.text_zones = {}
        self.labels = {}
        self.checkbox = {}
        if not resizable:
            self.window.resizable(0, 0)

    def get_canvas_dimensions(self) -> (int, int):
        if self.canvas is None:
            return -1, -1
        return self.canvas.winfo_width() - self.CANVAS_PADDING, self.canvas.winfo_height() - self.CANVAS_PADDING

    def get_extreme_coordinates_canvas_content(self) -> (int, int, int, int):
        (width, height) = self.get_canvas_dimensions()
        if width != -1 and height != -1:
            return int(-width / 2), int(width / 2), int(-height / 2), int(height / 2)
        else:
            return -1, -1, -1, -1

    def __add_widget(self, widget, side: str) -> TkInterface:
        widget.pack(side=side, padx=5, pady=5)
        return self

    def add_button(self, text: str = "Default text", command=default_function,
                   position: str = CENTER, root=None) -> TkInterface:
        if root is None:
            root = self.window

        button = Button(root, text=text, command=command)
        return self.__add_widget(widget=button, side=position)

    def add_checkbox(self, ident: str, text: str = "Default text", checked: bool = False,
                     position: str = CENTER, root=None) -> TkInterface:
        if root is None:
            root = self.window

        val = BooleanVar(value=checked)
        self.checkbox[ident] = [Checkbutton(root, text=text, var=val), val]
        if checked:
            self.checkbox[ident][0].select()
        return self.__add_widget(widget=self.checkbox[ident][0], side=position)

    def is_checkbox_checked(self, ident: str) -> bool:
        cb = self.checkbox.get(ident)
        if cb is not None:
            return cb[1].get()
        return False

    def add_canvas(self, width: int = 1200, height: int = 800, bg: str = "white",
                   position: str = CENTER, root=None) -> TkInterface:
        if root is None:
            root = self.window

        self.canvas = Canvas(root, width=width, height=height, bg=bg)
        return self.__add_widget(widget=self.canvas, side=position)

    def add_label(self, ident: str, text: str = "Default text", font: int = 16, position: str = TOP, root=None)\
            -> TkInterface:
        if root is None:
            root = self.window

        self.labels[ident] = Label(root, text=text, font=font)
        return self.__add_widget(widget=self.labels[ident], side=position)

    def label_set_text(self, ident: str, text: str) -> TkInterface:
        self.labels[ident]['text'] = text
        return self

    def add_text_zone(self, ident: str, text: str = "Default text", width: int = -1, height: int = 100,
                      scrollable: bool = True, position: str = BOTTOM, root=None) -> TkInterface:
        if width == -1:
            width = self.get_canvas_dimensions()[0] + self.CANVAS_PADDING

        if root is None:
            root = self.window

        entry_frame = Frame(root, width=width, height=height)
        self.__add_widget(widget=entry_frame, side=position)

        scrollbar_width = 0
        if scrollable:
            scrollbar_width = 18

        self.text_zones[ident] = Text(entry_frame)
        self.text_zones[ident].place(
            x=0, y=0, height=height, width=width - scrollbar_width)

        if scrollable:
            scrollbar = Scrollbar(entry_frame, orient=VERTICAL)
            scrollbar.place(x=width - scrollbar_width, y=0, height=height)
            scrollbar.config(command=self.text_zones[ident].yview)
            self.text_zones[ident].config(yscrollcommand=scrollbar.set)

        self.text_zone_change_text(ident, text)
        return self

    def text_zone_change_text(self, ident: str, text: str) -> TkInterface:
        if len(self.text_zones) == 0:
            pass
        else:
            self.text_zones[ident].delete(1.0, END)
            self.text_zones[ident].insert(END, text)
        return self

    def start(self) -> None:
        self.window.mainloop()
