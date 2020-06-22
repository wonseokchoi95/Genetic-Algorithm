#pragma once

#include <iostream>
#include <vector>
#include <time.h>
#include <Windows.h>
#include <algorithm>
#include <iostream>
#include <string>

const static int NumofGene = 100;

struct Gene
{
	Gene(int x_, int y_, int in, int out)
		: x(x_), y(y_), In(in), Out(out) {}
	bool operator<(const Gene g) const
	{
		return In < g.In;
	}
	int x;
	int y;
	int In;
	int Out;
};

struct InOut {
	unsigned int Order;
	int In;
	int Out;
	void swap(InOut& rhs)
	{
		unsigned int tmp_order = Order;
		int tmp_in = In;
		int tmp_out = Out;
		this->Order = rhs.Order;
		this->In = rhs.In;
		this->Out = rhs.Out;

	}
};

static std::vector<InOut> list;
//! Chromosome object has the information about block in-out data.
//! It can print its data.
class Chromosome {

public:
	Chromosome();
	Chromosome(std::vector<InOut> list);
	~Chromosome();

	enum class DIRECTION {
		EAST,
		WEST,
		SOUTH,
		NORTH,
	};

private:
	std::vector<int> in_order;
	std::vector<int> out_order;
	int order_;
	int cost;
	int in_cost;
	int out_cost;
	int blk_x_size;
	int blk_y_size;
	std::vector<Gene> blks;
	void setInCost(int in_cost_) { in_cost = in_cost_; }
	void setOutCost(int out_cost_) { out_cost = out_cost_; }

public:
	static int ListGenerator(unsigned int length);
	void Print();
	int UpdateGene(int at, Gene blk);
	int UpdateGene(std::vector<int> atlist, std::vector<Gene> genelist);
	std::vector<Gene> Getblks() const { return blks; }
	int getXSize() { return blk_x_size; }
	int getYSize() { return blk_y_size; }
	std::string getCost() const { return std::to_string(cost); }
	std::string getInCost() const { return std::to_string(in_cost); }
	std::string getOutCost() const { return std::to_string(out_cost); }

	int CalcInCost();
	int CalcOutCost();
	int CalcCost() { cost = in_cost + out_cost; return 0; }

	// In-cost calculate function
	int CalcSpecificInCost(DIRECTION dir, Gene blk);
	int CalcSpecificOutCost(DIRECTION dir, Gene blk);

	bool operator<(Chromosome s) const {
		if (cost == s.cost)
			return order_ < s.order_;
		else return cost < s.cost;
	}

	// Assignment operator Overloading
	Chromosome& operator=(const Chromosome& s) {
		this->blks.clear();
		this->blks = s.blks;
		CalcInCost();
		CalcOutCost();
		CalcCost();

		return *this;
	}

	void Clear();

public:
};


// Cross Over the Chromosome.
class CrossOver
{
public:
	CrossOver(Chromosome& parentA, Chromosome& parentB);
	~CrossOver();
private:
	int length;

	//! Block the copy constructor.
	CrossOver(const CrossOver&);
	
	//! Block the Assignment operator constructor.
	CrossOver& operator=(const CrossOver&) {};
	std::vector<int> bit_array;
	void BitArrayBuilder();
	void Print();
	std::vector<int> SortInValue(std::vector<Gene> genelist);
public:
};

class Mutation
{

public:
	Mutation(Chromosome& set);
	~Mutation() {}


};