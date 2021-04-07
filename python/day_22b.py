#!/usr/bin/python3
import copy

def PlayRecursiveCombat(players, player_decks):
    previous_rounds = list()
    while len(player_decks) > 1:
        for i in range(0, len(previous_rounds)):
            if player_decks[0] == previous_rounds[i][0] and player_decks[1] == previous_rounds[i][1]:
                return 1, player_decks, players
        else:
            previous_rounds.append([copy.deepcopy(player_decks[0]),  copy.deepcopy(player_decks[1])])
        winning_player = -1
        hightest_card = -1
        top_cards = list()
        n_remaining_greater_than_card_value = True
        for i in range(0, len(player_decks)):
            if hightest_card < player_decks[i][0]:
                hightest_card = player_decks[i][0]
                winning_player = players[i]
            if player_decks[i][0] > len(player_decks[i]) - 1:
                n_remaining_greater_than_card_value = False;
            top_cards.append(player_decks[i].pop(0))

        if n_remaining_greater_than_card_value:
            subgame_player_decks = list()
            for i in range(0, len(top_cards)):
                subgame_player_decks.append(player_decks[i][0:top_cards[i]]);
            result = PlayRecursiveCombat(copy.deepcopy(players), subgame_player_decks)
            winning_player = result[0]
        for i in range(0, len(players)):
            if players[i] != winning_player:
                top_cards.append(top_cards.pop(0))
            else:
                for top_card in top_cards:
                    player_decks[i].append(top_card)
                break
        indices_to_remove = list()
        for i in range(0, len(player_decks)):
            if len(player_decks[i]) == 0:
                indices_to_remove.append(i)
        for i in range(0, len(indices_to_remove)):
            player_decks.pop(indices_to_remove[len(indices_to_remove) - i - 1])
            players.pop(indices_to_remove[len(indices_to_remove) - i - 1])
    return players[0], player_decks, players

def main():
    file = open("../input/day_22_input")
    players = list()
    player_decks = list()
    for line in file:
        line = line.strip('\n')
        if line == "":
            continue
        if line[0:6] == "Player":
            player_decks.append([])
            players.append(int(line[7:len(line)-1]))
        else:
            player_decks[-1].append(int(line))
    total_players = len(players)
    result = PlayRecursiveCombat(players, player_decks)
    winning_player, player_decks, players = result
    player_deck = list()
    score = 0
    for i in range(0, len(players)):
        if players[i] == winning_player:
            player_deck = player_decks[i]
    for i in range(0, len(player_deck)):
        score = score + ((i+1) * player_deck[len(player_deck) - 1 - i]);
    print(score)
    return score

if __name__ == "__main__":
    main()
