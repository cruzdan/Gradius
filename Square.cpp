#include "square.h"

//add a Square element to a Square arrary on the "index" of the array and the index is increased
void addToEndSquare(Square array[], int& index, Square element) {
	array[index] = element;
	index++;
}

//delete a Square element of a Square array with the correspondient id, reorganize the elements and the index is decremented
void deleteOfSquares(Square array[], int& index, int id) {
	for (int i = 0; i < index; i++) {
		if (array[i].id == id) {
			for (int j = i + 1; j < index; j++) {
				array[j - 1] = array[j];
				array[j - 1].id = j - 1;
			}
			index--;
			break;
		}
	}
}

//add a CenterSquare element to a CenterSquare arrary on the "index" of the array and the index is increased
void addToEndCenterSquare(CenterSquare array[], int& index, CenterSquare element) {
	array[index] = element;
	index++;
}

//delete a CenterSquare element of a CenterSquare array with the correspondient id, reorganize the elements and the index is decremented
void deleteOfCenterSquares(CenterSquare array[], int& index, int id) {
	for (int i = 0; i < index; i++) {
		if (array[i].progress == id) {
			for (int j = i + 1; j < index; j++) {
				array[j - 1] = array[j];
			}
			index--;
			break;
		}
	}
}