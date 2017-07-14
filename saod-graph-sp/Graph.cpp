#include "Graph.h"

Graph::Node::Ptr Graph::Nodes::Get(int idx) {
	Node::Ptr p;
	for (auto &it : _nodes)
		if (it->index == idx) {
			p = it;
			return p;
		};
	return p;
}

Graph::Node::Ptr Graph::Nodes::operator()(int idx) {
	Graph::Node::Ptr sp = Get(idx);
	if (!sp) {
		sp.reset(new Node{ idx });
		_nodes.push_back(sp);
		//_nodes.sort(Node::Comparator_());
		//_nodes.insert(sp);
	}
	return sp;
}


bool Graph::Nodes::Add(int idx, double weight = 0) {
	if (Get(idx))
		return false;
	_nodes.push_back(make_shared<Node>(idx, weight));
	//_nodes.sort(Node::Comparator());
	//_nodes.insert(make_shared<Node>(idx, weight));
	return true;
}

bool Graph::Nodes::Del(int idx) {
	if (owner->verges.IsNodeLinked(idx))
		return false;
	for (auto &it : _nodes)
		if (it->index == idx) {
			_nodes.remove(it);
			//_nodes.erase(it);
			return true;
		}
	return false;
}

void Graph::Nodes::ReCalcDegrees(list<Node::Ptr> nodes, list<Verge::Ptr> verges) {
	for (auto &node : nodes) {
		node->indeg = 0;
		node->outdeg = 0;
		for (const auto &verge : verges) {
			if (verge->A->index == node->index)
				node->outdeg++;
			if (verge->B->index == node->index)
				node->indeg++;
		}
	}
}

void Graph::Nodes::ReCalcDegrees()
{
	for (auto &node : _nodes) {
		node->indeg = 0;
		node->outdeg = 0;
		for (const auto &verge : *owner->verges.List()) {
			if (verge->A->index == node->index)
				node->outdeg++;
			if (verge->B->index == node->index)
				node->indeg++;
		}
	}
}

void Graph::Nodes::Print() {
	Print(_nodes);
	//for (const auto &it : _nodes)
	//	cout << it->index << "(" << it->weight << ", " << it->level << ", " << it->indeg << ", " << it->outdeg << ")\t";
	//cout << endl;
}

void Graph::Nodes::Print(list<Node::Ptr> nodes)
{
	for (const auto &it : nodes)
		cout << it->index << "(" << it->weight << ", " << it->level << ", " << it->indeg << ", " << it->outdeg << ")\t";
	cout << endl;
}

list<Graph::Node::Ptr> Graph::Nodes::Level(int level) {
	list<Graph::Node::Ptr> nlist;
	for (const auto &it : _nodes)
		if (it->level == level)
			nlist.push_back(it);
	return nlist;
}

list<Graph::Node::Ptr> Graph::Nodes::Nearby(int idx) {
	list<Node::Ptr> nlist;
	for (const auto &v : *owner->verges.List())
		if (v->A->index == idx)
			nlist.push_back(v->B);
	return nlist;
}

bool Graph::Verges::Add(int A_idx, int B_idx, double weight = 0) {
	Node::Ptr sp_A = owner->nodes(A_idx);
	Node::Ptr sp_B = owner->nodes(B_idx);
	// ”величиваем полустепени захода и исхода у вершин
	sp_A->outdeg++;
	sp_B->indeg++;
	_verges.push_back(make_shared<Verge>(sp_A, sp_B, weight));
	//_verges.sort(Verge::Comparator());
	//_verges.insert(make_shared<Verge>(sp_A, sp_B, weight));
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
		_verges.push_back(sp);
		//_verges.sort(Verge::Comparator());
		//_verges.insert(sp);
	}
	// ”величиваем полустепени захода и исхода у вершин
	sp->A->outdeg++;
	sp->B->indeg++;
	return sp;
}

bool Graph::Verges::Del(int A_idx, int B_idx) {
	for (auto &it : _verges) {
		if (it->A->index == A_idx && it->B->index == B_idx) {
			//_verges.remove(it);
			// ”меньшаем полустепени захода и исхода у вершин A и B:
			it->A->outdeg--;
			it->B->indeg--;
			_verges.remove(it);
			return true;
		}
	}
	return false;
}

bool Graph::Verges::IsNodeLinked(int idx) {
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
	for (const auto &n_col : *nodes.List())
		for (const auto &n_row : *nodes.List()) {
			auto v = verges.Get(n_col->index, n_row->index);
			if (v)
				_conn_map[n_col->index][n_row->index] = v->weight;
			else
				_conn_map[n_col->index][n_row->index] = Inf;  // Infinity
		}
}

void Graph::PrintConnectivityMatrix() { // ¬ыводим граф в виде матрицы смежности
	_re_map();
	// Print Header:
	cout << "[ ]\t";
	for (auto &iter : _conn_map)
		cout << "[" << iter.first << "]\t";
	cout << endl;
	// Print Rows:
	for (auto &iter : _conn_map) {
		cout << "[" << iter.first << "]\t";
		for (auto iter2 : iter.second) {
			(iter2.second == Inf) ? cout << "-\t" : cout << iter2.second << "\t";
		}
		cout << endl;
	}
}

void Graph::ReCalcNodesLevels() {
	for (auto &it : *nodes.List()) it->level = -1; // clear levels for nodes
	int cur_level = 0; 
	list<Node::Ptr> n1{ *nodes.List() }; // make copy of nodes
	list<Verge::Ptr> v1{ *verges.List() }; // make copy of verges
	while (!n1.empty()) {
		Nodes::ReCalcDegrees(n1, v1);
		cout << "***" << endl;
		Nodes::Print(n1);
		cout << "***" << endl;
		for (const auto &it : n1)
			if (it->indeg == 0)
				nodes(it->index)->level = cur_level;
		n1.remove_if([](auto &n) { return (n->indeg == 0); });
		v1.remove_if([](auto &v) { return (v->A->indeg == 0); });
		++cur_level;
	}

	
}

