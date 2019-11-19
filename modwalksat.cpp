#include <bits/stdc++.h>
#include <random>       
#include <chrono>  
using namespace std;

/*
	Total runtime of the code is ~ 1.5 minutes, runtime is more at higher m/n ratios due to more number of cnfs attempted.
	Observe the m/n value increase as the program moves forward to gain an idea of how far along the program is.

	Every function has a description above it
	Some lines have descriptions after, where needed.

	This file creates an intermediate csv file "graph.csv", which is used by plot_graph.py to plot the graph.

	For each m/n ratio, i have generated 30 random 3-CNFs, and tested them with the algorithm.
	However, since cnf's are more likely to be unsatisfiable at higher m/n ratios,
	I have increased the total number of cnfs generated for each m/n from 30 to 50 after m/n crosses 6,
	so that there is more likelihood of generating a satisfiable formula.
	However, due to the fact that there are only few satisfiable formulas that end up being generated because
	of this low probability, there is a lot of noise in the graph when m/n is greater than 5 due to less data used in the graph. 
	There may be high variations in the latter part(m/n > 6) of all the graphs obtained upon multiple run-throughs of the code.
*/

class valuation
{
public:
	vector <int> valuations; //true false values for n symbols
	int n; //n is the number of symbols
	
	valuation(vector <int> vals)
	{
		valuations = vals;
		n = vals.size();
	}
	valuation(int n)
	{

		this->n = n;
		valuations.resize(n,0);
		for(int i = 0; i < n; i++)
			valuations[i] = rand()%2;
	}

	/*
		Description of print_valuation():
		Prints the current valuation
	*/
	void print_valuation()
	{
		cout<<'{';
		for(int j = 0; j < n; j++)
		{
			cout<<'\''<<j+1<<"\'="<<valuations[j]; //variable number with value representing true/false
			if(j != n-1)
				cout<<", ";
		}
		cout<<'}'<<endl;
	}

	/*
		Description of bestflips():
		finds the best v symbols to flip at a particular stage
	*/
	void bestflips(int v,vector<int> all_symbols) 
	{
		int n = all_symbols.size();

		vector <int> flippedsymbols;

		if(n<=v){
			flippedsymbols = all_symbols;
		}
		else{
			for(int i = 0; i < v; i++)
			flippedsymbols.push_back(all_symbols[i]);
		}
		for(int i=0;i<flippedsymbols.size();i++)
			valuations[flippedsymbols[i]-1] = 1 - valuations[flippedsymbols[i]-1];
	}

	/*
		Description of randomlyflip():
		randomly flips v symbols from the model, by shuffling all symbols, then choosing the first v
	*/
	void randomlyflip(int v,vector <int> all_symbols)
	{
		int n = all_symbols.size(); // n is the number of possible symbols we can flip
		vector <int> flippedsymbols;

		if(n > v)
		{
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
			shuffle (all_symbols.begin(), all_symbols.end(), std::default_random_engine(seed));
			for(int i = 0; i < v; i++)
				flippedsymbols.push_back(all_symbols[i]);
		}
		else
		{
			flippedsymbols = all_symbols;
		}
		for(int i=0;i<flippedsymbols.size();i++)
			valuations[flippedsymbols[i]-1] = 1 - valuations[flippedsymbols[i]-1];
	}
	
};


class Formula
{
public:
	int n, m;
	vector <vector <int> > all_clauses;
	Formula()
	{
		n=0;
		m=0;
	}

	//copies clause_vars into clauses, but each row has a 0 at the end 
	Formula(int n, int m, vector<vector <int> > clause_vars) 
	{
		this->n = n;
		this->m = m;
		all_clauses.resize(m);

		for(int i = 0; i < m; i++)
		{
			for(int j:clause_vars[i])
				all_clauses[i].push_back(j);
			all_clauses[i].push_back(0); //end of clause
		}	
	}

	/*
		Description of printFormula():
		prints the cnf Formula
	*/
	void printFormula()
	{
		for(int i = 0; i < m; i++)
		{
			cout<<'(';
			for(int j = 0; j < all_clauses[i].size() - 1; j++)
			{
				cout<<all_clauses[i][j];
				if(j != all_clauses[i].size() - 2)
					cout<<'v';
			}
			cout<<')';

			if(i != m-1)
				cout<<'^';
		}
		cout<<endl;
	}

	/*
		Description of satisfiable():
		returns true if the valuation v satisfies the cnf
	*/
	bool satisfiable(valuation v)
	{
		for(vector<int> c:all_clauses)
		{
			for(int i:c)
			{
				//if its a postivie literal and true, or negative literal and false
				if((i>0 && v.valuations[i-1] == 1) || (i<0 && v.valuations[-i - 1] == 0))
					break;

				if(i == 0)
					return false;
			}
		}
		return true;
	}

	/*
		Description of false_vars():
		This function sorts the symbols in order of how many false clauses they appear in
		For example, if symbol 1 occurs in 3 false clauses, and 2 occurs in 2 false clauses, then 3 would come before 2, and so on.
		It returns a sorted vector of pairs, each pair having a symbol, followed by the number of clauses that symbol is currently false in. 
	*/
	vector <pair<int, int>> false_vars(valuation v) 
	{
		vector <int> counts;
		vector <pair<int, int> > sorted_symbols;
		counts.resize(n);

		for(vector<int> specific_clause:all_clauses)
		{
			vector <int> clause_counts;
			clause_counts.resize(n, 0);
			//specific_clause is a specific clause, symbol is a specific symbol in the clause
			//clause_counts is a vector of size n, representing which symbols are false in a clause.
			for(int symbol:specific_clause)
			{
				//if the clause is true, break
				if((symbol>0 && v.valuations[symbol-1] == 1) || (symbol<0 && v.valuations[-symbol - 1] == 0))
					break;
				else if(symbol == 0)
				{
					for(int j = 0; j < n; j++)
						counts[j] += clause_counts[j];
				}

				else if(symbol!=0)
					clause_counts[abs(symbol)-1] += 1;		
			}
		}

		for(int i = 0; i < n; i++)
		{
			if(counts[i] != 0)
				//count then symbol
				sorted_symbols.push_back(make_pair(counts[i],i+1));
		}

		sort(sorted_symbols.begin(),sorted_symbols.end());
		//custom comparator for sort, sorted by the se
		/*sort(sorted_symbols.begin(), sorted_symbols.end(), [](pair<int, int> a, pair<int, int> b)
		{
			return a.second > b.second;
		});*/
		return sorted_symbols;
	}
};

/*
	Description of modified_walksat()
	This function follows the algorithm provided in the question description
	With probability p, the function chooses to flip the best v symbols
	with probability 1-p, the function chooses to flip any random v symbols, so that the algorithm does not get stuck
	Algorithm runs maxit times for each v from 1 to maxv
*/

valuation modified_walksat(Formula f, float p, int maxit, int maxv, int &runtime, bool &status)
{
	
	valuation m(f.n);
	int v = 0;
	runtime = 0;
	while(v <= maxv)
	{
		v++;

		for(int k = 0; k < maxit; k++)
		{
			runtime++;
			if(f.satisfiable(m))
			{
				status=true;
				return m;
			}

			vector <pair<int, int> > symbol_count;//count of false clauses of each symbols
			vector <int> symbols;
			symbol_count = f.false_vars(m); //this is the vector of pairs of variable and its current number of false clauses

			for(pair<int, int> p:symbol_count)
				//second has the symbol
				symbols.push_back(p.second);

			double temp = (double)rand()/(double)RAND_MAX;
			if(temp >= p){
				m.randomlyflip(v,symbols);
			}
			else{
				m.bestflips(v ,symbols);
			}
		}
	}

	status = false;
	return m;
}

/*
	Description of random_cnf_generator():
	This function randomly generates 3 CNF formulas. 
	I have chosen to fix the number of literals in each clause for uniformity,and also so that there are no unit clauses
	This function works by randomly shuffling all the symbols, using shuffle function, and then choosing the first 3
	Each literal chosen has a 0.5 probability of being positive or negative, for uniformity
*/

Formula random_cnf_generator(int n, int m)
{
	Formula formula;
	formula.n = n;
	formula.m = m;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	vector<vector<int>> clauses;
	vector<int> symbols;
	symbols.resize(n,0);
	for(int i=0;i<n;i++)
	{
		symbols[i]=i+1;
	}
	for(int i=0;i<m;i++)
	{
		//for each clause, randomly generate symbols
		shuffle (symbols.begin(), symbols.end(), std::default_random_engine(seed));
		vector<int> clause;
		for(int j=0;j<3;j++)
		{
			int symbol = symbols[j];
			int choose = rand()%2;
			if(choose)
				clause.push_back(symbol);
			else 
				clause.push_back(-symbol);
		}

		clause.push_back(0);
		clauses.push_back(clause);
	}
	formula.all_clauses = clauses;
	return formula;
}


/*
	Description of main():
	Here:
	maxv=5
	maxit=80
	p=0.35

	I have randomly generated 40 different 3-CNFs for 100 different m/n ratios, incremented by 0.065 each time
	n is randomly generated, and m is obtained from (m/n) and n.
	The runtime and the m/n values have been stored in 2 vectors.
	These vectors have been used along with a file pointer, to construct a csv that is used by the python code to construct the graph.
	While constructing the graph, the ratios have been grouped into 45 buckets of size ~ 0.15, and the log of the
	mean runtime has been taken for each bucket while plotting.
	Since cnf's are more likely to be unsatisfiable at higher m/n ratios, I have increased j_limit from 40 to 60 so that there is more likelihood of 
	generating a satisfiable formula.
	However, due to the fact that there are only few satisfiable formulas that are generated, there is a lot of noise in the graph when m/n 
	is greater than 5. There may be high variations in this part of the graph upon multiple run throughs of the code.
*/

int main(int argc, char* argv[])
{

	srand(time(0));
	rand();
	int maxv, maxit;
	float p;
	maxv=5;
	maxit=60;
	p=0.35;
	vector <vector <int> > C;
	vector<int> runtimes;
	vector<float> ratios;
	bool status;
	float ratio = 1.0;
	float temp_ratio;
	int j_limit=40;
	//j_limit cnf's for each m/n of the 100 m/n values chosen
	for(int i=0;i<100;i++) 
	{
		cout<<"Value of ratio: "<<ratio<<endl;
		if(ratio>6)
			j_limit=60;
		for(int j=0;j<j_limit;j++){
			rand();
			Formula formula;
			cout<<"Cnf "<<(j+1)<<":\n";
			int n =(rand()%8) + 3;
			int m = n*ratio;
			cout<<"\nn : "<<n<<endl<<"m : "<<m<<endl<<"m/n : "<<ratio<<endl;
			temp_ratio =(double)m/(double)n;
			formula = random_cnf_generator(n,m);
			formula.printFormula();
			valuation solution(n);
			cout<<"Initial valuation:\n";
			solution.print_valuation();
			int runtime;
			solution = modified_walksat(formula,p,maxit,maxv,runtime,status);	
			if(status==true)
			{
				cout<<"Solution:\n";
				solution.print_valuation();
				runtimes.push_back(runtime);//runtimes is y, ratios is x
				ratios.push_back(temp_ratio);
				cout<<"\nRuntime: "<<runtime;
			}
			else
			{
				cout<<"Failure, with runtime: "<<runtime<<endl;
			}
			cout<<endl<<endl;
		}	
		ratio+=0.065;
	}
	fstream myfile;
	myfile.open("graph.csv",fstream::out| fstream::trunc);
	myfile<< "X,Y\n";
	for(int i=0;i<ratios.size();i++)
	{
		myfile << ratios[i]<<","<<runtimes[i];
		myfile<<"\n";
	}
	myfile.close();
	return 0;

}