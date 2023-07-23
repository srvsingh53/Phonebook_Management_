#include <iostream>
#include <string>
using namespace std;

enum Color { RED, BLACK };

struct Contact {
    string name;
    string phoneNumber;
};

struct Node {
    Contact contact;
    Color color;
    Node* parent;
    Node* left;
    Node* right;
};

typedef Node* NodePtr;

class Phonebook {
private:
    NodePtr root;
    NodePtr TNULL;

    NodePtr createNode(const Contact& contact) {
        NodePtr node = new Node;
        node->contact = contact;
        node->color = RED;
        node->left = TNULL;
        node->right = TNULL;
        return node;
    }

    void initializeNULLNode(NodePtr node, NodePtr parent) {
        node->contact = { "", "" };
        node->parent = parent;
        node->left = nullptr;
        node->right = nullptr;
        node->color = BLACK;
    }

    void leftRotate(NodePtr x) {
        NodePtr y = x->right;
        x->right = y->left;
        if (y->left != TNULL) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rightRotate(NodePtr y) {
        NodePtr x = y->left;
        y->left = x->right;
        if (x->right != TNULL) {
            x->right->parent = y;
        }
        x->parent = y->parent;
        if (y->parent == nullptr) {
            root = x;
        } else if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }
        x->right = y;
        y->parent = x;
    }

    void insertFix(NodePtr k) {
        NodePtr u;
        while (k->parent != nullptr && k->parent->color == RED) {
            if (k->parent == k->parent->parent->right) {
                u = k->parent->parent->left; // uncle
                if (u->color == RED) {
                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->left) {
                        k = k->parent;
                        rightRotate(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    leftRotate(k->parent->parent);
                }
            } else {
                u = k->parent->parent->right; // uncle

                if (u->color == RED) {
                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    rightRotate(k->parent->parent);
                }
            }
            if (k == root) {
                break;
            }
        }
        root->color = BLACK;
    }

    void insert(NodePtr node, const Contact& contact) {
        NodePtr prev = nullptr;
        while (node != TNULL) {
            prev = node;
            if (contact.name < node->contact.name) {
                node = node->left;
            } else if (contact.name > node->contact.name) {
                node = node->right;
            } else {
                node->contact.phoneNumber = contact.phoneNumber;
                return;
            }
        }
        NodePtr newNode = createNode(contact);
        newNode->parent = prev;
        if (prev == nullptr) {
            root = newNode;
        } else if (contact.name < prev->contact.name) {
            prev->left = newNode;
        } else {
            prev->right = newNode;
        }
        insertFix(newNode);
    }

    NodePtr findNode(const string& name, NodePtr node) {
        if (node == TNULL || node->contact.name == name) {
            return node;
        }
        if (name < node->contact.name) {
            return findNode(name, node->left);
        }
        return findNode(name, node->right);
    }

    NodePtr findMinNode(NodePtr node) {
        while (node->left != TNULL) {
            node = node->left;
        }
        return node;
    }

    void removeFix(NodePtr x) {
        NodePtr s;
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                s = x->parent->right;
                if (s->color == RED) {
                    s->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    s = x->parent->right;
                }

                if (s->left->color == BLACK && s->right->color == BLACK) {
                    s->color = RED;
                    x = x->parent;
                } else {
                    if (s->right->color == BLACK) {
                        s->left->color = BLACK;
                        s->color = RED;
                        rightRotate(s);
                        s = x->parent->right;
                    }

                    s->color = x->parent->color;
                    x->parent->color = BLACK;
                    s->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                s = x->parent->left;
                if (s->color == RED) {
                    s->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    s = x->parent->left;
                }

                if (s->right->color == BLACK && s->left->color == BLACK) {
                    s->color = RED;
                    x = x->parent;
                } else {
                    if (s->left->color == BLACK) {
                        s->right->color = BLACK;
                        s->color = RED;
                        leftRotate(s);
                        s = x->parent->left;
                    }

                    s->color = x->parent->color;
                    x->parent->color = BLACK;
                    s->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    void remove(const string& name) {
        NodePtr z = findNode(name, root);
        if (z == TNULL) {
            cout << "Contact not found in the phonebook." << endl;
            return;
        }
        NodePtr x, y;
        Color originalColor = z->color;
        if (z->left == TNULL) {
            x = z->right;
            rbTransplant(z, z->right);
        } else if (z->right == TNULL) {
            x = z->left;
            rbTransplant(z, z->left);
        } else {
            y = findMinNode(z->right);
            originalColor = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y;
            } else {
                rbTransplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            rbTransplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (originalColor == BLACK) {
            removeFix(x);
        }
        cout << "Contact removed successfully." << endl;
    }

    void rbTransplant(NodePtr u, NodePtr v) {
        if (u->parent == nullptr) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    void inorderTraversal(NodePtr node) {
        if (node != TNULL) {
            inorderTraversal(node->left);
            cout << "Name: " << node->contact.name << ", Phone: " << node->contact.phoneNumber << endl;
            inorderTraversal(node->right);
        }
    }

public:
    Phonebook() {
        TNULL = new Node;
        TNULL->color = BLACK;
        TNULL->left = nullptr;
        TNULL->right = nullptr;
        root = TNULL;
    }

    void insertContact(const Contact& contact) {
        insert(root, contact);
        cout << "Contact added successfully." << endl;
    }

    void searchContact(const string& name) {
        NodePtr node = findNode(name, root);
        if (node == TNULL) {
            cout << "Contact not found in the phonebook." << endl;
        } else {
            cout << "Name: " << node->contact.name << ", Phone: " << node->contact.phoneNumber << endl;
        }
    }

    void displayAllContacts() {
        inorderTraversal(root);
    }

    void removeContact(const string& name) {
        remove(name);
    }

    void changePhoneNumber(const string& name, const string& newPhoneNumber) {
        NodePtr node = findNode(name, root);
        if (node == TNULL) {
            cout << "Contact not found in the phonebook." << endl;
        } else {
            node->contact.phoneNumber = newPhoneNumber;
            cout << "Phone number updated successfully." << endl;
        }
    }
};

int main() {
    Phonebook phonebook;
    Contact contact;
    int choice;
    string name, phoneNumber;

    do {
        cout << "\n----- Phonebook Management System -----" << endl;
        cout << "1. Add New Contact" << endl;
        cout << "2. Search Contact" << endl;
        cout << "3. Display All Contacts" << endl;
        cout << "4. Remove Contact" << endl;
        cout << "5. Change Phone Number" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter Name: ";
                cin.ignore();
                getline(cin, contact.name);
                cout << "Enter Phone Number: ";
                getline(cin, contact.phoneNumber);
                phonebook.insertContact(contact);
                break;
            case 2:
                cout << "Enter Name to Search: ";
                cin.ignore();
                getline(cin, name);
                phonebook.searchContact(name);
                break;
            case 3:
                cout << "\n--- All Contacts ---" << endl;
                phonebook.displayAllContacts();
                break;
            case 4:
                cout << "Enter Name to Remove: ";
                cin.ignore();
                getline(cin, name);
                phonebook.removeContact(name);
                break;
            case 5:
                cout << "Enter Name to Change Phone Number: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter New Phone Number: ";
                getline(cin, phoneNumber);
                phonebook.changePhoneNumber(name, phoneNumber);
                break;
            case 6:
                cout << "Exiting Phonebook Management System. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 6);

return 0;
}
