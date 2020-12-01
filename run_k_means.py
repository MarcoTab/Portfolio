# Marco Tabacman
# CSCI 1913 --- Project 1 (k-means applied to photos)
# Run the k-means algorithm on a photo!

from k_means import *

print("Good morning / beautiful afternoon! I hope you've had a wonderful day grading!")
filename = input('Input the filename> ')
img = read_ppm(filename)
k = int(input('Input amount of colors to reduce to> '))

means, assignments = k_means(img, k)
width, height = get_width_height(img)

new_photo = []
for i in range(width):
    new_photo.append([])
    for j in range(height):
        new_photo[i].append((0, 0, 0))

for j in range(height):
    for i in range(width):
        new_photo[i][j] = means[assignments[i][j]]


filename = input('Name of the modified picture> ')
save_ppm(filename, new_photo)
print('All done!')
