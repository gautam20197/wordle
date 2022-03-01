#include "wordle.h"

class Guesser {
    public:
        Guesser(PrefixTreeNode* root): root(root) {}

        std::vector<int> checker(std::string guess, std::string golden) {
            std::vector<int> ans;
            for (int i = 0; i < golden.length(); i++) {
                ans.push_back(-1);
            }

            std::unordered_map<char, int> freq;
            for (int i = 0; i < golden.length(); i++) {
                if (golden[i] == guess[i]) {
                    ans[i] = 1;
                } else {
                    freq[golden[i]]++;
                }
            }

            for (int i = 0; i < guess.length(); i++) {
                if (ans[i] != 1) {
                    if (freq[guess[i]] > 0) {
                        ans[i] = 0;
                        freq[guess[i]]--;
                    }
                }
            }
            return ans;
        }
        
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