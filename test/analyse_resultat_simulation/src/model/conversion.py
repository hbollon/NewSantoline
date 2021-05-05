import json

from analyse_resultat_simulation.src.utils.conversion import adapt_coords_to_canvas


def adapt_data_to_canvas(factor_height, factor_width, height_canvas, width_canvas,
                         new_list, points, old_points_details=None, points_details=None, initial_outline=None,
                         burning_points=None):
    convertedGrid = adapt_grid_square_coords_to_canvas(factor_height, factor_width, height_canvas, new_list,
                                                       width_canvas)
    convertedPoints = adapt_points_coords_to_canvas(factor_height, factor_width, height_canvas, new_list,
                                                    points, width_canvas)
    convertedPointsDetails = None
    if points_details is not None:
        convertedPointsDetails = \
            adapt_points_with_details_coords_to_canvas(factor_height, factor_width, height_canvas,
                                                       new_list, points_details, width_canvas)
    converted_old_detailed_points = None
    if old_points_details is not None:
        converted_old_detailed_points = \
            adapt_points_with_details_coords_to_canvas(factor_height, factor_width, height_canvas,
                                                       new_list, old_points_details.points, width_canvas)

    convertedInitial_outline = None
    if initial_outline is not None:
        convertedInitial_outline = adapt_initial_outline_to_canvas(factor_height, factor_width, height_canvas,
                                                                   new_list, initial_outline, width_canvas)

    converted_burning_points = None
    if burning_points is not None:
        converted_burning_points = adapt_burning_points_to_canvas(factor_height, factor_width, height_canvas,
                                                                  new_list, burning_points, width_canvas)

    return convertedGrid, convertedPoints, convertedPointsDetails, converted_old_detailed_points, \
        convertedInitial_outline, converted_burning_points


def adapt_grid_square_coords_to_canvas(factor_height, factor_width, height_canvas, new_list, width_canvas):
    factor = {"x": factor_width, "y": factor_height}
    size_canvas = {"x": width_canvas, "y": height_canvas}
    convertedGrid = json.loads(json.dumps(new_list))
    if 'orig_cellule_coords' in convertedGrid[0]:
        min_val = {"x": new_list[0]['orig_cellule_coords'][0], "y": new_list[0]['orig_cellule_coords'][1]}
        for i in enumerate(convertedGrid):
            point = {"x": new_list[i]['orig_cellule_coords'][0], "y": new_list[i]['orig_cellule_coords'][1]}
            convertedGrid[i]['orig_cellule_coords'] = (point["x"], point["y"])
            convertedGrid[i]['cellule_coords'] = adapt_coords_to_canvas(
                factor, min_val, point, size_canvas)
    else:
        min_val = {"x": new_list[0]['cellule_coords'][0], "y": new_list[0]['cellule_coords'][1]}
        for i in enumerate(convertedGrid):
            point = {"x": new_list[i]['cellule_coords'][0], "y": new_list[i]['cellule_coords'][1]}
            convertedGrid[i]['orig_cellule_coords'] = convertedGrid[i]['cellule_coords']
            convertedGrid[i]['cellule_coords'] = adapt_coords_to_canvas(
                factor, min_val, point, size_canvas)
    return convertedGrid


def adapt_points_coords_to_canvas(factor_height, factor_width, height_canvas, new_list, test_2, width_canvas):
    if 'orig_cellule_coords' in new_list[0]:
        min_val = {"x": new_list[0]['orig_cellule_coords']
                   [0], "y": new_list[0]['orig_cellule_coords'][1]}
    else:
        min_val = {"x": new_list[0]['cellule_coords']
                   [0], "y": new_list[0]['cellule_coords'][1]}
    size_canvas = {"x": width_canvas, "y": height_canvas}
    factor = {"x": factor_width, "y": factor_height}
    convertedPoints = json.loads(json.dumps(test_2))
    for i in enumerate(convertedPoints):
        point = {"x": test_2[i][0], "y": test_2[i][1]}
        convertedPoints[i] = adapt_coords_to_canvas(
            factor, min_val, point, size_canvas)
    return convertedPoints


def adapt_points_with_details_coords_to_canvas(factor_height, factor_width, height_canvas, new_list, points_details,
                                               width_canvas):
    if 'orig_cellule_coords' in new_list[0]:
        min_val = {"x": new_list[0]['orig_cellule_coords']
                   [0], "y": new_list[0]['orig_cellule_coords'][1]}
    else:
        min_val = {"x": new_list[0]['cellule_coords']
                   [0], "y": new_list[0]['cellule_coords'][1]}
    size_canvas = {"x": width_canvas, "y": height_canvas}
    factor = {"x": factor_width, "y": factor_height}
    convertedPointsDetails = json.loads(json.dumps(points_details))
    for i in enumerate(convertedPointsDetails):
        point_coords = {"x": points_details[i]['coordonnees_non_converties'][0],
                        "y": points_details[i]['coordonnees_non_converties'][1]}
        convertedPointsDetails[i] = {
            "id": points_details[i]["id"],
            "indice": points_details[i]["indice"],
            "ancetre": points_details[i]["ancetre"],
            "precedent": points_details[i]["precedent"],
            "suivant": points_details[i]["suivant"],
            "temps": points_details[i]["temps"],
            "creation": points_details[i]["creation"],
            "coordonnees_non_converties": points_details[i]['coordonnees_non_converties'],
            "coordonnees_calcul": points_details[i]["coordonnees_calcul"],
            "coordonnees": adapt_coords_to_canvas(factor, min_val, point_coords, size_canvas),
            "vitesse": points_details[i]["vitesse"]
        }
    return convertedPointsDetails


def adapt_initial_outline_to_canvas(factor_height, factor_width, height_canvas, new_list,
                                    initial_outline, width_canvas):
    factor = {"x": factor_width, "y": factor_height}
    min_val = {"x": new_list[0]['cellule_coords']
               [0], "y": new_list[0]['cellule_coords'][1]}
    size_canvas = {"x": width_canvas, "y": height_canvas}
    convertedInitial_outline = json.loads(json.dumps(initial_outline))
    for i in enumerate(convertedInitial_outline["contourInitial"]):
        point = {"x": initial_outline["contourInitial"][i][0], "y": initial_outline["contourInitial"][i][1]}
        convertedInitial_outline["contourInitial"][i] = adapt_coords_to_canvas(factor, min_val, point, size_canvas)
    return convertedInitial_outline


def convert_to_displayable_data(actual_points):
    actual_points_to_display = list(map(lambda p: {
        "id": p["id"],
        "indice": p["indice"],
        "ancetre": p["ancetre"],
        "precedent": p["precedent"],
        "suivant": p["suivant"],
        "temps": p["temps"],
        "creation": p["creation"],
        "coordonnees": p["coordonnees_calcul"],
        "vitesse": p["vitesse"]
    }, actual_points))
    return actual_points_to_display


def convert_burning_p_to_displayable_data(burning_points):
    actual_points_to_display = list(map(lambda p: {
        "t_ignition": p["t_ignition"],
        "indice_allume": p["indice_allume"],
        "coordonnees": p["coordonnees_calcul"]
    }, burning_points))
    return actual_points_to_display


def adapt_burning_points_to_canvas(factor_height, factor_width, height_canvas,
                                   new_list, burning_points, width_canvas):
    if 'orig_cellule_coords' in new_list[0]:
        min_val = {"x": new_list[0]['orig_cellule_coords']
                   [0], "y": new_list[0]['orig_cellule_coords'][1]}
    else:
        min_val = {"x": new_list[0]['cellule_coords']
                   [0], "y": new_list[0]['cellule_coords'][1]}
    size_canvas = {"x": width_canvas, "y": height_canvas}
    factor = {"x": factor_width, "y": factor_height}
    converted_burning_points = json.loads(json.dumps(burning_points))
    for i in enumerate(converted_burning_points):
        point_coords = {"x": burning_points[i]['coordonnees_non_converties'][0],
                        "y": burning_points[i]['coordonnees_non_converties'][1]}
        converted_burning_points[i] = {
            "coordonnees_calcul": converted_burning_points[i]["coordonnees_calcul"],
            "coordonnees": adapt_coords_to_canvas(factor, min_val, point_coords, size_canvas),
            "t_ignition": burning_points[i]["t_ignition"],
            "indice_allume": burning_points[i]["indice_allume"]
        }
    return converted_burning_points
