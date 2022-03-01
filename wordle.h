#include <iostream>
#include <vector>
#include <unordered_map>

class PrefixTreeNode {
    public:
        PrefixTreeNode(int level, char node_char): level(level), node_char(node_char), num_words(0) {}

        int get_num_words() {
            return num_words;
        }

        char get_node_char() {
            return node_char;
        }

        void addWord(std::string word) {
            num_words++;

            if (level == 4) {
                return;
            }

            char curr_char = word[level + 1];
            if (children.count(curr_char)) {
                children[curr_char]->addWord(word);
            } else {
                PrefixTreeNode* child = new PrefixTreeNode(level+1, curr_char);
                children[curr_char] = child;
                child->addWord(word); 
            }
        }

        void visualizeTree(int spaces) {
            std::cout << std::string( spaces, '-' );
            printf("Node (%c, %d)\n", node_char, num_words);
            auto it = children.begin();
            while (it != children.end()) {
                it->second->visualizeTree(spaces + 2);
                it++;
            }
        }

        ~PrefixTreeNode() {
            auto it = children.begin();
            while (it != children.end()) {
                it->second->~PrefixTreeNode();
            }
            delete(this);
        }


    private:
        char node_char;
        int num_words;
        std::unordered_map<char, PrefixTreeNode*> children;
        int level;
};