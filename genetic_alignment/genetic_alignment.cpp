#include "functions.h"
#include <list>
#include <math.h>  
#include <vector>  
#include <algorithm>
#include <string>

using namespace std;

int aux_size;

vector<string> alpha;
vector<int> alpha_len;
vector< pair< int,vector<string> > > population;
vector< pair< int, vector<string> > > best_population;
vector< pair< int, vector<string> > > wortst_population;

int alignment_len = 0;
int kseq;

//por recomendaçao do paper o valor do mehlor fator pode ser entre 1.2 a 1.5
float fator = 1.2;
int populationSize = 200;
int generations = 500;
float crossover_ratio = 0.3;
float mutation_ratio = 0.7; 
float elitism_rate = 0.2;
int gap_penalizate = -3;
int nro_selection = 3;
float merge_space_ratio = 0.8;
float change_space_ratio = 0.8;
float decrease_search_ratio = 0.8;

Score *score;

vector<int> randomgenerator(int size, int new_size){
	
	std::vector<int> question_numbers;
	for (unsigned int i = 0; i < new_size; ++i)
   		 question_numbers.push_back(i);
	//std::srand((pseudo1+111)*(pseudo2+37)*time(NULL));
	//std::srand((unsigned)(time(NULL)));
	std::random_shuffle(question_numbers.begin(),question_numbers.end());

	std::vector<int> resize_question_numbers;
	for (unsigned int i = 0; i < size; ++i)
   		 resize_question_numbers.push_back(question_numbers[i]);
    
	return resize_question_numbers;
}

string get_permutation(string i_sequence,int alignment_size,int new_alignment_size)
{
	vector<char> sequence_sorted(new_alignment_size,'-');

	std::vector<int> sequence_permutation = randomgenerator(alignment_size,new_alignment_size);

	sort(sequence_permutation.begin(), sequence_permutation.end());
	
	for(int i = 0; i<alignment_size; i++)
		sequence_sorted[sequence_permutation[i]] = i_sequence[i];

	string s_sequence_sorted = "";
	for(int i = 0; i<new_alignment_size; i++)
		s_sequence_sorted += sequence_sorted[i];

	//return sorted positions
	return s_sequence_sorted;
}

string add_gaps_end(string sequence, int max_size_sequence)
{
	cout<<"ADD GAPS END"<<endl;
	for(int i = 0;i<max_size_sequence;i++)
		sequence += "-";
	return sequence;
}

string add_gaps_begin(string sequence, int max_size_sequence)
{
	cout<<"ADD GAPS BEGIN"<<endl;
	string gaps_sequence = "";
	for(int i = 0;i<max_size_sequence;i++)
		gaps_sequence += "-";
	sequence = gaps_sequence+sequence;

	return sequence;
}

/**
   Shifts a block in the sequence (ending at the specified position) to the left.
 */
string shift_left(string seq, int endPos, bool begFound)
{
	//Can't deal with shifts at the very beginning
	if (endPos == 0) return seq;
	
	char endElem = seq[endPos-1];
	int begPos = 0;
	int newBegPos = 0;
	begFound = false;
	
	for (int i=endPos-1; i>=0; i--)
	{
		char elem = seq[i];
		//We found our match (logic varies depending on whether we
		// are shuffling gaps or elements).
		if (!begFound && ((elem == '-' && endElem != '-')
				|| (elem != '-' && endElem == '-')))
		{
			begPos = i+1;
			begFound = true;
		}
		//Now we need to find the end of the gap.
		else if (begFound && ((elem != '-' && endElem != '-')
				|| (elem == '-' && endElem == '-')))
		{
			newBegPos = i+1;
			break;
		}
	}

	string begin = seq.substr(0, newBegPos-0);
	string block = seq.substr(newBegPos, begPos-newBegPos);
	string gapBlock = seq.substr(begPos, endPos-begPos);
	string tail = seq.substr(endPos, seq.size()-endPos);
	return begin + gapBlock + block + tail;
}

/**
   Shifts a block in the sequence (starting at the specified position) to the right.
*/
string shift_right(string seq, int startPos, bool endFound)
{
	char startElem = seq[startPos];
	int endPos = seq.size();
	int newEndPos = seq.size();
	endFound = false;
	
	for (int i=startPos+1; i<seq.size(); i++)
	{
		char elem = seq[i];
		//We found our match (logic varies depending on whether we
		// are shuffling gaps or elements).
		if (!endFound && ((elem == '-' && startElem != '-')
				|| (elem != '-' && startElem == '-')))
		{
			endPos = i;
			endFound = true;
		}
		//Now we need to find the end of the gap.
		else if (endFound && ((elem != '-' && startElem != '-')
				|| (elem == '-' && startElem == '-')))
		{
			newEndPos = i;
			break;
		}
	}
	

	string begin = seq.substr(0, startPos);
	string block = seq.substr(startPos, endPos-startPos);
	string gapBlock = seq.substr(endPos, newEndPos-endPos);
	string tail = seq.substr(newEndPos, seq.size()-newEndPos);
	
	return begin + gapBlock + block + tail;
}

string remove_column_gaps(vector<string> individual, int pos_column, int pos_row)
{
	vector<string> child_individual = individual;

	//Pick a random sequence to modify
	int seq_number = pos_row;
	string seq = child_individual[seq_number];
	
	//pick a random shifting point
	int shift_pos = pos_column;
	
	bool endFound;
	seq = shift_left(seq, shift_pos,endFound);
	if(endFound)
		return 	seq;
	
	seq = shift_right(seq, shift_pos,endFound);
	
	return 	seq;
}

vector<string> decrease_search_space(vector<string> individual)
{
	vector<string> individual2 = individual;
	string temp_sequence;
	string new_sequence;

	int c=individual2[0].size()-1;

	int min_no_gaps = 1;
	int count_min_no_gaps = 0;
	
	while(c>=0)
	{	
		vector<string> temp_individual = individual2;
		
		for (int s=0; s<individual2.size(); s++)
		{	
			temp_sequence = individual2[s];

			if (temp_sequence[c] != '-')
			{
				if(count_min_no_gaps > min_no_gaps){
					count_min_no_gaps = 0;
					break;}
				else
				{		
					new_sequence = remove_column_gaps(individual2, c, s);
					temp_individual[s] = new_sequence;
				}
				count_min_no_gaps += 1;
			}
			if(s == individual2.size()-1){
				individual2 = temp_individual;
				count_min_no_gaps = 0;}
		}
		
		c--;
	}

	return individual2;
}

vector<string> change_space_operator(vector<string> individual)
{
	vector<string> child_individual = individual;

	//Pick a random sequence to modify
	int seqNumber = rand()%child_individual.size();
	string seq = child_individual[seqNumber];
	
	//pick a random shifting point
	int shiftPos = rand()%seq.size();
	bool Found;
	
	int rdn = rand()%2;
	if (rdn == 1)
		seq = shift_left(seq, shiftPos,Found);
	else
		seq = shift_right(seq, shiftPos,Found);
	
	child_individual[seqNumber] = seq;
	return child_individual;
}

vector<string> merge_space_operator(vector<string> individual)
{
	vector<string> child_individual = individual;
	bool Found;
	//Pick a random sequence to modify
	int seqNumber = rand()%child_individual.size();
	string seq = child_individual[seqNumber];
	
	//pick a random shifting point
	string::size_type shiftPos = seq.find('-');
	//cout<<"gapIndex: "<<gapIndex<<endl;
	if (shiftPos == -1)
		return individual;
	
	int rdn = rand()%2;
	if (rdn == 1)
		seq = shift_left(seq, shiftPos,Found);
	else
		seq = shift_right(seq, shiftPos,Found);
	
	child_individual[seqNumber] = seq;
	return child_individual;
}


vector<string> mutation_remove_column(vector<string> individual)
{
	vector<string> individual2 = individual;
	string tempSequence;
	bool bAllGaps;
	string temp;
	string newSequence;
	vector<string> individual_rcolumn;
	bool repeat = false;
	int c=individual2[0].size()-1;
	while(c>=0)
	{			
		bAllGaps = true;
		for (int s=0; s<individual2.size(); s++)
		{
			//cout<<tempSequence[c]<<endl;
			tempSequence = individual2[s];
			if (tempSequence[c] != '-')
			{
				bAllGaps = false;
				break;
			}
		}
		
		if (bAllGaps)
		{		
			repeat = true;		
			for (int s=0; s<individual2.size(); s++)
			{						
				temp = individual2[s];
				newSequence = temp.substr(0,c) + temp.substr(c+1);
				//cout<<newSequence<<endl;
				// set the sequence back in the alignment
				individual_rcolumn.push_back(newSequence);
			}
			if(repeat)
			{
				individual2 = individual_rcolumn;
				individual_rcolumn.clear();
				c = individual2[0].size()-1;
			}
		}
		c--;
	}
	return individual2;
}

/*vector<string> mutation_add_gap(vector<string> individual1)
{
	cout<<"MUTATION ADD GAP"<<endl;
	vector<string>	individual_addgap;
	cout<<"individual add gap size: "<<individual1.size()<<endl;
	for (int i=0; i<individual1.size(); i++)
	{
		string sequence = individual1[i];
		int gapPosition = rand()%sequence.size();
		string beforeGap = sequence.substr(0,gapPosition);
		string afterGap = sequence.substr(gapPosition+1);
		string new_sequence = beforeGap + "-" + afterGap;
		individual_addgap.push_back(new_sequence);
	}

	cout<<"new individual add gap size: "<<individual_addgap.size()<<endl;

	for(int i = 0;i<individual_addgap.size();i++)
		cout<<individual_addgap[i]<<endl;
	
	return individual_addgap;	
}*/

vector<string> mutation_remove_gap(vector<string> individual1)
{
	cout<<"MUTATION REMOVE GAP"<<endl;
	string seq;
	string beforeGap;
	string afterGap;
	int pos = 0;
	int gapIndex = 0;
	
	vector<string>	individual_removegap;
 	cout<<"individual mutation remove gap size: "<<individual1.size()<<endl;
	//For each sequence, delete a gap
	for(int i=0; i<individual1.size(); i++)
	{
		seq = individual1[i];	
		string::size_type gapIndex = seq.find('-');
		if (gapIndex == -1)
			return individual1;
		
		string beforeGap = seq.substr(0,gapIndex);
		string afterGap = seq.substr(gapIndex+1);
		string new_sequence = beforeGap + afterGap;
		individual_removegap.push_back(new_sequence);
	}
	
	cout<<"new individual mutation remove gap size: "<<individual_removegap.size()<<endl;

	for(int i = 0;i<individual_removegap.size();i++)
		cout<<individual_removegap[i]<<endl;

	return individual_removegap;	
}



void mutation(vector<string> individual1)
{	
	cout<<"MUTATION"<<endl;
	vector<string> individual_mutation;

	float merge_space_operator_ratio = (float)(rand())/(float)(RAND_MAX);
	cout<<"probabilistic merge space operator: "<<merge_space_operator_ratio<<endl;
	if(merge_space_operator_ratio <= merge_space_ratio)
		individual_mutation = merge_space_operator(individual1);
	else
	{
		float change_space_operator_ratio = (float)(rand())/(float)(RAND_MAX);
		cout<<"probabilistic change space operator: "<<change_space_operator_ratio<<endl;
		if(change_space_operator_ratio <= change_space_ratio)
			individual_mutation = change_space_operator(individual1);
		else
		{
			float decrease_search_space_ratio = (float)(rand())/(float)(RAND_MAX);
			cout<<"probabilistic decrease search space: "<<decrease_search_space_ratio<<endl;
			if(decrease_search_space_ratio <= decrease_search_ratio)
				individual_mutation = decrease_search_space(individual1);
			else
				individual_mutation = mutation_remove_gap(individual1);
		}
	}
	wortst_population.push_back(std::make_pair(0,mutation_remove_column(individual_mutation)));
}


string replace(string word, string target, string replacement){
    int len, loop=0;
    string nword="", let;
    len=word.length();
    len--;
    while(loop<=len){
        let=word.substr(loop, 1);
        if(let==target){
            nword=nword+replacement;
        }else{
            nword=nword+let;
        }
        loop++;
    }
    return nword;

}

void crossover(vector<string> individual1, vector<string> individual2)
{
	cout<<"CROSSOVER"<<endl;
	//srand(time(NULL));

	cout<<"IMPRIMIR INDIVIDUOS"<<endl;
	for(int i = 0;i<individual1.size();i++)
		cout<<individual1[i]<<endl;
	cout<<endl;
	for(int i = 0;i<individual2.size();i++)
		cout<<individual2[i]<<endl;

	

	int min_size = MAX_SIZE;

	for(int i=0; i<individual1.size();i++)
	{
		string min_individual = replace(individual1[i],"-", "");
		if(min_individual.size() <= min_size)
			min_size = min_individual.size();
	}

	vector<string> new_individual1;
	vector<string> new_individual2;

	vector<string> new_individual1_left;
	vector<string> new_individual1_right;
	vector<string> new_individual2_left;
	vector<string> new_individual2_right;

	int random_vertical= (rand()%individual1.size())+1;
	cout<<"random vertical: "<<random_vertical<<endl;

	int max_size_new_seq_indiv1_left = 0;
	int max_size_new_seq_indiv1_right = 0;
	int max_size_new_seq_indiv2_left = 0;
	int max_size_new_seq_indiv2_right = 0;

	for(int i = 0;i<kseq;i++)
	{
		string seq_indiv1 = individual1[i];
		string seq_indiv2 = individual2[i];
		int size_seq_indiv1 = 0;
		string new_seq_indiv1_left = "";
		string new_seq_indiv1_right = "";

		int size_seq_indiv2 = 0;
		string new_seq_indiv2_left = "";
		string new_seq_indiv2_right = "";
	
		for(int i = 0;i<seq_indiv1.size();i++)
		{
			if(size_seq_indiv1<random_vertical)
			{	
				if(seq_indiv1[i]!='-')
					size_seq_indiv1 += 1;
				new_seq_indiv1_left += seq_indiv1[i];
			}
			else
				new_seq_indiv1_right += seq_indiv1[i];
		}

		for(int i = 0;i<seq_indiv2.size();i++)
		{
			if(size_seq_indiv2<random_vertical)
			{
				if(seq_indiv2[i]!='-')
					size_seq_indiv2 += 1;
				new_seq_indiv2_left += seq_indiv2[i];
			}
			else
				new_seq_indiv2_right += seq_indiv2[i];
		}

		new_individual1_left.push_back(new_seq_indiv1_left);
		new_individual1_right.push_back(new_seq_indiv1_right);
		new_individual2_left.push_back(new_seq_indiv2_left);
		new_individual2_right.push_back(new_seq_indiv2_right);
	
	}

	for(int i = 0;i<new_individual1_left.size();i++)
	{
		if(max_size_new_seq_indiv1_left<new_individual1_left[i].size())
			max_size_new_seq_indiv1_left = new_individual1_left[i].size();
	}

	for(int i = 0;i<new_individual1_right.size();i++)
	{
		if(max_size_new_seq_indiv1_right<new_individual1_right[i].size())
			max_size_new_seq_indiv1_right = new_individual1_right[i].size();
	}

	for(int i = 0;i<new_individual2_left.size();i++)
	{
		if(max_size_new_seq_indiv2_left<new_individual2_left[i].size())
			max_size_new_seq_indiv2_left = new_individual2_left[i].size();
	}

	for(int i = 0;i<new_individual2_right.size();i++)
	{
		if(max_size_new_seq_indiv2_right<new_individual2_right[i].size())
			max_size_new_seq_indiv2_right = new_individual2_right[i].size();
	}

	//add gaps begin and end
	vector<string> gaps_new_individual1_left;
	vector<string> gaps_new_individual1_right;
	vector<string> gaps_new_individual2_left;
	vector<string> gaps_new_individual2_right;

	for(int i = 0;i<new_individual1_left.size();i++)
	{
		string sequence_individual_left = "";
		if(new_individual1_left[i].size()<max_size_new_seq_indiv1_left)
			sequence_individual_left = add_gaps_end(new_individual1_left[i],max_size_new_seq_indiv1_left-new_individual1_left[i].size());
		else
			sequence_individual_left = new_individual1_left[i];
		gaps_new_individual1_left.push_back(sequence_individual_left);
	}

	for(int i = 0;i<new_individual1_right.size();i++)
	{
		string sequence_individual_right = "";
		if(new_individual1_right[i].size()<max_size_new_seq_indiv1_right)
			sequence_individual_right = add_gaps_begin(new_individual1_right[i],max_size_new_seq_indiv1_right-new_individual1_right[i].size());
		else
			sequence_individual_right = new_individual1_right[i];
		gaps_new_individual1_right.push_back(sequence_individual_right);
	}

	for(int i = 0;i<new_individual2_left.size();i++)
	{
		string sequence_individual_left = "";
		if(new_individual2_left[i].size()<max_size_new_seq_indiv2_left)
			sequence_individual_left = add_gaps_end(new_individual2_left[i],max_size_new_seq_indiv2_left-new_individual2_left[i].size());
		else
			sequence_individual_left = new_individual2_left[i];
		gaps_new_individual2_left.push_back(sequence_individual_left);
	}

	for(int i = 0;i<new_individual2_right.size();i++)
	{
		string sequence_individual_right = "";
		if(new_individual2_right[i].size()<max_size_new_seq_indiv2_right)
			sequence_individual_right = add_gaps_begin(new_individual2_right[i],max_size_new_seq_indiv2_right-new_individual2_right[i].size());
		else
			sequence_individual_right = new_individual2_right[i];
		gaps_new_individual2_right.push_back(sequence_individual_right);
	}

	cout<<"IMPRIMIR INDIVIDUOS VERTICALES LEFT RIGHT"<<endl;

	for(int i = 0;i<gaps_new_individual1_left.size();i++)
		cout<<gaps_new_individual1_left[i]<<endl;
	cout<<endl;
	for(int i = 0;i<gaps_new_individual1_right.size();i++)
		cout<<gaps_new_individual1_right[i]<<endl;
	cout<<endl;
	for(int i = 0;i<gaps_new_individual2_left.size();i++)
		cout<<gaps_new_individual2_left[i]<<endl;
	cout<<endl;
	for(int i = 0;i<gaps_new_individual2_right.size();i++)
		cout<<gaps_new_individual2_right[i]<<endl;
	cout<<endl;

	//add offprint1 and offprint2 to population
	//offprint2
	for(int i = 0; i<gaps_new_individual1_left.size();i++)
	{
		string sequ_offprint2 = gaps_new_individual1_left[i]+gaps_new_individual2_right[i];
		new_individual1.push_back(sequ_offprint2);
	}

	//offprint1
	for(int i = 0; i<gaps_new_individual2_left.size();i++)
	{
		string sequ_offprint1 = gaps_new_individual2_left[i]+gaps_new_individual1_right[i];
		new_individual2.push_back(sequ_offprint1);
	}

	cout<<"IMPRIMIR INDIVIDUOS VERTICALES"<<endl;
	for(int i = 0;i<new_individual1.size();i++)
		cout<<new_individual1[i]<<endl;

	cout<<endl;
	for(int i = 0;i<new_individual2.size();i++)
		cout<<new_individual2[i]<<endl;
	cout<<endl;

	//mutation
	float indiv1_mutation_ratio = (float)(rand())/(float)(RAND_MAX);
	cout<<"individuo1 mutation ratio vertical: "<<indiv1_mutation_ratio<<endl;

	if(indiv1_mutation_ratio<= mutation_ratio)
		mutation(mutation_remove_column(new_individual2));
	else
		wortst_population.push_back(std::make_pair(0,mutation_remove_column(new_individual2)));

	float indiv2_mutation_ratio = (float)(rand())/(float)(RAND_MAX);
	cout<<"individuo2 mutation ratio vertical: "<<indiv2_mutation_ratio<<endl;
	if(indiv2_mutation_ratio<= mutation_ratio)
		mutation(mutation_remove_column(new_individual1));
	else
		wortst_population.push_back(std::make_pair(0,mutation_remove_column(new_individual1)));
}

vector<string> ruleta()
{
	vector<int> l_selection_ruleta = randomgenerator(nro_selection, population.size());
	vector< pair< int, vector<string> > > temp_population;

	for(int i = 0; i < l_selection_ruleta.size();i++){
		temp_population.push_back(std::make_pair(population[l_selection_ruleta[i]].first,population[l_selection_ruleta[i]].second));}
	sort(temp_population.begin(), temp_population.end());

	return temp_population[0].second;
}

void elitism()
{
	sort(population.begin(), population.end());

	cout<<"size population begin: "<<population.size()<<endl;
	for(int i = 0; i<elitism_rate*populationSize;i++)
	{
		cout<<"1 population[i].first: "<<endl;
		best_population.push_back(std::make_pair(population[i].first,population[i].second));
	}

	cout<<"BEST POPULATION"<<endl;
	for(int i = 0;i<best_population.size();i++)
	{
		vector<string> v = best_population[i].second;
		for(int j = 0;j<v.size();j++)
		{
			cout<<v[j]<<endl;
		}
		cout<<"best population: "<<best_population[i].first<<endl;
	}
	
	int size_new_population = elitism_rate*populationSize;
	cout<<"size_new_population"<<size_new_population<<endl;
	while(size_new_population < populationSize)
	{
		vector<string> new_individuals = ruleta();
		vector<string> new_individuals2 = ruleta();

		float new_crossover_ratio = (float)(rand())/(float)(RAND_MAX);
		cout<< "new_crossover_ratio: " <<new_crossover_ratio<<endl;
		if(new_crossover_ratio <= crossover_ratio)
		{		
			crossover(new_individuals,new_individuals2);
			size_new_population += 2;
		}
		
		float new_mutation_ratio = (float)(rand())/(float)(RAND_MAX);
		cout<< "new_mutation_ratio: " <<new_mutation_ratio<<endl;
		if(new_mutation_ratio <= mutation_ratio)
		{
			mutation(new_individuals);
			size_new_population += 1;	
		}

		float new_mutation_ratio2 = (float)(rand())/(float)(RAND_MAX);
		cout<< "new_mutation_ratio2: " <<new_mutation_ratio2<<endl;
		if(new_mutation_ratio2 <= mutation_ratio)
		{
			mutation(new_individuals2);
			size_new_population += 1;	
		}
	}

	population.clear();
	population = best_population;
	for(int i=0; i<wortst_population.size(); i++)
		population.push_back(std::make_pair(wortst_population[i].first,wortst_population[i].second));
	best_population.clear();
	wortst_population.clear();
}

int evaluateFitness(int objectiveFitness)
{
	for(int i = 0;i<population.size();i++)
	{
		int individualFitness = population[i].first;
		int ind_individualFitness = i;
		cout<<"ind_individualFitness: "<<ind_individualFitness<<endl;
		cout<<"individual Fitness: "<<individualFitness<<" objectiveFitness: "<<objectiveFitness<<endl;
		if(individualFitness < objectiveFitness)
			return i;
	}
	return 0;
}

int score_sum_of_pairs(vector<char> pairs)
{
	int score_pairs = 0;
	for(int i = 0;i<pairs.size()-1;i++)
	{
		for(int j = i+1;j<pairs.size();j++)
		{
			if(pairs[i] != pairs[j])	
			{
				if(pairs[i] == '-' || pairs[j] == '-')
					score_pairs += score->getMismatch();
				else
					score_pairs += score->getGap();
			}
		}
	}
	cout<<"score pairs: "<<score_pairs<<endl;
	return score_pairs;
}

int sum_of_pairs(vector<string> individual) 
{
	int score_pairs = 0;

	int max_size = individual.size();
	int max_sequence = individual[0].size();
	
	for(int i = 0;i<max_sequence;i++)
	{
		vector<char> columns;
		for(int j = 0; j<max_size;j++)
		{
			string sequence = individual[j];
			columns.push_back(sequence[i]);
		}
		score_pairs += score_sum_of_pairs(columns);
	}
	cout<<"sum score pairs: "<<score_pairs<<endl;
	return score_pairs;
}

int clustal()
{
	cout<<"CLUSTAL"<<endl;
	string str[kseq] = {"AT-TG-CC-A--TT","AT-GG-CC-A--TT","A--T--CCAATTTT","ATCTT--C----TT","AC-TGACC------"};
	vector<string>  individual_objetive;
	for(int i = 0; i<kseq;i++)
		individual_objetive.push_back(str[i]);
	return sum_of_pairs(individual_objetive);
}

void fitness()
{
	vector<string> individual;
	vector<int> score;
	cout<<"fitness "<<endl;
	cout<<"population size: "<<population.size()<<endl;
	for(int i=0; i<population.size(); i++)
		cout<<population[i].second.size()<<endl;
	
	for(int j = 0; j<population.size();j++)
		vector<string> l_permutation = population[j].second;

	cout<<"population size: "<<population.size()<<endl;
	for(int i = 0; i<population.size();i++)
	{
		individual =  population[i].second;
		population[i].first += sum_of_pairs(individual);
		cout<<"population[i].first "<<population[i].first<<endl;
	}
}

void generateInitialPopulation()
{
	int max_sequence_size = 0;
	for(int i = 0; i <kseq; i++ )
	{
		if(alpha_len[i]>max_sequence_size)
			max_sequence_size = alpha_len[i];
	}
	
	
	// create a new sequence length based on the given percentage
	int new_alignment_size = 0;

	new_alignment_size = ceil(max_sequence_size *fator);
	cout<< max_sequence_size << " "<<fator<<" "<< new_alignment_size << endl;

	string permutation;
	
	// create the population
	for(int j = 0; j<populationSize;j++)
	{
		vector<string> l_permutation;
		for (int i=0; i<kseq; i++)
		{
			permutation = get_permutation(alpha[i],alpha_len[i],new_alignment_size);
			cout << permutation;
			cout<<endl;
			l_permutation.push_back(permutation);
		}
		cout<<endl;
		population.push_back(std::make_pair(0,mutation_remove_column(l_permutation)));
	}
}



int main(void) {
	double t_start, t_end;
	vector<string> alignment;
	int alignment_len;
	string sequence;
	unsigned timeit = (unsigned)(time(NULL));
	cout<<timeit<<endl;
	std::srand(timeit);
	double m,M,g;

	cin >> kseq;

	for(int i = 0 ; i < kseq; i++) {
		cin >> sequence;
			alpha.push_back(sequence);
		alpha_len.push_back(sequence.size());
	}

	cin >> M >> m >> g;

	score = new Score(M, m, g);

	generateInitialPopulation();

        int objetive_function = clustal();
	cout<<"objetive_function "<<objetive_function<<endl;

	int stop = 0;
	while(stop<generations)
	{
		cout<<(stop+1)<<" GERATIONS--------------------------------"<<endl;
		cout<< "population size: " <<population.size()<<endl;

		fitness();
		int indx_best_score = evaluateFitness(objetive_function);
		if(indx_best_score != 0)
		{
			//cout<<"score best_fitness: "<<indx_best_score<<endl;
			int best_fitness = population[indx_best_score].first;

			cout<<"score best fitness: "<<best_fitness<<endl;

			for(int i = 0; i<population[indx_best_score].second.size();i++)
				cout<<population[indx_best_score].second[i]<<endl;
			break;	
		}

		elitism();
		stop += 1;
	}

	//fitness();
    return 0;
};
