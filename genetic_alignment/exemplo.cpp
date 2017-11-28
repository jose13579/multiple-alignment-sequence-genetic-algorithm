#include "functions.h"
#include <list>
#include <math.h>  
#include <vector>  
#include <algorithm>
#include <string>

using namespace std;
vector<string> vec;
int kseq;
float horizontal_crossover = 0.3;
float vertical_crossover = 0.5;

string add_gaps_end(string sequence, int max_size_sequence)
{
	for(int i = 0;i<max_size_sequence;i++)
		sequence += "-";
	return sequence;
}

string add_gaps_begin(string sequence, int max_size_sequence)
{
	string gaps_sequence = "";
	for(int i = 0;i<max_size_sequence;i++)
		gaps_sequence += "-";
	sequence = gaps_sequence+sequence;
	return sequence;
}

vector<string> mutation_remove_column(vector<string> individual)
{
	string tempSequence;
	bool bAllGaps;
	string temp;
	string newSequence;
	vector<string> individual_rcolumn;
	
	// loop through the columns, starting at the end
	for (int c=individual[0].size()-1; c>=0; c--)
	{			
		bAllGaps = true;
		// loop through the sequences of the selected alignments
		for (int s=0; s<individual.size(); s++)
		{
			tempSequence = individual[s];
			//cout<<"tempSequence: "<<tempSequence<<endl;
			if (tempSequence[c] != '-')
			{
				bAllGaps = false;
				break;
			}
		}
		
		// if all the sequences have a gap in this column,
		// then remove them
		if (bAllGaps)
		{				
			// loop through the sequences of the selected alignments
			for (int s=0; s<individual.size(); s++)
			{						
				temp = individual[s];
				//cout<<"c: "<<c<<endl;
				// remove the gap column
				newSequence = temp.substr(0,c) + temp.substr(c+1);
				cout<<newSequence<<endl;
				// set the sequence back in the alignment
				individual_rcolumn.push_back(newSequence);
			}
		}
	}
	
	return individual_rcolumn;
}

vector<string> mutation_add_gap(vector<string> individual1)
{
	vector<string>	individual_addgap;
	srand((unsigned)(time(NULL)));
	for (int i=0; i<individual1.size(); i++)
	{//srand(i*individual1.size());
		string sequence = individual1[i];
		//srand((unsigned)(time(NULL)));
		int gapPosition = rand()%sequence.size();
		string beforeGap = sequence.substr(0,gapPosition);
		string afterGap = sequence.substr(gapPosition+1);
		string new_sequence = beforeGap + "-" + afterGap;
		individual_addgap.push_back(new_sequence);
	}

	for(int i = 0;i<individual_addgap.size();i++)
		cout<<individual_addgap[i]<<endl;
	
	return individual_addgap;	
}

vector<string> mutation_remove_gap(vector<string> individual1)
{
	string seq;
	string beforeGap;
	string afterGap;
	int pos = 0;
	int gapIndex = 0;
	
	vector<string>	individual_removegap;
 
	//For each sequence, delete a gap
	for(int i=0; i<individual1.size(); i++)
	{
		seq = individual1[i];
		
		//Start looking for a gap from a random position.
		string::size_type gapIndex = seq.find('-');
		
		//If we don't find any gap at all, this mutation won't work.
		// In this case, return the parent alignment unchanged.
		if (gapIndex == -1)
			return individual1;
		
		string beforeGap = seq.substr(0,gapIndex);
		string afterGap = seq.substr(gapIndex+1);
		string new_sequence = beforeGap + afterGap;
		individual_removegap.push_back(new_sequence);
	}
	
	for(int i = 0;i<individual_removegap.size();i++)
		cout<<individual_removegap[i]<<endl;

	return individual_removegap;	
}

vector<string> mutation(vector<string> individual1)
{
	srand((unsigned)(time(NULL)));
	float add_gapp_ratio = (float)(rand())/(float)(RAND_MAX);
	cout<<"add_gapp_ratiol: "<<add_gapp_ratio<<endl;

	vector<string> individual_mutation;
	if(add_gapp_ratio <= 0.5)
		individual_mutation = mutation_add_gap(individual1);

	float remove_gapp_ratio = (float)(rand())/(float)(RAND_MAX);
	cout<<"add_remove_ratiol: "<<remove_gapp_ratio<<endl;

	if(add_gapp_ratio <= 0.5)
		individual_mutation = mutation_remove_gap(individual1);

	return individual_mutation;
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
	srand((unsigned)(time(NULL)));
	float horizontal_ratio = (float)(rand())/(float)(RAND_MAX);
	cout<<"rand horizontal: "<<horizontal_ratio<<endl;
	if(horizontal_ratio <= horizontal_crossover)
	{
		vector<string> new_individual1;
		vector<string> new_individual2;

		int random_horizontal = (rand()%(kseq-1))+1;
		cout<<"random_horizontal: "<<random_horizontal<<endl;

		for(int i = 0; i<random_horizontal;i++)
		{
			new_individual1.push_back(individual1[i]);
			new_individual2.push_back(individual2[i]);
		}

		for(int i = random_horizontal; i<kseq;i++)
		{
			new_individual2.push_back(individual1[i]);
			new_individual1.push_back(individual2[i]);
		}
	}
	srand((unsigned)(time(NULL)));
	float vertical_ratio = (float)(rand())/(float)(RAND_MAX);
	cout<<"rand vertical: "<<horizontal_ratio<<endl;
	if(vertical_ratio <= vertical_crossover)
	{

		int min_size = 5000;

		for(int i=0; i<individual1.size();i++)
		{
			string min_individual = replace(individual1[i],"-", "");
			if(min_individual.size() < min_size)
				min_size = min_individual.size(); //without gaps
		}
		//cout<<"min_size: "<<min_size<<endl;
	
		vector<string> new_individual1;
		vector<string> new_individual2;

		vector<string> new_individual1_left;
		vector<string> new_individual1_right;
		vector<string> new_individual2_left;
		vector<string> new_individual2_right;

		int random_vertical= (rand()%individual1.size())+1;
		cout<<"random_vertical: "<<random_vertical<<endl;

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
			new_individual1.push_back(sequ_offprint1);
		}

		for(int i = 0; i<new_individual1.size();i++)
			cout<<new_individual1[i]<<endl;
		cout<<endl;

		for(int i = 0; i<new_individual2.size();i++)
			cout<<new_individual2[i]<<endl;
		cout<<endl;
	}
}

int main(void) {

	string sequence;
	vector<string> vec1;
	vector<string> vec2;
	vector<string> vec3;
	cin >> kseq;

	for(int i = 0; i<kseq;i++)
	{
		cin >> sequence;
		vec1.push_back(sequence);
	}

	for(int i = 0; i<kseq;i++)
	{
		cin >> sequence;
		vec2.push_back(sequence);
	}
	//crossover(vec1,vec2);
	mutation(vec1);

	for(int i = 0; i<kseq;i++)
	{
		cin >> sequence;
		vec3.push_back(sequence);
	}
	mutation_remove_column(vec3);
	
	return 0;

}
