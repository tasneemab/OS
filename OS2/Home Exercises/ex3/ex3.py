# import section
import re

# user input
choose = input("Please enter which program do you want (a/b): ")

# first program
if choose == 'a':
    # open the read files
    big_cities = open("largest_cities.txt")
    codes = open("us_postal_codes.csv")

    # create a dictionary
    dict = {}

    # take the post code from the second read file
    next(codes)
    for line2 in codes:
        try:
            name2 = re.search(r"([\d]+),((\w*(\ \w*){0,2}([a-z]))).", line2)
            dict.update({name2.group(2): name2.group(1)})
        except:
            pass

    # go to next line in the first read file
    next(big_cities)

    # take the big city name
    for line in big_cities:
        name = re.search(r"([0-9]+)(\w*(\ \w*){0,2}([a-z]))([A-Z])", line)
        x = name.group(2)
        print((x) + " " + (dict.get(name.group(2))))

    # close files
    big_cities.close()
    codes.close()

# second program
elif choose == 'b':

    # open read and write files
    log = open("atoms2.log")
    out = open("out.txt", "w")

    # do the mission by the terms
    for line in log:
        try:
            name = re.search(r"RUN.([\d]+).COMPLETED. OUTPUT IN FILE ([a-z]+).dat. (\d+\.)", line)
            # write to the output file
            out.write(name.group(2) + " " + name.group(1) + "\n")
        except:
            pass

    # close files
    out.close()
    log.close()
