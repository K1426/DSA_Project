//auto
struct TrieNode
{
    TrieNode* children[26];
    bool endofWord;
    bool isLastNode;
    TrieNode()
    {
        endofWord = false;
        isLastNode = true;
        for(int i = 0; i < 26; i++) children[i] = nullptr;
    }
};

class Trie
{
    private:
    TrieNode* root;
    public:
    Trie()
    {
        root = new TrieNode();
    }
    void insert(std::string& word)
    {
        int index = -1;
        TrieNode* node = root;
        for (char& c : word)
        {
            index = c - 'a';
            if (node->children[index] == nullptr)
            {
                node->children[index] = new TrieNode();
                node->isLastNode = false;
            }
            node = node->children[index];
        }
        node->endofWord = true;
    }
    bool search(std::string& word)
    {
        int index = -1;
        TrieNode* node = root;
        for (char& c : word)
        {
            index = c - 'a';
            if (node->children[index] == nullptr) return false;
            node = node->children[index];
        }
        return node->endofWord;
    }
    bool startsWith(std::string& prefix)
    {
        int index = -1;
        TrieNode* node = root;
        for (char& c : prefix)
        {
            index = c - 'a';
            if (node->children[index] == nullptr) return false;
            node = node->children[index];
        }
        return true;
    }
    void print(TrieNode* node, std::string& prefix)
    {
        if (node->endofWord) std::cout << prefix << "\n";
        for (int i = 0; i < 26; i++)
            if (node->children[i] != nullptr)
                print(node->children[i], prefix + char('a' + i));
    }

    int printAutoSuggestions(std::string& query)
    {
        int index = -1;
        TrieNode* node = root;
        for (char& c : query)
        {
            index = c - 'a';
            if (node->children[index] == nullptr) return 0;
            node = node->children[index];
        }
        if (node->isLastNode)
        {
            cout << query << endl;
            return -1;
        }
        print(node, query);
        return 1;
    }
};