#include "../include/tmesh/tmParam.h"
#include "../include/tmesh/tmNode.h"
#include "../include/tmesh/tmEdge.h"
#include "../include/tmesh/tmTri.h"
#include "../include/tmesh/tmBdry.h"
#include "../include/tmesh/tmMesh.h"
#include "../include/tmesh/tmQtree.h"
#include "../include/tmesh/tmFront.h"
#include "../include/tmesh/tmList.h"

#include "../include/tmesh/tinyexpr.h"

#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>


tmDouble x, y;
te_variable vars[] = {{"x", &x}, {"y", &y}};
te_expr *expr;

static inline tmDouble sizeFun( tmDouble xy[2] )
{
  x = xy[0];
  y = xy[1];
  return te_eval(expr);
}


/*************************************************************
* 
*************************************************************/
int mesh(char *input)
{
  tmParam *file;
  
  int i,j;

  tmSizeFunUser sizeFunUser;

  tmDouble  globSize;
  tmDouble *globBbox = NULL;
  int       qtreeSize;
  bstring   sizeFunExpr;

  tmDouble (*nodes)[2] = NULL;
  int        nNodes;

  int     (*extBdryEdges)[2]  = NULL;
  int      *extBdryEdgeMarker = NULL;
  tmDouble *extBdryRefinement = NULL;
  int       nExtBdryEdges;
  int       extBdryMarker;

  int     (**intrEdges)[2] = NULL;
  int      **intrEdgeMarkers = NULL;
  tmDouble **intrEdgeRefinements = NULL;
  int       *nIntrEdges = NULL;
  int       *intrBdryMarkers = NULL;
  int        nIntrBdrys = 0;
    

  /*----------------------------------------------------------
  | Load parameter file and clear comments
  ----------------------------------------------------------*/
  struct bstrList *buf;

  file = tmParam_create( input );
  buf  = tmParam_removeComments(file->txtlist, "#");
  bstrListDestroy(file->txtlist);
  file->txtlist = buf;

  /*----------------------------------------------------------
  | Extract mandatory meshing parameters
  ----------------------------------------------------------*/
  check( tmParam_extractParam(file->txtlist, 
       "Global element size:", 1, &globSize) != 0,
      "Missing parameter <Global element size:>");

  check( tmParam_extractArray(file->txtlist,
        "Mesh bounding box:", 1, &globBbox)  != 0,
      "Missing parameter <Mesh bounding box:>");

  /*----------------------------------------------------------
  | Extract optional meshing parameters
  ----------------------------------------------------------*/
  if ( tmParam_extractParam(file->txtlist, 
       "Number of quadtree elements:", 0, &qtreeSize) == 0 )
  {
    qtreeSize = 100;
  }

  if ( tmParam_extractParam(file->txtlist,
       "Size function:", 2, &sizeFunExpr) != 0 )
  {
    int err;
    expr = te_compile(sizeFunExpr->data, vars, 2, &err);
    check(err == 0, "Wrong size function definition");
    tmPrint("SIZE FUNCTION: %s", sizeFunExpr->data);
    sizeFunUser = sizeFun;
  }
  else {
    sizeFunExpr = NULL;
    sizeFunUser = NULL;
  }

  /*----------------------------------------------------------
  | Extract node definitions
  ----------------------------------------------------------*/
  tmParam_readNodeCoords(file->txtlist, &nodes, &nNodes);

  /*----------------------------------------------------------
  | Extract exterior boundary definition
  ----------------------------------------------------------*/
  tmParam_readExtBdryData(file->txtlist,
                          &extBdryEdges, 
                          &extBdryEdgeMarker, 
                          &extBdryRefinement, 
                          &nExtBdryEdges, 
                          &extBdryMarker);

  /*----------------------------------------------------------
  | Extract interior boundary definitions
  ----------------------------------------------------------*/
  tmParam_readIntBdryData(file->txtlist,
                          &intrEdges, 
                          &intrEdgeMarkers,
                          &intrEdgeRefinements,
                          &nIntrEdges, 
                          &intrBdryMarkers,
                          &nIntrBdrys);

  /*----------------------------------------------------------
  | Build the mesh
  ----------------------------------------------------------*/
  tmDouble xyMin[2] = { globBbox[0], globBbox[1] };
  tmDouble xyMax[2] = { globBbox[2], globBbox[3] };

  tmMesh *mesh = tmMesh_create(xyMin, xyMax, 
                               qtreeSize, 
                               globSize,
                               sizeFunUser);


  /*----------------------------------------------------------
  | Add nodes to the mesh
  ----------------------------------------------------------*/
  tmNode **nodes_ptr = calloc(nNodes, sizeof(tmNode*));
  tmNode  *newNode;

  for (i = 0; i < nNodes; i++)
  {
    newNode = tmNode_create(mesh, nodes[i]);
    nodes_ptr[i] = newNode;
  }

  /*----------------------------------------------------------
  | Add exterior boundary to the mesh
  ----------------------------------------------------------*/
  tmEdge *bdryEdge;
  tmBdry *bdry_ext = tmMesh_addBdry(mesh, 
                                    FALSE, 
                                    extBdryMarker);

  for (i = 0; i < nExtBdryEdges; i++)
  {
    int i0 = extBdryEdges[i][0];
    int i1 = extBdryEdges[i][1];

    check( i0 < nNodes && i1 < nNodes,
        "Defined nodes do not fit to exterior boundary definition.");

    bdryEdge = tmBdry_edgeCreate(bdry_ext, 
                                 nodes_ptr[i0],
                                 nodes_ptr[i1],
                                 extBdryEdgeMarker[i],
                                 extBdryRefinement[i]);
  }


  /*----------------------------------------------------------
  | Add interior boundaries to the mesh
  ----------------------------------------------------------*/
  tmBdry *bdry_int;
  for (j = 0; j < nIntrBdrys; j++)
  {
    bdry_int = tmMesh_addBdry(mesh, 
                              TRUE, 
                              intrBdryMarkers[j]);

    for (i = 0; i < nIntrEdges[j]; i++)
    {
      int i0 = intrEdges[j][i][0];
      int i1 = intrEdges[j][i][1];

      check( i0 < nNodes && i1 < nNodes,
          "Defined nodes do not fit to interior boundary with marker %d.",
          intrBdryMarkers[j]);

      bdryEdge = tmBdry_edgeCreate(bdry_int, 
                                   nodes_ptr[i0],
                                   nodes_ptr[i1],
                                   intrEdgeMarkers[j][i],
                                   intrEdgeRefinements[j][i]);
    }
  }

  /*--------------------------------------------------------
  | Create mesh
  --------------------------------------------------------*/
  clock_t tic_1 = clock();
  tmMesh_ADFMeshing(mesh);
  clock_t tic_2 = clock();

  /*--------------------------------------------------------
  | Print the mesh data 
  --------------------------------------------------------*/
  //tmMesh_printMesh(mesh);
  tmMesh_printMeshIncomflow(mesh);


  tmPrint("----------------------------------------------\n");
  tmPrint("TMesh finished\n");
  tmPrint("----------------------------------------------\n");
  tmPrint("Number of elements: %d\n", mesh->no_tris);
  tmPrint("Meshing time      : %e sec\n", 
      (double) (tic_2 - tic_1) / CLOCKS_PER_SEC );
  tmPrint("----------------------------------------------\n");

  tmMesh_destroy(mesh);



  te_free(expr);
  free(globBbox);
  bdestroy(sizeFunExpr);

  free(nodes);
  free(nodes_ptr);

  free(extBdryEdges);
  free(extBdryEdgeMarker);
  free(extBdryRefinement);

  for (i = 0; i < nIntrBdrys; i++)
  {
    free(intrEdges[i]);
    free(intrEdgeMarkers[i]);
    free(intrEdgeRefinements[i]);
  }
  free(intrEdges);
  free(intrEdgeMarkers);
  free(intrEdgeRefinements);
  free(nIntrEdges);
  free(intrBdryMarkers);

  tmParam_destroy( file );

  return 0;



error:

  te_free(expr);
  free(globBbox);
  bdestroy(sizeFunExpr);

  free(nodes);
  
  free(extBdryEdges);
  free(extBdryEdgeMarker);
  free(extBdryRefinement);

  for (i = 0; i < nIntrBdrys; i++)
  {
    free(intrEdges[i]);
    free(intrEdgeMarkers[i]);
    free(intrEdgeRefinements[i]);
  }
  free(intrEdges);
  free(intrEdgeMarkers);
  free(intrEdgeRefinements);
  free(nIntrEdges);
  free(intrBdryMarkers);

  tmParam_destroy( file );

  return 1;

}
