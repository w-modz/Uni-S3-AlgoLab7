#include <iostream>
#include <string>
#include <array>
#include <ctime>
#include <random>
#include <forward_list>
#include <sstream>

#define RESIZE_THRESHHOLD 0.75

using namespace std;

#define DEFAULT_SIZE 10
#define EXPANSION_MULTIPLIER 2

template<typename T>
struct Node
{
	string key;
	T value;
};

template<typename T>
class HashTable
{
public:
	vector<vector<Node<T>>> table;
	uint32_t occupied_size;
	uint32_t max_size;

public:
	HashTable()
	{
		max_size = DEFAULT_SIZE;
		occupied_size = 0;
		table.resize(max_size);
	}

	~HashTable()
	{
		Clear();
		max_size = 0;
		occupied_size = 0;
	}

	void Insert(string key, T value)
	{
		if (((double)occupied_size + 1) / max_size >= RESIZE_THRESHHOLD)
		{
			Rehash();
		}
		Node<T> new_pair = { key, value };
		int hashc = Hash(key);
		if (table[hashc].size() == 0)
		{
			// in case of empty vector
			table[hashc].push_back(new_pair);
			occupied_size++;
			return;
		}
		for (int i = 0; i < table[hashc].size(); i++)
		{
			// checking for identical keys
			if (table[hashc][i].key == key)
			{
				table[hashc][i].value = value;
				return;
			}
		}
		// in case of no identical keys
		table[hashc].push_back(new_pair);
		return;
	}

	Node<T>* Find(string key)
	{
		for (int i = 0; i < table[Hash(key)].size(); i++)
		{
			if (table[Hash(key)][i].key == key);
			{
				return &(table[Hash(key)][i]);
			}
		}
		return NULL;
	}

	bool Delete(string key)
	{
		if (Find(key) != NULL && table[Hash(key)].size() == 1)
		{
			table[Hash(key)].erase(table[Hash(key)].begin(), table[Hash(key)].begin());
			occupied_size--;
			return true;
		}
		else if (Find(key) != NULL && table[Hash(key)].size() > 1)
		{
			for (int i = 0; i < table[Hash(key)].size(); i++)
			{
				if (table[Hash(key)][i].key == key);
				{
					Node<T>* last = &table[Hash(key)].back();
					table[Hash(key)][i] = *last;
					table[Hash(key)].pop_back();
				}
			}
			table[Hash(key)].shrink_to_fit();
			return true;
		}
		else
		{
			return false;
		}
	}

	void Clear()
	{
		table.erase(table.begin(),table.end());
		table.resize(max_size);
		occupied_size = 0;
	}

	void PrintState(bool short_notation = true)
	{
		int printed = 0;
		printf("Current maximum size of Hash Table: %i\n",max_size);
		printf("Current state of Hash Table:\n");
		printf("----------------------------\n");
		for (int i = 0; i < table.size(); i++)
		{
			for (int j = 0; j < table[i].size(); j++)
			{
				if (table[i].size() > 0)
				{
					printf("[%i]", i);
					printf(" {%s, %d} ", table[i][j].key.c_str(), table[i][j].value);
				}
			}
			if (table[i].size() > 0)
			{
				printed++;
				printf("\n");
			}
			if (short_notation && printed >= 30)
			{
				printf("This is the short notation of the table showing the first 30 elements\n\n");
				return;
			}
		}
		printf("\n");
	}

	int Hash(string key)
	{
		int hash_value = 0;
		int length = key.length();
		for (int i = 0; i < length; i++)
		{
			//hash_value += (int)key[i] * pow(31, length - (i));
			hash_value = hash_value * 31 + (int)key[i];
		}
		//return (max_size + (hash_value % max_size)) % max_size;
		return hash_value % max_size;
	}

	void Rehash()
	{
		vector<vector<Node<T>>> old_table = table;
		table.clear();
		max_size *= EXPANSION_MULTIPLIER;
		table.resize(max_size);
		occupied_size = 0;

		for (int i = 0; i < old_table.size(); i++)
		{
			for (int j = 0; j < old_table[i].size(); j++)
			{
				Insert(old_table[i][j].key, old_table[i][j].value);
			}
		}
		old_table.clear();
	}

	void PrintStats()
	{
		int non_empty_vectors = 0, longest_vector = 0, pair_amount = 0;
		double average_lenght = 0;
		for (int i = 0; i < table.size(); i++)
		{
			average_lenght += table[i].size();
			if (table[i].size() > 0)
			{
				non_empty_vectors++;
			}
			if (table[i].size() > longest_vector)
			{
				longest_vector = table[i].size();
			}
			for (int j = 0; j < table[i].size(); j++)
			{
				pair_amount++;
			}
		}
		average_lenght = average_lenght / max_size;
		printf("Hash Table Statistics\n");
		printf("----------------------------\n");
		printf("Amount of non-empty lists: %i\n",non_empty_vectors);
		printf("Amount of pairs in table: %i\n",pair_amount);
		printf("Average list lenght: %lf\n", average_lenght);
		printf("Longest list lenght: %i\n\n", longest_vector);
	}
};

string GenerateRandomString(int length)
{
	string str = "";
	auto chars = "abcdefghijklmnopqrstuvwxyz";
	for (int i = 0; i < length; i++)
	{
		str += chars[rand() % strlen(chars)];
	}
	return str;
}

int main()
{
	//HashTable<int>* ht = new HashTable<int>;
	////printf("%d\n",ht->Hash("a"));
	////ht->Insert("abcdef", 10);
	////ht->Insert("abcdef", 12);
	////printf("%s %d\n", ht->table[ht->Hash("abcdef")][0].key.c_str(), ht->table[ht->Hash("abcdef")][0].value);
	////////ht->Find("abcdef");
	////printf("%s %d\n", ht->Find("abcdef")->key.c_str(), ht->Find("abcdef")->value);
	////printf("%d \n", ht->Find("a"));

	////////printf("%s",ht->ToString(1));


	////printf("%d \n", ht->Delete("a"));
	////printf("%d \n", ht->Delete("abcdef"));

	/*ht->Insert("abcdef", 1);
	ht->Insert("dsadas", 2);
	ht->Insert("afdgds", 3);
	ht->Insert("shudus", 4);
	ht->Insert("jsdkjd", 5);
	ht->Insert("dsajjj", 6);
	ht->Insert("uyhjmk", 7);
	ht->Insert("hdufhu", 8);
	ht->Insert("dkjjdd", 9);
	ht->Insert("dmmdmm", 10);
	ht->Insert("dsaijd", 11);
	ht->Insert("jdsndd", 12);
	ht->Insert("a", 13);
	ht->Insert("b", 14);
	ht->Insert("x", 15);
	ht->Insert("c", 16);
	ht->Insert("s", 17);
	ht->Insert("sa", 18);
	ht->Insert("aaaa", 19);
	ht->Insert("dsadsa", 20);
	ht->Insert("kkkkkk", 21);*/
	///*for (int i = 0; i < ht->table[ht->Hash("abcdef")].size(); i++)
	//{
	//	printf("%s %d\n", ht->table[ht->Hash("abcdef")][i].key.c_str(), ht->table[ht->Hash("abcdef")][i].value);
	//}*/
	/*for (int i = 0; i < ht->table.size(); i++)
	{
		printf("[%i]",i);
		for (int j = 0; j < ht->table[i].size(); j++)
		{
			printf(" {%s, %d} ",ht->table[i][j].key.c_str(), ht->table[i][j].value);
		}
		printf("\n");
	}*/
	/*ht->PrintState(true);
	ht->PrintStats();

	ht->Clear();*/
	/*srand(time(0));
	for (size_t i = 0; i < 10; i++)
	{
		printf("%s\n",GenerateRandomString(6).c_str());
	}*/
	srand(time(0));
	const int MAX_ORDER = 6; // m a k s y m a l n y rzad wielkosci d o d a w a n y c h danych
	HashTable<int>* ht = new HashTable <int>(); // w tym p r z y k l a d z i e tablica m i e s z a j a c a par < string ,
		for (int o = 1; o <= MAX_ORDER; o++)
		{
			const int n = pow(10, o); // rozmiar danych
			// dodawanie do tablicy m i e s z a j a c e j
			clock_t t1 = clock();
				for (int i = 0; i < n; i++)
					ht->Insert(GenerateRandomString(6), i); // klucze losowe 6 - znakowe , a jako wartosci indeks petli
			clock_t t2 = clock();

			std::cout << "HT state: " << "\n";
			ht->PrintState();
			std::cout << "Total time of inserting = " << ((t2 - t1) / (double)CLOCKS_PER_SEC) * 1000 << "ms\n";
			std::cout << "Average time of inserting = " << ((t2 - t1) / (double)CLOCKS_PER_SEC) * 1000 / n << "ms\n";
			
			const int m = pow(10, 4);
			int hits = 0;
			t1 = clock();
			for (int i = 0; i < m; i++)
			{
				Node<int>* entry = ht -> Find(GenerateRandomString(6)); // w y s z u k i w a n i e wg losowego klucza
				if (entry != NULL)
					hits++;
			}
			t2 = clock();
			std::cout << "Amount of hits: "<< hits << "\n";
			std::cout << "Total time of searching = " << ((t2 - t1) / (double)CLOCKS_PER_SEC) * 1000 << "ms\n";
			std::cout << "Average time of searching = " << ((t2 - t1) / (double)CLOCKS_PER_SEC) * 1000 / n << "ms\n";
			ht->PrintStats();
			// wypis statystyk ( dlugosci list kryjacych sie w tablicy m i e s z a j a c e j )
			ht->Clear(); // c z y s z c z e n i e tablicy m i e s z a j a c e j
		}
	delete ht;
	return 0;
}

