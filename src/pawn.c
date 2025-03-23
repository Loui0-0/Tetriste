#include <stdlib.h>
#include <stdio.h>
#include "config.h"
#include "pawn.h"

Pawn* newPawn(int colorId,int shapeId){
	Pawn* pawn = malloc(sizeof(pawn));
	if(pawn == NULL){
		return NULL;
	}
	pawn->colorId = colorId;
	pawn->shapeId = shapeId;
	return pawn;
}

Pawn** newPawnHolder(int colorId,int shapeId){
	/*
		cette fonction alloue un ptr vers un ptr qui pointe vers un struct pawn cette
		structure de donnée nous permettra de faire des decallages efficaces
	*/
	Pawn* pawn = newPawn(colorId,shapeId);

	if (pawn == NULL) {
		return NULL;
	}

	Pawn** pawnPtrPtr = malloc(sizeof(Pawn*));

	if (pawnPtrPtr == NULL) {
		free(pawn);
		return NULL;
	}

	*pawnPtrPtr = pawn;
	return pawnPtrPtr;

}



int pawn_getShapeId(Pawn** pawnPtrPtr){
	return (*pawnPtrPtr)->shapeId; 
}

int pawn_getColorId(Pawn** pawnPtrPtr){
	return (*pawnPtrPtr)->colorId;
}

void pawnHolder_free(Pawn** pawnPtrPtr){
	
	if (pawnPtrPtr == NULL){
		return;
	}
	Pawn* toFree = *pawnPtrPtr;
	
	free(pawnPtrPtr);
	
	if (toFree == NULL){
		return;
	}
	free(toFree);
}

void pawn_Display(Pawn** pawnPtrPtr){
	int ShpId = pawn_getShapeId(pawnPtrPtr);
  int ColId = pawn_getColorId(pawnPtrPtr);
  printf("c%d s%d, ",ColId ,ShpId);
}
