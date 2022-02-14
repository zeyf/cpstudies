#include <iostream>
#include <vector>
#include <string>

using namespace std;

class TrieNode
{
    public:
        TrieNode()
        {
            children = vector<TrieNode *> (62, NULL);
            numWords = 0;
            wordFlag = false;
        };

        TrieNode *getChild(unsigned int index)
        {
            return children[index];
        };

        void addChild(unsigned int index)
        {
            children[index] = new TrieNode();
        };

        unsigned int getNumWords()
        {
            return numWords;
        };

        bool isWord()
        {
            return wordFlag;
        };

        void incrementNumWords()
        {
            numWords++;
        };

        void decrementNumWords()
        {
            numWords--;
        };

        void setWord(bool status)
        {
            wordFlag = status;
        };

    private:
        vector<TrieNode*> children;
        unsigned int numWords;
        bool wordFlag;
};

class Trie
{
    public:
        Trie ()
        {
            root = new TrieNode();
        };

        TrieNode *search(string term)
        {
            unsigned int k = 0;
            TrieNode *c = root;
            while (c != NULL && k < term.size())
            {
                c = c->getChild(charToIndex(term[k]));
                k++;
            };

            return c;
        };

        bool insert(string term)
        {
            TrieNode *searchResult = search(term);
            if (searchResult != NULL && searchResult->isWord())
                return false;
            
            unsigned int k = 0;
            TrieNode *c = root;
            while (k  < term.size())
            {
                unsigned int index = charToIndex(term[k]);
                TrieNode *next = c->getChild(index);
                if (next == NULL)
                {
                    c->addChild(index);
                };

                c->incrementNumWords();
                c = c->getChild(index);
                k++;
            };

            c->setWord(true);
            c->incrementNumWords();

            return true;
        };


        void printWords(TrieNode *node, bool reverse=false, string str="")
        {
            if (!reverse && node->isWord())
                cout << str << '\n';

            for (int x = (!reverse ? 0 : 61); (!reverse ? (x < 62) : (x >= 0)) ; (!reverse ? (x++) : (x--)))
                if (node->getChild(x) != NULL)
                {
                    printWords(node->getChild(x), reverse, str + string(1, indexToChar(x)));
                };

            if (reverse && node->isWord())
                cout << str << '\n';
        };

        

    private:
        TrieNode *root;

        bool isUpperCase(char c)
        {
            return c >= 'A' && c <= 'Z';
        };

        bool isLowerCase(char c)
        {
            return c >= 'a' && c <= 'z';
        };

        bool isNumber(char c)
        {
            return c >= '0' && c <= '9';
        };

        unsigned int charToIndex(char c)
        {
            if (isLowerCase(c))
                return (c - 'a');
            else if (isUpperCase(c))
                return 26 + (c - 'A');
            else if (isNumber(c))
                return 52 + (c - '0');
        };

        char indexToChar(unsigned int index)
        {
            if (index >= 0 && index <= 25)
                return 'a' + index;
            else if (index >= 26 && index <= 51)
                return 'A' + (index - 26);
            else if (index >= 52 && index <= 61)
                return '0' + (index - 52);
        };

};

