//sort_algo
#include <vector>
#include <unordered_map>

int getMax(int a[], int n)
{
    int max = a[0];
    for (int i = 1; i < n; i++)
        if (a[i] > max)
            max = a[i];
    return max;
}

int getMin(int a[], int n)
{
    int max = a[0];
    for (int i = 1; i < n; i++)
        if (a[i] < max)
            max = a[i];
    return max;
}

float getrank(std::pair<int, float>& a) {return a.second;}

void quickSort(std::vector<std::pair<int, float>>& a, int start, int end, float (*f)(std::pair<int, float>&))
{
    //ensure the numbers to be sorted are more than one
    if (start >= end) return;
    
    //select pivot element (here it is center element)
    float pivot = f(a[(start + end) / 2]);
    int i = start - 1, j = end + 1;
    
    while (true)
    {
        //decrease i until larger or equal element is found
        do i++; while (f(a[i]) < pivot);
        //decrease j until smaller or equal element is found
        do j--; while (f(a[j]) > pivot);
        //if all elements have been traversed break
        if (i >= j) break;
        //swap the elements that were on wrong side
        a[i].swap(a[j]);
    }

    //do the same with array on left of pivot
    quickSort(a, start, j, f);
    //and then with array on right of pivot
    quickSort(a, j + 1, end, f);
}

std::vector<std::pair<int, float>> single_search(std::string& word)
{
    clean_token(word);
    if (word == "") return {};
    if (lexicon.find(word) == lexicon.end()) return {};
    int wordID = lexicon[word];
    std::cout << "word id " << wordID << std::endl;
    std::vector<std::pair<int, float>> ranks = barrel_inverted_index->get_word(wordID)->ranks;
    quickSort(ranks, 0, ranks.size() - 1, getrank);
    return ranks;
}

void rank(std::vector<std::pair<int, float>>& ranks, int n)
{
    std::unordered_map<int, std::pair<int, float>> temp;
    std::vector<std::pair<int, float>>* res = new std::vector<std::pair<int, float>>[n];
    for (auto& [docID, rank] : ranks)
    {
        temp[docID].first++;
        temp[docID].second+=rank;
    }
    ranks.clear();
    for (auto& [docID, rank_crit] : temp)
    {
        res[n - rank_crit.first].push_back({docID, rank_crit.second});
    }
    for (int i = 0; i < n; i++)
    {
        quickSort(res[i], 0, res[i].size() - 1, getrank);
        for (auto& e : res[i]) ranks.push_back(e);
    }
    delete[] res;
    temp.clear();
}

std::vector<std::pair<int, float>> multi_search(std::string& query)
{
    int wordID = 0, n = 0;
    std::string word = "";
    std::vector<std::pair<int, float>> ranks{}, temp{};
    std::stringstream ss(query);
    while (ss >> word)
    {
        clean_token(word);
        if (word == "") continue;
        if (lexicon.find(word) == lexicon.end()) continue;
        wordID = lexicon[word];
        temp = barrel_inverted_index->get_word(wordID)->ranks;
        for (auto& i : temp) ranks.push_back(i);
        temp.clear();
        n++;
    }
    if (n == 1)
    {
        quickSort(ranks, 0, ranks.size() - 1, getrank);
        return ranks;
    }
    rank(ranks, n);
    return ranks;
}