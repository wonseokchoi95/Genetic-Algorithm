#include "Genetic_Algorithm.h"

Chromosome::Chromosome()
	: order_(0), cost(0), in_cost(0), out_cost(0), blk_x_size(0), blk_y_size(0)
{
	
	blk_x_size = static_cast<int> (sqrt(NumofGene));
	blk_y_size = static_cast<int> (sqrt(NumofGene));
	//ListGenerator(NumofGene);

	order_ = NumofGene;
	std::vector<int> order;
	bool first_time = false;
	while (order.size() != list.size())
	{
		Sleep(1);
		int num = (unsigned int)rand() % list.size();
		if (first_time == false)
		{
			order.push_back(num); first_time = true; continue;
		}
		for (unsigned int num_ = 0; num_ < order.size(); num_++)
		{
			if (num == order[num_])
				break;
			if ((num != order[num_]) && (num_ == order.size() - 1))
				order.push_back(num);
		}
	}
	int iter = 0;
	for (int y = 0; y < blk_y_size; y++)
	{
		for (int x = 0; x < blk_x_size; x++)
		{
			Gene blk(x, y, list[order.at(iter)].In, list[order.at(iter)].Out);
			this->blks.push_back(blk);
			iter++;
		}
	}
}

Chromosome::Chromosome(std::vector<InOut> list)
	: order_(list.size()), cost(0), in_cost(0), out_cost(0), blk_x_size(static_cast<int> (sqrt(list.size()))), blk_y_size(static_cast<int> (sqrt(list.size())))
{
	CalcInCost();
	CalcOutCost();
	CalcCost();



	for (int y = 0; y < blk_y_size; y++)
	{
		for (int x = 0; x < blk_x_size; x++)
		{
			Gene blk(	x, 
						y, 
						list[blk_y_size * y + x].In, 
						list[blk_y_size * y + x].Out);
			this->blks.push_back(blk);
		}
	}
}

Chromosome::~Chromosome()
{
}

void Chromosome::Print()
{
	int i = 1;
	for(auto blk : blks)
	{
		std::cout << blk.In << "-" << blk.Out << "\t";
		if (i % blk_x_size == 0) std::cout << "\n";
		i++;
	}
	std::cout << "\n";
}

int Chromosome::UpdateGene(int at, Gene blk)
{
	blks.at(at).In = blk.In;
	blks.at(at).Out = blk.Out;
	blks.at(at).x = blk.x;
	blks.at(at).y = blk.y;

	//if (list.size() != 0)
	//{
	//	list.clear();
	//	list.shrink_to_fit();
	//}

	return 0;
}

int Chromosome::UpdateGene(std::vector<int> atlist, std::vector<Gene> genelist)
{
	if (atlist.size() != genelist.size())
		std::cout << "size is not fit" << std::endl;
	
	for (int i = 0; i < atlist.size(); i++)
	{
		blks.at(atlist[i]).In = genelist[i].In;
		blks.at(atlist[i]).Out = genelist[i].Out;
		blks.at(atlist[i]).x = atlist.size() % 4;
		blks.at(atlist[i]).y = atlist.size() / 4;
	}

	return 0;
}

int Chromosome::ListGenerator(unsigned int length)
{		
	std::vector<int> in_order;
	std::vector<int> out_order;

	bool x_first_time = false;
	bool y_first_time = false;
	while (in_order.size() != length) {


		int num = (int)rand() % length + 1;
		if (x_first_time == false)
		{
			in_order.push_back(num); 
			x_first_time = true; 
			continue;
		}
		if (num == 0) continue;
		for (int num_ = 0; num_ < in_order.size(); num_++)
		{
			if (num == in_order[num_])
				break;
			if ((num != in_order[num_]) && (num_ == in_order.size() - 1))
			{
				in_order.push_back(num);
				break;
			}
		}
	}
	//std::cout << "\n";
	while (out_order.size() != length) {

		int num = (int)rand() % length + 1;
		if (y_first_time == false)
		{
			out_order.push_back(num); y_first_time = true; continue;
		}
		if (num == 0) continue;
		for (int num_ = 0; num_ < out_order.size(); num_++)
		{
			if (num == out_order[num_])
				break;
			if ((num != out_order[num_]) && (num_ == out_order.size() - 1))
			{
				out_order.push_back(num);
				//std::cout << std::to_string(num) << " ";
			}
		}
	}

	//std::cout << "\n";
	for (int i = 0; i < length; i++)
	{
		InOut tmp = { i, in_order[i], out_order[i] };
		list.push_back(tmp);
	}
	
	return 0;
}

void Chromosome::Clear()
{
	blks.shrink_to_fit();
	blks.clear();
}

int Chromosome::CalcInCost()
{
	// calculate the 'in-cost'
	int in_cost = 0;
	
	for (unsigned int i = 1; i <= Getblks().size(); i++)
	{
	// Check the order
		for (unsigned int j = 0; j < Getblks().size(); j++)
		{
			if (Getblks().at(j).In != i)
				continue;

			// If it is the correct order, 
			std::vector<int> cost;
			cost.push_back(CalcSpecificInCost(DIRECTION::EAST, Getblks().at(j)));
			cost.push_back(CalcSpecificInCost(DIRECTION::WEST, Getblks().at(j)));
			cost.push_back(CalcSpecificInCost(DIRECTION::SOUTH, Getblks().at(j)));
			cost.push_back(CalcSpecificInCost(DIRECTION::NORTH, Getblks().at(j)));
			std::sort(cost.begin(), cost.end());

			in_cost += cost.at(0);	
		}
	}
	setInCost(in_cost);

	return 0;
}

int Chromosome::CalcOutCost()
{	
	// calculate the 'out-cost'
	int out_cost = 0;
	for (unsigned int i = 1; i <= Getblks().size(); i++)
	{
		// Check the order
		for (unsigned int j = 0; j < Getblks().size(); j++)
		{
			if (Getblks().at(j).Out != i)
				continue;

			// If it is the correct order, 
			std::vector<int> cost;
			cost.push_back(CalcSpecificOutCost(DIRECTION::EAST, Getblks().at(j)));
			cost.push_back(CalcSpecificOutCost(DIRECTION::WEST, Getblks().at(j)));
			cost.push_back(CalcSpecificOutCost(DIRECTION::SOUTH, Getblks().at(j)));
			cost.push_back(CalcSpecificOutCost(DIRECTION::NORTH, Getblks().at(j)));
			std::sort(cost.begin(), cost.end());

			out_cost += cost.at(0);
		}
	}
	setOutCost(out_cost);

	return 0;
}

int Chromosome::CalcSpecificInCost(DIRECTION dir, Gene blk)
{
	int cost = 0;
	for (auto tmp_blk : blks)
	{
		switch (dir)
		{
		case Chromosome::DIRECTION::EAST:
			if ((tmp_blk.x < blk.x) && (tmp_blk.y == blk.y) && (tmp_blk.In < blk.In)) 
				cost += (tmp_blk.x + 1) * 2;
			break;
		case Chromosome::DIRECTION::WEST:
			if ((tmp_blk.x > blk.x) && (tmp_blk.y == blk.y) && (tmp_blk.In < blk.In))
				cost += (blk_x_size - tmp_blk.x) * 2;
			break;
		case Chromosome::DIRECTION::SOUTH:
			if ((tmp_blk.y < blk.y) && (tmp_blk.x == blk.x) && (tmp_blk.In < blk.In))
				cost += (tmp_blk.y + 1) * 2;
			break;
		case Chromosome::DIRECTION::NORTH:
			if ((tmp_blk.y > blk.y) && (tmp_blk.x == blk.x) && (tmp_blk.In < blk.In))
				cost += (blk_y_size - tmp_blk.y) * 2;
			break;
		default:
			std::cout << "Calculating In Cost Error Occurred!!" << std::endl;
			break;
		}
	}
	return cost;
}

int Chromosome::CalcSpecificOutCost(DIRECTION dir, Gene blk)
{
	int cost = 0;
	for (auto tmp_blk : blks)
	{
		switch (dir)
		{
		case Chromosome::DIRECTION::EAST:
			if ((tmp_blk.x < blk.x) && (tmp_blk.y == blk.y) && (tmp_blk.Out < blk.Out))
				cost += (tmp_blk.x + 1) * 2;
			break;
		case Chromosome::DIRECTION::WEST:
			if ((tmp_blk.x > blk.x) && (tmp_blk.y == blk.y) && (tmp_blk.Out > blk.Out))
				cost += (blk_x_size - tmp_blk.x) * 2;
			break;
		case Chromosome::DIRECTION::SOUTH:
			if ((tmp_blk.y > blk.y) && (tmp_blk.x == blk.x) && (tmp_blk.Out > blk.Out))
				cost += (blk_y_size - tmp_blk.y) * 2;
			break;
		case Chromosome::DIRECTION::NORTH:
			if ((tmp_blk.y < blk.y) && (tmp_blk.x == blk.x) && (tmp_blk.Out < blk.Out))
				cost += (tmp_blk.y + 1) * 2;
			break;
		default:
			std::cout << "Calculating Out Cost Error Occurred!!" << std::endl;
			break;
		}
	}
	return cost;
}

CrossOver::CrossOver(Chromosome & parent1, Chromosome & parent2)
	:length(0)
{
	length = parent1.getXSize() * parent1.getYSize();
	BitArrayBuilder();
	//Print();
	Chromosome child1;
	Chromosome child2;
	std::vector<Gene> child1_tmp;
	std::vector<Gene> child2_tmp;

	for (int y = 0; y < parent1.getYSize(); y++)
	{
		for (int x = 0; x < parent1.getXSize(); x++)
		{
			// If the bit is 1, copy the parent 1 to child 1, and parent 2 to child 2.
			if (bit_array[parent1.getXSize() * y + x] == 1)
			{
				Gene blk1(x, y, parent1.Getblks()[parent1.getXSize() * y + x].In, parent1.Getblks()[parent1.getXSize() * y + x].Out);
				child1.UpdateGene(parent1.getXSize() * y + x, blk1);

				Gene blk2(x, y, parent2.Getblks()[parent2.getXSize() * y + x].In, parent2.Getblks()[parent1.getXSize() * y + x].Out);
				 
				child2_tmp.push_back(blk2);
				std::sort(child2_tmp.begin(), child2_tmp.end());
			}

			// If the bit is 0, copy the parent 1 to child 2, and parent 2 to child 1.
			else {
				// Parent 1의 블록들은 저장한 후, Parent 2의 해당 위치 크기 순으로 재배치한다.

				Gene blk1(x, y, parent2.Getblks()[parent2.getXSize() * y + x].In, parent2.Getblks()[parent1.getXSize() * y + x].Out);
				child2.UpdateGene(parent1.getXSize() * y + x, blk1);

				Gene blk2(x, y, parent1.Getblks()[parent1.getXSize() * y + x].In, parent1.Getblks()[parent1.getXSize() * y + x].Out);

				child1_tmp.push_back(blk2);
				std::sort(child1_tmp.begin(), child1_tmp.end());

			}
		}
	}

	// 모든 데이터를 tmp에 입력한 후, 각자의 parent를 비교하여 재배치한다.

	// start with first-in block.
	for (unsigned int In = 1; In <= parent1.Getblks().size(); In++)
	{
		for (unsigned int i = 0; i < child2.Getblks().size(); i++)
		{
			if (parent2.Getblks()[i].In == In && bit_array[i] == 0)
			{
				Gene blk(child2.Getblks()[i].x, child2.Getblks()[i].y, child1_tmp.front().In, child1_tmp.front().Out);

				// Delete front-In data.
				child1_tmp.erase(child1_tmp.begin());
				child1.UpdateGene(child1.getXSize() * child2.Getblks()[i].y + child2.Getblks()[i].x, blk);
			}
		}

	}

	// start with first-in block.
	for (unsigned int In = 1; In <= parent2.Getblks().size(); In++)
	{
		for (unsigned int i = 0; i < child1.Getblks().size(); i++)
		{
			if (parent1.Getblks()[i].In == In && bit_array[i] == 1)
			{
				Gene blk(child1.Getblks()[i].x, child1.Getblks()[i].y, child2_tmp.front().In, child2_tmp.front().Out);

				// Delete front-In data.
				child2_tmp.erase(child2_tmp.begin());
				child2.UpdateGene(child2.getXSize() * child1.Getblks()[i].y + child1.Getblks()[i].x, blk);
			}
		}

	}

	// After copy parent, move data from child to parent.
	parent1 = std::move(child1);
	parent2 = std::move(child2);
	bit_array.clear();
}

CrossOver::~CrossOver()
{
}

void CrossOver::BitArrayBuilder()
{
	for (int i = 0; i < length; i++)
	{
		int num = (unsigned int)rand() % 2;
		bit_array.push_back(num);
		Sleep(1);
	}

}

void CrossOver::Print()
{
	std::cout << "Bit array is" << std::endl;

	for (unsigned int i = 0; i < bit_array.size(); i++)
	{
		std::cout << bit_array[i] << "\t";
		if ((i + 1) % (int)sqrt(length) == 0) std::cout << "\n";
	}
	std::cout << "\n";
}

std::vector<int> CrossOver::SortInValue(std::vector<Gene>)
{
	return std::vector<int>();
}

Mutation::Mutation(Chromosome & set)
{
	std::vector<InOut> tmp_list;
	int i = 0;
	for (auto gene : set.Getblks())
	{
		InOut tmp_gene = { i, gene.In, gene.Out };
		tmp_list.push_back(tmp_gene);
		i++;
	}

	std::vector<int> order;
	bool first_time = false;
	while (order.size() != list.size())
	{
		Sleep(1);
		int num = (unsigned int)rand() % list.size();
		if (first_time == false)
		{
			order.push_back(num); first_time = true; continue;
		}
		for (unsigned int num_ = 0; num_ < order.size(); num_++)
		{
			if (num == order[num_])
				break;
			if ((num != order[num_]) && (num_ == order.size() - 1))
				order.push_back(num);
		}
	}
	int j = 0;
	for (auto& inout : tmp_list)
	{
		if (inout.Order != order[j])
		{
			std::swap(inout, tmp_list[order[j]]);
		}
		j++;
	}


	Chromosome mutated(tmp_list);
	set = std::move(mutated);
}
