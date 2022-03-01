#include "wordle.h"

int main() {
    PrefixTreeNode* root = new PrefixTreeNode(-1, '$');

    std::vector<std::string> words{"CHAIR", "STILL", "CHIME", "CREAM", "ULCER", "SHILL", "ULTRA", "APPLE", "HORSE", "SPINS", "CATER"};
    for (std::string word:words) {
        root->addWord(word);
    }

    root->visualizeTree(0);
    return 0;
}

