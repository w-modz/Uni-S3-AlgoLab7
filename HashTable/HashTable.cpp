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
class DynamicArray
{
public:
	T* values;
	uint32_t max_size = DEFAULT_SIZE;
	uint32_t actual_size = 0;
	DynamicArray(void)
	{
		values = new T[DEFAULT_SIZE];
	}

	DynamicArray(int size)
	{
		values = new T[size];
	}

	~DynamicArray()
	{
		delete[] values;
		values = nullptr;
	}

	uint32_t GetSize()
	{
		return max_size;
	}

	uint32_t GetActualSize()
	{
		return actual_size;
	}

	void IncrementActualSize()
	{
		actual_size++;
	}

	void Set(uint32_t index, T value)
	{
		if (index > actual_size)
		{
			throw std::out_of_range("Index out of bounds");
		}
		values[index] = value;
	}
	T Get(uint32_t index)
	{
		if (index >= actual_size)
		{
			throw std::out_of_range("Index out of bounds");
		}
		return values[index];
	}

	void Append(T value)
	{
		if (actual_size + 1 > max_size)
		{
			Expand();
		}
		Set(actual_size, value);
		actual_size++;
	}

	void Clear()
	{
		delete[] values;
		actual_size = 0;
		T* new_values = new T[max_size];
		values = new_values;
	}

private:
	void Expand()
	{
		const uint32_t new_size = max_size * EXPANSION_MULTIPLIER;
		T* new_values = new T[new_size];
		for (int i = 0; i < actual_size; i++)
		{
			new_values[i] = values[i];
		}
		delete[] values;
		values = new_values;
		max_size = new_size;
	}
};

template<typename T>
class HashTable
{
//private:
public:
    DynamicArray<std::vector<std::pair<std::string, T>>>* table;
    
    uint16_t occupied_size;
    uint16_t max_size;

public:
	HashTable()
	{
		table = new DynamicArray<std::vector<std::pair<std::string, T>>>;
		max_size = table->max_size;
	}

    void Insert(string key, T value)
    {
		if ( ((double)occupied_size+1) / table->max_size >= RESIZE_THRESHHOLD)
		{
			Rehash();
		}
		pair<string, T> new_pair = { key, value };
		// checking for identical keys
		for (int i = 0; i < table->values[Hash(key)].size(); i++)
		{
			if (table->values[Hash(key)][i].first == key);
			{
				table->values[Hash(key)][i].second = value;
				return;
			}
		}
		// in case of no identical keys
		table->values[Hash(key)].push_back(new_pair);
		table->IncrementActualSize();
		occupied_size++;
		return;
	}

    std::pair<std::string, T>* Find(string key)
    {
		table->values[Hash(key)];
		for (int i = 0; i < table->values[Hash(key)].size(); i++)
		{
			if (table->values[Hash(key)][i].first == key);
			{
				return &(table->values[Hash(key)][i]);
			}
		}
		return NULL;
	}

    bool Delete(string key)
    {
		std::pair<std::string, T>* kv_pair = Find(key);
		if (kv_pair != NULL && table->values[Hash(key)].size() == 1)
		{
			delete kv_pair;
			occupied_size--;
			table->actual_size--;
			return true;
		}
		else if (kv_pair != NULL && table->values[Hash(key)].size() != 1)
		{
			for (int i = 0; i < table->values[Hash(key)].size(); i++)
			{
				if (table->values[Hash(key)][i].first == key);
				{
					std::pair<std::string, T>* last = &table->values[Hash(key)].back();
					table->values[Hash(key)][i] = *last;
					delete last;
				}
			}
			occupied_size--;
			table->values[Hash(key)].shrink_to_fit();
			return true;
		}
		else
		{
			return false;
		}
		
	}

    void Clear()
    {
		table->Clear();
		occupied_size = 0;
	}

    string ToString(uint32_t number_to_display)
    {
		if (number_to_display > table->actual_size)
		{
			throw std::out_of_range("Index out of scope");
		}
		std::ostringstream output_stream;
		output_stream << "[";
		int found = 0;
		int i = 0;
		while (found < number_to_display)
		{
			//std::pair<std::string, T> element = table->values[i];
			if (table->values[i].size() != 0)
			{
				output_stream << "{" << i << "} " << /*table->values[i][0].first.c_str() <<*/ "\n";
				found++;
			}
			i++;
		}
		output_stream << "]";
	}

    int Hash(string key)
    {
        int hash_value = 0;
        int length = key.length();
        for (int i = 0; i < length; i++)
        {
            hash_value += (int)key[i] * pow(31, length - (i));
        }
        /*hash_value % table->max_size;
        if (hash_value < 0)
        {
            hash_value += table->max_size;
        }*/
        return (table->max_size + (hash_value % table->max_size)) % table->max_size;
    }

    void Rehash()
    {
		DynamicArray<std::vector<std::pair<std::string, T>>>* old_table = table;
		max_size *= EXPANSION_MULTIPLIER;
		occupied_size = 0;
		table = new DynamicArray<std::vector<std::pair<std::string, T>>>(max_size);
		for (int i = 0; i < old_table->GetSize(); i++)
		{
			for (int j = 0; j < old_table->values[i].size(); j++)
			{
				Insert(old_table->values[i][j].first, old_table->values[i][j].second);
			}
		}
		delete[] old_table;
		printf("rehashed\n");
	}

    void PrintStats()
    {}
};

int main()
{
	HashTable<int>* ht = new HashTable<int>;
	//printf("%d\n",ht->Hash("a"));
	//ht->Insert("abcdef", 10);
	//ht->Insert("abcdef", 12);
	//printf("%s %d\n", ht->table->values[ht->Hash("abcdef")][0].first.c_str(), ht->table->values[ht->Hash("abcdef")][0].second);
	////ht->Find("abcdef");
	//printf("%s %d\n", ht->Find("abcdef")->first.c_str(), ht->Find("abcdef")->second);
	//printf("%d \n", ht->Find("a"));

	////printf("%s",ht->ToString(1));


	//printf("%d \n", ht->Delete("a"));
	//printf("%d \n", ht->Delete("abcdef"));

	ht->Insert("abcdef", 1);
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
	ht->Insert("kkkkkk", 21);

	printf("%s %d\n", ht->table->values[ht->Hash("abcdef")][0].first.c_str(), ht->table->values[ht->Hash("abcdef")][0].second);
	printf("%i\n",ht->max_size);
	printf("%i\n", ht->table->max_size);

	//ht->table->Clear();
	//ht->Clear();
}

