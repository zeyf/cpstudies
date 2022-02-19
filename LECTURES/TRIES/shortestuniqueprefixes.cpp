#include "tries.h"


void getShortestUniquePrefixes(Trie &t, TrieNode *node, vector<string> &res, string str="")
{
    if (node->getNumWords() == 1)
    {
        res.push_back(str);
        return;
    };

    for (int x = 36; x < 62; x++)
        if (node->getChild(x) != NULL)
            getShortestUniquePrefixes(t, node->getChild(x), res, str + string(1, t.indexToChar(x)));
};

int main()
{
    unsigned int i = 0, n;
    string buffer;
    vector<string> res;
    Trie t = Trie();
    cin >> n;

    while (i < n)
    {
        cin >> buffer;
        t.insert(buffer);
        i++;
    };

    getShortestUniquePrefixes(t, t.search(""), res);

    for (int x = 0; x < res.size(); x++)
        cout << "Word #" << x + 1 << ": " << res[x] << endl;

    return 0;
};