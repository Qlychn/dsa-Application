#include <iostream>
#include <string>
#include <filesystem>

using namespace std;
using namespace std::filesystem;

struct FileNode {
    path filePath;
    FileNode* next;

    FileNode(path p) {
        filePath = p;
        next = nullptr;
    }
};

struct FileStack {
    FileNode* top;

    FileStack() {
        top = nullptr;
    }

    void push(path p) {
        FileNode* newNode = new FileNode(p);
        newNode->next = top;
        top = newNode;
    }

    path pop() {
        if (top == nullptr) {
            return path();
        }
        FileNode* temp = top;
        path p = temp->filePath;
        top = top->next;
        delete temp; 
        return p;
    }

    bool isEmpty() {
        return top == nullptr;
    }
};

int main() {
    string targetPath, searchKey, choiceInput;

    while (true) {
        cout << "\n--- Manual Data Structure Sorter (Type '~Exit' to quit) ---\n";
        cout << "Current Location: " << current_path() << "\n";
        
        cout << "Enter destination folder name: ";
        getline(cin, targetPath);

        if (targetPath == "~Exit") {
            break;
        }

        if (targetPath.empty()) {
            continue;
        }

        if (targetPath.back() != '/' && targetPath.back() != '\\') {
            targetPath += "/";
        }

        try {
            create_directories(targetPath);
        } catch (const exception& e) {
            cout << "Error creating directory: " << e.what() << "\n";
            continue;
        }

        cout << "Sort by: \n   [1] File Extension \n   [2] Name Keyword \n   Choice: ";
        getline(cin, choiceInput);

        if (choiceInput == "~Exit") {
            break;
        }

        cout << "Enter search term: ";
        getline(cin, searchKey);

        if (searchKey == "~Exit") {
            break;
        }

        FileStack moveStack;

        for (auto& entry : directory_iterator("./")) {
            if (is_regular_file(entry)) {
                string filename = entry.path().filename().string();
                bool match = false;

                if (choiceInput == "1" && entry.path().extension() == searchKey) {
                    match = true;
                } else if (choiceInput == "2" && filename.find(searchKey) != string::npos) {
                    match = true;
                }

                if (match) {
                    if (filename != "prototype.exe" && entry.path().extension() != ".cpp") {
                        moveStack.push(entry.path());
                    }
                }
            }
        }

        int moveCount = 0;

        while (!moveStack.isEmpty()) {
            path currentFile = moveStack.pop();
            string filename = currentFile.filename().string();

            try {
                rename(currentFile, targetPath + filename);
                cout << "Successfully Moved: " << filename << "\n";
                moveCount++;
            } catch (const exception& e) {
                cout << "Failed to move " << filename << ": " << e.what() << "\n";
            }
        }

        cout << "Process finished. Total files moved: " << moveCount << "\n";
    }

    cout << "Program closed.\n";
    return 0;
}
