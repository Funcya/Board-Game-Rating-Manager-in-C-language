#include <stdio.h>
#include <string.h>
#define WORDLENGTH 41
#define MAX_GAMES 20         
#define MAX_USERS 30        

typedef struct boardGame
{
    char boardName[WORDLENGTH];
    int rating;
} boardGame;

typedef struct player
{
    char username[WORDLENGTH];
    boardGame boardGames[MAX_GAMES];
    int numOfBoardGames;
} Player;

void readFromFile(Player players[], int *nrOfPlayers, char filename[]);
int findPlayer(Player players[], char choice[], int nrOfPlayers);
void updateExistingPlayer(Player players[], char username[], char boardName[], int rating, int *nrOfPlayers);
void createNewPlayer(Player players[], char username[], char boardName[], int rating, int *nrOfPlayers);
void sortPlayerNameAlphabetically(Player players[], int nrOfPlayers);
int startMenu(Player players[], int *nrOfPlayers);

void adminMenu(Player players[], int *nrOfPlayers);
void checkAddUserConditions(Player players[], int *nrOfPlayers);
void addUser(Player players[], char choice[], int *nrOfPlayers);
void checkRemoveUserConditions(Player players[], int *nrOfPlayers);
void checkRemoveUserWarning(Player players[], char choice[], int *nrOfPlayers);
void removeUser(Player players[], int playerIndex, int *nrOfPlayers);
void printAllUsers(Player players[], int nrOfPlayers);
void printAllUsersAndRatings(Player players[], int nrOfPlayers);

void playerMenu(Player players[], int playerIndex);
void printGames(Player players[], int playerIndex);
void checkAddGameConditions(Player players[], int playerIndex);
int checkDuplicateBoardNames(Player players[], char choice[], int playerIndex);
void addGame(Player players[], char choice[], int playerIndex);         
int getBoardGameRating();
void checkSearchGamesConditions(Player players[], int playerIndex);
int searchGames(Player players[], int playerIndex, char choice[]);
void checkRemoveGameConditions(Player players[], int playerIndex);
void removeGame(Player players[], int playerIndex, int boardGameIndex);
void writeToFile(Player players[], int nrOfPlayers, char filename[]); 

int main()
{
    char filename[WORDLENGTH];
    Player players[MAX_USERS];
    int nrOfPlayers = 0;

    printf("Welcome to boardgame ratings.\n");
    readFromFile(players, &nrOfPlayers, filename);
    startMenu(players, &nrOfPlayers);
    writeToFile(players, nrOfPlayers, filename);
    return 0;
}


void readFromFile(Player players[], int *nrOfPlayers, char filename[])
{
    char username[WORDLENGTH];
    char boardName[WORDLENGTH];
    int rating;
    FILE *fp;

    printf("Which file do you want to use: ");
    scanf("%s", filename);
    fp = fopen(filename, "r");

    if(fp != NULL)
    {
        while(fscanf(fp, "%s %s %d", username, boardName, &rating) == 3)
        {
            if(findPlayer(players, username, *nrOfPlayers) >= 0 )
            {
                updateExistingPlayer(players, username, boardName, rating, nrOfPlayers);
            } else
            {
                createNewPlayer(players, username, boardName, rating, nrOfPlayers);
            }
        }
        fclose(fp);
        sortPlayerNameAlphabetically(players, *nrOfPlayers);
    } else
    {
        fp = fopen(filename, "a");
        fclose(fp);
    }
}


int findPlayer(Player players[], char choice[], int nrOfPlayers)   
{
    for(int i = 0; i < nrOfPlayers; i++)                
    {
        if(strcmp(choice, players[i].username) == 0)
        {
            return i;
        }
    }
    return -1;
}


void updateExistingPlayer(Player players[], char username[], char boardName[], int rating, int *nrOfPlayers)
{
    int playerIndex = findPlayer(players, username, *nrOfPlayers);     
    int gamesCount = players[playerIndex].numOfBoardGames;              

    if(*nrOfPlayers <= MAX_USERS)           
    {
        strcpy(players[playerIndex].boardGames[gamesCount].boardName, boardName);
        players[playerIndex].boardGames[gamesCount].rating = rating;
        players[playerIndex].numOfBoardGames++;
    }
}


void createNewPlayer(Player players[], char username[], char boardName[], int rating, int *nrOfPlayers)
{
    if((*nrOfPlayers) < MAX_USERS)
    {
        Player player;
        strcpy(player.username, username);
        strcpy(player.boardGames[0].boardName, boardName);
        player.boardGames[0].rating = rating;
        player.numOfBoardGames = (rating == 0) ? 0 : 1;        

        players[*nrOfPlayers] = player;
        (*nrOfPlayers)++;
    }
}


void sortPlayerNameAlphabetically(Player players[], int nrOfPlayers)
{
    for (int i = 0; i < nrOfPlayers - 1; i++) 
    {
        for (int j = 0; j < nrOfPlayers - i - 1; j++) 
        {
            if (strcmp(players[j].username, players[j + 1].username) > 0) 
            {
                Player temp = players[j];
                players[j] = players[j + 1];
                players[j + 1] = temp;
            }
        }
    }
}


int startMenu(Player players[], int *nrOfPlayers)                       
{
    char choice[WORDLENGTH];
    while(1)
    { 
        printf("Please enter user name, admin or quit: ");
        scanf("%s", choice);

        if(strcmp (choice, "admin") == 0)
        {
            adminMenu(players, nrOfPlayers);          
        } else if ((strcmp(choice, "quit") == 0))
        {
            break;
        } else
        {
            if(findPlayer(players, choice, *nrOfPlayers) >= 0) 
            {
                int playerIndex = findPlayer(players, choice, *nrOfPlayers);
                playerMenu(players, playerIndex);
            } else
            {
                printf("user does not exist\n");
            }
        }
    }
}


void adminMenu(Player players[], int *nrOfPlayers)                          
{
    int choice;
    while(1)
    {
        printf("Administration\n"
                "\t  1) Add user\n"
                "\t  2) Remove user\n"
                "\t  3) Print all users\n"
                "\t  4) Print all users and all their ratings\n"
                "\t  5) Exit\n"
                "Choose: ");
        scanf("%d", &choice);

        switch (choice) 
        {
            case 1:
                checkAddUserConditions(players, nrOfPlayers);
                break;
            case 2:
                checkRemoveUserConditions(players, nrOfPlayers);
                break;
            case 3:
                printAllUsers(players, *nrOfPlayers);        
                break;
            case 4:
                printAllUsersAndRatings(players, *nrOfPlayers);     
                break;
            case 5:
                return;
        }
    }
}


void checkAddUserConditions(Player players[], int *nrOfPlayers)   
{
    char choice[WORDLENGTH];
    while (1)
    {   
        printf("Register new user (q to quit): ");
        scanf("%s", choice);

        if(strcmp(choice, "q") == 0)
        {
            return; 
        } else if (findPlayer(players, choice, *nrOfPlayers) >= 0)
        {
            printf("User name exists! Please choose another.\n");
        } else if (*nrOfPlayers >= MAX_USERS)
        {
            printf("The register is full!\n");
        } else
        {
            addUser(players, choice, nrOfPlayers);
        }
    }
}


void addUser(Player players[], char choice[], int *nrOfPlayers)
{
    strcpy(players[(*nrOfPlayers)].username, choice);
    players[(*nrOfPlayers)].numOfBoardGames = 0;
    (*nrOfPlayers)++;
    sortPlayerNameAlphabetically(players, *nrOfPlayers);
}


void checkRemoveUserConditions(Player players[], int *nrOfPlayers)         
{
    char choice[WORDLENGTH];
    while(1)
    {
        printf("Remove user (q to quit): ");
        scanf("%s", choice);

        if(strcmp (choice, "q") == 0)
        {
            return;
        } else if(findPlayer(players, choice, *nrOfPlayers) >= 0)
        {
            checkRemoveUserWarning(players, choice, nrOfPlayers);
        } else
        {
            printf("User do not exist! Please choose another.\n");
        }
    }
}


void checkRemoveUserWarning(Player players[], char choice[], int *nrOfPlayers)
{
    int playerIndex = findPlayer(players, choice, *nrOfPlayers);

    if(players[playerIndex].numOfBoardGames > 0)
    {
        printf("Warning: User has rated games.\n");
        printf("Do you still want to remove %s (y/n)? ", choice);
        scanf("%s", choice);

        if(strcmp (choice, "y") == 0)
        {
            removeUser(players, playerIndex, nrOfPlayers); 
        } 
    } else
    {
        removeUser(players, playerIndex, nrOfPlayers); 
        printf("Removed\n");
    }  
}


void removeUser(Player players[], int playerIndex, int *nrOfPlayers)
{
    for(int i = playerIndex; i < (*nrOfPlayers) - 1; i++)
    {
        players[i] = players[i+1];
    }
    (*nrOfPlayers)--;
}


void printAllUsers(Player players[], int nrOfPlayers)
{
    if(nrOfPlayers == 0)
    {
        printf("No users registered\n");
    } else
    {
        printf("Users:\n_____________________________________\n");
        for(int i = 0; i < nrOfPlayers; i++)
        {
            printf("%s\n", players[i].username);
        }
    }
}


void printAllUsersAndRatings(Player players[], int nrOfPlayers) 
{
    printf("Users and boardgames:\n_____________________________________\n");
    for(int i = 0; i < nrOfPlayers; i++)
    {   
        printf("%s\n", players[i].username);

        if(players[i].numOfBoardGames == 0)
        {
            printf("\t   No games registered\n");
        } else
        {
            for(int j  = 0; j < players[i].numOfBoardGames; j++)
            {
                printf("\t   %-16s   %d\n", players[i].boardGames[j].boardName,  players[i].boardGames[j].rating);
            }
        }
    }
}


void playerMenu(Player players[], int playerIndex)              
{
    int choice;
    while(1)
    {
        printf("%s's boardgames\n", players[playerIndex].username);
        printf("\t  1) Print games\n"
               "\t  2) Add game\n"
               "\t  3) Search games\n"
               "\t  4) Remove game\n"
               "\t  5) Exit\n"
                "Choose: ");        
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                printGames(players, playerIndex);
                break;
            case 2:
                checkAddGameConditions(players, playerIndex);
                break;
            case 3:
                checkSearchGamesConditions(players, playerIndex);
                break;
            case 4:
                checkRemoveGameConditions(players, playerIndex);
                break;
            case 5:
                return;
        }
    }
}


void printGames(Player players[], int playerIndex)
{
    for(int i = 0; i < players[playerIndex].numOfBoardGames; i++)
    {
        printf("          %-20s %-20d\n", players[playerIndex].boardGames[i].boardName, players[playerIndex].boardGames[i].rating);
    }
}


void checkAddGameConditions(Player players[], int playerIndex)
{
    char choice[WORDLENGTH];
    while(1)
    {
         if(players[playerIndex].numOfBoardGames >= MAX_GAMES)
        {
            printf("Your game register is full.\n");
            return; 
        }

        printf("Register new boardgame (q to quit): ");      
        scanf("%s", choice);
 
        if(strcmp(choice, "q") == 0)
        {
            return;
        } else if(checkDuplicateBoardNames(players, choice, playerIndex) == 1)
        {
            printf("Boardgame already added.\n");
        } else
        {
            addGame(players, choice, playerIndex);
        }
    }
}


int checkDuplicateBoardNames(Player players[], char choice[], int playerIndex)   
{
    for(int i = 0; i < players[playerIndex].numOfBoardGames; i++)
    {
        if(strcmp(choice, players[playerIndex].boardGames[i].boardName) == 0)
        {
            return 1;
        }
    }
    return -1;
}


void addGame(Player players[], char choice[], int playerIndex)
{
    int rating = getBoardGameRating();
    int fillIndex = players[playerIndex].numOfBoardGames;
    strcpy(players[playerIndex].boardGames[fillIndex].boardName, choice);
    players[playerIndex].boardGames[fillIndex].rating = rating;
    players[playerIndex].numOfBoardGames++;          
}


int getBoardGameRating()
{
    int rating = 0;
    while(1)
    {
        printf("Rating (1-10): ");
        scanf("%d", &rating);

        if(rating < 1 || rating > 10)
        {
            printf("Illegal value!\n");
        } else
        {
            return rating;
        }
    }
}


void checkSearchGamesConditions(Player players[], int playerIndex)
{
    if(players[playerIndex].numOfBoardGames == 0)
    {
        printf("No games registered\n");
        return;
    }
    
    char choice[WORDLENGTH];
    while(1)
    {
        printf("Search (q to quit): ");
        scanf("%s", choice);

        if(strcmp(choice, "q") == 0)
        {
            return;
        } else
        {
            searchGames(players, playerIndex, choice);
        }
    }
}


int searchGames(Player players[], int playerIndex, char choice[])  
{   
    int nrOfStringMatches = 0;
    int boardGameIndex;
    for (int i = 0; i < players[playerIndex].numOfBoardGames; i++) 
    {
       if (strstr(players[playerIndex].boardGames[i].boardName, choice) != NULL) 
        {
            printf("%-19s %d\n", players[playerIndex].boardGames[i].boardName, players[playerIndex].boardGames[i].rating);
            nrOfStringMatches++;
            boardGameIndex = i;
        }
    }

    if(nrOfStringMatches == 1)
    {  
        return boardGameIndex;
    } else
    {
        return -1;
    }
}


void checkRemoveGameConditions(Player players[], int playerIndex)       
{
    char choice[WORDLENGTH];
    while(1)
    {
        if(players[playerIndex].numOfBoardGames == 0)
        {
            printf("No games registered\n");
            return;
        }

        printf("Search boardgame to remove (q to quit): ");
        scanf("%s", choice);

        int boardGameIndex = searchGames(players, playerIndex, choice); 
        if(strcmp(choice, "q") == 0)
        {
            return;
        } else if(boardGameIndex >= 0)
        {
            removeGame(players, playerIndex, boardGameIndex);
        } else
        {
            printf("You did not find one unique boardgame.\n");
        } 
    }
}


void removeGame(Player players[], int playerIndex, int boardGameIndex)
{
    char choice[WORDLENGTH];
    printf("Do you want to remove this game (y/n): ");
    scanf("%s", choice);

    if(strcmp(choice, "y") == 0)
    {
        for(int i = boardGameIndex; i < players[playerIndex].numOfBoardGames - 1; i++)
        {
            strcpy(players[playerIndex].boardGames[i].boardName, players[playerIndex].boardGames[i+1].boardName);
            players[playerIndex].boardGames[i].rating = players[playerIndex].boardGames[i+1].rating;
        }
        players[playerIndex].numOfBoardGames--;
    }
}


void writeToFile(Player players[], int nrOfPlayers, char filename[])              
{
    FILE *fp;
    fp = fopen(filename, "w");

    for (int i = 0; i < nrOfPlayers; i++) 
    {
        if (players[i].numOfBoardGames > 0) 
        {
            for (int j = 0; j < players[i].numOfBoardGames; j++) 
            {
                fprintf(fp, "%s %s %d\n", players[i].username, players[i].boardGames[j].boardName, players[i].boardGames[j].rating);
            }
        } else 
        {
            fprintf(fp, "%s NULL 0\n", players[i].username);
        }
    }
    fclose(fp);
    printf("Saving all data to %s\nGoodbye!", filename);
}