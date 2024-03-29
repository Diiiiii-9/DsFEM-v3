/*
 * This header file is part of the tmesh library.  
 * This code was written by Florian Setzwein in 2020, 
 * and is covered under the MIT License
 * Refer to the accompanying documentation for details
 * on usage and license.
 */
#ifndef TMESH_TMFRONT_H
#define TMESH_TMFRONT_H

#include "tmTypedefs.h"

/**********************************************************
* tmBdry: Advancing front structure
**********************************************************/
typedef struct tmFront {

  /*-------------------------------------------------------
  | Parent mesh properties
  -------------------------------------------------------*/
  tmMesh *mesh;

  /*-------------------------------------------------------
  | Properties of the advancing front
  -------------------------------------------------------*/
  int   no_edges;

  /*-------------------------------------------------------
  | Advancing front edges
  -------------------------------------------------------*/
  tmEdge  *edges_head;
  tmList  *edges_stack;
  tmQtree *edges_qtree;

} tmFront;


/**********************************************************
* Function: tmFront_create()
*----------------------------------------------------------
* Create a new tmFront structure and returns a pointer
* to it.
*----------------------------------------------------------
* @return: Pointer to a new tmFront structure
**********************************************************/
tmFront *tmFront_create(tmMesh *mesh);

/**********************************************************
* Function: tmFront_destroy()
*----------------------------------------------------------
* Destroys a tmFront structure and frees all its
* memory.
*----------------------------------------------------------
* @param *mesh: pointer to a tmFront to destroy
*
**********************************************************/ 
void tmFront_destroy(tmFront *front);

/**********************************************************
* Function: tmFront_edgeCreate()
*----------------------------------------------------------
* Function to create a new edge on the advancing front
*----------------------------------------------------------
* @param front: advancing front structure to add edge to
* @param n1,n2: start / ending node defining the edge
* @param t:     triangle that is located to the right
*               of the new edge
* 
**********************************************************/
tmEdge *tmFront_edgeCreate(tmFront *front, 
                           tmNode  *n1, 
                           tmNode  *n2,
                           tmTri   *t);

/**********************************************************
* Function: tmFront_addEdge()
*----------------------------------------------------------
* Function to add an edge to a tmFront structure
* This edge is the new head of the front structure
*----------------------------------------------------------
* @param front: advancing front structure to add edge to
* @param n1,n2: start / ending node defining the edge
* @param t:     triangle that is located to the right
*               of the new edge
* 
**********************************************************/
tmListNode *tmFront_addEdge(tmFront *front, tmEdge *edge);

/**********************************************************
* Function: tmFront_remEdge()
*----------------------------------------------------------
* Function to remove an edge from a tmBdry structure
*----------------------------------------------------------
*
**********************************************************/
void tmFront_remEdge(tmFront *front, tmEdge *edge);

/**********************************************************
* Function: tmFront_init()
*----------------------------------------------------------
* Function to initialize the advancing front of a mesh
* --> Clone of all current boundaries
*----------------------------------------------------------
* 
**********************************************************/
void tmFront_init(tmMesh *mesh);

/**********************************************************
* Function: tmFront_sortEdges()
*----------------------------------------------------------
* Function to sort the advancing front edges 
* according to their length in ascending order 
*----------------------------------------------------------
* 
**********************************************************/
void tmFront_sortEdges(tmMesh *mesh);

/**********************************************************
* Function: tmFront_advance()
*----------------------------------------------------------
* Function to advance the front edges 
* by one step
*----------------------------------------------------------
* @parameter mesh:  pointer to mesh structure
* @parameter eStart: pointer to advancing front edge
*                    which will be advanced
* @return boolean:  if advancement was successfull
* 
**********************************************************/
tmBool tmFront_advance(tmMesh *mesh, tmEdge *e_ad);

/**********************************************************
* Function: tmFront_update()
*----------------------------------------------------------
* Function to update the front edges with a new node
*----------------------------------------------------------
* @param n: new node which is included in the front
* @param e: current edge which will be replaced
* 
**********************************************************/
void tmFront_update(tmMesh *mesh, 
                    tmNode *n, 
                    tmEdge *e, 
                    tmTri  *t);

/**********************************************************
* Function: tmFront_refine()
*----------------------------------------------------------
* Refine the edges of the advancing front structure 
* according to a size function
*----------------------------------------------------------
* @param *mesh: pointer to mesh containing the front
* @return: 
**********************************************************/
void tmFront_refine(tmMesh *mesh);

/**********************************************************
* Function: tmFront_splitEdge()
*----------------------------------------------------------
* Split an edge by inserting a new node on its centroid
* and splitting it into two new edges
* This edge will be pointing from its node n1 to the new
* node and a new edge will be created, which points
* from the new node to n2.
*----------------------------------------------------------
* @param *front: pointer to front
* @param *edge: pointer to a tmEdge 
* @return: pointer to newly created first edge
**********************************************************/
tmEdge *tmFront_splitEdge(tmFront *front, tmEdge *edge);

#endif
