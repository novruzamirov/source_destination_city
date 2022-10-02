/*@Author
Student name: <Novruz Amirov>
Student ID: <150200903>
E-mail: <amirov20@itu.edu.tr>
Date: <28.12.2021>
*/
#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <stack>

using namespace std;
//Normally I use multiple .h and .cpp files, but in calico it took only main.cpp file, therefore I wrote all code in only main.cpp

//Node and Tree class is used for creating and prunning binary search trees for each file
//ListNode and LinkedList is used for each city to memorize durations of each city
class Node
{
public:
    int num_of_city;
    int parent_dist;
    int total_dist;
    Node *parent;
    Node *left;
    Node *right;
    Node(int data)
    {
        this->num_of_city = data;
        this->parent_dist = 0;
        this->total_dist = 0;
        this->parent = NULL;
        this->left = NULL;
        this->right = NULL;
    }
};
class Tree
{
public:
    Node *root;
    void preorder_traverse(Node *n)
    {
        if (n != NULL)
        {
            cout << "CITY" << n->num_of_city << " ";
            this->preorder_traverse(n->left);
            this->preorder_traverse(n->right);
        }
    }
    Tree()
    {
        this->root = NULL;
    }
    void printPreOrder(char choice)
    {
        if (choice == '1')
            cout << "FRIEND-1: ";
        else
            cout << "FRIEND-2: ";

        this->preorder_traverse(this->root);
        cout << endl;
    }
};
class ListNode
{
public:
    int city_number;
    int dist_f1;
    int dist_f2;
    int total_dist;
    ListNode(int data)
    {
        this->city_number = data;
        this->dist_f1 = 0;
        this->dist_f2 = 0;
        this->total_dist = 0;
    };
    ListNode *next;
};
class LinkedList
{
public:
    ListNode *head;
    LinkedList()
    {
        this->head = NULL;
    };
};
//arrF1 and arrF2 is used for to store integer nums seperately which acquired from .txt files
vector<int> arrF1;
vector<int> arrF2;
Tree friend1;
Tree friend2;
LinkedList city_list;
Node *found = NULL;
void extractIntegerWords(string str, char choice) //this function is used for acquiring integers from string
{
    stringstream ss;
    ss << str;
    string temp;
    int found;
    while (!ss.eof())
    {
        ss >> temp;
        if (stringstream(temp) >> found)
        {
            if (choice == '1')
            {
                arrF1.push_back(found);
            }
            else
            {
                arrF2.push_back(found);
            }
        }
        temp = "";
    }
}
string read_file(string file_name) // this function is used for reading file and store datas in one string
{
    string s;
    ifstream file_obj;
    file_obj.open(file_name, ifstream::in);
    char symbol = 0;

    if (!file_obj.is_open())
    {
        cerr << "Could not open the file" << endl;
        exit(1);
    }

    while (file_obj.get(symbol))
    {
        s.push_back(symbol);
        if (symbol == 'Y')
        {
            s.push_back(' ');
        }
    }
    return s;
}
Node *find_node_in_bst(int data, char choice) //this function is used for returning a node if it is in bst, otherwise return NULL
{
    stack<Node *> st;
    if (choice == '1')
    {
        st.push(friend1.root);
    }
    else
    {
        st.push(friend2.root);
    }

    while (!st.empty())
    {
        Node *curr = st.top();
        if (curr->num_of_city == data)
        {
            return curr;
        }
        st.pop();
        if (curr->right != NULL)
        {
            st.push(curr->right);
        }
        if (curr->left != NULL)
        {
            st.push(curr->left);
        }
    }

    return NULL;
}
bool in_list(int c) //this function is used for searching a city if it is already exists in LinkedList or not.
{
    ListNode *temp = city_list.head;
    while (temp != NULL)
    {
        if (temp->city_number == c)
        {
            return true;
        }
        else
        {
            temp = temp->next;
        }
    }
    return false;
}
void add_to_list(int b) // this function is used for adding listnode to linkedlist if it is not in linkedlist
{
    if (in_list(b))
    {
        return;
    }
    ListNode *new_list_node = new ListNode(b);
    if (city_list.head == NULL)
    {
        city_list.head = new_list_node;
        new_list_node->next = NULL;
    }
    else
    {
        ListNode *temp = city_list.head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = new_list_node;
        new_list_node->next = NULL;
    }
}
void add_to_bst(int a, int b, int c, char choice) // this function is used for creating binary tree according to arrF1 and arrF2
{
    Node *temp = friend1.root;
    if (choice == '2')
        temp = friend2.root;

    if (temp == NULL)
    {
        Node *new_node = new Node(a);
        add_to_list(a);
        if (choice == '1')
            friend1.root = new_node;
        else
            friend2.root = new_node;
    }
    Node *source_city = find_node_in_bst(a, choice);
    Node *city = find_node_in_bst(b, choice);
    if (city != NULL)
    {
        if (city->total_dist > (source_city->total_dist + c))
        {
            if (city->parent->left == city)
            {
                city->parent->left = NULL;
            }
            else if (city->parent->right == city)
            {
                city->parent->right = NULL;
            }
            if (source_city->left == NULL)
            {
                source_city->left = city;
            }
            else if (source_city->right == NULL)
            {
                source_city->right = city;
            }
            city->parent = source_city;
            city->parent_dist = c;
            Node *temp2 = city;
            int total_cost = 0;
            while (temp2 != temp)
            {
                total_cost += temp2->parent_dist;
                temp2 = temp2->parent;
            }
            city->total_dist = total_cost;
        }
        else
            return;
    }
    else if (city == NULL)
    {
        Node *new_node = new Node(b);
        // ListNode *new_list_node = new ListNode(b);
        add_to_list(b);
        if (source_city->left == NULL)
        {
            source_city->left = new_node;
        }
        else if (source_city->right == NULL)
        {
            source_city->right = new_node;
        }
        new_node->parent = source_city;
        new_node->parent_dist = c;
        Node *temp2 = new_node;
        int total_cost = 0;
        while (temp2 != temp)
        {
            total_cost += temp2->parent_dist;
            temp2 = temp2->parent;
        }
        new_node->total_dist = total_cost;
    }
}
void show_list() //this function is used for imporing each listnode of linkedlist by adding dist_f1, dist_f2, and total dist
{
    ListNode *temp = city_list.head;
    int f1, f2;
    while (temp != NULL)
    {
        Node *a = find_node_in_bst(temp->city_number, '1');
        if (a == NULL)
            temp->dist_f1 = -1;
        else
            temp->dist_f1 = a->total_dist;

        Node *b = find_node_in_bst(temp->city_number, '2');
        if (b == NULL)
            temp->dist_f2 = -1;
        else
            temp->dist_f2 = b->total_dist;

        temp->total_dist = temp->dist_f1 + temp->dist_f2;
        temp = temp->next;
    }
    cout << endl;
}
ListNode *min_city() // this function returns the minimum distance city node from linkedlist
{
    ListNode *temp = city_list.head;
    ListNode *min = city_list.head;
    while (temp != NULL)
    {
        if (temp->dist_f1 != -1 && temp->dist_f2 != -1)
        {
            min = temp;
        }
        temp = temp->next;
    }
    temp = city_list.head;

    while (temp != NULL)
    {
        if (temp->total_dist < min->total_dist && (temp->dist_f1 != -1 && temp->dist_f2 != -1)) //problem
        {
            min = temp;
        }

        temp = temp->next;
    }
    return min;
}

int main(int argc, char *argv[])
{
    system("clear");
    extractIntegerWords(read_file(argv[1]), '1');
    extractIntegerWords(read_file(argv[2]), '2');

    int i = 0;
    while (i < arrF1.size())
    {
        add_to_bst(arrF1[i], arrF1[i + 1], arrF1[i + 2], '1');
        i += 3;
    }
    friend1.printPreOrder('1');

    int j = 0;
    while (j < arrF2.size())
    {
        add_to_bst(arrF2[j], arrF2[j + 1], arrF2[j + 2], '2');
        j += 3;
    }
    friend2.printPreOrder('2');
    show_list();
    ListNode *min = min_city();
    cout << "MEETING POINT: CITY" << min->city_number << endl;
    cout << "TOTAL DURATION COST: " << min->total_dist << endl;

    return EXIT_SUCCESS;
}
