#include <iostream>
#include <climits>

using namespace std;

//κλάση για κάθε φύλλο του δέντρου
class Node {
public:
	int data;	//αριθμός
	Node* left;	//δείχνει ,αν υπάρχει, το αριστερό παιδί του
	Node* right;	// δείχνει ,αν υπάρχει, το δεξιό παιδί του
	Node() {
		data = 0;
		left = right = NULL;
	}
};


//συνάρτηση για υπολογιμό του ύψους κάθε φύλλου
int height(Node* node) {
	int h = 0;	//αρχικοποίηση
	if (node != NULL) {	//αν το φύλλο δεν υπάρχει τότε το ύψος θα είναι μηδέν
		int left_subtree = height(node->left);	//αναδρομικός υπολογισμός του ύψους του αριστερού υποδέντρου
		int right_subtree = height(node->right);	//αναδρομικός υπολογισμός του ύψους του δεξιού υποδέντρου
		int max_height = left_subtree > right_subtree ? left_subtree : right_subtree;	//έβρεση του μεγίστου
		h = 1 + max_height;	//ύψος = μέγιστο + 1
	}
	return h;	//αποτέλεσμα
}
//συνάρτηση υπολογισμού της ισορροπίας bf(κόμβου)
int BalanceFactor(Node* node) {
	int left_height = height(node->left);
	int right_height = height(node->right);
	int bf = left_height - right_height;	
	return bf;
}

//δημιουργία νέου κόμβου
Node* Create_Node(int data) {
	Node* newNode = new Node();	//δημιουργήται αντικείμενο κλάσης Node
	//εισάγωγή δεδομένων
	newNode->data = data;
	newNode->right = newNode->left = NULL;
	return newNode;//αποτέλεσμα
}
//Right Right περιστροφή
Node* Right_Right(Node* parent) {
	Node* temp;
	temp = parent->right;
	parent->right = temp->left;
	temp->left = parent;
	return temp;
}

//Left Left περιστροφή
Node* Left_Left(Node* parent) {	//bf > 1
	Node* temp;
	temp = parent->left;
	parent->left = temp->right;
	temp->right = parent;
	return temp;
}

//Left Right περιστροφή
Node* Left_Right(Node* parent) {
	Node* temp;
	temp = parent->left;
	parent->left = Right_Right(temp);
	return Left_Left(parent);

}

//Right Left περιστροφή
Node* Right_Left(Node* parent) {
	Node* temp;
	temp = parent->right;
	parent->right = Left_Left(temp);
	return Right_Right(parent);
}

Node* AVL_Balance(Node* root) {

	if (BalanceFactor(root) > 1) {
		//Left Left περιστροφή
		if (BalanceFactor(root->left) > 0) {
			cout << "Left rotation happend" << endl;
			return Left_Left(root);
		}
		else {	//Left Right περιστροφή
			cout << "Left Right rotation happend" << endl;
			return Left_Right(root);
		}

	}
	else if (BalanceFactor(root) < -1) {
		//Right Right περιστροφή
		if (BalanceFactor(root->right) < 0) {
			cout << "Right Right rotation happened" << endl;
			return Right_Right(root);
		}
		else {	//Rigth Left περιστροφή
			cout << "Right left rotation happend" << endl;
			return Right_Left(root);
		}
	}
	return root;
}

//εισαγωωγή φύλλου στο δέντρο
Node* Insert_Node(Node* root, int data, Node* Max_Node) {
	if (root == NULL) {	//αν είναι άδειο
		root = Create_Node(data);	//δημιουργήται ένας απλός κόμβος
	}
	else if (data <= root->data) {	//αν η τιμή που δόθηκε είναι μικρότερη ίση από τον τρέχον κόμβο 
		root->left = Insert_Node(root->left, data,Max_Node);	//πηγαίνει στα αριστερά
		root = AVL_Balance(root);

	}
	else {
		root->right = Insert_Node(root->right, data,Max_Node);//αν όχι πηγαίνει δεξιά
		root = AVL_Balance(root);
	}
	//υπολογιστμός και αποθήευση μεγίστου
	if (root->data >= Max_Node->data) {
		Max_Node->data = root->data;
		Max_Node->left = root->left;
		Max_Node->right = root->right;
	}
	return root;
}
//συνάρτηση για εύρεση κόμβου με την ελάχιστη τιμή
Node* Smallest_Node(Node* root) {
	Node* temp = new Node();
	temp->data = LONG_MAX;
	//προδιάταξη και αναζήτηση
	if (root != NULL) {
		Smallest_Node(root->left);
		if (root->data < temp->data) {
			temp->data = root->data;
		}
		Smallest_Node(root->right);
	}
	return temp;
}
//διαγραφή κόμβου
Node* Delete_Node(Node* root,int key) {
	//εύρεση κόμβου
	if (root == NULL)return root;
	if (key == root->data) {
		//Αν ο κόμβος είναι φύλλο ή έχει ένα μόνο παιδί
		if (root->left == NULL){//δεξί παιδί
			Node* temp = root->right;
			free(root);
			return temp;
		}
		else if (root->right == NULL) {//αριστερό παιδι
			Node* temp = root->left;
			free(root);
			return temp;
		}
		//αλλιώς αν ο κόμβος έχει 2 παιδιά ,αναζητούμε το μικρότερο απόγονο στο δεξιό υποδέντρο 
		Node* small = Smallest_Node(root->right);

		//ο κόμβος που διαγράφτηκε παίρνει αυτήν την τιμή
		root->data = small->data;
		//ο μικρότερος κόμβος του δεξιού υποδέντρου διαγράφεται
		root->right = Delete_Node(root->right,small->data);
	}
	// προσπέλαση δέντρου
	else if (key < root->data) {//αριστερά 
		root->left = Delete_Node(root->left, key);
	}
	else {
		root->right =Delete_Node(root->right, key);
	}	
	AVL_Balance(root);//ισορρόπηση
	return root;
}

int main() {
	Node* root = NULL;//αντικείμενο στο οποίο αποθηκέυεται η ρίζα του δέντρου
	Node* Max_Value_Node = new Node();//αντικείμενο για την αποθήκευση του κόμβου με τη μέγιστη τιμή
	Max_Value_Node->data = -1;	//αρχικοποίηση
	//εισαγωγή νέων κόμβων και ανανέωση δεδομένων της ρίζας
	root = Insert_Node(root, 15,Max_Value_Node); 
	root = Insert_Node(root, 10, Max_Value_Node);
	root = Insert_Node(root, 20, Max_Value_Node);
	root = Insert_Node(root, 9, Max_Value_Node);
	root = Insert_Node(root, 8, Max_Value_Node);
	root = Insert_Node(root, 22, Max_Value_Node);
	root = Insert_Node(root, 24, Max_Value_Node);
	//διαγραφή κόμβων και ανανέωση δεδομένων της ρίζας
	root = Delete_Node(root, 15);
	root = Delete_Node(root, 20);
	root = Delete_Node(root, 22);
	root = Insert_Node(root, 80, Max_Value_Node);
	root = Insert_Node(root, 99, Max_Value_Node);
	//ο μέγιστος κόμβος
	cout << "Max is " << Max_Value_Node->data << endl;	//μέγιστο
	return 0;
}
