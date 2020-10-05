from ..libs import singleton

import os
import time
import json


class FileIO(metaclass=singleton.Singleton):

    def write(self, filename, message):
        writing_file = open(filename, "w+")
        writing_file.write(message)
        writing_file.close()

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
        print(content)
        return content

    def close(self):
        self._pipe.close()
