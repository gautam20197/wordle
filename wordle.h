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

        void increment_parent_words(int added_words) {
            num_words += added_words;
            if (num_words > 0) {
                enabled = true;
            }
            if (parent) {
                parent->increment_parent_words(added_words);
            }
        }

        void disableChildren() {
            enabled = false;
            auto it = children.begin();

            while (it != children.end()) {
                it->second->disableChildren();
                it++;
            }
            
            num_words = 0;
        }

        int enableChildren() {
            enabled = true;

            int enabled_words = 0;
            auto it = children.begin();

            while (it != children.end()) {
                enabled_words += it->second->enableChildren();
                it++;
            }

            if (level == 4) {
                enabled_words = 1;
            }

            num_words = enabled_words;

            return num_words;
        }

        void disableNode() {
            if (enabled) {
                int words = num_words;
                disableChildren();

                if (parent && parent->is_enabled()) {
                    parent->decrement_parent_words(words);
                }
            }
        }

        void enableNode() {
            if (!enabled) {
                enableChildren();

                if (parent) {
                    parent->increment_parent_words(num_words);
                }
            }
        }

        void disableNodeWithChar(char avoid_char, std::unordered_set<int>& levels_not_disable) {
            if (enabled) {
                if (node_char == avoid_char && levels_not_disable.count(level) == 0) {
                    this->disableNode();
                }   else {
                    auto it = children.begin();
                    while (it != children.end()) {
                        it->second->disableNodeWithChar(avoid_char, levels_not_disable);
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