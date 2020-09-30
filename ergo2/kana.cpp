#include <iostream>
#include <climits>

using namespace std;

//����� ��� ���� ����� ��� �������
class Node {
public:
	int data;	//�������
	Node* left;	//������� ,�� �������, �� �������� ����� ���
	Node* right;	// ������� ,�� �������, �� ����� ����� ���
	Node() {
		data = 0;
		left = right = NULL;
	}
};


//��������� ��� ��������� ��� ����� ���� ������
int height(Node* node) {
	int h = 0;	//������������
	if (node != NULL) {	//�� �� ����� ��� ������� ���� �� ���� �� ����� �����
		int left_subtree = height(node->left);	//����������� ����������� ��� ����� ��� ��������� ����������
		int right_subtree = height(node->right);	//����������� ����������� ��� ����� ��� ������ ����������
		int max_height = left_subtree > right_subtree ? left_subtree : right_subtree;	//������ ��� ��������
		h = 1 + max_height;	//���� = ������� + 1
	}
	return h;	//����������
}
//��������� ����������� ��� ���������� bf(������)
int BalanceFactor(Node* node) {
	int left_height = height(node->left);
	int right_height = height(node->right);
	int bf = left_height - right_height;	
	return bf;
}

//���������� ���� ������
Node* Create_Node(int data) {
	Node* newNode = new Node();	//������������ ����������� ������ Node
	//�������� ���������
	newNode->data = data;
	newNode->right = newNode->left = NULL;
	return newNode;//����������
}
//Right Right ����������
Node* Right_Right(Node* parent) {
	Node* temp;
	temp = parent->right;
	parent->right = temp->left;
	temp->left = parent;
	return temp;
}

//Left Left ����������
Node* Left_Left(Node* parent) {	//bf > 1
	Node* temp;
	temp = parent->left;
	parent->left = temp->right;
	temp->right = parent;
	return temp;
}

//Left Right ����������
Node* Left_Right(Node* parent) {
	Node* temp;
	temp = parent->left;
	parent->left = Right_Right(temp);
	return Left_Left(parent);

}

//Right Left ����������
Node* Right_Left(Node* parent) {
	Node* temp;
	temp = parent->right;
	parent->right = Left_Left(temp);
	return Right_Right(parent);
}

Node* AVL_Balance(Node* root) {

	if (BalanceFactor(root) > 1) {
		//Left Left ����������
		if (BalanceFactor(root->left) > 0) {
			cout << "Left rotation happend" << endl;
			return Left_Left(root);
		}
		else {	//Left Right ����������
			cout << "Left Right rotation happend" << endl;
			return Left_Right(root);
		}

	}
	else if (BalanceFactor(root) < -1) {
		//Right Right ����������
		if (BalanceFactor(root->right) < 0) {
			cout << "Right Right rotation happened" << endl;
			return Right_Right(root);
		}
		else {	//Rigth Left ����������
			cout << "Right left rotation happend" << endl;
			return Right_Left(root);
		}
	}
	return root;
}

//��������� ������ ��� ������
Node* Insert_Node(Node* root, int data, Node* Max_Node) {
	if (root == NULL) {	//�� ����� �����
		root = Create_Node(data);	//������������ ���� ����� ������
	}
	else if (data <= root->data) {	//�� � ���� ��� ������ ����� ��������� ��� ��� ��� ������ ����� 
		root->left = Insert_Node(root->left, data,Max_Node);	//�������� ��� ��������
		root = AVL_Balance(root);

	}
	else {
		root->right = Insert_Node(root->right, data,Max_Node);//�� ��� �������� �����
		root = AVL_Balance(root);
	}
	//������������ ��� ��������� ��������
	if (root->data >= Max_Node->data) {
		Max_Node->data = root->data;
		Max_Node->left = root->left;
		Max_Node->right = root->right;
	}
	return root;
}
//��������� ��� ������ ������ �� ��� �������� ����
Node* Smallest_Node(Node* root) {
	Node* temp = new Node();
	temp->data = LONG_MAX;
	//���������� ��� ���������
	if (root != NULL) {
		Smallest_Node(root->left);
		if (root->data < temp->data) {
			temp->data = root->data;
		}
		Smallest_Node(root->right);
	}
	return temp;
}
//�������� ������
Node* Delete_Node(Node* root,int key) {
	//������ ������
	if (root == NULL)return root;
	if (key == root->data) {
		//�� � ������ ����� ����� � ���� ��� ���� �����
		if (root->left == NULL){//���� �����
			Node* temp = root->right;
			free(root);
			return temp;
		}
		else if (root->right == NULL) {//�������� �����
			Node* temp = root->left;
			free(root);
			return temp;
		}
		//������ �� � ������ ���� 2 ������ ,���������� �� ��������� ������� ��� ����� ��������� 
		Node* small = Smallest_Node(root->right);

		//� ������ ��� ����������� ������� ����� ��� ����
		root->data = small->data;
		//� ���������� ������ ��� ������ ���������� �����������
		root->right = Delete_Node(root->right,small->data);
	}
	// ���������� �������
	else if (key < root->data) {//�������� 
		root->left = Delete_Node(root->left, key);
	}
	else {
		root->right =Delete_Node(root->right, key);
	}	
	AVL_Balance(root);//����������
	return root;
}

int main() {
	Node* root = NULL;//����������� ��� ����� ������������ � ���� ��� �������
	Node* Max_Value_Node = new Node();//����������� ��� ��� ���������� ��� ������ �� �� ������� ����
	Max_Value_Node->data = -1;	//������������
	//�������� ���� ������ ��� �������� ��������� ��� �����
	root = Insert_Node(root, 15,Max_Value_Node); 
	root = Insert_Node(root, 10, Max_Value_Node);
	root = Insert_Node(root, 20, Max_Value_Node);
	root = Insert_Node(root, 9, Max_Value_Node);
	root = Insert_Node(root, 8, Max_Value_Node);
	root = Insert_Node(root, 22, Max_Value_Node);
	root = Insert_Node(root, 24, Max_Value_Node);
	//�������� ������ ��� �������� ��������� ��� �����
	root = Delete_Node(root, 15);
	root = Delete_Node(root, 20);
	root = Delete_Node(root, 22);
	root = Insert_Node(root, 80, Max_Value_Node);
	root = Insert_Node(root, 99, Max_Value_Node);
	//� �������� ������
	cout << "Max is " << Max_Value_Node->data << endl;	//�������
	return 0;
}
