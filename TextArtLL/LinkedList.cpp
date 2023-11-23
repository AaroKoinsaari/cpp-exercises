#include <iostream>
#include <Windows.h>
#include "Definitions.h"
using namespace std;


Node* newCanvas()
{

	//creates a new node with a blank canvas in it
	Node* current = new Node;
	initCanvas(current->item);
	current->next = NULL;

	return current;
	//return NULL;
}


Node* newCanvas(Node* oldNode)
{
	// TODO: Write the code for the function

	//Creates a new node and copies the contents of the contents of the old node into it
	Node* newNode = new Node;
	for (int x = 0; x < MAXROWS; x++) {
		for (int y = 0; y < MAXCOLS; y++) {
			newNode->item[x][y] = oldNode->item[x][y];
		}
	}
	newNode->next = NULL;

	return newNode;
}


void play(List& clips)
{
	// loops as long as the ESCAPE key is not currently being pressed
	while (!(GetKeyState(VK_ESCAPE) & 0x8000))
	{
		// TODO: Write the code for the function
		if (clips.count < 2)
			return;
		int count = clips.count;


		playRecursive(clips.head, clips.count);
	}

}


void playRecursive(Node* head, int count)
{
	// TODO: Write the code for the function
	if (count == 0)
		return;
	//play next clip 

	playRecursive(head->next, count - 1);
	displayCanvas(head->item);
	printf("Hold <ESC> to stop\t");
	printf("Clips: %2d", count);
	// Pause for 100 milliseconds to slow down animation
	Sleep(100);
}


void addUndoState(List& undoList, List& redoList, Node*& current)
{
	// TODO: Write the code for the function

	addNode(undoList, newCanvas(current));
	////Add the new node to the front of the undo list


	////Delete the redo list
	deleteList(redoList);

}


void restore(List& undoList, List& redoList, Node*& current)
{
	// TODO: Write the code for the function
	if (undoList.head == NULL)
		return;
	//add the current node to the redo list
	addNode(redoList, current);

	//take a node from the undoList 
	//and make it into the current node
	current = removeNode(undoList);


}


void addNode(List& list, Node* nodeToAdd)
{
	// TODO: Write the code for the function
	nodeToAdd->next = list.head; // next pointer to the current head
	list.head = nodeToAdd;
	list.count++;
}


Node* removeNode(List& list)
{
	// TODO: Write the code for the function
	if (list.head == NULL)
		return NULL;

	Node* remove = list.head;
	list.head = remove->next; // head to the next node
	remove->next = NULL; // unlink the node from list
	list.count--;

	return remove;
	return NULL;
}


void deleteList(List& list)
{
	// TODO: Write the code for the function
	Node* current = list.head; // starting at the head

	while (current != NULL) {
		Node* next = current->next; // store pointer to next node
		delete current; // freeing the memory
		current = next;
	}
	// resetting the values in the list
	list.count = 0;
	list.head = NULL;


}


bool loadClips(List& clips, char filename[])
{
	//// TODO: Write the code for the function
	//erase the list
	deleteList(clips);

	char fullFileName[FILENAMESIZE];
	int i = 1;
	while (true)
	{
		snprintf(fullFileName, FILENAMESIZE, "%s-%d.txt", filename, i);

		Node* current = newCanvas();
		bool loaded = loadCanvas(current->item, fullFileName);
		if (loaded)
		{
			addNode(clips, current);
			i++;
		}
		if(!loaded && i < 2)
		{
			delete current;
			return false;
		}
		else if (!loaded && i > 2)
		{
			return true;
		}
	}
	
}


bool saveClips(List& clips, char filename[])
{
	// TODO: Write the code for the function

	Node* current = clips.head;

	//checks to make sure that there is stuff to save
	if (current == NULL)
	{
		return false;
	}

	int clipNumber = 1;

	//Goes until current == NULL which would indicate the end of the list
	//Calls the saveCanvas function in order to save the files
	while (current != NULL)
	{
		char clipPath[FILENAMESIZE];
		snprintf(clipPath, FILENAMESIZE, "%s-%d", filename, clipNumber);
		saveCanvas(current->item, clipPath);
		current = current->next;
		clipNumber++;
	}
	return true;
}