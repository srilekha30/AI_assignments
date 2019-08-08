#include<stdio.h>
#include<time.h>
#include<math.h>
#include<stdlib.h>

typedef struct node{
    int puzzle[16];
    int empty;
    float cost;
    int g;
}node;

typedef struct queue{
    node *game;
    struct queue* next;

}queue;

queue* open;
queue* close;


int checkEquality(node* game1,node* game2){

    int i;
    for(i=0;i<16;i++){

        if(game1->puzzle[i] != game2->puzzle[i]){

            return 0;
        }
    }
    return 1;
}

void swap(node *game,int i,int j){
    int dummy ;
    dummy = game->puzzle[j];
    game->puzzle[j] = game->puzzle[i];
    game->puzzle[i] = dummy;
    game->empty = i;
}
void printGameInNode(node *matrix){
    int i,j=0;
    for(i=0;i<16;i++){
            printf("%d ",matrix->puzzle[i]);
        j++;
        if(j%4==0){

            printf("\n");
        }
    }
}


int checkLists(node* game){

    queue* open_head = open;
    while(open_head->game!=NULL){

        if(checkEquality(open_head->game,game)==1){

            if(game->cost < open_head->game->cost){

                open->game->cost = game->cost;
            }
            return 1;
        }
        open_head = open_head->next;
    }

    queue* close_head = close;
    while(close_head->game!=NULL){

        if(checkEquality(close_head->game,game)==1){

            return 1;
        }
        close_head = close_head->next;
    }
    return -1;
}

int blankPosition(int value,node* game){

    int i;
    for(i=0;i<16;i++){

        if(game->puzzle[i] == value){

            return i;
        }
    }
}

queue* deleteNode(node* game){

    queue* listhead = open;
    queue* list=open;
    if(checkEquality(list->game,game)==1){

        list = list->next;
    }
    else{

        while(listhead->next!=NULL){

            if(checkEquality(listhead->next->game,game)==1){

                listhead->next = listhead->next->next;
                break;
            }
            listhead = listhead->next;
        }
    }
    return list;
}


int heuristic_1(int puzzle[][4]) {
    int misplaced=0;
    int number=1;
    int i,j;
    for (i=0;i<4;i++) {
        for(j=0;j<4;j++) {
            if(puzzle[i][j]!=number) {
                misplaced+=1;
            }
            number+=1;
        }
    }
    return misplaced;
}


float heuristic_2(node* game,node* initial){

    int i;
    int position;
    float cost=0;
    for(i=0;i<16;i++){

        position = blankPosition(game->puzzle[i],initial);
        cost += abs(((i/4)-(position/4)))+abs(((i%4)-(position%4)));
    }
    return cost;
}


void insertNodetoOpen(node *head,node* initial){

    queue* open_head = open;
    while(open_head->game!=NULL){

        open_head = open_head->next;
    }
    

    if(head->empty/4<3) { //down
        
        int hBlank= head->empty;
        
        node *child_2 = (node *)malloc(sizeof(node));

        queue* temp_2 =(queue* )malloc(sizeof(queue));

        swap(head,hBlank+4,hBlank);
        *child_2 = *head;
        swap(head,hBlank,hBlank+4);

        if(checkLists(child_2)==-1){
         
            child_2->cost=heuristic_2(child_2,initial);
            open_head->game = child_2;
            open_head->next = temp_2;
            open_head = open_head->next;
        }
        open_head->game=NULL;
        open_head->next=NULL;

    }
    if(head->empty/4>0) { //up
        int hBlank = head->empty;
       // int hBlankj = head->blankj;
        node *child_1 = (node *)malloc(sizeof(node));

        queue* temp =(queue* )malloc(sizeof(queue));

        swap(head,hBlank-4,hBlank);
        *child_1 = *head;
        swap(head,hBlank,hBlank-4);

        if(checkLists(child_1)==-1){
         
            child_1->cost=heuristic_2(child_1,initial);
            open_head->game = child_1;
            open_head->next = temp;
            open_head = open_head->next;
        }
        open_head->game=NULL;
        open_head->next=NULL;

    }
    
    if(head->empty%4>0) { //left
        int hBlank = head->empty;
        //int hBlankj = head->blankj;
        node *child_3 = (node *)malloc(sizeof(node));

        queue* temp_3 =(queue* )malloc(sizeof(queue));

        swap(head,hBlank-1,hBlank);
        *child_3 = *head;
        swap(head,hBlank,hBlank-1);

        if(checkLists(child_3)==-1){
         
            child_3->cost=heuristic_2(child_3,initial);
            open_head->game = child_3;
            open_head->next = temp_3;
            open_head = open_head->next;
        }
        open_head->game=NULL;
        open_head->next=NULL;

    }
    if(head->empty%4<3) { //next
        int hBlank = head->empty;
        //int hBlankj = head->blankj;
        node *child_4 = (node *)malloc(sizeof(node));

        queue* temp_4 =(queue* )malloc(sizeof(queue));

        swap(head,hBlank+1,hBlank);
        *child_4 = *head;
        swap(head,hBlank,hBlank+1);

        if(checkLists(child_4)==-1){
         
            child_4->cost=heuristic_2(child_4,initial);
            open_head->game = child_4;

            open_head->next = temp_4;
            open_head = open_head->next;
        }
        open_head->game=NULL;
        open_head->next=NULL;

    }   
}


//Heuristic - Number of misplaced tiles
node* optimalgame1(){

    queue* listhead = open;
    int count = 0;
    float minF = -1;
    int i;
    node* optgame = NULL;
    while(listhead->next!=NULL){

        count=0;
        for(i=0;i<15;i++){

            if(listhead->game->empty!=i){
                if(listhead->game->puzzle[i]!=i+1){

                    count++;
                }
            }
        }
        if(minF==-1){

            minF = count;//+listhead->game->cost;
            optgame = listhead->game;
        }
        else if(minF > count){//+listhead->game->cost){

            minF = count;//+listhead->game->cost;
            optgame = listhead->game;
        }
        listhead = listhead->next;
    }
    
    open=deleteNode(optgame);
    return optgame;
}

int checkGoal(node* game){

    int i=0;
    for(i=0;i<15;i++){

        if(game->puzzle[i]!=i+1){

            return -1;
        }
    }
    if(game->empty==15){

        return 1;
    }
    else{

        return -1;
    }
}

node* solvePuzzle15(node* game){

    node* initial = game; 
    queue* close_head = close;
    queue* node = (queue*)malloc(sizeof(queue));
    node->game=NULL;
    node->next=NULL;
    open->game=game;
    open->next=node;

    int moves=0;
    while(open->game!=NULL){

        /* printf("START\n");
        queue* listHead = open;
        while(listHead->game!=NULL){

            printGameInNode(listHead->game);
            listHead = listHead->next;
        } */

        close_head->game=optimalgame1();
        printf("\n");
        
        close_head->next = (queue* )malloc(sizeof(queue));
        
        if(checkGoal(close_head->game)==1){

            return close_head->game;
        }

        moves++;
        printGameInNode(close_head->game);
        insertNodetoOpen(close_head->game,initial);
        
        /* printf("Middel\n");
        listHead = open;
        while(listHead->game!=NULL){

            printGameInNode(listHead->game);
            listHead = listHead->next;
        }
        printf("END\n"); */
        close_head = close_head->next;
        /* if(moves==6){
            break;
        } */
    }
    return close_head->game;
}

int main(){
    srand((unsigned int) time(NULL));
    node *game =(node *)malloc(sizeof(node));
    open =(queue* )malloc(sizeof(queue));
    close =(queue* )malloc(sizeof(queue)); 
    
    game->empty = 15;
    game->cost=0;
    int i,j;
    int first[4][4]={{5,8,1,2},{9,11,15,10},{6,12,7,3},{4,13,14,0}};
    //int first[4][4]={{1,2,3,4},{5,6,7,8},{9,10,11,0},{13,14,15,12}};
   /* for(i=0;i<16;i++){

        if(game->empty!=i){
            game->puzzle[i] = 1 + rand()%15;
            for(j=0;j<i;j++){

                if(game->puzzle[i]==game->puzzle[j]){

                    i--;
                    break;
                }
            }
        }
        else{
            game->puzzle[i] = 0;
        }
    }*/
        for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            game->puzzle[i*4+j]=first[i][j];
            
        }
    }
    printf("Starting State:\n");
    printGameInNode(game);
    if(checkGoal(game)==1){

        printf("Final State:\n");
        printGameInNode(game);
    }
    else{
        
        node* finalgame=solvePuzzle15(game);
        if(finalgame!=NULL){

            printf("Solution found\n");
            printf("END State:\n" );
            printGameInNode(finalgame);            
        }
        else{

            printf("The puzzle cannot be solved.\n");
        }
    }
    return 0;
}
