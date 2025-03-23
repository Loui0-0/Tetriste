#ifndef PAWN_H
#define PAWN_H


typedef struct {
  /* Structure définissant les paramètres de la pièce par une un numéro de Shape 
  et un numéro de couleur*/
  int shapeId;
  int colorId;
}Pawn; 

Pawn*  newPawn(int colorId,int shapeId);
Pawn** newPawnHolder(int colorId,int shapeId);

int pawn_getShapeId(Pawn** pawnPtrPtr);
int pawn_getColorId(Pawn** pawnPtrPtr);
void pawnHolder_free(Pawn** pawnPtrPtr);
void pawn_Display(Pawn** pawnPtrPtr);

#endif //PAWN_H