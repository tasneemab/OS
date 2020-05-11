"""
written by: Tasneem Abu Odeh, id: 314902685 login: tasnimab
            Hanan Himouni     id: 318314663 login: hananhi
--------------------------------------------------------------
"""
"""
 this program use re library without using "if".
 the program open 2 read files.
 the program search information the largest cities and
 their postal from the read files by the terms.
 the program print the information

Input: 	2 read files
Output:  all the largest cities and their postal

"""
import re

big_cities = open("largest_cities.txt")
codes = open("us_postal_codes.csv")

next(big_cities)
dictionary = {}

for line in big_cities:
    name = re.search(r"([0-9]+)(\w*(\ \w*){0,2}([a-z]))([A-Z])", line)
    dictionary[name.group(2)] = 0

next(codes)
for line2 in codes:
    name2 = re.search(r"([\d]+),((\w*(\ \w*){0,2}([a-z]))).", line2)
    if name2:
        dictionary[name2.group(2)] = name2.group(1)

for i in dictionary:
    print(i, dictionary.get(i))

big_cities.close()
codes.close()
