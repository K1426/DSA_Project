//chain
#include <iostream>
#include <vector>

//structure for Word
struct Word
{
    int data;
    std::vector<std::pair<int, float>> ranks;
    Word* next = nullptr;
    Word() {}
    Word(int data)
    {
        this->data = data;
    }
    void insert_doc(int docID, float rank)
    {
        ranks.push_back({docID, rank});
    }
    /*
    //testing purposes
    void save_to_file(std::ofstream& f)
    {
        for (auto& hits : ranks)
        {
            f << data;
            if (hits.size() <= 1) {std::cout << "error " << data; return;}
            for (int pos : hits) f << " " << pos;
            f << "\n";
        }
    }
    */
};


//the linked list class
class LinkedList
{
    private:
    Word* head = nullptr;
    Word* tail = nullptr;
    Word* point = nullptr;

    public:
    //getters
    Word* headWord()
    {
        return head;
    }
    Word* tailWord()
    {
        return tail;
    }

    //add a Word
    Word* add(int key)
    {
        point = new Word(key);
        if (head == nullptr) head = point;
        if (tail != nullptr) tail->next = point;
        tail = point;
        return tail;
    }

    //check if a Word is here
    Word* contains(int val)
    {
        point = head;
        while (point != nullptr)
        {
            if (point->data == val) return point;
            point = point->next;
        }
        return nullptr;
    }

    /*
    //testing purposes
    void save(std::ofstream& f)
    {
        point = head;
        while (point != nullptr)
        {
            point->save_to_file(f);
            point = point->next;
        }
    }
    */
};


//the hash table class
class HashTable
{
    private:
    LinkedList *bucket = nullptr;
    int size = 0;
    int count = 0;

    public:
    //constructor
    HashTable(int tablesize)
    {
        size = tablesize;
        bucket = new LinkedList[size];
    }

    //insert a key
    Word* insert(int key)
    {
        count++;
        return bucket[key % size].add(key);
    }

    Word* get_word(int key)
    {
        return bucket[key % size].contains(key);
    }
    
    int getcount() {return count;}

    /*
    //testing purposes
    void save_barrels(std::string name)
    {
        std::ofstream f(name, std::ios::app);
        for (int i = 0; i < size; i++)
        {
            std::cout << i << " ";
            bucket[i].save(f);
        }
        f.close();
    }
    */
};
