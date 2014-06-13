#ifndef CreateOverCoatNode_H_
#define CreateOverCoatNode_H_

#include <maya/MPxNode.h>

class OverCoatNode : public MPxNode
{
public:
					OverCoatNode() {};
	virtual 		~OverCoatNode() {};
	virtual MStatus compute(const MPlug& plug, MDataBlock& data);
	static  void*	creator();
	static  MStatus initialize();

	static MObject	spline;
	static MObject color;
	static MObject thick;
	static MObject brush;
	static MObject	level;
	static MObject	transparency;   
	static MTypeId id;
	//added 
	static MObject spacing; 
//	static MObject ID; 
	//added 

	//House keeping variables     
	static int numOfSplinesCreated;
};
#endif