#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"

//list to track critical registers
struct regNode {
	int value;
	struct regNode *prev;
	struct regNode *next;
};

struct regNode* regList = NULL;

//goes through regList and returns 0 if absent, 1 if present
int searchList(int searchValue){
	struct regNode* ptrNode = regList;
	while(ptrNode != NULL){
		if(ptrNode->value == searchValue){
			return 1;
		}
		ptrNode = ptrNode->next;
	}
	return 0;
}

//free allocated memory of regList
void freeList(){
	struct regNode *ptr = regList;
	struct regNode *ptrNext;
	while(ptr != NULL){
		ptrNext = ptr->next;
		free(ptr);
		ptr = ptrNext;
	}
}

//works backwards to find all instructions that contribute to WRITE and marks them as critical
void findCritical(Instruction *writeInstr){
	Instruction *ptr = writeInstr;
	int var = writeInstr->field1;
	struct regNode* newNode = NULL;
	struct regNode* ptrNode = NULL;
	int critical1, critical2, critical3;

	while(ptr != NULL){
		//first find STORE instruction for WRITE 
		if(ptr->opcode == STORE && ptr->field1 == var){
			newNode = (struct regNode*)malloc(sizeof(struct regNode));
			newNode->value = var;
			newNode->next = NULL;
			newNode->prev = NULL;
			regList = newNode;

			newNode = (struct regNode*)malloc(sizeof(struct regNode));
			newNode->value = ptr->field2;
			newNode->next = NULL;
			newNode->prev = regList;
			regList->next = newNode;
			ptrNode = newNode;

			ptr->critical = 'y';
		}
		//find the rest of the instructions that contribute to WRITE
		else if(regList != NULL) {
			//LOAD & LOADI instructions
			if(ptr->opcode == LOAD || ptr->opcode == LOADI){
				critical1 = searchList(ptr->field1);
				critical2 = searchList(ptr->field2);
				if(critical1 || critical2){
					if(critical1 == 0){
						newNode = (struct regNode*)malloc(sizeof(struct regNode));
						newNode->value = ptr->field1;
						newNode->next = NULL;
						newNode->prev = ptrNode;
						ptrNode->next = newNode;
						ptrNode = newNode;
					}

					if(critical2 == 0){
						newNode = (struct regNode*)malloc(sizeof(struct regNode));
						newNode->value = ptr->field2;
						newNode->next = NULL;
						newNode->prev = ptrNode;
						ptrNode->next = newNode;
						ptrNode = newNode;
					}

					ptr->critical = 'y';
				}
			}
			
			//ADD, SUB, MUL, AND, XOR instructions
			else if(ptr->opcode != WRITE || ptr->opcode != READ){
				critical1 = searchList(ptr->field1);
				critical2 = searchList(ptr->field2);
				critical3 = searchList(ptr->field3);
				if(critical1 || critical2 || critical3){
					if(critical1 == 0){
						newNode = (struct regNode*)malloc(sizeof(struct regNode));
						newNode->value = ptr->field1;
						newNode->next = NULL;
						newNode->prev = ptrNode;
						ptrNode->next = newNode;
						ptrNode = newNode;
					}

					if(critical2 == 0){
						newNode = (struct regNode*)malloc(sizeof(struct regNode));
						newNode->value = ptr->field2;
						newNode->next = NULL;
						newNode->prev = ptrNode;
						ptrNode->next = newNode;
						ptrNode = newNode;
					}

					if(critical3 == 0){
						newNode = (struct regNode*)malloc(sizeof(struct regNode));
						newNode->value = ptr->field3;
						newNode->next = NULL;
						newNode->prev = ptrNode;
						ptrNode->next = newNode;
						ptrNode = newNode;
					}
					
					ptr->critical = 'y';
				}
			}
		}
		
		ptr = ptr->prev;

	}
	
	freeList();
}

void setCriticalValues(Instruction *head){
	Instruction *ptr;
	ptr = head;
	
	//assign all READ instructions as 'y' for critical
	//everything else will be temporarily assigned as 'n' for non-critical
	if(head->opcode == READ){
		head->critical = 'y';
	}
	else{
		head->critical = 'n';
	}

	do{
		ptr = ptr->next;
		if(ptr->opcode == READ){
			ptr->critical = 'y'; //y for yes, n for no
		}
		else{
			ptr->critical = 'n';
		}
	}while(ptr->next != NULL);

	//ptr is now last node
	//go backwards through the list, and call findCritical when a WRITE
	//instruction is found
        while(ptr != NULL){
		if(ptr->opcode == WRITE){
			findCritical(ptr);
			ptr->critical = 'y';
		}
		ptr = ptr->prev;
	}
}

void deleteDeadCode(Instruction *head){
        Instruction *ptr = head;
	Instruction *ptrPrev;
	Instruction *ptrNext;
	//iterate through instruction list and delete all code marked as 'n' in critical field
	while(ptr != NULL){
		if(ptr->critical == 'n'){
			ptrPrev = ptr->prev;
			ptrNext = ptr->next;

			if(ptrNext == NULL){
				ptrPrev->next = NULL;
			}
			else{
				ptrPrev->next = ptrNext;
				ptrNext->prev = ptrPrev;
			}

			free(ptr);
		}
		
		ptr = ptrNext;
	}
}

int main()
{
	Instruction *head;
	head = ReadInstructionList(stdin);

	if (!head) {
		WARNING("No instructions\n");
		exit(EXIT_FAILURE);
	}

	setCriticalValues(head);
	deleteDeadCode(head);	

	if (head) {
		PrintInstructionList(stdout, head);
		DestroyInstructionList(head);
	}
	return EXIT_SUCCESS;
}
