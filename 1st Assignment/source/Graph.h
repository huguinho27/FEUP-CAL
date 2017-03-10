/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <algorithm>
#include <queue>
#include <list>
#include <limits.h>
#include <cmath>
#include <stdlib.h>
using namespace std;

template <class T> class Edge;
template <class T> class Graph;

const int NOT_VISITED = 0;
const int BEING_VISITED = 1;
const int DONE_VISITED = 2;
const int INT_INFINITY = INT_MAX;


/*
 * ================================================================================================
 * Class Vertex
 * ================================================================================================
 */
template <class T>
class Vertex {
	T info;
	vector<Edge<T>  > adj;
	bool visited;
	bool processing;
	int indegree;
	double dist;

public:
	Vertex(T in);
	friend class Graph<T>;

	void addEdge(Vertex<T> *dest, double w);
	bool removeEdgeTo(Vertex<T> *d);
	vector<Edge<T> > getEdges() const;
	T getInfo() const;
	void setInfo(T info);
	int getDist() const;
	int getIndegree() const;
	bool operator<(const Vertex<T> vertex);

	Vertex* path;
};

template <class T>
struct vertex_greater_than {
    bool operator()(Vertex<T> * a, Vertex<T> * b) const {
        return a->getDist() > b->getDist();
    }
};

template <class T>
bool Vertex<T>::removeEdgeTo(Vertex<T> *d)
{
	d->indegree--; //adicionado do exercicio 5
	typename vector<Edge<T> >::iterator it= adj.begin();
	typename vector<Edge<T> >::iterator ite= adj.end();
	while (it!=ite) {
		if (it->dest == d) {
			adj.erase(it);
			return true;
		}
		else it++;
	}
	return false;
}

template <class T>
vector<Edge<T> > Vertex<T>::getEdges() const{
	return adj;
}

template <class T>
Vertex<T>::Vertex(T in): info(in), visited(false), processing(false), indegree(0), dist(0) {
	path = NULL;
}

template <class T>
void Vertex<T>::addEdge(Vertex<T> *dest, double w) {
	Edge<T> edgeD(dest,w);
	adj.push_back(edgeD);
}

template <class T>
T Vertex<T>::getInfo() const {
	return this->info;
}

template <class T>
int Vertex<T>::getDist() const {
	return this->dist;
}

template <class T>
void Vertex<T>::setInfo(T info) {
	this->info = info;
}

template <class T>
int Vertex<T>::getIndegree() const {
	return this->indegree;
}

/* ================================================================================================
 * Class Edge
 * ================================================================================================
 */
template <class T>
class Edge {
	int id;
	Vertex<T> * dest;
	double weight;
public:
	Edge(Vertex<T> *d, double w);

	friend class Graph<T>;
	friend class Vertex<T>;
	Vertex<T> * getDestination() const;
	bool operator<(const Edge<T>& e) const	{ return weight < e.weight;	}
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w){}

template <class T>
Vertex<T> * Edge<T>::getDestination() const { return dest; }

/* ================================================================================================
 * Struct Solution
 * ================================================================================================
 */
template <class T>
struct Solution
{
public:
	vector<Vertex<T> *> path;
	vector<double> pathPartialLengths;
	double pathLength;
	bool operator<(const Solution<T>& s) const { return pathLength < s.pathLength; }
};

/* ================================================================================================
 * Class Graph
 * ================================================================================================
 */
template <class T>
class Graph
{
	vector<Vertex<T> *> vertexSet;
	void dfs(Vertex<T> *v, vector<T> &res) const;

	int numCycles;
	void dfsVisit(Vertex<T> *v);
	void dfsVisit();
	void getPathTo(Vertex<T> *origin, list<T> &res);

	bool floydWarshall = false;
	int ** W;   //weight
	int ** P;   //path

public:
	bool addVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w);
	bool removeVertex(const T &in);
	bool removeEdge(const T &sourc, const T &dest);
	vector<T> dfs() const;
	vector<T> bfs(Vertex<T> *v) const;
	int maxNewChildren(Vertex<T> *v, T &inf) const;
	vector<Vertex<T> * > getVertexSet() const;
	int getNumVertex() const;

	Vertex<T>* getVertex(const T &v) const;
	void resetIndegrees();
	vector<Vertex<T>*> getSources() const;
	int getNumCycles();
	vector<T> getPath(const T &origin, const T &dest);
	void unweightedShortestPath(const T &v);
	bool isDAG();

	void floydWarshallShortestPath();
	int edgeCost(int vOrigIndex, int vDestIndex);
	vector<T> getfloydWarshallPath(const T &origin, const T &dest);
	void getfloydWarshallPathAux(int index1, int index2, vector<T> & res);

	Graph<T> makeGraphWithPOI(vector<T> POI);

	Solution<T> getShortestDeliveryPathBruteForce(T start, T end);
	void getShortestDeliveryPathPathBruteForceAux(Vertex<T> *v, Vertex<T> *destination, Solution<T> &bestSolution, Solution<T> solution, int &nSolutions);
	Solution<T> getShortestDeliveryPathGreedy(T start, T end);
	bool getShortestDeliveryPathGreedyAux(Vertex<T> *v, Vertex<T> *destination, Solution<T> &solution, double pathLength);
};

template <class T>
int Graph<T>::getNumVertex() const
{
	return vertexSet.size();
}

template <class T>
vector<Vertex<T> * > Graph<T>::getVertexSet() const
{
	return vertexSet;
}

template <class T>
int Graph<T>::getNumCycles() {
	numCycles = 0;
	dfsVisit();
	return this->numCycles;
}

template <class T>
bool Graph<T>::isDAG() {
	return (getNumCycles() == 0);
}

template <class T>
bool Graph<T>::addVertex(const T &in)
{
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	for (; it!=ite; it++)
		if ((*it)->info == in) return false;
	Vertex<T> *v1 = new Vertex<T>(in);
	vertexSet.push_back(v1);
	return true;
}

template <class T>
bool Graph<T>::removeVertex(const T &in)
{
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	for (; it!=ite; it++) {
		if ((*it)->info == in) {
			Vertex<T> * v= *it;
			vertexSet.erase(it);
			typename vector<Vertex<T>*>::iterator it1= vertexSet.begin();
			typename vector<Vertex<T>*>::iterator it1e= vertexSet.end();
			for (; it1!=it1e; it1++) {
				(*it1)->removeEdgeTo(v);
			}

			typename vector<Edge<T> >::iterator itAdj= v->adj.begin();
			typename vector<Edge<T> >::iterator itAdje= v->adj.end();
			for (; itAdj!=itAdje; itAdj++) {
				itAdj->dest->indegree--;
			}
			delete v;
			return true;
		}
	}
	return false;
}

template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w)
{
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	int found=0;
	Vertex<T> *vS, *vD;
	while (found!=2 && it!=ite ) {
		if ( (*it)->info == sourc )
			{ vS=*it; found++;}
		if ( (*it)->info == dest )
			{ vD=*it; found++;}
		it ++;
	}
	if (found!=2) return false;
	vD->indegree++;
	vS->addEdge(vD,w);

	return true;
}

template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest)
{
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	int found=0;
	Vertex<T> *vS, *vD;
	while (found!=2 && it!=ite ) {
		if ( (*it)->info == sourc )
			{ vS=*it; found++;}
		if ( (*it)->info == dest )
			{ vD=*it; found++;}
		it ++;
	}
	if (found!=2)
		return false;

	vD->indegree--;

	return vS->removeEdgeTo(vD);
}

template <class T>
vector<T> Graph<T>::dfs() const
{
	typename vector<Vertex<T>*>::const_iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::const_iterator ite= vertexSet.end();
	for (; it !=ite; it++)
		(*it)->visited=false;
	vector<T> res;
	it=vertexSet.begin();
	for (; it !=ite; it++)
	    if ( (*it)->visited==false )
	    	dfs(*it,res);
	return res;
}

template <class T>
void Graph<T>::dfs(Vertex<T> *v,vector<T> &res) const
{
	v->visited = true;
	res.push_back(v->info);
	typename vector<Edge<T> >::iterator it= (v->adj).begin();
	typename vector<Edge<T> >::iterator ite= (v->adj).end();
	for (; it !=ite; it++)
	    if ( it->dest->visited == false ){
	    	//cout << "ok ";
	    	dfs(it->dest, res);
	    }
}

template <class T>
vector<T> Graph<T>::bfs(Vertex<T> *v) const
{
	vector<T> res;
	queue<Vertex<T> *> q;
	q.push(v);
	v->visited = true;
	while (!q.empty()) {
		Vertex<T> *v1 = q.front();
		q.pop();
		res.push_back(v1->info);
		typename vector<Edge<T> >::iterator it=v1->adj.begin();
		typename vector<Edge<T> >::iterator ite=v1->adj.end();
		for (; it!=ite; it++) {
			Vertex<T> *d = it->dest;
			if (d->visited==false) {
				d->visited=true;
				q.push(d);
			}
		}
	}
	return res;
}

template <class T>
int Graph<T>::maxNewChildren(Vertex<T> *v, T &inf) const
{
	vector<T> res;
	queue<Vertex<T> *> q;
	queue<int> level;
	int maxChildren=0;
	inf =v->info;
	q.push(v);
	level.push(0);
	v->visited = true;
	while (!q.empty()) {
		Vertex<T> *v1 = q.front();
		q.pop();
		res.push_back(v1->info);
		int l=level.front();
		level.pop(); l++;
		int nChildren=0;
		typename vector<Edge<T> >::iterator it=v1->adj.begin();
		typename vector<Edge<T> >::iterator ite=v1->adj.end();
		for (; it!=ite; it++) {
			Vertex<T> *d = it->dest;
			if (d->visited==false) {
				d->visited=true;
				q.push(d);
				level.push(l);
				nChildren++;
			}
		}
		if (nChildren>maxChildren) {
			maxChildren=nChildren;
			inf = v1->info;
		}
	}
	return maxChildren;
}


template <class T>
Vertex<T>* Graph<T>::getVertex(const T &v) const
{
	for(unsigned int i = 0; i < vertexSet.size(); i++)
		if (vertexSet[i]->info == v) return vertexSet[i];
	return NULL;
}

template<class T>
void Graph<T>::resetIndegrees()
{
	//colocar todos os indegree em 0;
	for(unsigned int i = 0; i < vertexSet.size(); i++) vertexSet[i]->indegree = 0;

	//actualizar os indegree
	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		//percorrer o vector de Edges, e actualizar indegree
		for(unsigned int j = 0; j < vertexSet[i]->adj.size(); j++) {
			vertexSet[i]->adj[j].dest->indegree++;
		}
	}
}

template<class T>
vector<Vertex<T>*> Graph<T>::getSources() const
{
	vector< Vertex<T>* > buffer;
	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		if( vertexSet[i]->indegree == 0 ) buffer.push_back( vertexSet[i] );
	}
	return buffer;
}

template <class T>
void Graph<T>::dfsVisit()
{
	typename vector<Vertex<T>*>::const_iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::const_iterator ite= vertexSet.end();
	for (; it !=ite; it++)
		(*it)->visited=false;
	it=vertexSet.begin();
	for (; it !=ite; it++)
	    if ( (*it)->visited==false )
	    	dfsVisit(*it);
}

template <class T>
void Graph<T>::dfsVisit(Vertex<T> *v)
{
	v->processing = true;
	v->visited = true;
	typename vector<Edge<T> >::iterator it= (v->adj).begin();
	typename vector<Edge<T> >::iterator ite= (v->adj).end();
	for (; it !=ite; it++) {
		if ( it->dest->processing == true) numCycles++;
	    if ( it->dest->visited == false ){
	    	dfsVisit(it->dest);
	    }
	}
	v->processing = false;
}

template<class T>
vector<T> Graph<T>::getPath(const T &origin, const T &dest)
{
	list<T> buffer;
	Vertex<T>* v = getVertex(dest);

	buffer.push_front(v->info);
	while ( v->path != NULL &&  v->path->info != origin) {
		v = v->path;
		buffer.push_front(v->info);
	}
	if( v->path != NULL )
		buffer.push_front(v->path->info);


	vector<T> res;
	while( !buffer.empty() ) {
		res.push_back( buffer.front() );
		buffer.pop_front();
	}
	return res;
}

template<class T>
vector<T> Graph<T>::getfloydWarshallPath(const T &origin, const T &dest)
{
	int originIndex = -1, destinationIndex = -1;

	for(unsigned int i = 0; i < vertexSet.size(); i++)
	{
		if(vertexSet[i]->info == origin)
			originIndex = i;
		if(vertexSet[i]->info == dest)
			destinationIndex = i;

		if(originIndex != -1 && destinationIndex != -1)
			break;
	}

	vector<T> res;

	//se nao foi encontrada solucao possivel, retorna lista vazia
	if(W[originIndex][destinationIndex] == INT_INFINITY)
		return res;

	res.push_back(vertexSet[originIndex]->info);

	//se houver pontos intermedios...
	if(P[originIndex][destinationIndex] != -1)
	{
		int intermedIndex = P[originIndex][destinationIndex];

		getfloydWarshallPathAux(originIndex, intermedIndex, res);

		res.push_back(vertexSet[intermedIndex]->info);

		getfloydWarshallPathAux(intermedIndex,destinationIndex, res);
	}

	res.push_back(vertexSet[destinationIndex]->info);


	return res;
}

template<class T>
void Graph<T>::getfloydWarshallPathAux(int index1, int index2, vector<T> & res)
{
	if(P[index1][index2] != -1)
	{
		getfloydWarshallPathAux(index1, P[index1][index2], res);

		res.push_back(vertexSet[P[index1][index2]]->info);

		getfloydWarshallPathAux(P[index1][index2],index2, res);
	}
}

template<class T>
int Graph<T>::edgeCost(int vOrigIndex, int vDestIndex)
{
	if(vertexSet[vOrigIndex] == vertexSet[vDestIndex])
		return 0;

	for(unsigned int i = 0; i < vertexSet[vOrigIndex]->adj.size(); i++)
	{
		if(vertexSet[vOrigIndex]->adj[i].dest == vertexSet[vDestIndex])
			return vertexSet[vOrigIndex]->adj[i].weight;
	}

	return INT_INFINITY;
}

template<class T>
void Graph<T>::floydWarshallShortestPath()
{
	if(floydWarshall) return;

	W = new int * [vertexSet.size()];
	P = new int * [vertexSet.size()];
	for(unsigned int i = 0; i < vertexSet.size(); i++)
	{
		W[i] = new int[vertexSet.size()];
		P[i] = new int[vertexSet.size()];
		for(unsigned int j = 0; j < vertexSet.size(); j++)
		{
			W[i][j] = edgeCost(i,j);
			P[i][j] = -1;
		}
	}

	for(unsigned int k = 0; k < vertexSet.size(); k++)
		for(unsigned int i = 0; i < vertexSet.size(); i++)
			for(unsigned int j = 0; j < vertexSet.size(); j++)
			{
				//se somarmos qualquer coisa ao valor INT_INFINITY, ocorre overflow, o que resulta num valor negativo, logo nem convém considerar essa soma
				if(W[i][k] == INT_INFINITY || W[k][j] == INT_INFINITY)
					continue;

				int val = min ( W[i][j], W[i][k]+W[k][j] );
				if(val != W[i][j])
				{
					W[i][j] = val;
					P[i][j] = k;
				}
			}
	floydWarshall = true;
}

template <class T>
Solution<T> Graph<T>::getShortestDeliveryPathBruteForce(T start, T end)
{
	Solution<T> bestSolution;
	bestSolution.pathLength = INT_INFINITY;

	Vertex<T> * beginning = getVertex(start);
	Vertex<T> * destination = getVertex(end);

	//verifica que os nós inicio e destino pertencem ao grafo, caso contrario retorna lista vazia
	if(find(vertexSet.begin(), vertexSet.end(), beginning) == vertexSet.end() ||
			find(vertexSet.begin(), vertexSet.end(), destination) == vertexSet.end()){
		return bestSolution;
	}

	int nSolutions = 0;

	Solution<T> solution;
	solution.pathLength = 0;
	solution.path.push_back(beginning);

	getShortestDeliveryPathPathBruteForceAux(beginning, destination, bestSolution, solution, nSolutions);

	//cout << "solutions found: " << nSolutions << endl;
	return bestSolution;
}

template <class T>
void Graph<T>::getShortestDeliveryPathPathBruteForceAux(Vertex<T> *v, Vertex<T> *destination, Solution<T> &bestSolution, Solution<T> solution, int &nSolutions)
{
	if(v == destination){
		//se chegamos ao destino e todos os nós foram visitados temos uma solução
		if(solution.path.size() == getNumVertex()){
			nSolutions++;
			if(solution.pathLength < bestSolution.pathLength)
				bestSolution = solution;
		}
		else return;
	}else{
		typename vector<Edge<T> >::iterator itAdj;
		//experimenta todas as arestas que saem do nó actual
		for(itAdj = v->adj.begin(); itAdj != v->adj.end(); itAdj++)
		{
			//verifica se o nó destino da aresta ainda nao foi visitado
			if( find(solution.path.begin(), solution.path.end(), itAdj->dest ) == solution.path.end() )
			{
				Solution<T> solution1 = solution;
				solution1.path.push_back(itAdj->dest);
				solution1.pathLength += itAdj->weight;
				getShortestDeliveryPathPathBruteForceAux(itAdj->dest, destination, bestSolution, solution1, nSolutions);
			}
		}
	}
}

template <class T>
Solution<T> Graph<T>::getShortestDeliveryPathGreedy(T start, T end)
{
	Vertex<T> * beginning = getVertex(start);
	Vertex<T> * destination = getVertex(end);

	//verifica que os nós inicio e destino pertencem ao grafo, caso contrario retorna lista vazia
	if(find(vertexSet.begin(), vertexSet.end(), beginning) == vertexSet.end() ||
			find(vertexSet.begin(), vertexSet.end(), destination) == vertexSet.end()){
		Solution<T> s;
		return s;
	}

	Solution<T> sol;
	sol.pathLength = 0;
	getShortestDeliveryPathGreedyAux(beginning, destination, sol, 0);

	return sol;
}

template <class T>
bool Graph<T>::getShortestDeliveryPathGreedyAux(Vertex<T> *v, Vertex<T> *destination, Solution<T> &solution, double pathLength)
{
	if(v == destination){
		//se chegamos ao destino e todos os nós foram visitados chegamos à solução
		if(solution.path.size() == getNumVertex()-1 ){
			solution.path.push_back(v);
			solution.pathLength = pathLength;
			return true;
		}
		else return false;
	}

	//verifica se o novo nó ainda nao foi visitado
	if( find(solution.path.begin(), solution.path.end(), v ) != solution.path.end() )
		return false;

	solution.path.push_back(v);

	vector<Edge<T> > edges = v->adj;
	sort( edges.begin(), edges.end() );
	for(unsigned int i = 0; i < edges.size(); i++){
		if ( getShortestDeliveryPathGreedyAux(edges[i].dest, destination, solution, pathLength + edges[i].weight) )
			return true;
	}
	solution.path.pop_back();
	return false;
}


template <class T>
Graph<T> Graph<T>::makeGraphWithPOI(vector<T> POI)
{
	Graph<T> g;
	int indexes [POI.size()];

	//Adiciona todos os pontos de interesse ao novo grafo
	for(unsigned int i = 0; i < POI.size(); i++)
	{
		//bool found = false;

		//Obtem o index do ponto de interesse
		for(unsigned int j = 0; j < vertexSet.size(); j++)
		{
			if (vertexSet[j]->getInfo() == POI[i]){
				indexes[i] = j;
				g.addVertex( vertexSet[j]->getInfo() );
				//found = true;
			}
		}
		//Ponto de interesse nao faz parte do grafo - é ignorado em vez de retornar grafo vazio
		/*if (!found)
		{
			cout << "POI " << POI[i] << " doesn't belong to graph" << endl;
			Graph<T> emptyGraph;
			return emptyGraph;
		}*/
	}

	//Calcula distancias entre todos os pontos
	floydWarshallShortestPath();

	//Adiciona arestas entre todos os pontos de interesse com pesos calculados com Floyd Warshall
	for(unsigned int i = 0; i < POI.size(); i++)
	{
		for(unsigned int j = 0; j < POI.size(); j++)
		{
			if(i != j){
				int w = W[ indexes[i] ][ indexes[j] ];
				if (w != INT_INFINITY)
					g.addEdge(POI[i], POI[j], w);
			}
		}
	}
	return g;
}

#endif /* GRAPH_H_ */
