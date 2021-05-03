#! python3
# credits : 
#           - https://www.flaticon.com/authors/pixel-perfect 
#           - http://www.famfamfam.com/lab/icons/
#           - CreativeCommons 3.0

import sys
import os
from qgis.core import QgsApplication, QgsPoint
from core.view import santoline_view
import json
import shutil

    #getting qgis path
def getQGISDirectory():
    programFiles = "C:/Program Files"
    programFilesDirectory = next(os.walk(programFiles))[1];
    qgisVersions = []
    for name in programFilesDirectory:
        if(name.startswith("QGIS")):
            qgisVersions.append(name)
    if not qgisVersions :
        raise Exception('Aucune version QGIS!')
    qgisLastVersion = qgisVersions[len(qgisVersions)-1];
    qgisDirectory = str(programFiles).replace('/','\\') + "\\" +str(qgisLastVersion)
    return qgisDirectory
    

def createJsonPath():
    full_path = os.path.realpath(__file__)
    currentDirectory = os.path.dirname(full_path)

    dataDirectory = str(currentDirectory).replace('\SantolinePyQT5','') + "\data"
    windMapDirectory = str(dataDirectory) + "\maps"
    asctojsonparserDirectory = str(currentDirectory).replace('\SantolinePyQT5','') + "\src\AscToJsonParser"

    qgisDirectory = getQGISDirectory()

    #getting WindNinja path
    windNinja = "C:/WindNinja"
    windNinjaDirectory = next(os.walk(windNinja))[1];
    windNinjaVersions = []
    for name in windNinjaDirectory:
        if(name.startswith("WindNinja")):
            windNinjaVersions.append(name)
    if not windNinjaVersions :
        raise Exception('Aucune version WindNinja!')
    windNinjaLastVersion = windNinjaVersions[len(windNinjaVersions)-1];
    windNinjaDirectory = str(windNinja).replace('/','\\') + "\\" +str(windNinjaLastVersion)

    #Create json
    data = {
        "data_path" : dataDirectory,
        "qgis_path" : qgisDirectory,
        "windninja_path" : windNinjaDirectory,
        "asctojsonparser_path": asctojsonparserDirectory,
        "wind_map_generation_path" : windMapDirectory
    }

    #write Json
    with open('./paths.json', 'w') as outfile:
        json.dump(data, outfile)

def initEnvironment():
    files = {
        "./src/Epilobe/params.json": "./data/templates/params.json",
        "./data/maps/map.json": None,
        "./data/communication/parametreAlgo.json": None
    }

    for file, template in files.items():
        if os.path.isfile(file):
            os.remove(file)
        if template == None:
            open(file, 'a').close()
        else:
            shutil.copy(template, file)

def main():
    current_path = os.path.dirname(os.path.realpath(__file__))
    os.chdir(current_path)
    l = list(map(lambda arg: bytes(arg, 'utf-8'), sys.argv))
    qgisDirectory = getQGISDirectory() + "\\apps\\qgis"
    QgsApplication.setPrefixPath(qgisDirectory.replace("\\", "\\\\"), True)
    app = QgsApplication(l, True)
    app.initQgis()
    santoline_view.Santoline()
    sys.exit(app.exec_())
            
if (__name__ == "__main__"):
    createJsonPath()
    initEnvironment()
    main()