#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct hero {
    char name[40];
    int hp;
    int damageValue;
    int xp;
    int x;
    int y;
} hero;

typedef struct monster {
    char name[40];
    int hp;
    int damageValue;
    int x;
    int y;
} monster;

/*
  this function returns a substring of the given string
 */
char* strSubstring(int startIndex, int stopIndex, char *str) {
  /* Create the result string and allocate it. */
    char *result = malloc(sizeof(char) * (stopIndex - startIndex + 2));
    int i, j;

    /* Fill it. */
    for(i = startIndex, j = 0; i <= stopIndex; i++, j++){
        result[j] = str[i];
    }

    result[j] = '\0';

    return result;
}
/*fucntion that compares two strings */
int compareString(char *first, char *second) {
    int resultCode = 1;
    int i;
    for(i = 0; first[i] != '\0' || second[i] != '\0'; i++) {
        if(first[i] != second[i]) {
            resultCode = 0;
            return resultCode;
        }}
    return resultCode;
}

/*this function returns the hero's index which has the given name.
  it gets the name, heroes array, heroes count,*/
int getHeroIndexByName(char *name, hero *heroes, int heroesCount) {
    int i;
    for(i = 0; i < heroesCount; i++) {
        if(strcmp(heroes[i].name, name) == 0) {
            return i;}}
    return -1;
}
/* same idea with getByName. this gets by coordinate (x, y).*/
int getHeroIndexByCoordinate(int x, int y, hero *heroes, int heroesCount) {
    int i;
    for(i = 0; i < heroesCount; i++) {
        if(heroes[i].x == x && heroes[i].y == y) {
            return i;
        }}
    return -1;}
/* nothing different with hero functions.*/
int getMonsterIndexByName(char *name, monster *monsters, int monstersCount) {
    int i;
    for(i = 0; i < monstersCount; i++) {
        if(strcmp(monsters[i].name, name) == 0) {
            return i;
        }}
    return -1;
}
/*Nothing different with hero functions.*/
int getMonsterIndexByCoordinate(int x, int y, monster *monsters, int monstersCount) {
    int i;
    for(i = 0; i < monstersCount; i++) {
        if(monsters[i].x == x && monsters[i].y == y) {
            return i;
        }}
    return -1;
}
/*
  This function prints the map in file*/
void printMap(char **map, int x, int y, FILE *outputFile) {
    fprintf(outputFile, "MAP STATUS\n");
    int i, j;
    for(i = 0; i < x; i++) {
        for(j = 0; j < y; j++) {
            fprintf(outputFile, "%c ", map[i][j]);
        }
        fprintf(outputFile, "\n");
    }}
/*
 * this fucntion makes attack.
 * it also checks if m is dead or not.
 */
void heroAttack(hero *h, monster *m, char **map) {
    m->hp -= (h->damageValue);

    if(m->hp <= 0) {
        m->hp = 0;
        h->xp++;
        map[m->x][m->y] = '.';
    }}
/*
 * Almost nothing different with heroAttack function.*/
void monsterAttack(monster *m, hero *h, char **map) {
    h->hp -= (m->damageValue);

    if(h->hp <= 0) {
        h->hp = 0;

        map[h->x][h->y] = '.';
    }}
/*
 * This function takes the character's x position.
 * And it takes also the position value.
 *          8 1 2
 *          7 c 3
 *          6 5 4
 * Position is the value where we want to go to.
*/
int getTargetX(int x, int position) {
    if(position == 8 || position == 1 || position == 2){
      return x - 1;
    }
    else if(position == 7 || position == 3){
       return x;
     }
    else {
      return x + 1;}}
int getTargetY(int y, int position) {
    if(position == 8 || position == 7 || position == 6) return y - 1;
    else if(position == 1 || position == 5) return y;
    else return y + 1;
}
int main(int argc, char *argv[])
{

    if(argc != 4 || strstr(argv[1], "txt") == NULL || strstr(argv[2], "txt") == NULL){
        return 1;
    }
    /* we are opening files. */

    FILE *charsInputFile = fopen(argv[1], "r");
    FILE *commandsInputFile = fopen(argv[2], "r");
    FILE *outputFile = fopen(argv[3], "ab+");
    int i;
    /* firstly we loop over the chars text file
     * and we will just count the characters, to create our struct array.
     */

    int heroesCount = 0;
    int monstersCount = 0;

    if(charsInputFile == NULL){
        return 1;
    }

    char line[40];
    while(fgets(line, sizeof line, charsInputFile) != NULL){
        if(line[0] == 'H'){
            heroesCount++;
        } else {
            monstersCount++;
        }
    }
    fclose(charsInputFile);

    /* After getting the character count, we are creating our struct arrays */

    hero heroes[heroesCount];
    monster monsters[monstersCount];

    /* We will read again the chars input file.
     * This time we will parse it and create the character objects.
     */
    int heroIndex = 0;
    int monsterIndex = 0;

    charsInputFile = fopen(argv[1], "r");
    while(fgets(line, sizeof line, charsInputFile) != NULL){
        if(line[0] == 'H'){
            hero newHero;
            /*
             * the algorithm here is, to split line with commas.
             * the first term which determines character's type is not important.
             * thats why we only update our startIndex variable when we see the first comma.
             * after that, for each comma at index i,our current sting stops at (i - 1), and the next one starts at (i + 1)*/

            int startIndex = 0;
            int stopIndex = 0;
            int commaFound = 0;
            for(i = 0; i < 40; i++) {
                if(line[i] == ',') {
                    if(commaFound == 0) {
                        startIndex = i + 1;
                    } else if(commaFound == 1) {
                        stopIndex = i - 1;

                        char *name = strSubstring(startIndex, stopIndex, line);
                        strcpy(newHero.name, name);
                        free(name);

                        startIndex = i + 1;
                    } else if(commaFound == 2) {
                        stopIndex = i - 1;

                        char *hpStr = strSubstring(startIndex, stopIndex, line);

                        int hp;
                        sscanf(hpStr, "%d", &hp);
                        free(hpStr);

                        newHero.hp = hp;

                        startIndex = i + 1;
                    }

                    commaFound++;
                } else if(line[i] == '\0'){
                    char *damageValueStr = strSubstring(startIndex, i, line);
                    int damageValue;
                    sscanf(damageValueStr, "%d", &damageValue);
                    free(damageValueStr);

                    newHero.damageValue = damageValue;

                    break;
                }
            }
            /* we are filling our newHero's other informations. */

            newHero.xp = 0;
            newHero.x = 0;
            newHero.y = 0;
            heroes[heroIndex] = newHero;
            heroIndex++;
        } else {
          /*same as hero */

            monster newMonster;
            int startIndex = 0;
            int stopIndex = 0;
            int commaFound = 0;
            for(i = 0; i < 40; i++) {
                if(line[i] == ',') {
                    if(commaFound == 0) {
                        startIndex = i + 1;
                    } else if(commaFound == 1) {
                        stopIndex = i - 1;

                        char *name = strSubstring(startIndex, stopIndex, line);
                        strcpy(newMonster.name, name);
                        free(name);

                        startIndex = i + 1;
                    } else if(commaFound == 2) {
                        stopIndex = i - 1;

                        char *hpStr = strSubstring(startIndex, stopIndex, line);
                        int hp;
                        sscanf(hpStr, "%d", &hp);
                        free(hpStr);

                        newMonster.hp = hp;

                        startIndex = i + 1;
                    }

                    commaFound++;
                } else if(line[i] == '\0'){
                    char *damageValueStr = strSubstring(startIndex, i, line);
                    int damageValue;
                    sscanf(damageValueStr, "%d", &damageValue);
                    free(damageValueStr);
                    newMonster.damageValue = damageValue;

                    break;
                }
            }

            newMonster.x = 0;
            newMonster.y = 0;
            monsters[monsterIndex] = newMonster;
            monsterIndex++;
        }
    }
    fclose(charsInputFile);

    if(commandsInputFile == NULL) {
        return 1;
    }

    /* creating map  */
    char **map;

    int mapX, mapY;
    char line2[100];
    while(fgets(line2, sizeof line2, commandsInputFile) != NULL){
        /* LOADMAP command */
        if(line2[0] == 'L') {

            int secondSpaceIndex = -1;
            int lastIndex = -1;

            for(i = 8; line2[i] != '\0'; i++){
                if(line2[i] == ' ') {
                    if(secondSpaceIndex != -1){
                        lastIndex = i;
                        break;
                    }

                    secondSpaceIndex = i;
                }

                lastIndex = i;
            }

            /* after getting space indexes, I use strSubstring function to get those parts. */
            char *xStr = strSubstring(8, secondSpaceIndex - 1, line2);
            char *yStr = strSubstring(secondSpaceIndex + 1, lastIndex - 1, line2);
            sscanf(xStr, "%d", &mapX);
            sscanf(yStr, "%d", &mapY);
            free(xStr);
            free(yStr);
            map = (char **) malloc(mapX * sizeof(char *));
            for(i = 0; i < mapX; i++) {

                map[i] = (char *) malloc(sizeof(char) * (mapY + 1));

                int j;
                for(j = 0; j < mapY; j++) {
                    map[i][j] = '.';
                }
            }
        /* PUT command */
        } else if(line2[0] == 'P') {
            int firstSpaceIndex = 0;
            int secondSpaceIndex = 0;
            int startIndex = 0;
            int loopStartIndex = 0;
            /*
             * we again, will split the line with spaces.
             * if it is a PUT HERO line, our space-counter loop will start
             * at 9. index, otherwise 12. It is for skipping the first two
             * spaces. We don't want to count them.
             */

            if(line2[4] == 'H') loopStartIndex = 9;
            else loopStartIndex = 12;
            for(i = loopStartIndex; i < 100; i++) {
                int newChar = 0;
                if(startIndex == 0) startIndex = i;

                if(line2[i] == ' ') {
                    if(firstSpaceIndex == 0) firstSpaceIndex = i;
                    else if(secondSpaceIndex == 0) secondSpaceIndex = i;
                    else {

                        newChar = 1;
                    }
                }

                if(newChar == 1 || line2[i] == '\0') {
                    /* Parsing, and changing character's x and y positions. */
                    char *charName = strSubstring(startIndex, firstSpaceIndex - 1, line2);
                    char *xStr = strSubstring(firstSpaceIndex + 1, secondSpaceIndex - 1, line2);
                    char *yStr = strSubstring(secondSpaceIndex + 1, i - 1, line2);

                    int charX, charY;
                    sscanf(xStr, "%d", &charX);
                    sscanf(yStr, "%d", &charY);
                    free(xStr);
                    free(yStr);

                    if(line2[4] == 'H') {
                        int index = getHeroIndexByName(charName, heroes, heroesCount);
                        heroes[index].x = charX;
                        heroes[index].y = charY;
                    } else {
                        int index = getMonsterIndexByName(charName, monsters, monstersCount);
                        monsters[index].x = charX;
                        monsters[index].y = charY;
                    }

                    map[charX][charY] = charName[0];
                    firstSpaceIndex = 0;
                    secondSpaceIndex = 0;
                    startIndex = i + 1;
                    free(charName);
                    if(line2[i] == '\0') break;
                }
            }
        /* SHOW command */
        } else if(line2[0] == 'S') {
            /* SHOW HERO command */
            if(line2[5] == 'H') {
                fprintf(outputFile, "HERO STATUS\n");
                for(i = 0; i < heroesCount; i++) {
                    fprintf(outputFile, "%s HP: %d XP: %d\n", heroes[i].name, heroes[i].hp, heroes[i].xp);
                }
            /* SHOW MAP command */
            } else if(line2[6] == 'A') {
                printMap(map, mapX, mapY, outputFile);
            /* SHOW MONSTER command */
            } else {
                fprintf(outputFile, "MONSTER STATUS\n");
                for(i = 0; i < monstersCount; i++) {
                    fprintf(outputFile, "%s HP: %d\n", monsters[i].name, monsters[i].hp);
                }
            }
            fprintf(outputFile, "\n");
        /* ATTACK command */
        } else if(line2[0] == 'A') {
            /* ATTACK HERO command */
            if(line2[7] == 'H') {
                for(i = 0; i < heroesCount; i++) {
                    if(heroes[i].hp <= 0) continue;

                    /*for each direction from 1 to 8, we will get the index of monster
                     at that index if there is.*/
                    int adjacentMonsterIndex;
                    int j;
                    for(j = 1; j <= 8; j++) {
                        /* getting enemy coordinates. */
                        int targetX = getTargetX(heroes[i].x, j);
                        int targetY = getTargetY(heroes[i].y, j);

                        adjacentMonsterIndex = getMonsterIndexByCoordinate(targetX, targetY, monsters, monstersCount);
                        if(adjacentMonsterIndex != -1 && monsters[adjacentMonsterIndex].hp > 0) {
                            heroAttack(&heroes[i], &monsters[adjacentMonsterIndex], map);
                        }
                    }
                }

                fprintf(outputFile, "HEROES ATTACKED\n\n");

                int allDead = 1;
                int k;
                for(k = 0; k < monstersCount; k++) {
                    if(monsters[k].hp > 0) {
                        allDead = 0;
                        break;
                    }
                }
                /* if no one alive, end the program. */
                if(allDead == 1) {
                    fprintf(outputFile, "ALL MONSTERS ARE DEAD!\n");

                    for(i = 0; i < mapX; i++) {
                        free(map[i]);
                    }
                    free(map);
                    return 0;
                }
            } else {
                /* same things. */
                for(i = 0; i < monstersCount; i++) {
                    if(monsters[i].hp <= 0) continue;
                    int adjacentHeroIndex;
                    int j;
                    for(j = 1; j <= 8; j++) {
                        int targetX = getTargetX(monsters[i].x, j);
                        int targetY = getTargetY(monsters[i].y, j);

                        adjacentHeroIndex = getHeroIndexByCoordinate(targetX, targetY, heroes, heroesCount);
                        if(adjacentHeroIndex != -1 && heroes[adjacentHeroIndex].hp > 0) {
                            monsterAttack(&monsters[i], &heroes[adjacentHeroIndex], map);
                        }}}
                fprintf(outputFile, "MONSTERS ATTACKED\n\n");
                int allDead = 1;
                int k;
                for(k = 0; k < heroesCount; k++) {
                    if(heroes[k].hp > 0) {
                        allDead = 0;
                        break;
                    }
                }
                if(allDead == 1) {
                    fprintf(outputFile, "ALL HEROES ARE DEAD!\n");
                    for(i = 0; i < mapX; i++) {
                        free(map[i]);
                    }
                    free(map);
                    return 0;
                }
            }
        /* MOVE HERO command */
        } else if(line2[0] == 'M') {
            fprintf(outputFile, "HEROES MOVED\n");
            /* parsing flags. Same algorithm for PUT HERO command. */
            int firstSpaceIndex = 0;
            int secondSpaceIndex = 0;
            int startIndex = 0;

            for(i = 10; i < 100; i++) {
                int newChar = 0;
                if(startIndex == 0) startIndex = i;

                if(line2[i] == ' ') {
                    if(firstSpaceIndex == 0) firstSpaceIndex = i;
                    else if(secondSpaceIndex == 0) secondSpaceIndex = i;
                    else {
                        newChar = 1;
                    }
                }

                if(newChar == 1 || line2[i] == '\0') {
                    char *charName = strSubstring(startIndex, firstSpaceIndex - 1, line2);
                    char *xStr = strSubstring(firstSpaceIndex + 1, secondSpaceIndex - 1, line2);
                    char *yStr = strSubstring(secondSpaceIndex + 1, i - 1, line2);

                    int nextX, nextY;
                    sscanf(xStr, "%d", &nextX);
                    sscanf(yStr, "%d", &nextY);
                    free(xStr);
                    free(yStr);

                    /* after parsing hero's name, we get its index by name. */
                    int index = getHeroIndexByName(charName, heroes, heroesCount);

                    /* if next position is out of boundaries. */
                    if(nextX < 0 || nextX >= mapX || nextY < 0 || nextY >= mapY) {
                        fprintf(outputFile, "%s can't move. There is a wall.\n", heroes[index].name);
                    /* If there's another one at there. */
                    } else if(map[nextX][nextY] != '.') {
                        fprintf(outputFile, "%s can't move. Place is occupied.\n", heroes[index].name);
                    /* If a non-living hero tries to move.. */
                    } else if(heroes[index].hp <= 0) {
                        fprintf(outputFile, "%s can't move. Dead.\n", heroes[index].name);
                    /* And if there is not a problem to move. */
                    } else {
                        map[heroes[index].x][heroes[index].y] = '.';
                        map[nextX][nextY] = charName[0];
                        heroes[index].x = nextX;
                        heroes[index].y = nextY;
                    }

                    firstSpaceIndex = 0;
                    secondSpaceIndex = 0;
                    startIndex = i + 1;

                    free(charName);
                    if(line2[i] == '\0') break;
                }
            }
            fprintf(outputFile, "\n");
        }
    }

    for(i = 0; i < mapX; i++) {
        free(map[i]);
    }
    free(map);

    fclose(outputFile);
    fclose(commandsInputFile);

    return 0;
}
