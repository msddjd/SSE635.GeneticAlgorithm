// SSE635.Proj1.GeneticAlgorithms.cpp : Defines the entry point for the console application.
// Mike Dumas
// Genetic Algorithm that begins with 26 random lower case letters and counts how many
// generations it takes to produce the letters of English alphabet in alphabetical order.

#include "stdafx.h"
#include <string>
#include <vector>
#include <stdlib.h>     
#include <time.h>   
#include <iostream>

using namespace std;

class Chromosome
{
public:
	Chromosome(string newgene)
	{
		gene = newgene;
	}

	string gene;
	int score = -1;

};

vector<Chromosome> G_Population, G_Selection;
int G_highest_index = 0, G_highest_score = 0, G_past_high_score = 0, G_generation_count = 0;
bool G_found = false;

void initializePopulation(int size);
void printPopulation();
void printBestGene();
void calculateFitness();
void selectChromosomes(int minimum_score);
void findBestGene();
void performCrossover();
void performMutation();




int _tmain(int argc, _TCHAR* argv[])
{
	int minimum_score = 1;
	initializePopulation(52);
	printPopulation();
	calculateFitness(); //Can be improved by only calculating new chromsomes
	printPopulation();

	while (!G_found)
	{
		if ((G_generation_count > 2) && (minimum_score < 26))
			minimum_score = G_highest_score - 2;

		selectChromosomes(minimum_score);
		//printBestGene();
		performCrossover(); //Doing Multipoint Crossover will enhance solution
		performMutation(); //Doing Random mutation instead of decrement/increment will enhance solution, and dont change the best gene
		printBestGene();
	}
	cout << endl << "After " << G_generation_count << " Generations, Solution Found!!!" << endl;
	printBestGene();
	return 0;
}

void initializePopulation(int size)
{
	srand(time(NULL));
	//Generate random number from 97 to 122 corresponding to a-z in Ascii
	int asciiVal, pop_size = size;
	char asciiChar;
	string temp = "";

	for (int h = 0; h < pop_size; h++)
	{
		for (int i = 0; i < 26; i++)
		{
			asciiVal = rand() % 26 + 97;
			asciiChar = asciiVal;
			temp += asciiChar;
		}

		G_Population.push_back(Chromosome(temp));
		temp = "";
	}
}

void printPopulation()
{
	cout << endl << "\n\t\tPopulation\n" << endl;
	cout << "Index\t\t" << "Gene\t\t" << "\tScore" << endl;
	for (int i = 0; i < G_Population.size(); i++)
	{
		cout << i << ":\t" << G_Population[i].gene << "\t" << G_Population[i].score << endl;
	}

}

void printBestGene()
{
	findBestGene();
	cout << endl << "Generation#: " << G_generation_count << endl;
	cout << "BestGene: " << G_Population[G_highest_index].gene << "\tScore:" << G_Population[G_highest_index].score << endl;

}

void calculateFitness()
{
	cout << endl << "calculatingFitness...";
	int count;
	for (int h = 0; h < G_Population.size(); h++)
	{
		count = 0;
		for (int i = 0; i < G_Population[h].gene.length(); i++)
		{
			if (G_Population[h].gene[i] == char(i + 97))
				count++;
		}

		G_Population[h].score = count;
		if (count == 26)
			G_found = true;
	}
	findBestGene();
}

void selectChromosomes(int minimum_score)
{
	cout << endl << "selectingChromosomes...";
	for (int i = G_Population.size() - 1; i > 0; i--)
	{
		if (G_Population[i].score >= minimum_score)
		{
			G_Selection.push_back(G_Population[i]);
			G_Population.pop_back();
		}

	}
	G_Population.clear();
	G_Population = G_Selection;
	G_Selection.clear();
}

void findBestGene()
{
	cout << endl << "findingBestGene...";
	//Find Chromosome with Highest Score
	int highest_score = G_Population[0].score, highest_index = 0;
	string best_gene = "", current_gene = "";

	for (int i = 1; i < G_Population.size(); i++)
	{
		if (highest_score < G_Population[i].score)
		{
			highest_score = G_Population[i].score;
			highest_index = i;
			best_gene = G_Population[i].gene;
		}
	}
	G_highest_index = highest_index;
	G_highest_score = highest_score;
}

void performCrossover()
{
	findBestGene();
	cout << endl << "performingCrossover...";
	G_past_high_score = G_highest_score;
	//Mate first found highest chromosome with every other Selected Chromsome to produce children
	//Pick random letter from non-highest
	//Swap it with random letter from highest
	//Repeat until performed on all non-high Chromsomes

	srand(time(NULL));
	int rand_index1 = 0, rand_index2 = 0, initial_size = G_Population.size();
	string letter2 = "", current_gene = "", best_gene = G_Population[G_highest_index].gene;

	for (int i = 0; i < initial_size; i++)
	{
		if (i != G_highest_index)
		{
			rand_index1 = rand() % 26;
			rand_index2 = rand() % 26;
			current_gene = G_Population[i].gene;
			letter2 = current_gene[rand_index2];
			best_gene.replace(rand_index1, 1, letter2);
			G_Population.push_back(Chromosome(best_gene));
			best_gene = G_Population[G_highest_index].gene;
		}
	}
	calculateFitness();
}

void performMutation()
{
	findBestGene();

	if ((G_past_high_score == G_highest_score) && (!G_found))
	{
		cout << endl << "performingMutation...";

		srand(time(NULL));
		int rand_index = 0, ascii_letter = 0;
		string current_gene = "";
		char current_char_letter = 'a';
		string current_string_letter = "";

		for (int i = 0; i < G_Population.size(); i++)
		{
			if (i != G_highest_index) //Mutate All but Highest Index
			{
				rand_index = rand() % 26;
				ascii_letter = rand() % 26 + 97;
				current_gene = G_Population[i].gene;
				current_char_letter = current_gene[rand_index];
				current_char_letter = ascii_letter;
				current_string_letter = current_char_letter;
				current_gene.replace(rand_index, 1, current_string_letter);
				G_Population[i].gene = current_gene;
			}
		}
		calculateFitness();
	}
	G_generation_count++;
}

