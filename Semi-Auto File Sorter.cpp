#include <iostream>
#include <string>
#include <filesystem>
#include <queue>

using namespace std;
using namespace std::filesystem;

struct stackNode {
    path actualPath;
    stackNode* next;

    stackNode(path pathThing) {
        actualPath = pathThing;
        next = nullptr;
    }
};
struct stack {
    stackNode* head;

    stack() {
        head = nullptr;
    }
};
void push(stack& stackRef, path pathThing) {
    stackNode* newstackNode = new stackNode(pathThing);
    newstackNode->next = stackRef.head;
    stackRef.head = newstackNode;
};
void pop(stack& stackRef) {
    if (stackRef.head == nullptr) return;
    stackNode* temp = stackRef.head;
    path pathOfHead = temp->actualPath;
    stackRef.head = stackRef.head->next;
    delete temp;
};
bool isEmpty(stack& stackRef) {
    return stackRef.head == nullptr;
};
path peekStack(stack& stackRef) {
    if (isEmpty(stackRef)) return path();
    return stackRef.head->actualPath;
};
void printStack(stack& stackRef) {
    stackNode* current = stackRef.head;
    while (current != nullptr) {
        cout << current->actualPath << "\n";
        current = current->next;
    }
}


int main() {
    string menuChoice;
    bool running = true;
    bool inOption1 = false;
    queue<string> history;
    string destinationDirectory;

    path startingDirectory = current_path();
    path drive = startingDirectory.root_path();
    path directoryToSort = drive;

    while (running) {
        cout << "\n------------|-||--||| Semi-Auto File Sorter |||--||--|-----------\n\n"
             << "[ 1 ] Choose directory to sort (path)\n"
             << "[ 2 ] View sorting history for this session\n"
             << "[ 3 ] Start sorting chosen directory\n"
             << "[ ~End~ ] Close the program.\n";
        cout << "\nPlease enter the option you want to choose: ";
        getline(cin, menuChoice);    
// Option 1 -----------------------------------------------------------------------------------------------------------------------------------
        if (menuChoice == "1") { 
        inOption1 = true;
            while (inOption1){
                cout << "\nCurrent directory: " << directoryToSort;
                cout << "\nThe following are all children directories within the current directory:\n";

                directory_iterator filePointer(directoryToSort);
                directory_iterator end;
            
                while (filePointer != end) {
                    if (is_directory(filePointer->path())) {
                        path tempPath(filePointer->path());
                        cout << tempPath.stem() << "\n";
                    }
                    filePointer++;
                }

                cout << "\nAvalable commands: \n"
                    << "[ /*Any name in the list above*/ ] Enter that chosen directory\n"
                    << "[ ~Back~ ] Go back one directory\n"
                    << "[ ~Here~ ] Start sorting chosen directory.\n";
                cout << "\nPlease choose what you would like to do: ";
                getline(cin, menuChoice);

                if (menuChoice == "~Here~"){
                    cout << "Chosen directory: " << directoryToSort << "\n";
                    inOption1 = false;
                }else if (menuChoice == "~Back~") {
                    path newPath = directoryToSort.parent_path();
                    if (exists(newPath) && is_directory(newPath)) {
                        directoryToSort = newPath;
                    } else {
                        cout << "Directory not found. Try again.\n";
                    }
                } else {
                    path newPath = directoryToSort / menuChoice;
                    if (exists(newPath) && is_directory(newPath)) {
                        directoryToSort = newPath;
                    } else {
                        cout << "Directory not found. Try again.\n";
                    }
                }
            }
// Option 2 -----------------------------------------------------------------------------------------------------------------------------------
        } else if (menuChoice == "2") {
            if (history.empty() == true) {
                cout << "\nNothing has been sorted this sesion.\n";
            } else{
                queue<string> tempHistory(history);
                cout << "\nThe following are the actions done this current session"
                     << "(With most reacent actions being placed lowest)";
                while (!tempHistory.empty())
                {
                    cout << "\n" << tempHistory.front();
                    tempHistory.pop();
                }
            }
// Option 3 -----------------------------------------------------------------------------------------------------------------------------------
        } else if (menuChoice == "3") {

            stack moveStack;
            directory_iterator filePointer(directoryToSort);
            directory_iterator end;
        
            while (filePointer != end) {
                if (is_regular_file(filePointer->path())) {
                    path tempPath(filePointer->path());
                    push(moveStack, tempPath);
                }
                filePointer++;
            }

            cout << "\nEnter which directory would you like to put all the appropriate files in\n> ";
            getline(cin, destinationDirectory);

            try {
                create_directories(path(destinationDirectory));
                cout << "Destination ready: " << destinationDirectory << "\n";
            } catch (const exception& e) {
                cout << "Could not create directory: " << e.what() << "\n";
                continue;
            }

            cout << "\nThe following are all the files slected for sorting:\n";
            printStack(moveStack);
            cout << "\nHow would you like to sort them?\n"
             << "[ 1 ] Sort by whatevever text is in the file name\n"
             << "[ 2 ] Sort by specific file type\n"
             << "[ ~Back~ ] Return to main menu options\n";
            cout << "\nPlease choose what you would like to do: ";
            getline(cin, menuChoice);

            if (menuChoice == "1"){
                cout << "\nThis action will place all files with the input in their file name into:\n" << destinationDirectory << "\n\nPlease enter the search key word: ";
                getline(cin, menuChoice);
                
                string searchKey = menuChoice;
                stack filteredStack;
                stackNode* current = moveStack.head;

                while (current != nullptr) {
                    string filename = current->actualPath.filename().string();
                    if (filename.find(searchKey) != string::npos) {
                        push(filteredStack, current->actualPath);
                    }
                    current = current->next;
                }

                if (isEmpty(filteredStack)) {
                    cout << "No files matched \"" << searchKey << "\".\n";
                } else {
                    cout << "\nThe following files will be moved:\n";
                    printStack(filteredStack);

                    while (!isEmpty(filteredStack)) {
                        path currentFile = peekStack(filteredStack);
                        pop(filteredStack);
                        string filename = currentFile.filename().string();
                        path destination = path(destinationDirectory) / filename;

                        try {
                            rename(currentFile, destination);
                            cout << "Moved: " << filename << "\n";
                            history.push("Moved \"" + filename + "\" to " + destinationDirectory);
                        } catch (const exception& e) {
                            cout << "Failed to move " << filename << ": " << e.what() << "\n";
                        }
                    }
                }
            } else if (menuChoice == "2"){
                cout << "\nThis action will place all files with the input extension into:\n" << destinationDirectory << "\n\nPlease enter a file extension(.pdf, .docx, .png): ";
                getline(cin, menuChoice); 

                string searchKey = menuChoice;
                stack filteredStack;
                stackNode* current = moveStack.head;

                while (current != nullptr) {
                    string ext = current->actualPath.extension().string();
                    if (ext == searchKey) {
                        push(filteredStack, current->actualPath);
                    }
                    current = current->next; 
                }

                if (isEmpty(filteredStack)) {
                    cout << "No files matched extension \"" << searchKey << "\".\n"; 
                } else {
                    cout << "\nThe following files will be moved:\n";
                    printStack(filteredStack); 

                    while (!isEmpty(filteredStack)) {
                        path currentFile = peekStack(filteredStack); 
                        pop(filteredStack);
                        string filename = currentFile.filename().string();
                        path destination = path(destinationDirectory) / filename;

                        try {
                            rename(currentFile, destination); 
                            cout << "Moved: " << filename << "\n";
                            history.push("Moved \"" + filename + "\" to " + destinationDirectory);
                        } catch (const exception& e) {
                            cout << "Failed to move " << filename << ": " << e.what() << "\n";
                        }
                    }
                }
            } else if (menuChoice == "2"){
                continue;
            }
// Exit Option -----------------------------------------------------------------------------------------------------------------------------------
        } else if (menuChoice == "~End~") {
            running = false; 
            break; 
// Other Options -----------------------------------------------------------------------------------------------------------------------------------
        } else {
            cout << "/!\\ Please enter a valid Input/!\\"; 
            continue;
        }
    }
}