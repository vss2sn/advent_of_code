#!/usr/bin/python3

import copy


def main():
    file = open("../input/day_21_input")
    atoi = dict()  # allergens to ingredients
    itoc = dict()  # ingredients to count
    for line in file:
        line = line.replace("(", "").replace(")", "").strip("\n").replace(",", "")
        ingredients, allergens = line.split("contains ")
        ingredients = ingredients.split(" ")
        ingredients.remove("")
        allergens = allergens.split(" ")
        for allergen in allergens:
            if allergen in atoi:
                subset = list()
                for pot_ingredient in atoi[allergen]:
                    if pot_ingredient in ingredients:
                        subset.append(pot_ingredient)
                atoi[allergen] = subset

            else:
                atoi[allergen] = copy.deepcopy(ingredients)
        for ingredient in ingredients:
            if ingredient in itoc:
                itoc[ingredient] = itoc[ingredient] + 1
            else:
                itoc[ingredient] = 1
    for allergen in atoi:
        if len(atoi[allergen]) == 1:
            for other_allergen in atoi:
                if other_allergen != allergen:
                    if atoi[allergen][0] in atoi[other_allergen]:
                        atoi[other_allergen].remove(atoi[allergen][0])

    allergens = list()
    for allergen in atoi:
        allergens.append(allergen)
    allergens.sort()
    ans = ""
    for allergen in allergens:
        ans = ans + atoi[allergen][0] + ","
    ans = ans[0 : len(ans) - 1]
    print(ans)
    return ans


if __name__ == "__main__":
    main()
