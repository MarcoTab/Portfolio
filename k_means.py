# Marco Tabacman
# CSCI 1913 --- Project 1 (k-means applied to photos)
# k-means functions module

from image_utils import *
from math import inf


def compute_color_distance(means, color):

    """" Takes a color (r,g,b) and returns the index of which it is 'closest' to (in the means list) """

    closest = 0
    prev = inf
    for value in means:
        temp = sqrt((value[0] - color[0])**2 + (value[1] - color[1])**2 + (value[2] - color[2])**2)
        if temp < prev:
            prev = temp
            closest = means.index(value)
    return closest


def update_assign(means, image):

    """ Updates assignments list by using the image list and the means list. """

    width, height = get_width_height(image)

    assignments = []
    for i in range(width):
        assignments.append([])
        for j in range(height):
            assignments[i].append(0)

    for j in range(height):
        for i in range(width):
            temp = compute_color_distance(means, image[i][j])
            assignments[i][j] = temp
    return assignments


def update_means(image, assignments, k):

    """ Updates the means list by averaging the colors assigned to cluster i"""

    means = [(0, 0, 0)] * k

    for i in range(k):
        r_avg = 0
        g_avg = 0
        b_avg = 0
        assigned_colors = extract_colors(image, assignments, i)
        length = len(assigned_colors)
        if length != 0:
            for j in range(length):
                r_avg += assigned_colors[j][0]
                g_avg += assigned_colors[j][1]
                b_avg += assigned_colors[j][2]

            r_avg /= length
            g_avg /= length
            b_avg /= length

            means[i] = (round(r_avg), round(g_avg), round(b_avg))
        else:
            means[i] = (0, 0, 0)

    return means


def extract_colors(img, assign, cluster):

    """ Extracts all the colors that belong to the cluster """

    width, height = get_width_height(img)

    ext_colors = []

    for i in range(width):
        for j in range(height):
            if assign[i][j] == cluster:
                ext_colors.append(img[i][j])
    return ext_colors


def k_means(image, k):

    """ Creates the means and assignment list and updates them until assignments doesn't change. """

    means = []
    for i in range(k):
        means.append(random_color())
        
    means = sorted(means)

    assignments = update_assign(means, image)
    prev_assign = []

    count = 0
    while assignments != prev_assign:
        count += 1
        prev_assign = assignments
        means = sorted(update_means(image, assignments, k))
        assignments = update_assign(means, image)
        print('.', end=' ')
        if count % 11 == 0:
            print(end='\r')

    print()
    return means, assignments

