#include "guesser.h"

void test_checker(PrefixTreeNode* root) {
    Guesser* AI = new Guesser(root);
    std::string guess = "hassi";
    std::string golden = "hsrde";
    std::vector<int> ans = AI->checker(guess, golden);

    for (int i = 0; i < ans.size(); i++) {
        printf("%d ", ans[i]);
    }
    printf("\n");
}

void test_disable_node(PrefixTreeNode* root) {
    // testing the disableNode and enableNode
    printf("disabling\n");
    root->get_children()['S']->disableNode();
    root->visualizeTree(0, 4);

    printf("enabling\n");
    root->get_children()['S']->enableNode();
    root->visualizeTree(0, 4);
}

void test_permute_chars(Guesser* AI) {
    std::vector<char> yellow_chars{'a', 'c', 'e'};
    std::unordered_map<char, std::vector<int>> char_allowed_positions;
    char_allowed_positions['a'] = std::vector<int>{0,1};
    char_allowed_positions['c'] = std::vector<int>{2,1};
    char_allowed_positions['e'] = std::vector<int>{3,1,4,0};
    std::queue<std::pair<std::string,std::string>> ans = AI->permuteChars(yellow_chars, char_allowed_positions, "$$$$$");
    while(!ans.empty()) {
        printf("%s\n", ans.front().first.c_str());
        ans.pop();
    }
}

int main() {
    PrefixTreeNode* root = new PrefixTreeNode(-1, '$', nullptr);

    std::vector<std::string> words{"chair", "still", "chime", "cream", "ulcer", "shill", "ultra", "apple", "horse", "spins", "cater"};
    for (std::string word:words) {
        root->addWord(word);
    }

    // std::ifstream file("allowed_words.txt");
    // std::string word;
    // while (std::getline(file, word))
    // {
    //     root->addWord(word);
    // }

    // // will visualize only till the root and children
    // root->visualizeTree(0, 0);

    // find the first word to guess
    Guesser* AI = new Guesser(root);
    
    int attempts = AI->playGame("ulcer");
    printf("ATTEMPTS : %d\n", attempts);
    
    return 0;
}

