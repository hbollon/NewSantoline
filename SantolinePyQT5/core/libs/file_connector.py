from . import pipe
from ..libs import singleton

import os
import time
import json

class FileConnector(metaclass=singleton.Singleton):
    def __init__(self):
        self._pipe = pipe.PipeMultiplexed()
        self._pipe.connect("centraliseur")
        self._filename = os.path.expanduser("~\\Documents\\Santoline\\santoline_communication")

    def write(self, message):
        writing_file = open(self._filename, "w+")
        writing_file.write(message)
        writing_file.close()
        self._pipe.write("fichier ecrit ")
        self._pipe.read()

    def read(self):
        read = ""
        ok = False
        while not ok:
            read = self._pipe.read()
            if (len(read) > 0):
                ok = read[0] == 'f'
        reading_file = open(self._filename, "r")
        content = '\n'.join(reading_file.readlines())
        reading_file.close()
        self._pipe.write("lecture finis")
        print(content)
        return content
        
    def close(self):
        self._pipe.close()
