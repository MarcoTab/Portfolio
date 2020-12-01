from image_utils import *
from k_means import *
if __name__ == "__main__":

    file = input("image file> ")
    image = read_ppm(file)

    # Test writing it unmodified
    save_ppm("unmodified.ppm", image)

    # rotate the colors, it's a simple manipulation, but it's always a fun one.
    width, height = get_width_height(image)
    for x in range(width):
        for y in range(height):
            r, g, b = image[x][y]
            image[x][y] = (255-r, 255-g, 255-b)
    save_ppm("modified.ppm", image)

    assigns = [[0, 0, 0],
               [0, 0, 0]]
    means = [(255, 200, 250), (105, 0, 105), (35, 35, 35)]
    image = [[(245, 200, 255), (108, 20, 100), (40, 60, 80)],
             [(5, 2, 4), (450, 120, 300), (1, 1, 65)]]
    update_assign(means, image)
    print(assigns)

    k = 3
    image = [[(25, 35, 55), (72, 230, 55), (150, 106, 189)],
             [(20, 217, 51), (173, 94, 205), (134, 56, 42)]]

    means, assignments = k_means(image, k)
    print(means)
    print('----------------')
    print(assignments)
