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

        std::vector<std::string> permuteChars(std::vector<char>& yellow_chars, std::unordered_map<char, std::vector<int>>& char_allowed_positions) {
            std::vector<std::string> ans;
            std::unordered_set<int> fixed_positions;

            int total_with_repititions = 1;
            for (auto elem:char_allowed_positions) {
                total_with_repititions *= elem.second.size();
            }

            for (int i = 0; i < total_with_repititions; i++) {
                fixed_positions.clear();
                std::string curr = "$$$$$";
                bool is_repeated = false;
                int string_index = i;
                for (int j = 0; j < yellow_chars.size(); j++) {
                    char curr_char = yellow_chars[j];
                    int index = string_index % char_allowed_positions[curr_char].size();
                    int position = char_allowed_positions[curr_char][index];
                    if (fixed_positions.count(position)) {
                        is_repeated = true;
                        break;
                    } else {
                        fixed_positions.insert(position);
                        curr[position] = curr_char;
                    }
                    string_index /= char_allowed_positions[curr_char].size();
                }
                if (!is_repeated) {
                    ans.push_back(curr);
                }
            }
            return ans;
        }
        
        std::string guessWord() {
            std::string guess = "";
            PrefixTreeNode* curr = root;
            int curr_level = -1;
            while (curr_level < 5) {
                if (curr->get_level() != -1) {
                    guess += curr->get_node_char();
                }

                auto curr_children = curr->get_children();
                auto it = curr_children.begin();
                int max_num = 0;

                while (it != curr_children.end()) {
                    int curr_num = it->second->get_num_words();
                    if (curr_num > max_num && it->second->is_enabled()) {
                        curr = it->second;
                        max_num = curr_num;
                    }
                    it++;
                }
                curr_level++;
            }
            return guess;
        }

        bool allCorrect(std::vector<int>& ans) {
            for (int i:ans) {
                if (i != 1) {
                    return false;
                }
            }
            return true;
        }

        int playGame(std::string golden) {
            std::vector<int> ans;
            for (int i = 0; i < golden.length(); i++) {
                ans.push_back(-1);
            }

            int attempts = 0;

            while (!allCorrect(ans) && attempts < 5) {
                attempts++;
                std::string guess = guessWord();
                printf("GUESS %d : %s\n", attempts, guess.c_str());
                ans = checker(guess, golden);

                if(allCorrect(ans)) {
                    return attempts;
                }

                for (int i = 0; i < ans.size(); i++) {
                    if (ans[i] == 1) {
                        root->disableNodeWithoutChar(guess[i], i);
                        if (char_fixed_positions.count(guess[i])) {
                            char_fixed_positions[guess[i]].insert(i);
                        } else {
                            std::unordered_set<int> char_fixed_pos_set;
                            char_fixed_pos_set.insert(i);
                            char_fixed_positions[guess[i]] = char_fixed_pos_set;
                        }
                    } else if (ans[i] == -1) {
                        if (char_fixed_positions.count(guess[i])) {
                            root->disableNodeWithChar(guess[i], char_fixed_positions[guess[i]]);
                        } else {
                            std::unordered_set<int> char_fixed_pos_set;
                            root->disableNodeWithChar(guess[i], char_fixed_pos_set);
                        }
                        
                    }
                }
                root->visualizeTree(0, 0);
            }
            return attempts;  
        }

    private:
        PrefixTreeNode* root;
        std::unordered_map<char, std::unordered_set<int>> char_fixed_positions;
};