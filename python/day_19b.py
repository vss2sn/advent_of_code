#!/usr/bin/python3

# No need to change the input data
update_rules = dict()
update_rules[8] = "42 | 42 8"
update_rules[11] = "42 31 | 42 11 31"

known_rules = dict()
subsections_of_rules = dict()
previously_seen_pairs = dict()

def DoesMsgFollowRule(message, source):
    if source in known_rules:
        if known_rules[source] == message:
            return True
        else:
            return False
    if (message, source) in previously_seen_pairs:
        return previously_seen_pairs[(message, source)]
    for subsection in subsections_of_rules[source]:
        parts = len(subsection)
        if parts == 1:
            if(DoesMsgFollowRule(message, subsection[0])):
                previously_seen_pairs[(message, source)] = True
                return True;
        elif parts == 2:
            for i in range(0, len(message) - 1):
                if DoesMsgFollowRule(message[0:i + 1], subsection[0]) and DoesMsgFollowRule(message[i + 1:len(message)], subsection[1]):
                    previously_seen_pairs[(message, source)] = True
                    return True;
        elif parts == 3:
            for i in range(0, len(message) - 2):
                for j in range(i+1, len(message) - 1):
                    if DoesMsgFollowRule(message[0 : i + 1], subsection[0]) and DoesMsgFollowRule(message[i + 1 : j + 1], subsection[1]) and DoesMsgFollowRule(message[j + 1 : len(message)], subsection[2]):
                        previously_seen_pairs[(message, source)] = True
                        return True;
    previously_seen_pairs[(message, source)] = False
    return False


def main():
    file = open("../input/day_19_input")
    messages = list()
    for line in file:
        line = line.strip('\n')
        if line == "":
            continue
        index = line.find(':')
        if index != -1:
            rule_lhs = int(line[0:index])
            rule_rhs = line[index + 1:len(line)]
            if rule_lhs in update_rules:
                rule_rhs = update_rules[rule_lhs]
            if rule_lhs in known_rules:
                rule_rhs = known_rules[lhs]
            index = rule_rhs.find("\"")
            if index != -1:
                known_rules[rule_lhs] = rule_rhs[index + 1]
            else:
                rule_subsection = list()
                num_in_rule_section = 0
                for c in rule_rhs:
                    if c == ' ':
                        if num_in_rule_section > 0:
                            rule_subsection.append(num_in_rule_section)
                        num_in_rule_section = 0
                    elif c == '|':
                        if rule_lhs not in subsections_of_rules:
                            subsections_of_rules[rule_lhs] = list()
                        subsections_of_rules[rule_lhs].append(rule_subsection)
                        rule_subsection = list()
                    else:
                        num_in_rule_section = 10 * num_in_rule_section + int(c)
                rule_subsection.append(num_in_rule_section)
                if rule_lhs not in subsections_of_rules:
                    subsections_of_rules[rule_lhs] = list()
                subsections_of_rules[rule_lhs].append(rule_subsection);
        else:
            messages.append(line)

    res = 0;
    for message in messages:
        if DoesMsgFollowRule(message, 0):
           res += 1
        previously_seen_pairs.clear();
    print(res)
    return 0;

if __name__ == "__main__":
    main()
