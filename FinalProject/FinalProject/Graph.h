/** @file Graph.h */

#ifndef GRAPH_PROJECT

#define GRAPH_PROJECT

#include <vector>
#include <list>
#include <map>
#include "Edge.h"
#include "PrecondViolatedExcept.h"



/** An adjacency list representation of an undirected,
* weighted graph. */

template<typename T,typename W>
class Graph
{
	int numVertices;   /** Number of vertices in the graph. */
	int numEdges;      /** Number of edges in the graph. */

					   /** Adjacency list representation of the graph;
					   * the map pair consists of the second vertex (key)
					   * and the edge weight (value). */
	vector<map<T, W> > adjList;
	vector<T> vertices;	 /*  This vector stores all of the vertices in the graph           */

	/**
		Return if the vertex exists in the graph
	*/
	const bool vertexExists(const T &v);

	/**
		adds a new vertex and returns its location in the vertices vector
	*/
	const unsigned int addVertex(const T &v);

	/**
		returns the location of the vertex in the vertices vector
	*/
	const unsigned int vertexLoc(const T &v);

	
public:
	/** Constructor.
	* @pre The graph is empty.
	* @post The graph is initialized to hold n vertices. */
	Graph(const int n);

	/** Constructor.
	* @pre The graph is empty. */
	Graph();

	/** Constructor.
	* @pre The graph is empty.
	* @post The graph is initialized to hold n vertices. */
	void Init(const int n);

	/** Determines the number of vertices in the graph.
	* @pre None.
	* @post None.
	* @return The number of vertices in the graph. */
	const int getNumVertices() const;

	/** Determines the number of edges in the graph.
	* @pre None.
	* @post None.
	* @return The number of edges in the graph. */
	const int getNumEdges() const;

	/** Determines the weight of an edge.
	* @pre The edge exists in the graph.
	* @post None.
	* @return The weight of the edge parameter. */
	const int getWeight(const Edge<T, W>& e) const;

	/** Creates an edge in the graph.
	* @pre The vertices exist in the graph.
	* @post Adds to both v and w's list. */
	void add(const Edge<T, W>& e);

	/** Finds the vector connecting v and edge.
	* @pre The edge exists - throws exception if no edge exists
	* @post None.
	* @returns the edge that is used to connect between v and w. */
	const W  findEnd(const T &v, const W &weight);

}; // end Graph
   // End of header file


template<typename T, typename W>
const bool Graph<T, W>::vertexExists(const T &v)
{
	if (std::find(vertices.begin(), vertices.end(), v) != vertices.end()) {
		return true;
	}
	else {
		return false;
	}
	return false;
}

template<typename T, typename W>
const unsigned int Graph<T, W>::addVertex(const T &v)
{
	vertices.push_back(v);
	return static_cast<int>(vertices.size() - 1);
}

template<typename T, typename W>
 const unsigned int Graph<T, W>::vertexLoc(const T &v)
{
	 int i{ 0 };
	 vector<T>::iterator it;

	 for (it = vertices.begin(); it < vertices.end(); it++, i++)
	 {
		 if (*it == v)
		 {
			 break;
		 }
	 }
	 return i;
}

template<typename T,typename W>
Graph<T,W>::Graph(const int n)
{
	map<T, W> element;
	adjList.assign(n, element);
	numVertices = n;
	numEdges = 0;
}  // end constructor

template<typename T, typename W>
Graph<T, W>::Graph()
{
}

template<typename T, typename W>
void Graph<T, W>::Init(const int n)
{
	map<T, W> element;
	adjList.assign(n, element);
	numVertices = n;
	numEdges = 0;
}

template<typename T,typename W>
const int Graph<T,W>::getNumVertices() const
{
	return numVertices;
}  // end getNumVertices

template<typename T,typename W>
const int Graph<T,W>::getNumEdges() const
{
	return numEdges;
}  // end getNumEdges

template<typename T,typename W>
const int Graph<T,W>::getWeight(const Edge<T, W>& e) const
{
	return e.weight;
}  // end getWeight

template<typename T,typename W>
void Graph<T,W>::add(const Edge<T, W>& e)
{

	T v{ e.v }, w{ e.w };
	int vectorPos_1;

	// This will add the vertex if it doesn't exist
	if (!vertexExists(v))
	{
		vectorPos_1 = addVertex(v);
	}
	// Return the location of the vertex
	else
	{
		vectorPos_1 = vertexLoc(v);
	}


	W weight{ e.weight };
	adjList[vectorPos_1].insert(make_pair(w, weight));
	numEdges++;
}  // end add



template<typename T,typename W>
const W Graph<T,W>::findEnd(const T &v, const W &weight)
{
	int loc = vertexLoc(v);
	map<T, W>::iterator it;

	map<T, W> m = adjList[loc];
	for (it = m.begin(); it != m.end(); ++it)
		if (it->second == weight)
			return it->first;

	if (it == m.end())
	{
		throw PrecondViolatedExcept("findEdge Exception raised: No edge was found between the vertices");
	}
	return it->second;

}  // end findEdge


#endif