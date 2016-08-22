#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <inttypes.h>
#include <fstream>

using namespace std;

#include "def.h"

class HashEntry {
	private:
		long from;
		long to;
	public:
		HashEntry(long from_val, long to_val) {
			this->from = from_val;
			this->to = to_val;
		}

		long getFrom() {
			return from;
		}

		long getTo() {
			return to;
		}
};

class HashMap {
	private:
		HashEntry **table;
		int64_t TABLE_SIZE;
	public:
		HashMap(int64_t size) {
			this->TABLE_SIZE = size;
			table = new HashEntry*[size];
			for (long i = 0; i < size; i++)
				table[i] = NULL;
		}



		long get(long from, long to) {
			long hash = (from % TABLE_SIZE);
			while (table[hash] != NULL && (table[hash]->getFrom() != from || table[hash]->getTo() != to))
				hash = (hash + 1) % TABLE_SIZE;
			if (table[hash] == NULL)
				return -1;
			else
				return table[hash]->getFrom();
		}

		int put(long from, long to) {
			long hash = (from % TABLE_SIZE);
			while (table[hash] != NULL && (table[hash]->getFrom() != from || table[hash]->getTo() != to))
				hash = (hash + 1) % TABLE_SIZE;
			if (table[hash] != NULL)
				return 1;
			table[hash] = new HashEntry(from, to);
			return 0;
		} 



		~HashMap() {
			for (long i = 0; i < TABLE_SIZE; i++)
				if (table[i] != NULL)
					delete table[i];
			delete[] table;
		}
};


int main(int argc, char* argv[])
{
	ifstream rptr;
	rptr.open(argv[1], ifstream::in);
	if(!rptr.is_open()) {
		fprintf(stderr, 
				"The file %s couldn't be opened for reading (Does it exist?)!\n", 
				argv[1]);
		return EXIT_FAILURE;
	}

	/*read the content from the input file through stringstream*/

	string newline;
	stringstream ss;
	double temp;
	int64_t n, m;
	int64_t *ei = NULL, *ej = NULL;
	double *w = NULL;
	int64_t edge_count = 0;
	int64_t ss_count;

	while(getline(rptr, newline))
	{
		ss.clear();
		ss << newline;
		/*check format of each line*/
		while(ss >> temp)
		{
			ss_count ++;
			if(ss_count > 3)
			{
				break;
			}
		}
		/*skip empty line*/
		if(ss_count == 0)
		{
			ss.clear();
			continue;
		}
		/*wrong format*/
		else if(ss_count != 3)
		{
			fprintf(stderr, "Invalid Input!\n");
			rptr.close();
			return EXIT_FAILURE;
		}
		else
		{
			ss.clear();
			ss << newline;
			if(edge_count == 0)
			{
				ss >> n;
				ss >> n;
				ss >> m;
				ei = (int64_t *)malloc(sizeof(int64_t) * m);
				ej = (int64_t *)malloc(sizeof(int64_t) * m);
				w = (double *)malloc(sizeof(double) * m);
			}
			else
			{
				/*check edge number*/
				if(edge_count > m)
				{
					fprintf(stderr, "Wrong Edge Number!\n");
					free(ei);
					free(ej);
					free(w);
					rptr.close();
					return EXIT_FAILURE;
				}
				ss >> ei[edge_count - 1];
				ss >> ej[edge_count - 1];
				ss >> w[edge_count - 1];
				/*detect index error*/
				if(ei[edge_count - 1] >= n || ej[edge_count - 1] >= n)
				{
					fprintf(stderr, "Exceed Index in Line %ld!\n", edge_count + 1);
					rptr.close();
					return EXIT_FAILURE;
				}
				/*detect diagonal edges*/
				if(ei[edge_count - 1] == ej[edge_count - 1])
				{
					fprintf(stderr, "Diagonal Edges Found in Line %ld!\n", edge_count + 1);
					rptr.close();
					return EXIT_FAILURE;
				}
				/*detect negative weight*/
				if(w[edge_count - 1] < 0)
				{
					fprintf(stderr, "Negative Weights Found in Line %ld!\n", edge_count + 1);
					rptr.close();
					return EXIT_FAILURE;
				}
			}
			edge_count ++;
			ss_count = 0;
		}
	}

	if((edge_count - 1) != m)
	{
		fprintf(stderr, "Wrong Edge Number!\n");
		free(ei);
		free(ej);
		free(w);
		rptr.close();
		return EXIT_FAILURE;
	}
	rptr.close();


	HashMap *edge_table = new HashMap(2 * m);

	/*detect repeated edges*/
	for(int64_t i = 0; i < m; i ++)
	{
		if(edge_table->put(ei[i], ej[i]))
		{
			fprintf(stderr, "Reapted Edge in line %ld!\n", i + 2);
			free(ei);
			free(ej);
			free(w);
			delete edge_table;
			return EXIT_FAILURE;
		}
	}

	/*check symmetric*/
	for(int64_t i = 0; i < m; i ++)
	{
		if(edge_table->get(ej[i], ei[i]) == -1)
		{
			fprintf(stderr, "Symmetric Error!\n");
			free(ei);
			free(ej);
			free(w);
			delete edge_table;
			return EXIT_FAILURE;
		}
	}



	// TODO check symmetric
	// TODO check for diagonal edges
	// TODO check for negative weights
	// TODO validate input
	// TODO check for repeated edges

	fprintf(stderr, "loaded file %s : %12" PRId64 " nodes   %12" PRId64 " edges\n",
			argv[1], n, m);

	double ret;
	size_t outputlen = n;
	int64_t *output = (int64_t *)malloc(sizeof(int64_t) * n);
	ret = densest_subgraph(n, m, ei, ej, w, output, &outputlen);
	cout << ret << endl;
	cout << outputlen << endl;
	for (size_t j = 0; j < outputlen; ++j) {
		cout << output[j] << endl;
	}

	return 0;
}
