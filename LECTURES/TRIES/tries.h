#include <iostream>
#include <vector>
#include <string>

using namespace std;

// class for the TrieNode's in the Trie.
class TrieNode
{
  public:

    // initialize the trie with 62 children to NULL, numWords to 0, and wordFlag to false.
    // 62 children maps for: 0-9 for digit characters, 10-35 for uppercase letters, 36-61 for lowercase letters
    TrieNode()
    {
      children = vector<TrieNode*> (62, NULL);
      numWords = 0;
      wordFlag = false;
    };

    // increment the number of words in a subtree at specific node.
    void incrementNumWords()
    {
      numWords++;
    };

    // decrement the number of words in a subtree at specific node.
    void decrementNumWords()
    {
      numWords--;
    };

    // toggle the status of any given node, which is representative of a given word existing.
    void toggleWord()
    {
      wordFlag = !wordFlag;
    };

    // get a pointer to a specific child based on an index.
    TrieNode *getChild(unsigned int index)
    {
      return children[index];
    };

    // add a child to an index. this has to be this way, else setting NULL = to an address does nothing.
    // the dereferencing of the array index is key. this properly creates a node at a given index.
    void addChild(unsigned int index)
    {
      children[index] = new TrieNode();
    };

    void deleteChild(unsigned int index)
    {
      delete children[index];
      children[index] = NULL;
    };

    // returns the status of a node and if it is a word.
    bool isWord()
    {
      return wordFlag;
    };

    // returns the number of words in a given subtree.
    unsigned int getNumWords()
    {
      return numWords;
    };

  private:
    vector<TrieNode*> children;
    unsigned int numWords;
    bool wordFlag;

};

// class for the trie with many conveniences that trie often need.
class Trie
{
  public:
    // initializes the very root to be non-NULL.
    Trie()
    {
      root = new TrieNode();
    };

    // searches for a given word/term in a trie.
    TrieNode *search(string term)
    {
      // iteration starting at 0 for the indexing of the word and a sliding pointer starting from root.
      unsigned int k = 0;
      TrieNode *c = root;

      // iterate through the trie in the direction of the word.
      // if c == NULL, stop. that means that the word is not in the dictionary.
      // more specifically, the trie has not even extended as deep as the length of that word
      // in that given sequence for a particular word.
      // if the path does exist to that word, the terminating condition will be the second.
      // this would mean that you would stop at the node that is the "location" for a given word/term.
      while (c != NULL && k < term.size())
      {
        c = c->getChild(charToIndex(term[k]));
        k++;
      };

      // this allows access to the data of a given node: like numWords, isWord, and so on.
      return c;
    };

    // searches inserts a particular word.
    bool insert(string term)
    {
      // searches for the term first.
      // if this term is found as a node (non-NULL) and is marked as a word, return false. no need to insert. 
      TrieNode *searchResult = search(term);
      if (searchResult != NULL && searchResult->isWord())
        return false;
      
      // k and c to start iterating down the path for the given word/term.
      unsigned int k = 0;
      TrieNode *c = root;
      while (k < term.size())
      {
        // gets the particular index for a alphanumeric character in a string.
        unsigned int childIndex = charToIndex(term[k]);
        // if the child at the childIndex for the character is null, create/add the child at the index.
        if (c->getChild(childIndex) == NULL)
          c->addChild(childIndex);
        
        // since the root is meant to not count as a word, check a node is not the root.
        // if it is not the root, we should increment the word at a node as a new word will be added to its subtree.
        if (c != root)
            c->incrementNumWords();

        // move the sliding pointer down to the given children based on the character's child index.
        c = c->getChild(childIndex);
        k++;
      };

      // at this point, c is sitting at the location of the given word inserted.
      // since we are inclusive of a node itself in its subtree in respect to numwords,
      // increment this node's numWords. also toggle the status of the word.
      // by default the isWord flag of a given word is meant to be false, so toggling it will switch to it's inverse; true.
      // return true. we have successfully incremented the word.
      c->incrementNumWords();
      c->toggleWord();
      return true;
    };

    // gets all words in the trie and sorts to an array if needed, and in a given order(ascending or descending)
    // the order depends on the bool value of the reverse variable.
    unsigned int getWords(TrieNode *node, vector<string> &words, bool reverse=false,  string str="")
    {
      // stores if a location holds a word.
      unsigned int res = node->isWord() ? 1 : 0;

      // if reverse is false and node is a word, pushback now. this is done preorder for ascending order.
      if (!reverse && node->isWord())
        words.push_back(str);

      // a very custom for loop that has a binary starting point, condition, and direction of variable switching.
      // this is to handle for the differences that arise when sorting in ascending vs descending order.
      // if a node at an index is non NULL, traverse down the path.
      for (int x = (!reverse ? 0 : 61); (!reverse ? (x < 62) : (x >= 0)); (!reverse ? (x++) : (x--)))
        if (node->getChild(x) != NULL)
          res += getWords(node->getChild(x), words, reverse, str + string(1, indexToChar(x)));

      // if reverse is true and a node is a word, pushback now. this is done postorder for descending order.
      if (reverse && node->isWord())
        words.push_back(str);
      
      return res;
    };

    // returns the count of words starting with a given word/term by utilizing search for node data (numWords)
    unsigned int countWordsStartingWith(string term)
    {
      TrieNode *searchResult = search(term);
      return (searchResult != NULL) ? searchResult->getNumWords() : 0;
    };

    // gets all words in the trie with a given substring. superior than the getWordsWithCharacter function. does that. one size strings do the same.
    unsigned int getWordsWithSubstring(TrieNode *node, string substring, vector<string> &words, bool ancestralSubstringFound=false, string str="", unsigned int k=0)
    {
      // store the count of words with the given substring.
      unsigned int res = 0;

      if (ancestralSubstringFound || k == substring.size())
      {
        // if not a word, do not push the string nor count.
        if (node->isWord())
        {        
          words.push_back(str);
          res++;
        }

        // set the flag at the current recursive call to true, call-wide, to be able to handle all subtree calls the same. all will be preserved and stored.
        ancestralSubstringFound = true;
      };

      // iterate through all possible children, go down if not null.
      for (int x = 0; x < 62; x++)
        if (node->getChild(x) != NULL)
        {
          // if the substring has been found previously, do not do any character comparisons. we dont care
          if (ancestralSubstringFound)
            res += getWordsWithSubstring(node->getChild(x), substring, words, ancestralSubstringFound, str + string(1, indexToChar(x)), k);
          else
          {
            // ancestralSubstring has not been found in this else

            // if the index that will be traversed down do matches the current kth letter of the substring, recurse and increment k for next letter.
            // else if not matching, set k back to 0, start back down at the first letter in the substring for deeper recursive calls to build up the word as it goes down.
            if (x == charToIndex(substring[k]))
              res += getWordsWithSubstring(node->getChild(x), substring, words, ancestralSubstringFound, str + string(1, indexToChar(x)), k + 1);
            else
              res += getWordsWithSubstring(node->getChild(x), substring, words, ancestralSubstringFound, str + string(1, indexToChar(x)), 0);
          };
        };

      return res;
    };

  // handles all 4 cases of deletion of a given string
  bool del(string term)
  {
    TrieNode *searchResult = search(term);
    // case #1: path to word does not exist.
    // case #2: path to word exists, but is not a word.
    // case #3/4: path to word exists, is a word, and has at least one word in subtree.
    if (searchResult == NULL)
      return false;
    else if (!searchResult->isWord())
      return false;
    else if (searchResult->getNumWords() >= 1)
      deleter(root, term);

    return true;
  };

  // handles case #3 and #4 on deletion of words in the trie
  // case #3: no words in subtree of a word after deletion
  // case #4: has words in subtree of a word after deletion
  int deleter(TrieNode *node, string term, unsigned int k=0)
  {
    // if not the root, decrement the number of words at a given node on the way down
    if (node != root)
      node->decrementNumWords();

    // if at the node of the word itself
    if (k == term.size())
    {
      // toggle the word's flag to off (would be on)
      node->toggleWord();
      // case #4: word has words in subtree of word aside from word itself, return -1. delete
      // else return number of words and delete until mismatch of numwords at a node with subresults
      // in this else case, this means a node with other children down other paths have children
      // so do not delete this node and stop all deletion actions with -1.
      if (node->getNumWords() > 0)
        return -1;
      else
        return node->getNumWords();
    };

    // gets count / subtree subresult.
    int wordFurtherSubtreeWordCount = deleter(node->getChild(charToIndex(term[k])), term, k + 1);
    // if at a termination of action case, do nothing and return back up the tree.
    if (wordFurtherSubtreeWordCount == -1)
      return -1;

    // since a valid node deletion case, delete the child of the word while ascending up the call stack postorder.
    node->deleteChild(charToIndex(term[k]));

    // once the first subtree numwords mismatch is found, stop and terminate any and all deletion by returning -1.
    if (node->getNumWords() != wordFurtherSubtreeWordCount)
      return -1;

    // return subresult. propagate a given action back up the tree.
    return wordFurtherSubtreeWordCount;
  };

  // takes in positive integers and converts to string for input.
  string intToString(unsigned int n)
  {
    string res = "";
    int x = n;
    while (x > 0)
    {
      res = string(1, '0' + (x % 10)) + res;
      x /= 10;
    };

    return res;
  };

  // takes in a string of a number and converts to int.
  int stringToInt(string number)
  {
    unsigned int res = 0, m = 1;
    for (int x = number.size() - 1; x >= 0; x--)
    {
      res += m * (number[x] - '0');
      m *= 10;
    };

    return res;
  };

    // calculate the proper character for a given base shifted index.
    // in ascii, numbers come first as [48, 57], then uppercase as [65, 90], and finally lowercase as [97, 122].
    unsigned int indexToChar(unsigned int index)
    {
      if (index >= 0 && index < 10)
        return '0' + index;
      else if (index >= 10 && index < 36)
        return 'A' + (index - 10);
      else if (index >= 36 && index < 62)
        return 'a' + (index - 36);
    };

  private:
    TrieNode *root;

    // return bool of whether a character is uppercase or not
    bool isUpperCase(char c)
    {
      return (c >= 'A' && c <= 'Z');
    };

    // return bool of whether a character is lowercase or not
    bool isLowerCase(char c)
    {
      return (c >= 'a' && c <= 'z');
    };

    // return bool of whether a character is a digit or not
    bool isDigit(char c)
    {
      return (c >= '0' && c <= '9');
    };

    // calculate the proper index base shift for a given character.
    // in ascii, numbers come first as [48, 57], then uppercase as [65, 90], and finally lowercase as [97, 122].
    unsigned int charToIndex(char c)
    {
      if (isDigit(c))
        return c - '0';
      else if (isUpperCase(c))
        return 10 + (c - 'A');
      else if (isLowerCase(c))
        return 36 + (c - 'a');
    };

};