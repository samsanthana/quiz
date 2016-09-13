/**
 *
 *      "Usage: <prog_name> <input word file>" << endl;
 */
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <fstream>
#include <cstring>
using namespace std;

typedef vector<string> wordlist_t;

class Node {

public:

    Node() { letter = ' '; isLeaf = false; }
    ~Node () {}
    char Letter() { return letter; }
    void setLetter (char c) { letter = c; }
    bool checkIsLeaf() { return isLeaf; }
    bool setisLeaf() { isLeaf = true; }
    void addChild(Node *child) { children.push_back(child); }
    Node *findNode (char c);
    vector<Node *> getchildren() { return children; }

private:
    char letter;
    bool isLeaf;
    vector <Node *> children;
};


class Rtree {

public:

    Rtree();
    ~Rtree();
    void InsertWord (string s);
    bool isExist (string s);
    vector<string> getAllPrefixes(string s);

private:
    Node *root;
};

Rtree::Rtree() 
{
    root = new Node();        
}


Rtree::~Rtree()
{
   
}

Node *Node::findNode (char c) 
{   
    
    for (vector<Node *>::iterator it = children.begin(); it != children.end(); ++it)
    {   
        if ((*it)->Letter() == c)
        {
            return *it;
        }

    }
    return NULL;
}

void Rtree::InsertWord(string word)
{
    Node *curr = root;
    int cnt = 0; 
    
    for (cnt = 0; cnt < word.length(); cnt++) {
        Node *child = curr->findNode(word[cnt]);
        if (NULL != child) {
            curr = child;
        } else {
            Node *tempNode = new Node();
            tempNode->setLetter(word[cnt]);
            curr->addChild(tempNode);
            curr = tempNode;
        }
    }
   
    if (cnt == word.length()){
        curr->setisLeaf();
    }

}

bool Rtree::isExist(string word)
{
    Node *curr = root;
    
    while (NULL != curr) {
        for (int cnt = 0; cnt < word.length(); cnt++) {
            Node *tmpNode = curr->findNode(word[cnt]);
            if (NULL == tmpNode) {
                return false;
            }
            curr = tmpNode;
        }
        return curr->checkIsLeaf();
    }
}

vector <string> Rtree::getAllPrefixes(string word) {
   int len = 0;
   int maxLen = 0;
   static int wcnt = 0;
   vector<string> prefixes;
   Node *curr = root;
 
   for (int cnt = 0; cnt < word.length(); cnt++) {
      Node *tmpNode = curr->findNode(word[cnt]);
      if (NULL == tmpNode) {
          return prefixes;
      }
      curr = tmpNode;
      len++;

      if (curr->checkIsLeaf()) {
          prefixes.push_back(word.substr(0,len));
          maxLen = len;
      }   
   }
   return prefixes; 
}

static string FindLongestCompound(wordlist_t wordlist) {
    Rtree *rtree = new Rtree();
    vector <string> vword;
    vector <string>().swap(vword);
    vector < vector<string> > vqueue;
    vector < vector<string> >().swap(vqueue);
    vector <string> prefixes;
    vector <string> qprefixes;

    vqueue.reserve(1000000);

    for(wordlist_t::iterator it = wordlist.begin(); it != wordlist.end(); ++it) 
    {
        rtree->InsertWord (it->c_str());
    }

    int cnt = 0; 
    for(wordlist_t::iterator it = wordlist.begin(); it != wordlist.end(); ++it) 
    {
        string word = it->c_str();
       
        prefixes = rtree->getAllPrefixes(word);


        for (vector<string>::iterator pit = prefixes.begin(); pit != prefixes.end(); ++pit){
            string prefix = pit->c_str();
           
            vword.clear();
            vword.push_back(word);
            vword.push_back(word.substr(prefix.size(), word.size()));
            vqueue.push_back(vword);
        }
    }
   
    string longest = "";
    for(int qcnt = 0; qcnt < vqueue.size(); qcnt++) {
        string qword = vqueue[qcnt][0]; 
        string qsuffix = vqueue[qcnt][1];
       
        if (rtree->isExist(qsuffix) && (longest.size() < qword.size())) { 
            longest = qword;
            vqueue.pop_back();             
        } else {
            qprefixes = rtree->getAllPrefixes(qsuffix);
            if (qprefixes.empty()) {
                continue;  
            }
            for (vector<string>::iterator pit = qprefixes.begin(); pit != qprefixes.end(); ++pit){
                string qprefix = pit->c_str();
                
                vword.push_back(qword);
                vword.push_back(qword.substr(qprefix.size(), qword.size()));
                vqueue.push_back(vword);
            }
        }
    }
    return longest;
}

int main(int argc, char **argv)
{
   wordlist_t wordlist;
   string line;

   if (argc != 2 || argv[2]) {
       cout << "Usage: <prog_name> <input word file>" << endl;
       return 0;
   }

   ifstream infile(argv[1]);

   if(!infile) {
       cout << "Error opening file" << endl;
       return 0;
   }

   while (getline(infile, line)) 
   {   
       wordlist.push_back(line);
   }   
   
   cout << "longest word:" << FindLongestCompound(wordlist) << endl; 
   
   return 0;
}
