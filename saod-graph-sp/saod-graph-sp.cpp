#include <memory>
#include <iostream>
#include <list>   
#include <algorithm>
#include <map>
#include <iterator> 

using namespace std;

class Graph {

public:

	class Node {
	public:
		int index; // Номер вершины
		double weight; // Вес вершины
		Node(int idx, double weight = 0) : index(idx), weight(weight) { }; // Конструктор, по умолчанию вес вершины = 0
		bool operator < (const Node & second) { return index < second.index; }; // Оператор < сравнения вершин по номеру
		bool operator == (const Node & second) { return index == second.index; }; // Оператор == сравнения вершин по номеру
		using Ptr = shared_ptr<Node>;
		struct Comparator
		{
			bool operator() (const Ptr &lhs, const Ptr &rhs) const
			{
				return lhs->index < rhs->index;
			}
		};
	};

	class Verge {
	public:
		Node::Ptr A; // Указатель на первую вершину
		Node::Ptr B; // Указатель на вторую вершину
		double verge_weight; // Вес ребра
		Verge(Node::Ptr A, Node::Ptr B, double weight) : A(A), B(B), verge_weight(weight) { }; // Конструктор
//verge(int A, int B, double weight) : node_A(make_shared<node>(new node(A))), node_B(make_shared<node>(new node(B))), verge_weight(weight) { }; // Конструктор
		bool operator < (const Verge & second) {
			return (A->index < second.A->index)
				|| (A->index == second.A->index
					&& B->index < second.B->index);
		};
		bool operator == (const Verge & second) {
			return A->index == second.A->index
				&& B->index == second.B->index;
		};
		using Ptr = shared_ptr<Verge>;
		struct Comparator
		{
			bool operator() (const Ptr &lhs, const Ptr &rhs) const
			{
				return (lhs->A->index < rhs->A->index)
					|| (lhs->A->index == rhs->A->index
						&& lhs->B->index < rhs->B->index);
			};
		};
	};

	class Nodes {
	private:
		list<Node::Ptr> _nodes;
	public:
		~Nodes() { _nodes.clear(); };
		Node::Ptr Get(int idx) {
			Node::Ptr p;
			for (auto& it : _nodes)
				if ((*it).index == idx) {
					p = it;
					return p;
				};
			return p;
		};

		bool Add(int idx, double weight = 0) {
			if (Get(idx))
				return false;
			_nodes.push_back(make_shared<Node>(idx, weight));
			_nodes.sort(Node::Comparator());
			return true;
		};
		
		bool Del(int idx) {
			for (auto it : _nodes) {
				if ((*it).index == idx) {
					_nodes.remove(it);
					return true;
				};
			};
			return false;
		};

		void Print() {
			for (const auto &it : _nodes)
				cout << it->index << "(" << it->weight << ")\t";
			cout << endl;
		};
	};

	class Verges {
	private:
		list<Verge> _verges;
	public:
		~Verges() { _verges.clear(); };
		bool Add(int A_idx, int B_idx);
		Verge::Ptr Get(int A_idx, int B_idx);
		bool Del(int A_idx, int B_idx);
		void Print();
	};

	Graph() {};
	Graph(Graph const &G) = delete; // Удаляем конструктор копирования
	Graph(Graph &&G) = delete; // Удаляем конструктор перемещения
	void *operator new(std::size_t) = delete; // Удаляем операторы new
	void *operator new[](std::size_t) = delete; //

	Nodes nodes;
	Verges verges;


};



int main()
{
	Graph G;

	G.nodes.Add(3);
	G.nodes.Add(1);
	G.nodes.Add(1);
	G.nodes.Add(5);
	G.nodes.Add(2);
	G.nodes.Del(2);

	G.nodes.Print();



	// The end
	cout << endl << "\n\nEnter x to exit...";
	cin.get();
}

