#!/usr/bin/env python3

import re
import copy

class Rule:

    def __init__(self, name, min_val1, max_val1, min_val2, max_val2):
        self.name = name
        self.min_val1 = int(min_val1)
        self.max_val1 = int(max_val1)
        self.min_val2 = int(min_val2)
        self.max_val2 = int(max_val2)

    def valid(self, val):
        return (val >= self.min_val1 and val <= self.max_val1) or (val >= self.min_val2 and val <= self.max_val2)

def getRules(f):
    pattern = re.compile(r'(.*?): (\d+)-(\d+) or (\d+)-(\d+)(.*?)')
    rules = dict()
    for line in f:
        line = line.strip().replace('\r', '').replace('\n', '')
        if re.match(pattern, line):
            vals = re.match(pattern, line).groups()
            rules[vals[0]] = Rule(vals[0], vals[1], vals[2], vals[3], vals[4])
        else:
            break
    return rules

def getRulePlace(f, rules):

    places = set()
    for i in range(0, len(rules)):
        places.add(i)
    rule_place = dict()
    for name in rules:
        rule_place[name] = copy.deepcopy(places)

    pattern = re.compile(r'(.*?):((.*?))')
    my_ticket = list()
    for line in f:
        if re.match(pattern, line):
            continue
        line = line.strip().replace('\r', '').replace('\n', '')
        if line == "":
            continue
        else:
            my_ticket = line.split(',')
            my_ticket = [int(i) for i in my_ticket]
            break

    for line in f:
        if re.match(pattern, line):
            continue
        line = line.strip().replace('\r', '').replace('\n', '')
        if line == "":
            continue
        else:
            vals = line.split(',')
            for index, val in enumerate(vals):
                if any(rule.valid(int(val)) for rule in rules.values()): # check if ticket valid
                    for name in rules:
                        if not rules[name].valid(int(val)):
                            if index in rule_place[name]:
                                rule_place[name].remove(index)
    return rule_place, my_ticket

def ensureOTOMatch(rule_place): # one to one match
    final_rule_places = copy.deepcopy(rule_place)
    to_remove = 0
    changed = True
    while changed:
        changed = False
        for rule in rule_place:
            if len(rule_place[rule]) == 1:
                to_remove = rule_place[rule].pop()
                final_rule_places[rule] = to_remove
                changed = True
                break
        if changed:
            for rule in rule_place:
                if len(rule_place[rule]) > 1:
                    rule_place[rule].remove(to_remove)
    return final_rule_places

def main():
    f = open('../cpp/day_16_data.txt')
    rules = getRules(f)
    rule_place, my_ticket = getRulePlace(f, rules)
    final_rule_places = ensureOTOMatch(rule_place)
    product = 1
    for key in final_rule_places:
        if key[0:9] == "departure":
            product *= my_ticket[final_rule_places[key]]
    print(product)



if __name__ == "__main__":
    main()
