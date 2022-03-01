#include "wordle.h"

int main() {
    PrefixTreeNode* root = new PrefixTreeNode(-1, '$');

    // std::vector<std::string> words{"CHAIR", "STILL", "CHIME", "CREAM", "ULCER", "SHILL", "ULTRA", "APPLE", "HORSE", "SPINS", "CATER"};
    // for (std::string word:words) {
    //     root->addWord(word);
    // }

    std::ifstream file("allowed_words.txt");
    std::string word;
    while (std::getline(file, word))
    {
        root->addWord(word);
    }

    // will visualize only till the root and children
    root->visualizeTree(0, true);
    return 0;
}

