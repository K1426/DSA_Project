//chain
#include <iostream>

//structure for Word
struct Word
{
    int data;
    std::map<int, std::vector<int>> hitlist;
    Word* next = nullptr;
    Word() {}
    Word(int data)
    {
        this->data = data;
    }
    void insert_hit(int docID, std::vector<int> hit)
    {
        hitlist[docID] = hit;
    }
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
        return point;
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
};