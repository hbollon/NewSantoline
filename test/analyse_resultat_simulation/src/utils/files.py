import json
import os


def import_json_file(relative_path: str) -> json:
    with open(os.path.join(os.path.dirname(__file__), relative_path), 'r') as f:
        return json.loads(f.read())
