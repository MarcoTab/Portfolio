# Marco Tabacman
# CSCI 1913 --- Project 1 (K-Means applied to photos)
# Run the k-means algorithm on a photo!

from k_means import *

print("Good morning / beautiful afternoon! I hope you've had a wonderful day grading!")

filename = input('Input the filename> ')
n = int(input('run how many> '))
img = read_ppm(filename)
k = int(input('Input amount of colors to reduce to> '))

right_count = 0
for i in range(n):

    means, assignments = k_means(img, k)
    if means == [(50, 40, 0), (200, 210, 0)] or means == [(200, 210, 0), (50, 40, 0)]:
        right_count += 1

print(right_count/n)
