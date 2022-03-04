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
    root->get_children()['S']->disableNode();
    root->visualizeTree(0, true);

    root->get_children()['S']->enableNode();
    root->visualizeTree(0, true);
}

int main() {
    PrefixTreeNode* root = new PrefixTreeNode(-1, '$', nullptr);

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
    root->visualizeTree(0, 0);

    // find the first word to guess
    Guesser* AI = new Guesser(root);
    
    int attempts = AI->playGame("horse");
    printf("ATTEMPTS : %d\n", attempts);
    
    return 0;
}

