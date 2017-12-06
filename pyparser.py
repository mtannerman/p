import csv

fileName = "input/4.csv"

with open(fileName) as f:
    for line in f.readlines():
        print(line)

print('\n')
with open(fileName) as f:
    csvReader = csv.reader(f, delimiter=',')
    for row in csvReader:
        fullStr = ""
        for elem in row:
            fullStr += (elem + str("||"))
        print(fullStr[0:-2])
