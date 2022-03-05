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

        std::queue<std::pair<std::string, std::string>> permuteChars(std::vector<char>& yellow_chars, std::unordered_map<char, std::vector<int>>& char_allowed_positions, std::string fixed) {
            std::queue<std::pair<std::string, std::string>> ans;

            int total_with_repititions = 1;
            for (auto elem:char_allowed_positions) {
                total_with_repititions *= elem.second.size();
            }

            for (int i = 0; i < total_with_repititions; i++) {
                std::string curr = fixed;
                std::string to_disable = "00000";
                bool is_repeated = false;
                int string_index = i;
                for (int j = 0; j < yellow_chars.size(); j++) {
                    char curr_char = yellow_chars[j];
                    int index = string_index % char_allowed_positions[curr_char].size();
                    int position = char_allowed_positions[curr_char][index];
                    if (curr[position] != '$') {
                        is_repeated = true;
                        break;
                    } else {
                        curr[position] = curr_char;
                        to_disable[position] = '1';
                    }
                    string_index /= char_allowed_positions[curr_char].size();
                }
                if (!is_repeated) {
                    ans.push(std::pair<std::string, std::string>(curr, to_disable));
                }
            }
            return ans;
        }
        
        bool guessWord(std::string& guess) {
            PrefixTreeNode* curr = root;
            bool completed = true;
            int curr_level = -1;
            while (curr_level < 4) {
                PrefixTreeNode* next_node;
                if (!curr->is_enabled()) {
                    return false;
                }
                if (guess[curr_level+1] != '$') {
                    if (curr->get_children().count(guess[curr_level + 1])) {
                        next_node = curr->get_children()[guess[curr_level + 1]];
                        if (!next_node->is_enabled()) {
                            return false;
                        }
                    } else {
                        return false;
                    }
                } else {
                    auto curr_children = curr->get_children();
                    auto it = curr_children.begin();
                    int max_num = 0;

                    while (it != curr_children.end()) {
                        int curr_num = it->second->get_num_words();
                        if (curr_num > max_num && it->second->is_enabled()) {
                            next_node = it->second;
                            max_num = curr_num;
                        }
                        it++;
                    }
                    guess[curr_level+1] = next_node->get_node_char();
                }
                curr_level++;
                curr = next_node;
            }
            return completed;
        }

        bool allCorrect(std::vector<int>& ans) {
            for (int i:ans) {
                if (i != 1) {
                    return false;
                }
            }
            return true;
        }

        std::queue<std::pair<std::string, std::string>> prepareNextWord(std::string guess, std::vector<int> checker_output) {
            std::vector<char> yellow_chars;

            std::string next_guess = "$$$$$";

            for (int i = 0; i < checker_output.size(); i++) {
                if (checker_output[i] == 1) {
                    // on ith level prune all nodes which are not guess[i]
                    root->disableNodeWithoutChar(guess[i], i);
                    if (char_fixed_positions.count(guess[i])) {
                        char_fixed_positions[guess[i]].insert(i);
                    } else {
                        std::unordered_set<int> char_fixed_pos_set;
                        char_fixed_pos_set.insert(i);
                        char_fixed_positions[guess[i]] = char_fixed_pos_set;
                    }
                    fixed_positions.insert(i);
                    next_guess[i] = guess[i];
                } else if (checker_output[i] == -1) {
                    // prune out all nodes in the tree with guess[i] but keep positions which 
                    // have been fixed for guess[i]
                    if (char_fixed_positions.count(guess[i])) {
                        root->disableNodeWithChar(guess[i], char_fixed_positions[guess[i]]);
                    } else {
                        std::unordered_set<int> char_fixed_pos_set;
                        root->disableNodeWithChar(guess[i], char_fixed_pos_set);
                    }
                    if (char_prohibited_positions.count(guess[i])) {
                        char_prohibited_positions[guess[i]].insert(i);
                    } else {
                        std::unordered_set<int> char_prohibited_pos_set;
                        char_prohibited_pos_set.insert(i);
                        char_prohibited_positions[guess[i]] = char_prohibited_pos_set;
                    }
                } else {
                    // collect all the yellow chars
                    yellow_chars.push_back(guess[i]);
                }
            }

            // find the positions possible for the yellow_chars and then find out all the possible string
            // permutations, disable the tree for those strings and figure out a guess
            if (yellow_chars.size()) {
                std::unordered_map<char, std::vector<int>> char_to_allowed_positions;
                for (int i = 0; i < yellow_chars.size(); i++) {
                    char curr = yellow_chars[i];
                    std::vector<int> allowed_positions;
                    for (int i = 0; i < 5; i++) {
                        if (fixed_positions.count(i) == 0 || char_prohibited_positions[curr].count(i) == 0) {
                            allowed_positions.push_back(i);
                        }
                    }
                    char_to_allowed_positions[curr] = allowed_positions;
                }
                return permuteChars(yellow_chars, char_to_allowed_positions, next_guess);
            } else {
                std::queue<std::pair<std::string,std::string>> ans;
                ans.push(std::pair<std::string, std::string>(next_guess, "00000"));
                return ans;
            }
        }

        int playGame(std::string golden) {
            int attempts = 0;
            std::queue<std::pair<std::string, std::string>> try_words;

            // begin with no set character
            try_words.push(std::pair<std::string, std::string>("$$$$$", "00000"));

            while (!try_words.empty()) {
                std::pair<std::string, std::string> guess_disabled = try_words.front();
                std::string guess = guess_disabled.first;
                std::string to_be_disabled = guess_disabled.second;
                for (int i = 0; i < guess.size(); i++) {
                    if (to_be_disabled[i] == '1') {
                        root->disableNodeWithoutChar(guess[i], i);
                    }
                }
                bool allowed = guessWord(guess);
                for (int i = 0; i < guess.size(); i++) {
                    if (to_be_disabled[i] == '1') {
                        root->enableNodeWithoutChar('$', i);
                    }        
                }
                if (allowed) {
                    attempts++;
                    printf("GUESS %d : %s\n", attempts, guess.c_str());
                    root->visualizeTree(0,0);
                    std::vector<int> ans = checker(guess, golden);
                    if (allCorrect(ans)) {
                        break;
                    }
                    try_words = prepareNextWord(guess, ans);
                } else {
                    try_words.pop();
                }
            }

            return attempts;
        }

    private:
        PrefixTreeNode* root;
        std::unordered_map<char, std::unordered_set<int>> char_fixed_positions;
        std::unordered_set<int> fixed_positions;
        std::unordered_map<char, std::unordered_set<int>> char_prohibited_positions;
};