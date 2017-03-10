#pragma once

#include "Item.h"
#include "Destination.h"
#include "Graph.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <time.h>
#include <sys/timeb.h>

using namespace std;

//int GetMilliCount();
//int GetMilliSpan(int nTimeStart);
//int testes();

int GetMilliCount()
{
	timeb tb;
	ftime( &tb );
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

int GetMilliSpan(int nTimeStart)
{
  int nSpan = GetMilliCount() - nTimeStart;
  if (nSpan < 0)
	  nSpan += 0x100000 * 1000;
  return nSpan;
}

int testes()
{
	Graph<Destination> g1;
	Destination a = Destination("Rua A",0,0,1);
	Destination b = Destination("Rua B",0,0,2);
	Destination c = Destination("Rua C",0,0,3);
	Destination d = Destination("Rua D",0,0,4);
	Destination e = Destination("Rua E",0,0,5);
	Destination f = Destination("Rua F",0,0,6);
	Destination g = Destination("Rua G",0,0,7);
	Destination h = Destination("Rua H",0,0,8);
	Destination i = Destination("Rua I",0,0,9);
	Destination j = Destination("Rua J",0,0,10);
	Destination k = Destination("Rua K",0,0,11);
	Destination l = Destination("Rua L",0,0,12);
	Destination m = Destination("Rua M",0,0,13);
	Destination n = Destination("Rua N",0,0,14);
	Destination o = Destination("Rua O",0,0,15);
	Destination p = Destination("Rua P",0,0,16);
	Destination q = Destination("Rua Q",0,0,17);
	Destination r = Destination("Rua R",0,0,18);
	Destination s = Destination("Rua S",0,0,19);
	Destination t = Destination("Rua T",0,0,20);
	Destination u = Destination("Rua U",0,0,21);
	Destination v = Destination("Rua V",0,0,22);
	Destination w = Destination("Rua W",0,0,23);
	Destination x = Destination("Rua X",0,0,24);
	Destination y = Destination("Rua Y",0,0,25);
	Destination z = Destination("Rua Z",0,0,26);

	g1.addVertex(a);
	g1.addVertex(b);
	g1.addVertex(c);
	g1.addVertex(d);
	g1.addVertex(e);
	g1.addVertex(f);
	g1.addVertex(g);
	g1.addVertex(h);
	g1.addVertex(i);
	g1.addVertex(j);
	g1.addVertex(k);
	g1.addVertex(l);
	g1.addVertex(m);
	g1.addVertex(n);
	g1.addVertex(o);
	g1.addVertex(p);
	g1.addVertex(q);
	g1.addVertex(r);
	g1.addVertex(s);
	g1.addVertex(t);
	g1.addVertex(u);
	g1.addVertex(v);
	g1.addVertex(w);
	g1.addVertex(x);
	g1.addVertex(y);
	g1.addVertex(z);

	for(int i = 27; i <= 1500; i++){
		Destination dest = Destination("",0,0,i);
		g1.addVertex(dest);
	}

	g1.addEdge(a,b,5);
	g1.addEdge(b,a,3);
	g1.addEdge(a,c,1);
	g1.addEdge(a,d,2);
	g1.addEdge(b,d,6);
	g1.addEdge(c,b,11);
	g1.addEdge(c,e,1);
	g1.addEdge(d,c,6);
	g1.addEdge(e,b,1);
	g1.addEdge(e,f,4);
	g1.addEdge(f,e,4);
	g1.addEdge(a,h,5);
	g1.addEdge(h,g,3);
	g1.addEdge(g,c,2);
	g1.addEdge(b,j,7);
	g1.addEdge(d,j,6);
	g1.addEdge(j,i,3);
	g1.addEdge(i,a,9);
	g1.addEdge(i,k,3);
	g1.addEdge(k,i,3);
	g1.addEdge(l,i,3);
	g1.addEdge(i,l,3);
	g1.addEdge(l,s,1);
	g1.addEdge(s,m,11);
	g1.addEdge(s,n,11);
	g1.addEdge(m,n,8);
	g1.addEdge(n,a,4);
	g1.addEdge(n,z,8);
	g1.addEdge(m,o,4);
	g1.addEdge(o,p,3);
	g1.addEdge(p,q,8);
	g1.addEdge(q,r,2);
	g1.addEdge(r,p,1);
	g1.addEdge(r,q,3);
	g1.addEdge(r,s,4);
	g1.addEdge(s,q,5);
	g1.addEdge(q,a,6);
	g1.addEdge(b,t,7);
	g1.addEdge(b,u,1);
	g1.addEdge(u,v,10);
	g1.addEdge(v,c,11);
	g1.addEdge(v,w,3);
	g1.addEdge(w,m,4);
	g1.addEdge(w,x,2);
	g1.addEdge(x,z,6);
	g1.addEdge(z,y,2);
	g1.addEdge(y,z,2);
	g1.addEdge(y,d,2);
	g1.addEdge(x,d,15);
	g1.addEdge(x,l,15);

	cout << "Num of vertices of g: " << g1.getNumVertex() << endl;

	//Points of Interest
	vector<Destination> POI;
	POI.push_back(a);
	POI.push_back(d);

	POI.push_back(b);
	POI.push_back(c);
	POI.push_back(e);
	POI.push_back(f);
	POI.push_back(g);
	POI.push_back(h);
	POI.push_back(j);
	POI.push_back(k);
	POI.push_back(l);
	//POI.push_back(m);
	//POI.push_back(n);
	//POI.push_back(o);
	//POI.push_back(p);
	//POI.push_back(q);
	//POI.push_back(r);

	/////////////////////////////////////////////////////////////////////////////////
	//FLOYD
	/////////////////////////////////////////////////////////////////////////////////

	int nTimeStart = GetMilliCount();

	Graph<Destination> g2 = g1.makeGraphWithPOI(POI);
	cout << "Num of vertices of g2: " << g2.getNumVertex() << endl;
	if (g2.getNumVertex() == 0) return 1;

	int nTimeElapsed = GetMilliSpan( nTimeStart );
	cout << nTimeElapsed << " milliseconds" << endl << endl;

	/////////////////////////////////////////////////////////////////////////////////
	//GREEDY
	/////////////////////////////////////////////////////////////////////////////////

	nTimeStart = GetMilliCount();

	Solution<Destination> sol2 = g2.getShortestDeliveryPathGreedy(a, d);
	cout << "solution total length: " << sol2.pathLength << endl;
	cout << "solution path: ";
	for(unsigned int i = 0; i < sol2.path.size()-1; i++){
		cout << sol2.path[i]->getInfo() << ", ";
	}
	cout << sol2.path[sol2.path.size()-1]->getInfo() << endl;

	nTimeElapsed = GetMilliSpan( nTimeStart );
	cout << nTimeElapsed << " milliseconds" << endl << endl;

	/////////////////////////////////////////////////////////////////////////////////
	//BRUTE FORCE
	/////////////////////////////////////////////////////////////////////////////////

	nTimeStart = GetMilliCount();

	Solution<Destination> sol1 = g2.getShortestDeliveryPathBruteForce(a, d);
	cout << "solution total length: " << sol1.pathLength << endl;
	cout << "solution path: ";
	for(unsigned int i = 0; i < sol1.path.size()-1; i++){
		cout << sol1.path[i]->getInfo() << ", ";
	}
	cout << sol1.path[sol1.path.size()-1]->getInfo() << endl;

	nTimeElapsed = GetMilliSpan( nTimeStart );
	cout << nTimeElapsed << " milliseconds" << endl << endl;

	/////////////////////////////////////////////////////////////////////////////////

	return 0;
}
