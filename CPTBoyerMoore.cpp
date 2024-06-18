#include <iostream>
using namespace std;

const int NO_OF_CHARS = 256;

// Bad Character Heuristic
void badCharHeuristic(string str, int size, int badchar[NO_OF_CHARS]) {
    for (int i = 0; i < NO_OF_CHARS; i++)
        badchar[i] = -1;

    for (int i = 0; i < size; i++)
        badchar[(int)str[i]] = i;
}

// Preprocessing for the strong good suffix rule
void preprocessStrongSuffix(int* shift, int* bpos, string pat, int m) {
    int i = m, j = m + 1;
    bpos[i] = j;

    while (i > 0) {
        while (j <= m && pat[i - 1] != pat[j - 1]) {
            if (shift[j] == 0)
                shift[j] = j - i;
            j = bpos[j];
        }
        i--;
        j--;
        bpos[i] = j;
    }
}

// Preprocessing for good suffix rule case 2
void preprocessCase2(int* shift, int* bpos, string pat, int m) {
    int j = bpos[0];
    for (int i = 0; i <= m; i++) {
        if (shift[i] == 0)
            shift[i] = j;
        if (i == j)
            j = bpos[j];
    }
}

// Search for pattern in text using the Boyer-Moore algorithm
void BoyerMooreSearch(string text, string pattern) {
    int m = pattern.size();
    int n = text.size();

    int badchar[NO_OF_CHARS];
    int bpos[m + 1];
    int shift[m + 1];

	// Initialize all occurrences of shift to 0
    for (int i = 0; i < m + 1; i++) 
        shift[i] = 0;

    // Preprocess the pattern for bad character heuristic
    badCharHeuristic(pattern, m, badchar);

	// Preprocess the pattern for strong good suffix rule
    preprocessStrongSuffix(shift, bpos, pattern, m);
    
    // Preprocess the pattern for good suffix rule case 2
    preprocessCase2(shift, bpos, pattern, m);

    int s = 0; // s is shift of the pattern with respect to text
    bool found = false; // Flag to check if pattern was found

	// Perform the search
    while (s <= (n - m)) {
        int j = m - 1;

        // Decrease index j while characters of pattern and text match at this shift s
        while (j >= 0 && pattern[j] == text[s + j])
            j--;

        // If the pattern is present at the current shift, print the shift
        if (j < 0) {
            cout << "Pattern occurs at shift = " << s << endl;
            found = true; // Pattern found

            // Shift the pattern so that the next character in text aligns with the last occurrence of it in pattern
            s += shift[0];
        } else {
            
            // Compute the shift for the bad character heuristic
        	int badCharShift = j - badchar[(int)text[s + j]];
        
			// Compute the shift for the good suffix rule
        	int goodSuffixShift = shift[j + 1];

        	// Use the maximum of the two shifts to ensure the pattern moves to the right position
        	s += max(goodSuffixShift, badCharShift);
        	
        }
        
    }

    if (!found) {
        cout << "Pattern not found" << endl;
    }
}

// Driver code
int main() {
    string text;
    string pattern;
    cout << "Key in the Text: "; 
    cin >> text;
    cout << "Key in the Pattern: ";
    cin >> pattern;
    BoyerMooreSearch(text, pattern);
    return 0;
}

