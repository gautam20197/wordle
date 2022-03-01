#include "guesser.h"

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

    // find the first word to guess
    Guesser* AI = new Guesser(root);
    std::string first_guess = AI->guessWord();
    printf("First guess: %s\n", first_guess.c_str());

    // testing the checker
    std::string guess = "hassi";
    std::string golden = "hsrde";
    std::vector<int> ans = AI->checker(guess, golden);

    for (int i = 0; i < ans.size(); i++) {
        printf("%d ", ans[i]);
    }
    printf("\n");
    return 0;
}

