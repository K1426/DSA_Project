//auto

const int size = 37;

int getindex(char c)
{
    if (c == '-') return size - 1;
    if (c > 47 && c < 58) return c - 22;
    return c - 'a';
}

struct TrieNode
{
    TrieNode* children[size];
    bool endofWord;
    bool isLastNode;
    TrieNode()
    {
        endofWord = false;
        isLastNode = true;
        for(int i = 0; i < size; i++) children[i] = nullptr;
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
    void insert(std::string word)
    {
        int index = -1;
        TrieNode* node = root;
        for (char& c : word)
        {
            index = getindex(c);
            if (node->children[index] == nullptr)
            {
                node->children[index] = new TrieNode();
                node->isLastNode = false;
            }
            node = node->children[index];
        }
        node->endofWord = true;
    }
    bool search(std::string word)
    {
        int index = -1;
        TrieNode* node = root;
        for (char& c : word)
        {
            index = getindex(c);
            if (node->children[index] == nullptr) return false;
            node = node->children[index];
        }
        return node->endofWord;
    }
    bool startsWith(std::string prefix)
    {
        int index = -1;
        TrieNode* node = root;
        for (char& c : prefix)
        {
            index = getindex(c);
            if (node->children[index] == nullptr) return false;
            node = node->children[index];
        }
        return true;
    }
    void print(TrieNode* node, std::string prefix, std::vector<std::string>& res)
    {
        if (node->endofWord) res.push_back(prefix);
        for (int i = 0; i < size; i++)
            if (node->children[i] != nullptr)
                print(node->children[i], prefix + char('a' + i), res);
    }

    std::vector<std::string> printAutoSuggestions(std::string query)
    {
        int index = -1;
        std::vector<std::string> res;
        TrieNode* node = root;
        for (char& c : query)
        {
            index = getindex(c);
            if (node->children[index] == nullptr) return {};
            node = node->children[index];
        }
        if (node->isLastNode)
        {
            return {query};
        }
        print(node, query, res);
        return res;
    }
};