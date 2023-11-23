#include <iostream>
using namespace std;

// Create an enumeration data type called Money
enum class Money { nickel = 5, dime = 10, quarter = 25, dollar = 100 };

const int MAXSIZE = 26;
const int PRICE = 130;

// Prints price to the screen in the format: $X.XX
// price represents an amount of money, in pennies
void PrintPrice(int price);

// Takes user input (1, 2, 3, 4) and converts it to a Money type
Money ConvertToMoney(int x);

// Prints a word to the screen stating what the Money type is
void PrintMoney(Money x);

// Prints the coins to dispense, which add up to amount
// Note: This will never need to dispense a dollar
void DispenseChange(int amount);

// TODO: Fix 

/**
 * @brief The main function of the program.
 * 
 * Simulates the payment section of a vending machine.
 * 
 * @return 1 if program runs correctly.
*/
int main()
{
	int balance = PRICE;	// keeps track of the balance after each payment 
	Money moneyAmount[MAXSIZE];	//array to store money items
	int moneyItems = 0;		// keeps track of number of inputs
	bool flag = true;		// keeps the while loop running as a true value 

	printf("*** Vending Machine ***\n\n");
	printf("Deposit ");
	PrintPrice(PRICE);
	printf(" for a soda\n\n");

	printf("1. deposit nickel\t 2: deposit dime\t 3: deposit quarter\t 4: deposit dollar\t 5: cancel transaction\n");

	// Ensures the program to keep running until balance is met or transaction is cancelled 
	// Runs the loops until a false condition of return 0 is met 
	while (flag != false)
	{
		int input;
		printf("Enter: > ");
		cin >> input;

		// checks to see if user input is 5: cancel transaction
		// if so return 0 to stop the loop 
		if (input == 5)
		{
			printf("\nTransaction cancelled\n");
			// checks to see if user requires any refunds
			// if so return the values back to user 
			if (balance < PRICE)
			{
				printf("Returning: ");
				printf(">> ");
				for (int i = 0; i <= moneyItems; i++)
				{
					PrintMoney(moneyAmount[i]);
				}
				printf("\n");
			}
			return 1;
		}

		// checks to see if user input is between 1 and 4
		// if so the program runs through the loop until balance is met
		else if ((input > 0) && (input < 5))
		{
			moneyAmount[moneyItems] = (ConvertToMoney(input));
			moneyItems++;

			balance -= (int)ConvertToMoney(input);

			// checks to see if user requires change 
			if (balance < 0)
			{
				printf("\nDispensing Soda\n\n");
				printf("Changed owed: $%d.%02d\n", abs(balance) / 100, abs(balance) % 100);
				printf("Returning: \n>> ");
				DispenseChange(abs(balance));
				printf("\n");
				return 0;
			}
			else if (balance == 0)
			{
				printf("\nDispensing Drink\n\n");
				balance = PRICE;
				return 0;
			}
			// or the loop continues until false condition is met 
			else
			{
				printf("\nAmount left to pay: ");
				PrintPrice(balance);
				printf("\n");
			}
		}
	}

	cout << endl;
	return 0;
}


/**
 * Prints price to the screen in the format: $X.XX.
 * 
 * @param price the amount of money in pennies.
*/
void PrintPrice(int price)
{
	printf("$%d.%02d", price / 100, price % 100);
}


/**
 * Prints the coins to dispense, which add up to amount.
 * 
 * @param amount amount used.
*/
void DispenseChange(int amount)
{
	if (amount >= 25) {
		for (int i = 0; i < amount / (int)Money::quarter; i++) {
			PrintMoney(Money::quarter);
		}
		amount %= (int)Money::quarter;
	}
	if (amount >= 10) {
		for (int i = 0; i < amount / (int)Money::dime; i++) {
			PrintMoney(Money::dime);
		}
		amount %= (int)Money::dime;
	}
	if (amount >= 5) {
		for (int i = 0; i < amount / (int)Money::nickel; i++) {
			PrintMoney(Money::nickel);
		}
		amount %= (int)Money::nickel;
	}
}


/**
 * Converts the user input to the corresponding enumeration class.
 * 
 * @param x input to convert.
*/
Money ConvertToMoney(int x)
{
	if (x == 1)
	{
		return Money::nickel;
	}
	else if (x == 2)
	{
		return Money::dime;
	}
	else if (x == 3)
	{
		return Money::quarter;
	}
	else if (x == 4)
	{
		return Money::dollar;
	}

	else return Money::nickel; // nickel is the default value
}


/**
 * Prints a word to the screen stating what the Money type is.
 * 
 * @param x input to print.
*/
void PrintMoney(Money x)
{
	if (x == Money::nickel)
	{
		printf("nickel");
	}
	else if (x == Money::dime)
	{
		printf("dime");
	}
	else if (x == Money::quarter)
	{
		printf("quarter");
	}
	else if (x == Money::dollar)
	{
		printf("dollar");
	}
}