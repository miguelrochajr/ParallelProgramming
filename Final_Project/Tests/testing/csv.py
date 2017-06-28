# Para criar o arquivo, execute o codigo da seguinte forma:
# python csv.py > <filename>.csv

expectedValue = "00011011"

array = list(expectedValue)

for i in range(0,len(expectedValue),2):
    try:
        line="{},{}".format(expectedValue[i], expectedValue[i+1])
        print(line)
    except:
        print("FINISHED")
