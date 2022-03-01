#include "wordle.h"

class Guesser {
    public:
        Guesser(PrefixTreeNode* root): root(root) {}
        
        std::string guessWord() {
            std::string guess = "";
            PrefixTreeNode* curr = root;
            while (curr->get_level() < 4) {
                int curr_level = curr->get_level();
                if (curr_level != -1) {
                    guess += curr->get_node_char();
                }

                auto curr_children = curr->get_children();
                auto it = curr_children.begin();
                int max_num = 0;

                while (it != curr_children.end()) {
                    int curr_num = it->second->get_num_words();
                    if (curr_num > max_num) {
                        curr = it->second;
                        max_num = curr_num;
                    }
                    it++;
                }
            }
            guess += curr->get_node_char();
            return guess;
        }
    private:
        PrefixTreeNode* root;
};