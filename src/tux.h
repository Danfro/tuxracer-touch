/* --------------------------------------------------------------------
EXTREME TUXRACER

Copyright (C) 1999-2001 Jasmin F. Patry (Tuxracer)
Copyright (C) 2010 Extreme Tuxracer Team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
---------------------------------------------------------------------*/

#ifndef TUX_H
#define TUX_H

#include "bh.h"
#include "mathlib.h"
#include <map>
#include <vector>

#define MAX_ACTIONS 8
#define	MAX_CHAR_NODES 256
#define	MAX_CHAR_MAT 32

#define MIN_SPHERE_DIV 3
#define MAX_SPHERE_DIV 16

struct TCharMaterial {
	TColor diffuse;
	TColor specular;
	float exp;
	string matline;
};

struct TCharAction {
	size_t num;
	int type[MAX_ACTIONS];
	TVector3d vec[MAX_ACTIONS];
	ETR_DOUBLE dval[MAX_ACTIONS];
	string name;
	string order;
	string mat;
};

struct TCharNode {
	TCharNode *parent;
	TCharNode *next;
	TCharNode *child;

	TCharAction* action;

	size_t node_idx;	// number in node_array
	size_t node_name;	// int identifier of node itself
	size_t parent_name;	// int identifier of parent
	size_t child_name;
	size_t next_name;

	string joint;
	TMatrix<4, 4> trans;
	TMatrix<4, 4> invtrans;
	ETR_DOUBLE radius;
	int divisions;
	TCharMaterial *mat;
	bool render_shadow;
	bool visible;
};

class CCharShape {
private:
	TCharNode *Nodes[MAX_CHAR_NODES];
	size_t Index[MAX_CHAR_NODES];
	size_t numNodes;
	bool useActions;
	bool newActions;
	vector<TCharMaterial> Materials;
	map<string, size_t> MaterialIndex;

	// nodes
	size_t GetNodeIdx (size_t node_name) const;
	TCharNode *GetNode (size_t node_name);
	void CreateRootNode ();
	bool CreateCharNode
	(int parent_name, size_t node_name, const string& joint,
	 const string& name, const string& order, bool shadow);
	bool VisibleNode (size_t node_name, float level);
	bool MaterialNode (size_t node_name, const string& mat_name);
	bool TransformNode(size_t node_name, const TMatrix<4, 4>& mat, const TMatrix<4, 4>& invmat);

	// material
	TCharMaterial* GetMaterial (const string& mat_name);
	void CreateMaterial (const string& line);

	// drawing
	void DrawCharSphere (int num_divisions);
	void DrawNodes (const TCharNode *node);
	TVector3d AdjustRollvector (const CControl *ctrl, const TVector3d& vel, const TVector3d& zvec);

	// collision
	bool CheckPolyhedronCollision(const TCharNode *node, const TMatrix<4, 4>& modelMatrix,
	                              const TMatrix<4, 4>& invModelMatrix, const TPolyhedron& ph);
	bool CheckCollision (const TPolyhedron& ph);

	// shadow
	void DrawShadowVertex(int& n, const TMatrix<4, 4>& mat);
	void DrawShadowSphere(const TMatrix<4, 4>& mat);
	void TraverseDagForShadow(const TCharNode *node, const TMatrix<4, 4>& mat);

	// testing and developing
	void AddAction (size_t node_name, int type, const TVector3d& vec, ETR_DOUBLE val);
public:
	CCharShape ();
	~CCharShape();
	bool useMaterials;
	bool useHighlighting;
	map<string, size_t> NodeIndex;

	// nodes
	bool ResetNode (size_t node_name);
	bool ResetNode (const string& node_trivialname);
	bool TranslateNode (size_t node_name, const TVector3d& vec);
	bool RotateNode (size_t node_name, int axis, ETR_DOUBLE angle);
	bool RotateNode (const string& node_trivialname, int axis, ETR_DOUBLE angle);
	void ScaleNode (size_t node_name, const TVector3d& vec);
	void ResetRoot () { ResetNode (0); }
	void ResetJoints ();

	// global functions
	void Reset ();
	void Draw ();
	void DrawShadow ();
	bool Load (const string& dir, const string& filename, bool with_actions);

	void AdjustOrientation (CControl *ctrl, bool eps,
	                        ETR_DOUBLE dist_from_surface, const TVector3d& surf_nml);
	void AdjustJoints (ETR_DOUBLE turnFact, bool isBraking,
	                   ETR_DOUBLE paddling_factor, ETR_DOUBLE speed,
	                   const TVector3d& net_force, ETR_DOUBLE flap_factor);
	bool Collision (const TVector3d& pos, const TPolyhedron& ph);

	// testing and tools
	bool   highlighted;
	size_t highlight_node;

	size_t GetNodeName (size_t idx) const;
	size_t GetNodeName (const string& node_trivialname) const;
	string GetNodeJoint (size_t idx) const;
	size_t GetNumNodes () const { return numNodes; }
	const string& GetNodeFullname (size_t idx) const;
	size_t GetNumActs (size_t idx) const;
	TCharAction *GetAction (size_t idx) const;
	void   PrintAction (size_t idx) const;
	void   PrintNode (size_t idx) const;
	void   RefreshNode (size_t idx);
	void   SaveCharNodes (const string& dir, const string& filename);
};

// only for char tools, the characters for playing are in
// CCharacter (game_ctrl)
extern CCharShape TestChar;

#endif
