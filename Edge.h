#ifndef EDGE_PROJECT

#define EDGE_PROJECT

/** @file Edge.h */

/** @class Edge
* An Edge class for graph implementations. */
template<typename T,typename W>
class Edge
{
public:
	T v, w;
	W weight;
	Edge(T firstVertex, T secondVertex, W  edgeWeight)
	{
		v = firstVertex;
		w = secondVertex;
		weight = edgeWeight;
	}  // end constructor
}; // end Edge
   // End of header file

#endif
