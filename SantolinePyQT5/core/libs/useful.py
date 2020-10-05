import os
import multiprocessing
import math

def allFiles(folder, extension):
    files = map(lambda t: 
                    map(lambda e: t[0] + "\\" + e, 
                        filter(lambda f: 
                                    f.endswith(extension), t[2])), 
                os.walk(folder))
    return [file for i in files for file in i]
    
def thread_quantity():
    threads_quantity = 2
    try:
        threads_quantity = multiprocessing.cpu_count()
    except NotImplementedError:
        print("cpu_count undefined. Using 2 core by default.")
    return threads_quantity
    
def degreesToRadian(degrees):
    return degrees * math.pi / 180.
    
def radianToDegrees(radian):
    return radians * 180. / math.pi
    
def closes_point(points, p):
    return min(points, key=lambda point: point.distance(p))
    
def sortClosestPoint(points):
    p0 = points[0]
    points.remove(p0)
    curr = closest_point(points, p0)
    points.remove(curr)
    result = [p0, curr]
    while (len(points) > 0):
        curr = closest_point(points, curr)
        points.remove(curr)
        result.append(curr)
    return result