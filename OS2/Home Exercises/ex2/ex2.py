"""
    Written by:
    Tasneem Abu Odeh, ID: 314902685, user: tasnimab
    Hanan Himouni, ID: 318314663 , user: hananhi

    Description:
    The program reads and analyzes a sql file and transformat it to CVS format by the terms.

    input: sql file
    output: cvs files

    """


# --------------------------------------------------------------------
"""
Initialize the variables we need
"""
inputFile = "demo.sql"
tablesCreation = "CREATE TABLE"
ignorWord = "KEY"
fillTables = "INSERT INTO"
# tablesName = ""
current = False
tablesLines = []
fillingEnd = 0  # indicator to number of INSERT_INTO lines

# --------------------------------------------------------------------
"""
This function strip line of data to a list
"""


def importData(line):
    table = []
    while ignorWord not in line:
        line1 = line.split()
        table.append(line1[0].strip("`") + ",")
        line = next(sql_input)
    return table

# --------------------------------------------------------------------
"""
The main part of the program.
"""
sql_input = open(inputFile, "r")
for line in sql_input:
    # set table tablesName and data parameters:
    if ignorWord in line:
        current = False
    if tablesCreation in line:
        tablesLines.clear()
        current = True
        line1 = line.split()
        tablesName = line1[2].strip("`") + ".csv"
        continue
    if current:
        tablesLines = importData(line)
        current = False
    # gather data in order to write it in the file:
    if fillTables in line:
        data = ""
        flag = False
        bco, bcc = 0, 0  # bracelet counter open / close

        if fillingEnd == 0:
            print("Exporting Data, Please wait...".format(tablesName))
        tablesLines.append("\n")
        fillingEnd += 1

        for ch in line:
            if ch == "(":
                bco += 1
                if not flag:
                    flag = True
                    data = ""  # reset data string
                    continue
            if ch == ")":
                bcc += 1
                if bcc == bco:
                    flag = False
                    bco, bcc = 0, 0
                    tablesLines.append(data + "\n")
                    continue
            if flag:  # append relevant characters to string
                data += ch
    else:
        # write data to cvs file
        if fillingEnd > 0:
            fillingEnd = 0
            export = open(tablesName, "w")
            export.write("".join(tablesLines))
            export.close()
print("Data Export Completed...")
sql_input.close()
