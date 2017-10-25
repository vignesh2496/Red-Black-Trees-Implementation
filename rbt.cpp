#include<bits/stdc++.h>
#define BLACK false
#define RED true
#define NIL INT_MIN
using namespace std;

// ASSUMPTION : No nodes with equal keys in a BST

struct node
{
    float data;
    node* left;
    node* right;
    bool color;
};

node* nullnode()
{
    node* temp = new node;
    temp->data = NIL;
    temp->left = NULL;
    temp->right = NULL;
    temp->color = BLACK;
    return temp;
}

node* newnode(float val)
{
    node* temp = new node;
    temp->data = val;
    temp->left = nullnode();
    temp->right = nullnode();
    temp->color = RED;
    return temp;
}

int get_black_height(node* root)
{
    if(root->data == NIL)
        return 0;
    return (root->color ? 0 : 1) + get_black_height(root->left);
}

node* right_rot(node* root)
{
    node* n1 = root->left;
    node* n2 = n1->right;
    root->left = n2;
    n1->right = root;
    return n1;
}

node* left_rot(node* root)
{
    node* n1 = root->right;
    node* n2 = n1->left;
    root->right = n2;
    n1->left = root;
    return n1;
}

node* check_fix_DRED(node* root, float val)
{
    // Left
    if(val < root->data)
    {
        // Left
        if(val < root->left->data)
        {
            if(root->left->left->color == RED && root->left->color == RED)
            {
                if(root->right->color == BLACK)
                {
                    root = right_rot(root);
                    root->color = BLACK;
                    root->right->color = RED;
                }
                else
                {
                    root->color = RED;
                    root->left->color = BLACK;
                    root->right->color = BLACK;
                }
            }
        }
        // Right
        else if(val > root->left->data)
        {
            if(root->left->right->color == RED && root->left->color == RED)
            {
                if(root->right->color == BLACK)
                {
                    root->left = left_rot(root->left);
                    root = right_rot(root);
                    root->color = BLACK;
                    root->right->color = RED;
                }
                else
                {
                    root->color = RED;
                    root->left->color = BLACK;
                    root->right->color = BLACK;
                }
            }
        }
    }
    // Right
    else if(val > root->data)
    {
        // Left
        if(val < root->right->data)
        {
            if(root->right->left->color == RED && root->right->color == RED)
            {
                if(root->left->color == BLACK)
                {
                    root->right = right_rot(root->right);
                    root = left_rot(root);
                    root->color = BLACK;
                    root->left->color = RED;
                }
                else
                {
                    root->color = RED;
                    root->left->color = BLACK;
                    root->right->color = BLACK;
                }
            }
        }
        // Right
        else if(val > root->right->data)
        {
            if(root->right->right->color == RED && root->right->color == RED)
            {
                if(root->left->color == BLACK)
                {
                    root = left_rot(root);
                    root->color = BLACK;
                    root->left->color = RED;
                }
                else
                {
                    root->color = RED;
                    root->left->color = BLACK;
                    root->right->color = BLACK;
                }
            }
        }
    }
    return root;
}

node* insert_helper(node* root, float val)
{
    if(root->data == NIL)
    {
        delete root;
        return newnode(val);
    }
    if(val < root->data)
        root->left = insert_helper(root->left, val);
    else if(val > root->data)
        root->right = insert_helper(root->right, val);
    else
        return root;
    // Check and fix Double RED problem
    return check_fix_DRED(root, val);
}

node* insert(node* root, float val)
{
    root = insert_helper(root, val);
    root->color = BLACK;
    return root;
}

node* get_succ(node* root)
{
    root = root->right;
    while(root->left->data != NIL)
        root = root->left;
    return root;
}

node* check_fix_DBLACK(node* root, float val, bool* DBLACK)
{
    // NOTE : The siblings will always exist as NON-NULL nodes.
    // Right is sibling
    if(val < root->data)
    {
        if(root->right->color == BLACK)
        {
            if(root->right->right->color == RED)
            {
                bool p_color = root->color;
                root = left_rot(root);
                root->color = p_color;
                root->left->color = BLACK;
                root->right->color = BLACK;
                *DBLACK = false;
            }
            else if(root->right->left->color == RED)
            {
                root->right = right_rot(root->right);
                root->right->color = BLACK;
                root->right->right->color = RED;
                bool p_color = root->color;
                root = left_rot(root);
                root->color = p_color;
                root->left->color = BLACK;
                root->right->color = BLACK;
                *DBLACK = false;
            }
            else
            {
                if(root->color == RED)
                {
                    root->color = BLACK;
                    *DBLACK = false;
                }
                root->right->color = RED;
            }
        }
        else
        {
            root = left_rot(root);
            root->color = BLACK;
            root->left->color = RED;
            root->left = check_fix_DBLACK(root->left, root->left->left->data, DBLACK);
        }
    }
    // Left is sibling
    else if(val > root->data)
    {
        if(root->left->color == BLACK)
        {
            if(root->left->left->color == RED)
            {
                bool p_color = root->color;
                root = right_rot(root);
                root->color = p_color;
                root->left->color = BLACK;
                root->right->color = BLACK;
                *DBLACK = false;
            }
            else if(root->left->right->color == RED)
            {
                root->left = left_rot(root->left);
                root->left->color = BLACK;
                root->left->left->color = RED;
                bool p_color = root->color;
                root = right_rot(root);
                root->color = p_color;
                root->left->color = BLACK;
                root->right->color = BLACK;
                *DBLACK = false;
            }
            else
            {
                if(root->color == RED)
                {
                    root->color = BLACK;
                    *DBLACK = false;
                }
                root->left->color = RED;
            }
        }
        else
        {
            root = right_rot(root);
            root->color = BLACK;
            root->right->color = RED;
            if(root->right->right->data == NIL)
                root->right = check_fix_DBLACK(root->right, INT_MAX, DBLACK);
            else
                root->right = check_fix_DBLACK(root->right, root->right->right->data, DBLACK);
        }
    }
    return root;
}

node* del_helper(node* root, float val, bool* DBLACK)
{
    if(root->data == NIL)
        return root;
    if(val < root->data)
        root->left = del_helper(root->left, val, DBLACK);
    else if(val > root->data)
        root->right = del_helper(root->right, val, DBLACK);
    else
    {
        if(root->left->data == NIL && root->right->data == NIL)
        {
            if(root->color == BLACK)
                *DBLACK = true;
            delete root->left;
            delete root->right;
            delete root;
            return nullnode();
        }
        else if(root->right->data == NIL)
        {
            if(root->color == BLACK && root->left->color == BLACK)
                *DBLACK = true;
            node* temp = root;
            root = root->left;
            root->color = BLACK;
            delete temp->right;
            delete temp;
            return root;
        }
        else if(root->left->data == NIL)
        {
            if(root->color == BLACK && root->right->color == BLACK)
                *DBLACK = true;
            node* temp = root;
            root = root->right;
            root->color = BLACK;
            delete temp->left;
            delete temp;
            return root;
        }
        else
        {
            node* temp = get_succ(root);
            root->data = temp->data;
            root->right = del_helper(root->right, temp->data, DBLACK);
            // Force it to go if it propagates till the root
            *DBLACK = false;
            return root;
        }
    }
    if(*DBLACK)
        root = check_fix_DBLACK(root, val, DBLACK);
    return root;
}

node* del(node* root, float val)
{
    bool DBLACK = false;
    return del_helper(root, val, &DBLACK);
}

float get_max(node* root)
{
    if(root->data == NIL)
        return 0;
    while(root->right->data != NIL)
        root = root->right;
    return root->data;
}

float get_min(node* root)
{
    if(root->data == NIL)
        return 0;
    while(root->left->data != NIL)
        root = root->left;
    return root->data;
}

node* fixer(node* root, node* dest, float val, int flag)
{
    if(root != dest)
    {
        if(flag)
            root->left = fixer(root->left, dest, val, flag);
        else
            root->right = fixer(root->right, dest, val, flag);
    }
    return check_fix_DRED(root, val);
}

node* merge(node* root1, node* root2)
{
    if(root1->data == NIL)
        return root2;
    if(root2->data == NIL)
        return root1;

    int h1 = get_black_height(root1), h2 = get_black_height(root2), diff = abs(h1 - h2), h = 0;
    float val1 = get_max(root1), val2 = get_min(root2), mid_val = (val1 + val2) / 2;
    if(h1 > h2)
    {
        node* temp = root1;
        while(temp->data != NIL)
        {
            h += temp->color ? 0 : 1;
            if(h == diff)
                break;
            temp = temp->right;
        }
        node* right_subtree = temp->right;
        temp->right = newnode(mid_val);
        temp->right->left = right_subtree;
        temp->right->right = root2;
        root1 = fixer(root1, temp, right_subtree->data, 0);
        root1->color = BLACK;
        return del(root1, mid_val);
    }
    else if(h1 < h2)
    {
        node* temp = root2;
        while(temp->data != NIL)
        {
            h += temp->color ? 0 : 1;
            if(h == diff)
                break;
            temp = temp->left;
        }
        node* left_subtree = temp->left;
        temp->left = newnode(mid_val);
        temp->left->left = root1;
        temp->left->right = left_subtree;
        root2 = fixer(root2, temp, left_subtree->data, 1);
        root2->color = BLACK;
        return del(root2, mid_val);
    }
    else
    {
        node* new_root = newnode(mid_val);
        new_root->color = BLACK;
        new_root->left = root1;
        new_root->right = root2;
        return del(new_root, mid_val);
    }
}

void preorder(node* root)
{
    if(root->data == NIL)
        return;
    char c = root->color ? 'R' : 'B';
    cout << root->data << c << "  ";
    preorder(root->left);
    preorder(root->right);
}

void inorder(node* root)
{
    if(root->data == NIL)
        return;
    char c = root->color ? 'R' : 'B';
    inorder(root->left);
    cout << root->data << c << "  ";
    inorder(root->right);
}

int main()
{
    node* root = nullnode();
    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 30);
    root = insert(root, 40);
    root = insert(root, 50);
    root = insert(root, 60);
    cout << "TREE 1" << endl << "PRE-ORDER : ";
    preorder(root);
    cout << endl;
    cout << "IN-ORDER : ";
    inorder(root);
    cout << endl << endl;

    node* root1 = nullnode();
    root1 = insert(root1, 70);
    root1 = insert(root1, 80);
    root1 = insert(root1, 90);
    cout << "TREE 2" << endl << "PRE-ORDER : ";
    preorder(root1);
    cout << endl;
    cout << "IN-ORDER : ";
    inorder(root1);
    cout << endl << endl;

    node* m = merge(root, root1);
    cout << "FINAL TREE" << endl << "PRE-ORDER : ";
    preorder(m);
    cout << endl;
    cout << "IN-ORDER : ";
    inorder(m);
    cout << endl;

    return 0;
}
