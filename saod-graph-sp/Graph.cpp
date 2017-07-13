#include "Graph.h"

Graph::Node::Ptr Graph::Nodes::Get(int idx) {
	Node::Ptr p;
	for (auto &it : _nodes)
		if ((*it).index == idx) {
			p = it;
			return p;
		};
	return p;
}

Graph::Node::Ptr Graph::Nodes::operator()(int idx) {
	Graph::Node::Ptr sp = Get(idx);
	if (!sp) {
		sp.reset(new Node{ idx });
		//_nodes.push_back(sp);
		//_nodes.sort(Node::Comparator());
		_nodes.insert(sp);
	}
	return sp;
}


bool Graph::Nodes::Add(int idx, double weight = 0) {
	if (Get(idx))
		return false;
	//_nodes.push_back(make_shared<Node>(idx, weight));
	//_nodes.sort(Node::Comparator());
	_nodes.insert(make_shared<Node>(idx, weight));
	return true;
}

bool Graph::Nodes::Del(int idx) {
	if (owner->verges.IsNodeIncluded(idx))
		return false;
	for (auto &it : _nodes)
		if (it->index == idx) {
			//_nodes.remove(it);
			_nodes.erase(it);
			return true;
		}
	return false;
}

void Graph::Nodes::Print() {
	for (const auto &it : _nodes)
		cout << it->index << "(" << it->weight << ")\t";
	cout << endl;
}


	
bool Graph::Verges::Add(int A_idx, int B_idx, double weight = 0) {
	Node::Ptr sp_A = owner->nodes(A_idx);
	Node::Ptr sp_B = owner->nodes(B_idx);
	//_verges.push_back(make_shared<Verge>(sp_A, sp_B, weight));
	//_verges.sort(Verge::Comparator());
	_verges.insert(make_shared<Verge>(sp_A, sp_B, weight));
	return true; //????????????????????????????????????????
}

Graph::Verge::Ptr Graph::Verges::Get(int A_idx, int B_idx) {
	Verge::Ptr sp;
	for (const auto &it : _verges) {
		if (it->A->index == A_idx && it->B->index == B_idx)
			sp = it;
	}
	return sp;
}

Graph::Verge::Ptr Graph::Verges::operator()(int A_idx, int B_idx) {
	Verge::Ptr sp = Get(A_idx, B_idx);
	if (!sp) {
		sp.reset(new Verge{ owner->nodes(A_idx), owner->nodes(B_idx) });
		//_verges.push_back(sp);
		//_verges.sort(Verge::Comparator());
		_verges.insert(sp);
	}
	return sp;
}

bool Graph::Verges::Del(int A_idx, int B_idx) {
	for (auto &it : _verges) {
		if (it->A->index == A_idx && it->B->index == B_idx) {
			//_verges.remove(it);
			_verges.erase(it);
			return true;
		}
	}
	return false;
}

bool Graph::Verges::IsNodeIncluded(int idx) {
	for (auto &it : _verges)
		if (it->A->index == idx || it->B->index == idx)
			return true;
	return false;
}

void Graph::Verges::Print() {
	for (const auto &it : _verges)
		cout << "(" << it->A->index << ")-(" << it->B->index << ")[" << it->weight << "]\t";
	cout << endl;
}

void Graph::_re_map() {
	_conn_map.clear();
	//for (const auto &n_col : *nodes.List())
	//	for (const auto &n_row : *nodes.List()) {
	for (const auto &n_col : *nodes.Set())
		for (const auto &n_row : *nodes.Set()) {
			auto v = verges.Get(n_col->index, n_row->index);
			if (v)
				_conn_map[n_col->index][n_row->index] = v->weight;
			else
				_conn_map[n_col->index][n_row->index] = Inf;  // Infinity
		}
}

void Graph::Print_Connectivity_Matrix() { // Выводим граф в виде матрицы смежности
	_re_map();
	// Header:
	cout << "[ ]\t";
	for (auto iter : _conn_map)
		cout << "[" << iter.first << "]\t";
	cout << endl;
	// Rows:
	for (auto iter : _conn_map) {
		cout << "[" << iter.first << "]\t";
		for (auto iter2 : iter.second) {
			(iter2.second == Inf) ? cout << "-\t" : cout << iter2.second << "\t";
		}
		cout << endl;
	}
}

