#!/bin/python3

import math
import os
import random
import re
import sys


#
# Complete the 'sortRoman' function below.
#
# The function is expected to return a STRING_ARRAY.
# The function accepts STRING_ARRAY names as parameter.
def convertRoman(last):
    romanValues = {'I': 1, 'V': 5, 'X': 10, 'L': 50, 'C': 100, 'D': 500, 'M': 1000}
    total = romanValues[last[-1]]
    for num1, num2 in zip(last[-1::-1], last[-2::-1]):
        if romanValues[num1] <= romanValues[num2]:
            total += romanValues[num2]
        else:
            total -= romanValues[num2]

    return total






def splitt(names):
    for x in names:
        first, last = x.split(" ")
    newLast = convertRoman(last)
    fullName = " ".join(first, newLast)
    reConnect(fullName)


def reConnect(name):
    names = []
    names.append(name)
    return names


def sortRoman(names):
    wanted = splitt(names)
    sorted(wanted)
    return wanted


if __name__ == '__main__':
    fptr = open(os.environ['OUTPUT_PATH'], 'w')

    names_count = int(input().strip())

    names = []

    for _ in range(names_count):
        names_item = input()
        names.append(names_item)

    result = sortRoman(names)

    fptr.write('\n'.join(result))
    fptr.write('\n')

    fptr.close()
