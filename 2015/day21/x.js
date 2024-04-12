let {maxBy,minBy} = require("./2015/day09/utils");

function solve(input, part) {

    let boss = new PlayerStatus(103, 9, 2, 0);
    let options = Array.from(getPlayerOptions(100));
    if(part === 1) {
        return minBy(options.filter(x => battle(x, boss)), x => x.goldSpent).goldSpent;
    }
    else {
        return maxBy(options.filter(x => !battle(x, boss)), x => x.goldSpent).goldSpent;
    }
}


class ShopItem {
    constructor(name, cost, damage, armor) {
        this.name = name;
        this.cost = cost;
        this.damage = damage;
        this.armor = armor;
    }
}

let weapons = [
    new ShopItem("Dagger",8,4,0),
    new ShopItem("Shortsword",10,5,0),
    new ShopItem("Warhammer",25,6,0),
    new ShopItem("Longsword",40,7,0),
    new ShopItem("Greataxe",74,8,0),
]

let armory = [
    new ShopItem("Leather",13,0,1),
    new ShopItem("Chainmail",31,0,2),
    new ShopItem("Splintmail",53,0,3),
    new ShopItem("Bandedmail",75,0,4),
    new ShopItem("Platemail",102,0,5),
]

function* getPlayerOptions(hitPoints) {
    let startStatus = new PlayerStatus(hitPoints,0,0,0);
    for (let weapon of weapons)
    {
        var ps = startStatus.powerupWith(weapon);
        yield ps;
        yield* addRings(ps);
        for (let armor of armory)
        {
            let ps2 = ps.powerupWith(armor);
            yield ps2;
            yield* addRings(ps2);
        }
    }
}

let rings = [
    new ShopItem("Damage +1", 25,1,0),
    new ShopItem("Damage +2", 50,2,0),
    new ShopItem("Damage +3", 100,3,0),
    new ShopItem("Defense +1", 20,0,1),
    new ShopItem("Defense +2", 40,0,2),
    new ShopItem("Defense +3", 80,0,3)
]

function* addRings(status)
{
    for (let ring1 of rings)
    {
        let with1Ring = status.powerupWith(ring1);
        yield with1Ring;
        for (let ring2 of rings)
        {
            if (ring2 != ring1) {
                yield with1Ring.powerupWith(ring2);
            }
        }
    }
}


function battle(player, boss, debug = false)
{
    while (player.hitPoints > 0 && boss.hitPoints > 0)
    {
        boss = boss.hitBy(player);
        if (debug) console.log(`Boss: ${boss.hitPoints}`);
        if (boss.hitPoints <= 0) break;
        player = player.hitBy(boss);
        if (debug) console.log(`Player: ${player.hitPoints}`);
    }
    return player.hitPoints > 0;
}

class PlayerStatus
{
    constructor(hp, d, a, g, s = "")
    {
        this.hitPoints = hp;
        this.damage = d;
        this.armor = a;
        this.goldSpent = g;
        this.setup = s;
    }

    powerupWith(item) {
        return new PlayerStatus(this.hitPoints, this.damage + item.damage, this.armor + item.armor, this.goldSpent + item.cost, this.setup + "," + item.name);
    }

    hitBy(opponent) {
        return new PlayerStatus(this.hitPoints - opponent.damage + this.armor, this.damage, this.armor, this.goldSpent, this.setup);
    }
}


function expected(part) {
    return part == 1 ? 121 : 201;
}

const fs = require("fs");
const inputString = fs.readFileSync("input", "utf8");
const input = inputString.split('\n');

const part1Result = solve(input, 1);
console.log(part1Result);

const part2Result = solve(input, 2);
console.log(part2Result);