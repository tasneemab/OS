def groceray():
    store = ['Tomatos', 'Cucumbers', ' Peppers', 'Onions', 'Potatos', 'Carrots', 'Zucchinis', 'Cabbages', 'Lettuces',
             'Eggplants']
    wanted = {}
    for i in store:
        index = int(input("How many {}?".format(i)))
        wanted[i] = index

        wanted = dict(sorted(wanted.items(), key=lambda pair: pair[1], reverse=True))
        print(wanted)


groceray()
