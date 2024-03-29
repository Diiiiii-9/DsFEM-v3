/*
 * This header file is part of the tmesh library.  
 * This code was written by Florian Setzwein in 2020, 
 * and is covered under the MIT License
 * Refer to the accompanying documentation for details
 * on usage and license.
 */
#ifndef TMESH_TMNODE_H
#define TMESH_TMNODE_H

#include "tmTypedefs.h"


/**********************************************************
* tmNode: Node 
**********************************************************/
typedef struct tmNode {

  /*-------------------------------------------------------
  | Parent mesh properties
  -------------------------------------------------------*/
  tmMesh *mesh;

  /*-------------------------------------------------------
  | Node coordinates 
  -------------------------------------------------------*/
  tmDouble xy[2];
  tmIndex  index; /* Only used for the mesh-output       */
  tmBool   is_active; /* Every node is active by default */ 

  /*-------------------------------------------------------
  | Properties of this node and its state
  -------------------------------------------------------*/
  tmListNode *stack_pos;
  tmListNode *qtree_pos;
  tmQtree    *qtree;

  /*-------------------------------------------------------
  | Boundary properties if node is located on a boundary
  -------------------------------------------------------*/
  tmBool  on_bdry;
  tmList *bdry_edges;
  int     n_bdry_edges;

  /*-------------------------------------------------------
  | All adjacent front edges to this node 
  -------------------------------------------------------*/
  tmBool  on_front;
  tmList *front_edges;
  int     n_front_edges;

  /*-------------------------------------------------------
  | All mesh edges to this node 
  -------------------------------------------------------*/
  tmList *mesh_edges;
  int     n_mesh_edges;

  /*-------------------------------------------------------
  | All adjacent triangles to this node 
  -------------------------------------------------------*/
  tmList *tris;
  int     n_tris;

  /*-------------------------------------------------------
  | Buffer variables ( e.g. for sorting )
  -------------------------------------------------------*/
  tmDouble dblBuf;
  tmIndex  indBuf;

  /*-------------------------------------------------------
  | Size function properties if node is on boundary
  -------------------------------------------------------*/
  tmDouble rho;
  tmDouble k;

} tmNode;


/**********************************************************
* Function: tmNode_create()
*----------------------------------------------------------
* Create a new tmNode structure and returns a pointer
* to it.
*----------------------------------------------------------
* @param mesh: parent mesh of the new node
* @param xy:   Node coordinates
*
* @return: Pointer to a new tmNode structure
**********************************************************/
tmNode *tmNode_create(tmMesh *mesh, tmDouble xy[2]);

/**********************************************************
* Function: tmNode_destroy()
*----------------------------------------------------------
* Destroys a tmNode structure and frees all its memory.
*----------------------------------------------------------
* @param *mesh: pointer to a tmNode to destroy
*
* @return: Returns TRUE if method works
**********************************************************/
void tmNode_destroy(tmNode *node);

/**********************************************************
* Function: tmNode_getBdryEdgeIn()
*----------------------------------------------------------
* Returns a list of ingoing boundary edges
*----------------------------------------------------------
* @param *mesh: pointer to a tmNode 
*
* @return: tmList of tmEdges
**********************************************************/
tmList *tmNode_getBdryEdgeIn(tmNode *node);

/**********************************************************
* Function: tmNode_getBdryEdgeOut()
*----------------------------------------------------------
* Returns a list of outgoing boundary edges
*----------------------------------------------------------
* @param *mesh: pointer to a tmNode 
*
* @return: tmList of tmEdges
**********************************************************/
tmList *tmNode_getBdryEdgeOut(tmNode *node);

/**********************************************************
* Function: tmNode_getFrontEdgeIn()
*----------------------------------------------------------
* Returns a list of ingoing advancing front edges
*----------------------------------------------------------
* @param *mesh: pointer to a tmNode 
*
* @return: tmList of tmEdges
**********************************************************/
tmList *tmNode_getFrontEdgeIn(tmNode *node);

/**********************************************************
* Function: tmNode_getFrontEdgeOut()
*----------------------------------------------------------
* Returns a list of outgoing advancing front edges
*----------------------------------------------------------
* @param *mesh: pointer to a tmNode 
*
* @return: tmList of tmEdges
**********************************************************/
tmList *tmNode_getFrontEdgeOut(tmNode *node);

/**********************************************************
* Function: tmNode_getNbrsFromSizeFun
*----------------------------------------------------------
* Function to estimate neighbouring nodes of a given node
* within a range that is estimated from a sizefunction
*----------------------------------------------------------
* 
**********************************************************/
tmList *tmNode_getNbrsFromSizeFun(tmNode *node);

/**********************************************************
* Function: tmNode_getFrontNbrs
*----------------------------------------------------------
* Returns a list of tmNodes that are connected to a
* given tmNode through advancing front edges
*
*----------------------------------------------------------
* 
**********************************************************/
tmList *tmNode_getFrontNbrs(tmNode *node);

/**********************************************************
* Function: tmNode_getAdjFrontEdge()
*----------------------------------------------------------
* Checks for a node n wether it is connected to a 
* node m through an advancing front edge.
* If yes, the edge is returned - otherwise a NULL pointer
* is returned
*
*----------------------------------------------------------
* @param n: node whose advancing front edges are checked
* @param m: node which is search for on edges other ends
* 
**********************************************************/
tmEdge *tmNode_getAdjFrontEdge(tmNode *n, tmNode *m);

/**********************************************************
* Function: tmNode_isValid()
*----------------------------------------------------------
* Checks, if a node is valid for the mesh genereation 
* process. The node may not be located too close 
* to preceeding edges in its vicinity in order to be valid
*
*----------------------------------------------------------
* @param node: node to be checked
* 
**********************************************************/
tmBool tmNode_isValid(tmNode *node);

#endif
