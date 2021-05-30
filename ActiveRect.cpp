#include "activeRect.h"

//add an ActiveRect element to an ActiveRect arrary on the "index" of the array and the index is increased
void addToEndActiveRect(ActiveRect array[], int& index, ActiveRect element) {
	array[index] = element;
	index++;
}

//delete an ActiveRect element with the correspondient id, reorganize the elements and the index is decremented
void deleteOfActiveRect(ActiveRect array[], int& index, int id) {
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

//add a Missile element to an Missile arrary on the "index" of the array and the index is increased
void addToEndMissile(Missile array[], int& index, Missile element) {
	array[index] = element;
	index++;
}

//delete a Missile element with the correspondient id, reorganize the elements and the index is decremented
void deleteOfMissile(Missile array[], int& index, int id) {
	for (int i = 0; i < index; i++) {
		if (array[i].id == id) {
			for (int j = i + 1; j < index; j++) {
				array[j - 1] = array[j];
			}
			index--;
			break;
		}
	}
}