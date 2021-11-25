#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define RAM_SIZE 100

struct node {
  int size;
  int adresse;
  int free;
  struct node *next;
};

typedef struct node block;

block *memory;

void init_memory(){
  memory = malloc(sizeof(block));
  memory->adresse = 0;
  memory->size = RAM_SIZE;
  memory->free = 1; // 1 = true
  memory->next = NULL;
}



void display(){

  int i = 1;
  block *temp;
  temp = memory;

  printf("\n");
  printf(" --------- Display Status --------- \n");
  printf("\n");
  printf(" ---------------------------------- \n");
  printf(" | Node | Adresse | Size | Status | \n");
  printf(" ---------------------------------- \n");
  while(temp != NULL){
    printf(" |  %d   |    %d    |  %d  |", i, temp->adresse, temp->size);
    if (temp->free == 1)
       printf("  free | \n");
    else
       printf("  used | \n");
    printf(" ---------------------------------- \n");
    temp=temp->next;
    i++;
    }
  printf("\n");
}

void first_fit(int size){

  block *temp;
  temp = memory;
  while (temp != NULL){
    if ( (temp->size >= size) && (temp->free == 1) ){

      temp->free = 0;

      if (temp-> size > size ){

        block *free;
        free = malloc(sizeof(block));
        free->size = temp->size - size;
        free->adresse = temp->adresse + size;
        free->free = 1;
        free->next = temp->next;

        temp->size = size;
        temp->next = free;
      }
      break;
    }
    else { temp = temp->next; }
  }
  if (temp == NULL)
  printf(" Error : No enough free space.\n");
}

void stop_process(int node_number){

  block *temp;
  temp = memory;
  int i = 1;

  while( (temp != NULL) && (i < node_number) ){
    temp = temp->next;
    i++;
  }
  if (temp == NULL)
    printf(" Error : Node unavaible.\n");
  else {
    if (temp->free == 1)
      printf(" Error : Node already free, no process to stop.\n");
    else {
      temp->free = 1;
      printf(" Node %d freed, process stoped.\n",node_number);
    }
  }
}

void best_fit(int size){
  block *temp;
  block *best_fit;
  temp = memory;
  best_fit = NULL;

  while (temp != NULL){
    if ( (temp->size >= size) && (temp->free == 1) ){
      best_fit = temp;
      break;
    }
    else temp = temp->next;
  }

  while (temp != NULL){
    if ( (temp->size >= size) && (temp->free == 1) ){
      if (best_fit->size > temp->size)
        best_fit = temp;
    }
    temp = temp ->next;
  }

  if (best_fit == NULL)
  printf(" Error : No enough free space.\n");
  else {
    best_fit->free = 0;

    if (best_fit-> size > size ){

      block *free;
      free = malloc(sizeof(block));
      free->size = best_fit->size - size;
      free->adresse = best_fit->adresse + size;
      free->free = 1;
      free->next = best_fit->next;

      best_fit->size = size;
      best_fit->next = free;
    }
  }
}

void worst_fit(int size){
  block *temp;
  block *worst_fit;
  temp = memory;
  worst_fit = NULL;

  while (temp != NULL){
    if ( (temp->size >= size) && (temp->free == 1) ){
      worst_fit = temp;
      break;
    }
    else temp = temp->next;
  }

  while (temp != NULL){
    if ( (temp->size >= size) && (temp->free == 1) ){
      if (worst_fit->size < temp->size)
        worst_fit = temp;
    }
    temp = temp ->next;
  }

  if (worst_fit == NULL)
  printf(" Error : No enough free space.\n");
  else {
    worst_fit->free = 0;

    if (worst_fit-> size > size ){

      block *free;
      free = malloc(sizeof(block));
      free->size = worst_fit->size - size;
      free->adresse = worst_fit->adresse + size;
      free->free = 1;
      free->next = worst_fit->next;

      worst_fit->size = size;
      worst_fit->next = free;
    }
  }
}

void compact_memory(){

  block *p = memory;
  block *t = NULL;
  block *q = NULL;

  while(p != NULL){
    if (p->free == 1){
      t = p;
      while ((p->next != NULL) && (p->next->free == 1)){
        p = p->next;
        t->size = t->size + p->size;
        t->next = p->next;
        q = p;
      }
      p = p->next;
      free(q);
    }
    else p = p->next;
  }
}

void reset_addresses(){

  block *temp = memory;
  int current_address = 0;

  while (temp != NULL){
    temp->adresse = current_address;
    current_address = current_address + temp->size;
    temp = temp->next;
  }
}

void defrag_memory(){

  block *temp = memory;
  block *used_blocks = NULL;
  block *free_blocks = NULL;

  while ( temp != NULL ) {
    if ( temp -> free == 0){
      if ( used_blocks == NULL ) {
        used_blocks = temp;
        temp = temp->next;
        used_blocks->next = NULL;
      }
      else {
        block *last_block = used_blocks;
        while ( last_block->next != NULL )
          last_block = last_block->next;
        last_block->next = temp;
        temp = temp->next;
        last_block = last_block->next;
        last_block->next = NULL;

      }
    }
    else {
      if ( free_blocks == NULL ) {
        free_blocks = temp;
        temp = temp->next;
        free_blocks->next = NULL;
      }
      else {
        block *last_block = free_blocks;
        while ( last_block->next != NULL )
          last_block = last_block->next;
        last_block->next = temp;
        temp = temp->next;
        last_block = last_block->next;
        last_block->next = NULL;

      }
    }
  }

  block *last_block = used_blocks;

  while ( last_block->next != NULL ) {
    last_block = last_block->next;
  }

  last_block->next = free_blocks;
  memory = used_blocks;

  reset_addresses();
}

void reset_memory(){

  block *temp = memory;
  block *prec = NULL;

  while ( temp != NULL ){
    prec = temp;
    temp = temp ->next;
    free(prec);
  }
  init_memory();
}


int main() {

  init_memory();
  int choice = 0;


  do{
    printf("\n");    printf(" ======= RAM Manager ======= \n");
    printf("\n");
    printf(" 1. Display Status.\n");
    printf(" 2. Add Process.\n");
    printf(" 3. Stop Process.\n");
    printf(" 4. Manage Free Blocks. \n");
    printf(" 5. Reset Memory. \n");
    printf("\n");
    printf(" 0. Exit.\n");
    printf("\n");

    printf(" - Enter choice : ");
    scanf("%d", &choice);
    printf("\n");
    switch (choice){
      case 1 : {
        display();
        break;
      }
      case 2 : {
        int size;
        int fitting_method;
        printf(" --------- Adding Process --------- \n");
        printf("\n");
        printf(" 1. First Fit.\n");
        printf(" 2. Best Fit.\n");
        printf(" 3. Worst Fit.\n");
        printf("\n");
        printf(" 0. Cancel.\n");
        printf("\n");
        do {
          printf(" - Enter fitting method : ");
          scanf("%d", &fitting_method);

          switch(fitting_method){
            case 1 : {
              printf(" - Enter size (First fit) : ");
              scanf("%d", &size);
              first_fit(size);
              break;
            }
            case 2 : {
              printf(" - Enter size (Best fit) : ");
              scanf("%d", &size);
              best_fit(size);
              break;
            }
            case 3 : {
              printf(" - Enter size (Worst fit) : ");
              scanf("%d", &size);
              worst_fit(size);
              break;
            }
            case 0 : {
              printf("\n");
              printf(" Canceling. \n");
              printf("\n");
              break;
            }
            default : {
              printf("\n");
              printf(" Error : choice unavaible.\n");
              printf("\n");
            }
          }
        } while (fitting_method != 1 && fitting_method != 2 && fitting_method != 3 && fitting_method != 0);
        break;
      }

      case 3 : {
        int node_number;
        printf(" --------- Stoping Process --------- \n");
        printf("\n");
        printf(" 0. Cancel. \n");
        printf("\n");
        printf(" - Enter node number : ");
        scanf("%d", &node_number);
        if (node_number != 0)
          stop_process(node_number);
        else
          printf(" Canceling. \n");
        break;
      }
      case 4 : {
        printf(" --------- Defraging Memory --------- \n");
        printf("\n");
        printf(" 1. Compaction.\n");
        printf(" 2. Defragmentation (Fast).\n");
        printf(" 3. Defragmentation (Full). \n");
        printf("\n");
        do {
          printf(" - Enter choice : ");
          scanf("%d", &choice);
          printf("\n");
          switch (choice){
            case 1 : {
              printf("\n");
              printf(" Compacting... \n");
              compact_memory();
              printf("\n");
              sleep(2);
              break;
            }
            case 2 : {
              printf("\n");
              printf(" Defragmenting ... \n");
              defrag_memory();
              printf("\n");
              sleep(2);
              break;
            }
            case 3 : {
              printf("\n");
              printf(" Full Defragmenting ... \n");
              defrag_memory();
              compact_memory();
              printf("\n");
              sleep(2);
              break;
            }
            case 0 : {
              printf("\n");
              printf(" Canceling. \n");
              printf("\n");
              break;
            }
            default : {
              printf("\n");
              printf(" Error : choice unavaible.\n");
              printf("\n");
              break;
            }
          }
        } while ( choice != 1 && choice != 2 && choice != 3 && choice != 0);
        break;
      }
      case 5 : {
        int confirmation = 0;
        printf(" --------- Reseting Memory --------- \n");
        printf("\n");
        printf(" Reseting memory will stop all process\n");
        printf(" and all date will be lost.\n");
        printf("\n");
        printf(" Confirm : \n");
        printf("      1.Yes                  2.No\n");
        printf("\n");
        do {
          printf(" - Enter choice : ");
          scanf("%d", &confirmation);
          switch (confirmation) {
            case 1 : {
              printf("\n");
              printf(" Reseting Memory... \n");
              reset_memory();
              printf("\n");
              sleep(2);
              printf(" Memory reseted. \n");
              break;
            }
            case 2 : {
              printf("\n");
              printf("  Reseting Canceled \n");
              printf("\n");
              break;
            }
            default : {
              printf("\n");
              printf(" Error : choice unavaible.\n");
              printf("\n");
              break;
            }
          }
        } while ( (confirmation != 1) && (confirmation != 2) );
        break;
      }
      case 0 : {
        printf("\n");
        printf(" EXIT...\n");
        printf("\n");
        break;
      }
      default : {
        printf("\n");
        printf(" Error : choice unavaible.\n");
        printf("\n");
      }
    }
  } while (choice!=0);
  sleep(2);
  return 0;
}
