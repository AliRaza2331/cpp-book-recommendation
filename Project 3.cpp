#include <iostream>
#include <string>
#include <algorithm> // For transform function
#include <cctype>    // For lower/upper Case inputs
#include <limits> // for numeric limits

using namespace std;

// Function to convert all string to lowercase
string toLowerCase(const string& str) { // manages case- insensitive input
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), // convert all in lowercase 
              [](unsigned char c) { return tolower(c); });
    return lowerStr;
}

// Binary tree node for a library of Books
struct BookNode {
    string title;
    string author;
    string genre;
    BookNode* left;
    BookNode* right;

    BookNode(string t, string a, string g)
        : title(t), author(a), genre(g), left(nullptr), right(nullptr) {}
};

// Class for managing the Book library
class Library {
private:
    BookNode* root; // root node for all functions

    // insert a new node to tree with attributes(title, author, genre)
    bool insertNode(BookNode*& node, string title, string author, string genre){
        if (node == nullptr) { // make sure to insert node at an empty spot
            node = new BookNode(title, author, genre);
            return true;
        }

        // prevents duplicates
        if (toLowerCase(title) == toLowerCase(node->title)) { // case sensitive
            cout << "Error: A book with the title \"" << title << "\" already exists in the library." << endl;
            return false;
    }
        // compares the new node in lexicogrphical order
        if (toLowerCase(title) < toLowerCase(node->title)) {
            return insertNode(node->left, title, author, genre); // to left if smaller
        } else {
            return insertNode(node->right, title, author, genre); // to right if greater
        }
    }
    
    // searches node by title name. note the tree nodes are sorted by the title
    BookNode* searchByTitle(BookNode* node, string title) {
        // searches for node till it reaches the end of tree
        if (node == nullptr || toLowerCase(node->title) == toLowerCase(title)) {
            return node;
        }
        // if not found in base tree, continues search in the appropriate subtree
        if (toLowerCase(title) < toLowerCase(node->title)) {
            return searchByTitle(node->left, title);
        } else {
            return searchByTitle(node->right, title);
        }
    }
    
    // traverses all nodes by author name 
    void searchByAuthor(BookNode* node, string author, bool& found) {
        if (node == nullptr) return; // stops recursion if endpoint reached of a branch
        // prints if author found
        if (toLowerCase(node->author) == toLowerCase(author)) {
            cout << "Found: \"" << node->title << "\" by " << node->author << endl;
            found = true; 
        }
        // recursively traverses through both left and right nodes as authors are not sorted unlike the titles. This also ensures all books from the searched author are found
        searchByAuthor(node->left, author, found);
        searchByAuthor(node->right, author, found);
    }
    
    // traverses the tree looking for books that match the searched genre
    void recommendByGenre(BookNode* node, string genre) {
        if (node == nullptr) return; // stops recursion if endpoint reached of a branch for that branch
        // prints books that match the genre
        if (toLowerCase(node->genre) == toLowerCase(genre)) {
            cout << "- \"" << node->title << "\" by " << node->author << endl;
        }
        // similar to searchByAuthor, this traverses the entire tree
        recommendByGenre(node->left, genre);
        recommendByGenre(node->right, genre);
    }
    
    // displays all books in library in a lexicographical order
    void displayAllBooks(BookNode* node) {
        if (node == nullptr) return; // stops recursion at the end of each branch
        
        // displays all books on the left side in a in-order traversal 
        displayAllBooks(node->left);
        cout << "- \"" << node->title << "\" by " << node->author << " (Genre: " << node->genre << ")" << endl;
        // ensure greater titles on right are visited after the left
        displayAllBooks(node->right);
    }
    
    // prevents memory leaks by deleting tree nodes when programs ends
    void deleteTree(BookNode* node) {
        if (node == nullptr) return;
        deleteTree(node->left); // delete left nodes and root node
        deleteTree(node->right); // delete right nodes
        delete node;
    }
    
    public:
    
    // constructor to initialise the Library
    Library() : root(nullptr) {}

    // destructor
    ~Library() {
        deleteTree(root);
    }
    
    // function to add books through user input with attribites
    void addBook(string title, string author, string genre) {

        // checks for empty attribute input for consistent book adding by user
        if (title.empty() || author.empty() || genre.empty()) {
            cout << "Error: All fields (title, author, genre) must be filled out." << endl;
            return;
        }

        // adds a book to the library by adding a node to the tree
        if (insertNode(root, title, author, genre)); // calls the privatefuntion
            cout << "Book added successfully: \"" << title << "\" by " << author << endl; // prints the newly added book
    }
    
    // function to search for books from a title name input
    void searchByTitle(string title) {
        BookNode* book = searchByTitle(root, title); // calls the privatefunction
        // if book found, prints the book title and author name
        if (book) {
            cout << "Found: \"" << book->title << "\" by " << book->author << endl;
        // if not found prints no book found
        } else {
            cout << "No book found with the title \"" << title << "\"." << endl;
        }
    }
    
    // function to seach for book from an author name input
    void searchByAuthor(string author) {
        bool found = false;
        searchByAuthor(root, author, found); // calls private function
        if (!found) {
            cout << "No books found by the author \"" << author << "\"." << endl;
        }
    }
    
    // outputs books from a given genre
    void recommendBooksByGenre(string genre) {
    cout << "Recommending books in the genre \"" << genre << "\":" << endl;
    cout << "Recommendations:" << endl;

    bool found = false; // tracks whether any books are found
    recommendByGenre(root, genre); // calls private function to traverse through the whole tree for same genre books
    
    // If no books are found, print "None Found"
    if (!found) {
        cout << "None Found" << endl;
    }

    }
    
    // displays all books (including dynamically stored ones)
    void displayAllBooks() {
    cout << "Displaying all books:" << endl;
    displayAllBooks(root); // calls the private function
    }
    
};

// Main function
int main() {
    Library library;
    int choice; // function selection choice for user

    cout << "Welcome to the Library Search and Recommendation System" << endl;

    do {
        cout << "\nChoose an option:" << endl;
        cout << "1. Add a Book" << endl;
        cout << "2. Search by Title" << endl;
        cout << "3. Search by Author" << endl;
        cout << "4. Recommend Books by Genre" << endl;
        cout << "5. Display All Books" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice)) { // checks for proper user input
            cin.clear(); // clears any wrong input 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clears leftover characters in user input buffer after wrong input
            cout << "Invalid input. Please enter a number between 1 and 6." << endl; // error message for wrong input
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clears leftovers from correct input
        
        switch (choice) {
            case 1: { // add a book with attributes title, author and genre
                string title, author, genre;
                cout << "Enter the title of the book: ";
                getline(cin, title);
                cout << "Enter the author of the book: ";
                getline(cin, author);
                cout << "Enter the genre of the book: ";
                getline(cin, genre);
                library.addBook(title, author, genre); // calls function
                break;
            }
            
            case 2: { // search for book by title
                string title;
                cout << "Enter the title to search for: ";
                getline(cin, title); // for whitespaces
                library.searchByTitle(title); // call function
                break;
            }
            
            case 3: { // search for book by author
                string author;
                cout << "Enter the author to search for: ";
                getline(cin, author); // for whitespaces
                library.searchByAuthor(author); // call function
                break;
            }
            
            case 4: { // search for books of given genre
                string genre;
                cout << "Enter the genre to recommend books for: ";
                getline(cin, genre); // for whitespaces
                library.recommendBooksByGenre(genre); // call function
                break;
            }
            
             case 5: // displays all books in library
                library.displayAllBooks(); // call function
                break;
                
            case 6: // Exit program
                cout << "Exiting the program!" << endl;
                break;
                
            default: // handles invalid user input
                cout << "Invalid input. Please enter a number between 1 and 6." << endl;
            }
            
        } while (choice != 6);

        return 0;
    }