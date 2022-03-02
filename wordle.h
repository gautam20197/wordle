#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <fstream>

class PrefixTreeNode {
    public:
        PrefixTreeNode(int level, char node_char, PrefixTreeNode* parent): level(level), node_char(node_char), num_words(0), 
        enabled(true), parent(parent) {}

        int get_num_words() {
            return num_words;
        }

        void set_num_words(int n) {
            num_words = n;
        }

        char get_node_char() {
            return node_char;
        }

        int get_level() {
            return level;
        }

        std::unordered_map<char, PrefixTreeNode*> get_children() {
            return children;
        }

        bool is_enabled() {
            return enabled;
        }

        void decrement_parent_words(int pruned_words) {
            num_words -= pruned_words;
            if (num_words == 0) {
                enabled = false;
            }
            if (parent) {
                parent->decrement_parent_words(pruned_words);
            }
        }

        void enable() {
            enabled = true;
        }

        void disableNode() {
            if (enabled) {
                enabled = false;
                auto it = children.begin();

                while (it != children.end()) {
                    it->second->disableNode();
                    it++;
                }

                if (parent && parent->is_enabled()) {
                    decrement_parent_words(num_words);
                }

                num_words = 0;
            }
        }

        void enableNode() {
            enable();
            auto it = children.begin();

            while (it != children.end()) {
                it->second->enableNode();
                it++;
            }
            if (level == 4) {
                num_words = 1;
            }
            parent->set_num_words(parent->get_num_words() + num_words);
        }

        void disableNodeWithChar(char avoid_char) {
            if (enabled) {
                if (node_char == avoid_char) {
                    this->disableNode();
                }   else {
                    auto it = children.begin();
                    while (it != children.end()) {
                        it->second->disableNodeWithChar(avoid_char);
                        it++;
                    }
                }
            }
        }

        void disableNodeWithoutChar(char golden_char, int golden_char_level) {
            if (enabled) {
                if (level < golden_char_level) {
                    auto it = children.begin();
                    while (it != children.end()) {
                        it->second->disableNodeWithoutChar(golden_char, golden_char_level);
                        it++;
                    }
                } else if (level == golden_char_level) {
                    if (node_char != golden_char) {
                        disableNode();
                    }
                } else {
                    return;
                }
            }
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
                PrefixTreeNode* child = new PrefixTreeNode(level+1, curr_char, this);
                children[curr_char] = child;
                child->addWord(word); 
            }
        }

        void visualizeTree(int spaces, int levels) {
            if (enabled) {
                std::cout << std::string( spaces, '-' );
                printf("Node enabled : %d, (%c, %d)\n", enabled, node_char, num_words);
                auto it = children.begin();
                while (it != children.end()) {
                    if (it->second->get_level() <= levels) {
                        it->second->visualizeTree(spaces + 2, levels);
                    }
                    it++;
                }
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
        bool enabled;
        PrefixTreeNode* parent;
};