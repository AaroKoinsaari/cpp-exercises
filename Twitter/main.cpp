/**
 * @file Twitter.cpp
 * @brief Simplified Twitter command line program
 * 
 * Program to implement a very simplified list of tweets 
 * from a single simulated Twitter account.
 * Tweets can be added, deleted, and liked.
 * 
 * @author Group 10
 * @version 2023-01-29
*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring> 
using namespace std;

const int MSGSIZE = 100;	// Maximum size for a tweet message
const int CAPACITY = 10;	// Maximum number of tweets

// Structure used to define a tweet entry
struct Tweet
{
	int id;
	char msg[MSGSIZE];
	int likes;
};


/*
* Prints out an entire timeline to the screen
* timeline = timeline of tweets to be printed
* usedSize = number of tweets in the timeline
* selected = position number of currently selected tweet
*/
void displayTimeline(const Tweet timeline[], int usedSize, int selected);


/*
* Edits currently selected tweet
* with a new message entered by the user.
* timeline = timeline in which the tweet is to be edited
* usedSize = number of tweets in the timeline
* selected = position number of currently selected tweet
* If 'selected' represents a valid array position, the
* selected tweet will be updated.
* If 'selected' is not valid a 'no tweet is selected message' will be
* displayed and no changes will be made.
*/
void doEditTweet(Tweet timeline[], int usedSize, int selected);


/*
* Adds a like to the currently selected tweet.
* timeline = timeline in which the tweet is to be edited
* usedSize = number of tweets in the timeline
* selected = position number of currently selected tweet
* If 'selected' represents a valid array position, the
* selected tweet will be updated.
* If 'selected' is not valid a 'no tweet is selected message' will be
* displayed and no changes will be made.
*/
void doLikeTweet(Tweet timeline[], int usedSize, int selected);


/*
* Deleted currently selected tweet.
* timeline = timeline in from which the entry is to be deleted
* usedSize = number of tweets in the timeline
* If 'selected' represents a valid array position:
*   the selected tweet will be deleted
*   usedSize will be updated to reflect the updated number of tweets in the timeline
*   selected will be updated to -1
* If 'selected' is not valid a 'no tweet is selected message' will be
* displayed and no changes will be made.
*/
void doDeleteTweet(Tweet timeline[], int& usedSize, int& selected);


/*
* If there is room in the timeline for new tweets, then this gets
* a new tweet from the user and adds it to the timeline.
* timeline = timeline in which the tweet is to be added
* usedSize = number of tweets in the timeline
* If tweet was able to be added, returns the position number in the
* timeline of where the item was added, and usedSize will be
* updated to reflect the number of tweets now in the timeline.
* If tweet was not able to be added, returns -1, and usedSize
* remains unchanged.
*/
int doAddTweet(Tweet timeline[], int& usedSize);


/*
* Adds a new tweet to the list
* timeline = timeline in which the entry is to be added
* usedSize = number of tweets in the timeline
* message = tweet message to be added
* If tweet was able to be added, returns the position number in the
* timeline of where the item was added, and usedSize will be
* updated to reflect the number of tweets now in the timeline.
* If tweet was not able to be added, returns -1, and usedSize
* remains unchanged.
*/
int addTweet(Tweet timeline[], int& usedSize, const char message[]);


/*
* Returns the next available ID number
* timeline = timeline in which to find the highest ID
* usedSize = number of tweets in the timeline
* If timeline is empty, returns 100;
* otherwise, returns 1 + highest ID number in the timeline
*/
int getNextId(Tweet timeline[], int usedSize);


/*
* Gets a tweet id from the user. Searches a timeline to try
* to find the specified tweet by its id.
* timeline = timeline to be searched
* usedSize = number of tweets in the timeline
* If the tweet is found, the position number of where the tweet
* is stored in the timeline will be returned.  If the tweet is
* not found, a 'not found message' will be printed, and
* the value -1 will be returned.
* If timeline is empty, an 'empty' message will be printed, and
* the value -1 will be returned.
*/
int selectTweet(const Tweet timeline[], int usedSize);

/**
 * @brief The main function of the program
 * 
 * Simulates functions from Twitter.
 * 
 * @return 1 if program runs correctly.
*/
int main()
{
	Tweet timeline[CAPACITY];	// Twitter timeline
	int choice;					// User's menu choice
	int usedSize = 0;			// Num of tweets in the timeline
	int selected = -1;			// Currently selected tweet
	int tmp;					// Temporary variable

	// Add some starter entries for testing purposes
	selected = addTweet(timeline, usedSize, "Where do they get the seeds to plant seedless watermelons?");
	selected = addTweet(timeline, usedSize, "Waffles are just pancakes with convenient boxes to hold your syrup.");
	selected = addTweet(timeline, usedSize, "Last night I even struck up a conversation with a spider. Turns out he's a web designer.");

	do
	{
		cout << "1. Display Timeline\n";
		cout << "2. Select Tweet\n";
		cout << "3. Add New Tweet\n";
		cout << "4. Edit Selected Tweet\n";
		cout << "5. Like Selected Tweet\n";
		cout << "6. Delete Tweet\n";
		cout << "7. Exit\n";
		cout << endl;
		cout << "Select: ";

		// Get the numeric entry from the menu
		cin >> choice;

		// Corrects issues where user might enter a non-integer value
		while (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Select: ";
			cin >> choice;
		}

		// Makes the 'enter' key that was pressed after the numeric entry be ignored
		// Should be used after getting any numeric input from the keyboard
		cin.ignore();

		switch (choice)
		{
		case 1:
			displayTimeline(timeline, usedSize, selected);
			break;
		case 2:
			tmp = selectTweet(timeline, usedSize);
			// if selected tweet exists, update selected variable;
			// otherwise leave it unchanged
			if (tmp > -1)
				selected = tmp;
			break;
		case 3:
			tmp = doAddTweet(timeline, usedSize);
			// if tweet was added, make it be the selected tweet;
			// otherwise leave it unchanged
			if (tmp > -1)
				selected = tmp;
			break;
		case 4:
			doEditTweet(timeline, usedSize, selected);
			break;
		case 5:
			doLikeTweet(timeline, usedSize, selected);
			break;
		case 6:
			doDeleteTweet(timeline, usedSize, selected);
			break;
		}

	} while (choice != 7);

	return 1;
}


/**
 * Adds tweet to the timeline
*/
int doAddTweet(Tweet timeline[], int& usedSize)
{
	// Check if there is room for a new tweet
	if (usedSize < CAPACITY)
	{

		//Get new tweet from the user
		char msg[MSGSIZE];
		cout << "\nPlease enter the message : ";
		cin.getline(msg, MSGSIZE);
		cout << endl;

		// Create new tweet and add it to timeline
		Tweet newTweet;

		newTweet.id = getNextId(timeline, usedSize);
		strncpy(newTweet.msg, msg, MSGSIZE);
		newTweet.likes = 0;

		int position = addTweet(timeline, usedSize, msg);

		if (position != -1)
		{
			//Tweet added, update usedSize
			return position;
		}
		else
		{
			//Tweet could not be added, return -1

			return -1;
		}

	}
	else

		cout << "\nError: Timeline is full.\n\n";

	return -1;
}


void doEditTweet(Tweet timeline[], int usedSize, int selected)
{
	//This is a comparison to check if the selected tweet is valid
	if (selected < usedSize && selected >= 0)
	{
		cout << "\nPlease enter something for the new tweet.\n";

		//cin.getline is used to take an input from the user to edit the contents of the tweet
		cin.getline(timeline[selected].msg, MSGSIZE);
		if ((cin.fail()))
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
	else
	{
		//If the tweet that is selected is not valid this message will show
		cout << "No tweet is selected.\n\n";
	}
}


void doLikeTweet(Tweet timeline[], int usedSize, int selected)
{

	// This is a comparison to check if the selected tweet is valid
	if (selected < usedSize && selected >= 0)
	{
		//This line of code will take the likes and add 1 to it
		timeline[selected].likes = timeline[selected].likes + 1;
	}
	else
	{
		//If the tweet that is selected is not valid this message will show
		cout << "No tweet is selected.\n\n";
	}

}


void displayTimeline(const Tweet timeline[], int usedSize, int selected)
{
	// Displays the col above the tweets and header overall
	cout << "\nTweets:\n\n";

	cout << "Sel     ID      Likes    Tweet\n";

	// Lets the user know the twitter timeline is empty
	if (usedSize == 0)
	{
		cout << "        ***** EMPTY *****\n";
		cout << "\n";
	}

	int tempSel = selected; // ensures the selected value will not get changed

	for (int i = 0; i < usedSize; i++)
	{
		// For displaying arrow with selected only

		if (i == tempSel)
		{
			printf("-->     %-11d%-8d%s\n", timeline[i].id, timeline[i].likes, timeline[i].msg);
		}
		// For any other print made to not include the arrow
		else
		{
			printf("        %-11d%-8d%s\n", timeline[i].id, timeline[i].likes, timeline[i].msg);
		}
	}
}


int addTweet(Tweet timeline[], int& usedSize, const char message[])
{
	// If the usedSize is less then the CAPACITY, then a new tweet can be added
	if (usedSize < CAPACITY)
	{
		int position = usedSize;

		int nextID = getNextId(timeline, usedSize);

		timeline[position].id = getNextId(timeline, usedSize); //Get the next available ID #
		strcpy(timeline[position].msg, message); //Copy the message into the tweet
		timeline[position].likes = 0; //Set the number of likes to zero

		usedSize++;

		return position;
	}
	else


		return -1;
}


int getNextId(Tweet timeline[], int usedSize)
{
	// TODO: Write code for the function
	if (usedSize == 0)
	{
		return 100;
	}
	else
	{
		int highestID = timeline[0].id;
		for (int i = 1; i < usedSize; i++)
		{
			if (timeline[i].id > highestID)
			{
				highestID = timeline[i].id;
			}
		}
		return highestID + 1;
	}

	//basically find the highest id number and add 1 to it. think of adding to the end of a partially filled array
	return 100;
}


void doDeleteTweet(Tweet timeline[], int& usedSize, int& selected)
{

	int tempSelect = selected; // The selected value does not change

	// checks if the selected is within the number of tweets
	if (selected > usedSize || selected == -1)
	{
		cout << "\nNo tweet is selected.\n\n";
	}
	else
	{
		// Checks if the selected tweet is the last tweet
		// If it is the last tweet, no shift is needed just to display one less tweet
		if (selected == usedSize)
		{
			usedSize--;
		}
		else
		{
			// shifts the tweets, so it fills the gap 
			for (int i = tempSelect; i < usedSize; i++)
			{
				timeline[i] = timeline[i + 1];
			}
			selected = -1;
			usedSize--;
		}
		cout << "\n";
		cout << "Tweet was deleted.\n\n";
	}
}

int selectTweet(const Tweet timeline[], int usedSize)
{
	// If usedSize is still at 0, there are no tweets in the timeline, returns -1
	if (usedSize == 0)
	{
		cout << "\n";
		cout << "Timeline is empty.\n\n";
		return -1;
	}

	int choiceID = 0;

	cout << "\nEnter Tweet ID: ";
	cin >> choiceID;
	cout << endl;


	// Corrects issues where user might enter a non-integer value
	while (cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Enter Tweet ID: ";
		cin >> choiceID;
		cout << endl;
	}
	cin.ignore();

	for (int selection = 0; selection < usedSize; selection++)
	{
		if (choiceID == timeline[selection].id)
		{
			return selection;
		}
	}

	cout << "ID was not found.\n\n";
	return -1;
}