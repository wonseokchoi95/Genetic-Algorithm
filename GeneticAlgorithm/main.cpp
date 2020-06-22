#include "Genetic_Algorithm.h"

int main()
{

	std::vector<Chromosome> Population = std::vector<Chromosome>(30);
	int Generation = 1;
	std::vector<int> Cost;
	
	// 1. Initialize Population
	for (unsigned int i = 0; i < Population.size(); i++)
	{
		Chromosome set;
		Population[i] = set;
		set.Clear();
	}

	// 2. Calculate Fitness
	int i = 1;
	for (auto& set : Population)
	{
		std::cout << "\n# " << std::to_string(i) << std::endl;
		set.Print();
		set.CalcInCost();
		set.CalcOutCost();
		set.CalcCost();
		std::cout << "in cost: " << set.getInCost() << "\t" << "out cost: " << set.getOutCost() << std::endl;
		i++;
	}

	while (true)
	{
		Generation++;
		Cost.push_back(stoi(Population[0].getCost()));
		std::cout << "\n# of Generation: " << std::to_string(Generation) << std::endl;
		// Sort the Chromosome with the cost.
		// If the cost between two Chromosome are same, sort with the order of Chromosome.
		std::sort(Population.begin(), Population.end());

		std::cout << "The Order of Cost is : ";
		for (auto set : Population)
			std::cout << set.getCost() << " ";
		std::cout << "\n";
		// 3. Check if  loop can be stopped.
		// TODO: Conditional statement should be comparement of past one.
		// If the value difference is not big, Stop GA.
		if (Generation > 30 && (Cost.at(Cost.size()) - Cost.at(Cost.size() - 20)) < 2)
		{
			std::cout << "The Least cost of chromosome is" << std::endl;
			Population[0].Print();
			break;
		}

		// 4. Make Next Generation by CrossOver Calculation
		for (unsigned int i = 1; i < Population.size() / 2; i++)
		{
			std::cout << "Parent 1: " << std::endl;
			Population[i].Print();
			std::cout << "Partent 2: " << std::endl;
			Population[i + 1].Print();
			CrossOver crossover(Population[i * 2 - 1], Population[i * 2]);
			std::cout << "Child 1: " << std::endl;
			Population[i].Print();
			std::cout << "Child 2: " << std::endl;
			Population[i + 1].Print();
		}
	}

	return 0;
}