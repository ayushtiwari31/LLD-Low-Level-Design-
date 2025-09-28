#include <iostream>
#include <vector>
#include <memory>
using namespace std;

// Step 1: Component (common interface)
class FileSystemItem {
public:
    virtual void show(int indent = 0) = 0;
    virtual ~FileSystemItem() = default;
};

// Step 2: Leaf (File)
class File : public FileSystemItem {
    string name;
public:
    File(string n) : name(n) {}
    void show(int indent = 0) override {
        cout << string(indent, ' ') << "- File: " << name << endl;
    }
};

// Step 3: Composite (Folder)
class Folder : public FileSystemItem {
    string name;
    vector<unique_ptr<FileSystemItem>> children;
public:
    Folder(string n) : name(n) {}
    void add(unique_ptr<FileSystemItem> item) {
        children.push_back(move(item));
    }
    void show(int indent = 0) override {
        cout << string(indent, ' ') << "+ Folder: " << name << endl;
        for (auto& child : children) {
            child->show(indent + 2); // recursive
        }
    }
};

// Step 4: Client
int main() {
    auto root = make_unique<Folder>("root");
    root->add(make_unique<File>("file1.txt"));
    root->add(make_unique<File>("file2.txt"));

    auto subFolder = make_unique<Folder>("docs");
    subFolder->add(make_unique<File>("resume.docx"));
    subFolder->add(make_unique<File>("project.pdf"));

    root->add(move(subFolder));

    root->show();
    return 0;
}
