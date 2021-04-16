#!/usr/bin/python3


def main():
    file = open("../input/day_22_input")
    players = list()
    player_decks = list()
    for line in file:
        line = line.strip("\n")
        if line == "":
            continue
        if line[0:6] == "Player":
            player_decks.append([])
            players.append(int(line[7 : len(line) - 1]))
        else:
            player_decks[-1].append(int(line))
    total_players = len(players)

    while len(player_decks) > 1:
        winning_player = -1
        hightest_card = -1
        top_cards = list()
        for i in range(0, len(player_decks)):
            if hightest_card < player_decks[i][0]:
                hightest_card = player_decks[i][0]
                winning_player = i
            top_cards.append(player_decks[i][0])
            player_decks[i].pop(0)
        top_cards.sort(reverse=True)
        for top_card in top_cards:
            player_decks[winning_player].append(top_card)
        for i in range(0, len(player_decks)):
            if len(player_decks[i]) == 0:
                player_decks.pop(i)
                players.pop(i)
                i = i - 1
    score = 0
    for player_deck in player_decks:
        for i in range(0, len(player_deck)):
            score = score + ((i + 1) * player_deck[len(player_deck) - 1 - i])
    print(score)
    return score


if __name__ == "__main__":
    main()
