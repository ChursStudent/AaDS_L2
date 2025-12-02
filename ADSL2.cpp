#include <iostream>
#include <fstream>
#include <cstdlib> // для rand() and srand()
#include <ctime>   // для time()
const char choice_str[146] = "0 - exit, 1 - find, 2 - insert, 3 - delete, 4 - max, 5 - min, 6 - plr, 7 - lpr, 8 - lrp, 9 - levels, 10 - height random, 11 - height increasing: ";
class Node //this = .. нельзя я думаю
{
private:
	int key;
	Node* p = nullptr;
	Node* l = nullptr;
	Node* r = nullptr;
	int balance = 0; //мб еще высота поддерева
	int color = 0; //0 ред 1 блэк
	struct Queue
	{
		Node* c = nullptr;
		Queue* next = nullptr;
	};
public:
	Node(int key)
	{
		this->key = key;
	}
	Node* find(int v)
	{
		if (this != nullptr)
		{
			if (this->key == v) return this;
			if (this->key > v) return this->l->find(v);
			else return this->r->find(v);
		}
		return nullptr;
	}
	int max()
	{
		Node* max = this;
		while (max->r != nullptr) max = max->r;
		return max->key;
	}
	int min()
	{
		Node* min = this;
		while (min->l != nullptr) min = min->l;
		return min->key;
	}
	void plr(char c = 'n')
	{
		if (this == nullptr) return;
		if (c == 'r')
		{
			if (this->color == 0) std::cout << "red" << " ";
			else std::cout << "black" << " ";
		}
		std::cout << this->key << ", ";
		this->l->plr(c);
		this->r->plr(c);
	}
	void lpr(char c = 'n')
	{
		if (this == nullptr) return;
		this->l->lpr(c);
		if (c == 'r')
		{
			if (this->color == 0) std::cout << "red" << " ";
			else std::cout << "black" << " ";
		}
		std::cout << this->key << ", ";
		this->r->lpr(c);
	}
	void lrp(char c = 'n')
	{
		if (this == nullptr) return;
		this->l->lrp(c);
		this->r->lrp(c);
		if (c == 'r')
		{
			if (this->color == 0) std::cout << "red" << " ";
			else std::cout << "black" << " ";
		}
		std::cout << this->key << ", ";
	}
	void level_print(char c = 'n')
	{
		Queue* queue = new Queue;
		Queue* cur = queue, * start = queue;
		cur->c = this;
		Node* nod;
		while (start->c)
		{
			nod = cur->c;
			cur = cur->next;
			start = cur;
			if (nod->l)
			{
				cur = new Queue;
				cur->c = nod->l;
				cur = cur->next;
			}
			if (nod->r)
			{
				cur = new Queue;
				cur->c = nod->r;
				cur = cur->next;
			}
			if (c == 'r')
			{
				if (nod->color == 0) std::cout << "red" << " ";
				else std::cout << "black" << " ";
			}
			std::cout << nod->key << ", ";
		}
		std::cout << "\n";
	}
	void insert(int v)
	{	//мб еще случай пустого списка
		if (this->key == v) return;
		if (this->key < v)
		{
			if (this->r == nullptr)
			{
				Node* t = new Node(v);
				this->r = t;
				t->p = this;
			}
			else this->r->insert(v);
		}
		else
		{
			if (this->l == nullptr)
			{
				Node* t = new Node(v);
				this->l = t;
				t->p = this;
			}
			else this->l->insert(v);
		}
	}
	void big_left_rot() //проверил
	{
		// не надо Node* t = this->p;
		this->r->right_rot();
		this->left_rot();
	}
	void big_right_rot()
	{
		// не надо Node* t = this->p;
		this->l->left_rot();
		this->right_rot();
	}
	void right_rot()
	{
		Node* t = this->l->r;
		if (this->p)
		{
			if (this == this->p->l)
				this->p->l = this->l;
			else this->p->r = this->l;
			this->l->p = this->p;
		}
		else this->r->p = nullptr;
		this->l->r = this;
		this->p = this->l;
		this->l = t;
	}
	void left_rot()
	{
		Node* t = this->r->l;
		if (this->p)
		{
			if (this == this->p->l)
				this->p->l = this->r;
			else this->p->r = this->r;
			this->r->p = this->p;
		}
		else this->r->p = nullptr;
		this->r->l = this;
		this->p = this->r;
		this->r = t;
	}
	void balancing_after_(char c)	//chenged recently
	{
		Node* prev, * cur = this;
		while (cur->p)
		{
			prev = this;
			cur = cur->p;
			if (prev == cur->l)
			{
				if (c == 'i') cur->balance++;
				else cur->balance--;
			}
			else
			{
				if (c == 'd') cur->balance--;
				else cur->balance++;
			}
			if (cur->balance == 0 && c == 'i') return;
			if ((cur->balance == 1 || cur->balance == -1) && c == 'd') return;
			if (cur->balance == 2)
			{
				if (cur->l->balance == 1) //l?
				{
					cur->right_rot();
					cur->balance = 0; //вот эти изм смущают (будто верно)
					cur = cur->p;
					cur->balance = 0;
				}
				else if (cur->l->balance == -1)	//здесь еще точно изм балансов
				{
					cur->big_right_rot();
					cur->balance = 0;
					cur = cur->p;
					cur->balance = 0;
					cur->l->balance = 0;
				}
			}
			if (cur->balance == -2)
			{
				if (cur->r->balance == -1)
				{
					cur->left_rot();
					cur->balance = 0;
					cur = cur->p;
					cur->balance = 0;
				}
				else if (cur->r->balance == 1)
				{
					cur->big_left_rot();
					cur->balance = 0;
					cur = cur->p;
					cur->balance = 0;
					cur->r->balance = 0;
				}
			}
			if (cur->balance == 0 && c == 'i') return;
			if (cur->balance != 0 && c == 'd') return;
		}
	}
	void insertAVL(int v)
	{
		this->insert(v);
		Node* t = this->find(v);
		t->balancing_after_('i');
	}
	void fixing_insertion()	//мб нужен еще итератор
	{
		if (!this->p)
		{
			this->color = 1;
			return;
		}
		while (this->p->color == 0)
		{
			if (this->p->p->l == this->p)
			{
				if (this->p->p->r->color == 0)
				{
					this->p->color = 1;
					this->p->p->r->color = 1;
					this->p->p->color = 0;
					this->color = this->p->p->color;
				}
				else
				{
					if (this->p->r == this)
					{
						this->color = this->p->color;
						this->left_rot();
					}
					this->p->color = 1;
					this->p->p->color = 0;
					this->p->p->right_rot();
				}
			}
			else
			{
				if (this->p->p->r->color == 0)
				{
					this->p->color = 1;
					this->p->p->r->color = 1;
					this->p->p->color = 0;
					this->color = this->p->p->color;
				}
				else
				{
					if (this->p->l == this)
					{
						this->color = this->p->color;
						this->right_rot();
					}
					this->p->color = 1;
					this->p->p->color = 0;
					this->p->p->left_rot();
				}
			}
		}
		Node* cur = this;
		while (cur->p) cur = cur->p;
		cur->color = 1;
	}
	void insertRB(int v) //по случаям из материалов
	{
		this->insert(v);
		Node* t = this->find(v);
		t->fixing_insertion();
	}
	Node* deleteThis()	//!changed recently
	{
		if (this->l == nullptr)	//no left
		{
			if (this->r == nullptr) {Node* t = this->p; delete this; return t; }//nor right (лист)
			else
			{
				if (this->p)	//not root (с правым)
				{
					if (this == this->p->l) { this->p->l = this->r; this->r->p = this->p; Node* t = this->r; delete this; return t; }
					else { this->p->r = this->r; this->r->p = this->p; Node* t = this->r; delete this; return t; }
				}
				else {
					Node * d = this; this->r->p = this->r; this->r->p = nullptr; delete d; return nullptr;	//обманул - сделал крюк в this
				}
			}
		}
		else if (this->r == nullptr)
		{
			if (this->p)	//not root (с левым)
			{
				if (this == this->p->l) { this->p->l = this->l; this->l->p = this->p; Node* t = this->l; delete this; return t; }
				else { this->p->r = this->l; this->l->p = this->p; Node* t = this->l; delete this; return t; }
			}
			else {
				Node* d = this; this->l->p = this->l; this->l->p = nullptr; delete d; return nullptr;	//обманул - сделал крюк в this
			}
		}
		else
		{
			int lmax = this->l->max();
			int lmaxcol = this->l->find(lmax)->color;
			this->l->find(lmax)->deleteThis();
			this->key = lmax;
			this->color = lmaxcol;
			return this;
		}
	}
	Node* delete_(int v)
	{
		if (this->find(v))
		{
			Node* t = this->find(v);
			return t->deleteThis();
		}
	}
	void deleteAVL(int v)
	{
		Node* f = this->delete_(v);
		if (f)
			f->balancing_after_('d');
	}
	void fixing_delete()	//мб еще проверку на наличие брата //мб еще итератор?
	{
		Node* cur = this;
		while (cur->p && cur->color == 1)
		{
			if (cur->p->l == cur)	//здесь ниже проверять
			{
				if (cur->p->r->color == 0)
				{
					cur->p->r->color = 1;
					cur->p->color = 0;
					cur->p->left_rot();
				}
				if ((!(cur->p->r->r) || cur->p->r->r->color == 1) && (!(cur->p->r->l) || cur->p->r->l->color == 1))
				{
					cur->p->r->color = 0;
				}
				else
				{
					if (!(cur->p->r->r) || cur->p->r->r->color == 1)
					{
						cur->p->r->l->color = 1;
						cur->p->r->color = 0;
						cur->p->r->right_rot();
					}
					cur->p->r->color = cur->p->color;
					cur->p->color = 1;
					cur->p->r->r->color = 1;
					cur->p->left_rot();
					while (cur->p) cur = cur->p;
				}
			}
			else	//здесь ниже проверять
			{
				if (cur->p->l->color == 0)
				{
					cur->p->l->color = 1;
					cur->p->color = 0;
					cur->p->right_rot();
				}
				if ((!(cur->p->l->r) || cur->p->l->r->color == 1) && (!(cur->p->l->l) || cur->p->l->l->color == 1))
				{
					cur->p->l->color = 0;
				}
				else
				{
					if (!(cur->p->l->l) || cur->p->l->l->color == 1)
					{
						cur->p->l->r->color = 1;
						cur->p->l->color = 0;
						cur->p->l->right_rot();
					}
					cur->p->l->color = cur->p->color;
					cur->p->color = 1;
					cur->p->l->l->color = 1;
					cur->p->left_rot();
					while (cur->p) cur = cur->p;
				}
			}
		}
		cur->color = 1;
		while (cur->p) cur = cur->p;
		cur->color = 1;
	}
	void deleteRB(int v) //по случаям из материалов
	{
		Node* f = this->delete_(v);
		if (f)
			f->fixing_delete();
	}
	int height()
	{
		Node* cur = this;
		if (!cur) return 0;
		return 1 + std::max(cur->l->height(), cur->r->height());
	}
};
void checkTreeHeightInRandom(Node* tree, short type)	//changed recently
{
	int k;
	for (int count = 1; count < 300; count++)
	{
		k = rand();
		if (!tree) tree = new Node(k);
		else
		{
			switch (type)
			{
			case 1: tree->insert(k); break;
			case 2: tree->insertAVL(k); break;
			case 3: tree->insertRB(k); break;
			default: break;
			}
		}
		if (count % 50 == 0)
		{
			std::cout << count << " " << tree->height() << "\n";
		}
	}
}
void checkTreeHeightInIncreasing(Node* tree, short type)	//changed recently
{
	int k = 1;
	for (int count = 1; count < 30000; count++, k++)
	{
		if (!tree) tree = new Node(k);
		else
		{
			switch (type)
			{
			case 1: tree->insert(k); break;
			case 2: tree->insertAVL(k); break;
			case 3: tree->insertRB(k); break;
			default: break;
			}
		}
		if (count % 5000 == 0)
		{
			std::cout << count << " " << tree->height() << "\n";
		}
	}
}
int main()
{
	/*создание пустых bst avl и rb
		цикл до выхода
		выбор работать с bst avl или rb
		свитч
		цикл до выхода
		выбор операции(+заполнить из файла и вывести высоту)
		свитч*/
	srand(time(0));
	Node* BSTtree = nullptr, * AVLtree = nullptr, * RBtree = nullptr;
	unsigned choice1 = 1, choice2 = 1;
	int val;
	std::cout << "0 - exit, 1 - BST, 2 - AVL, 3 - RB: ";
	std::cin >> choice1;
	while (choice1)
	{
		while (choice2)
		{
			std::cout << choice_str;
			std::cin >> choice2;
			switch (choice2)
			{
			case 1:
				std::cout << "insert int value: ";
				std::cin >> val;
				switch (choice1)
				{
				case 1:
					if (BSTtree->find(val)) std::cout << "value found\n";
					else std::cout << "value not found\n";
					break;
				case 2:
					if (AVLtree->find(val)) std::cout << "value found\n";
					else std::cout << "value not found\n";
					break;
				case 3:
					if (RBtree->find(val)) std::cout << "value found\n";
					else std::cout << "value not found\n";
					break;
				}
				break;
			case 2:
				std::cout << "insert int value: ";
				std::cin >> val;
				switch (choice1)
				{
				case 1:
					if (!BSTtree) BSTtree = new Node(val);
					else BSTtree->insert(val);
					break;
				case 2:
					if (!AVLtree) AVLtree = new Node(val);
					else AVLtree->insertAVL(val);
					break;
				case 3:
					if (!RBtree) RBtree = new Node(val);
					else RBtree->insertRB(val);
					break;
				}
				break;
			case 3:
				std::cout << "insert int value: ";
				std::cin >> val;
				switch (choice1)
				{
				case 1:
					BSTtree->delete_(val);
					break;
				case 2:
					AVLtree->deleteAVL(val);
					break;
				case 3:
					RBtree->deleteRB(val);
					break;
				}
				break;
			case 4:
				switch (choice1)
				{
				case 1:
					if (BSTtree)
						std::cout << "max BST value: " << BSTtree->max() << "\n";
					else std::cout << "BST tree is empty\n";
					break;
				case 2:
					if (AVLtree)
						std::cout << "max AVL value: " << AVLtree->max() << "\n";
					else std::cout << "AVL tree is empty\n";
					break;
				case 3:
					if (RBtree)
						std::cout << "max RB value: " << RBtree->max() << "\n";
					else std::cout << "RB tree is empty\n";
					break;
				}
				break;
			case 5:
				switch (choice1)
				{
				case 1:
					if (BSTtree)
						std::cout << "min BST value: " << BSTtree->min() << "\n";
					else std::cout << "BST tree is empty\n";
					break;
				case 2:
					if (AVLtree)
						std::cout << "min AVL value: " << AVLtree->min() << "\n";
					else std::cout << "AVL tree is empty\n";
					break;
				case 3:
					if (RBtree)
						std::cout << "min RB value: " << RBtree->min() << "\n";
					else std::cout << "RB tree is empty\n";
					break;
				}
				break;
			case 6:
				switch (choice1)
				{
				case 1:
					BSTtree->plr();
					std::cout << "\n";
					break;
				case 2:
					AVLtree->plr();
					std::cout << "\n";
					break;
				case 3:
					RBtree->plr('r');
					std::cout << "\n";
					break;
				}
				break;
			case 7:
				switch (choice1)
				{
				case 1:
					BSTtree->lpr();
					std::cout << "\n";
					break;
				case 2:
					AVLtree->lpr();
					std::cout << "\n";
					break;
				case 3:
					RBtree->lpr('r');
					std::cout << "\n";
					break;
				}
				break;
			case 8:
				switch (choice1)
				{
				case 1:
					BSTtree->lrp();
					std::cout << "\n";
					break;
				case 2:
					AVLtree->lrp();
					std::cout << "\n";
					break;
				case 3:
					RBtree->lrp('r');
					std::cout << "\n";
					break;
				}
				break;
			case 9:
				switch (choice1)
				{
				case 1:
					BSTtree->level_print();
					break;
				case 2:
					AVLtree->level_print();
					break;
				case 3:
					RBtree->level_print('r');
					break;
				}
				break;
			case 10:
				switch (choice1)
				{
				case 1:
					checkTreeHeightInRandom(BSTtree, 1);
					break;
				case 2:
					checkTreeHeightInRandom(AVLtree, 2);
					break;
				case 3:
					checkTreeHeightInRandom(RBtree, 3);
					break;
				}
				break;
			case 11:
				switch (choice1)
				{
				case 1:
					checkTreeHeightInIncreasing(BSTtree, 1);
					break;
				case 2:
					checkTreeHeightInIncreasing(AVLtree, 2);
					break;
				case 3:
					checkTreeHeightInIncreasing(RBtree, 3);
					break;
				}
				break;
			default:
				break;
			}
		}
		std::cout << "0 - exit, 1 - BST, 2 - AVL, 3 - RB: ";
		std::cin >> choice1;
	}
}