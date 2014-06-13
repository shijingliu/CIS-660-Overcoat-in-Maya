#include "OverCoatNode.h"

#include <maya/MPxNode.h>
#include <maya\MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNurbsCurveData.h>

#include <maya/MTime.h>
#include <maya/MFnMeshData.h>
#include <maya/MFnMesh.h>
#include <maya\MGlobal.h>

MObject OverCoatNode::spline;
MObject OverCoatNode::color;
MObject OverCoatNode::thick;
MObject OverCoatNode::brush;
MObject OverCoatNode::level;
MObject OverCoatNode::transparency;
MTypeId OverCoatNode::id( 0x80000 );
//added 
MObject OverCoatNode::spacing; 
//MObject OverCoatNode::ID; 
//added 
int OverCoatNode::numOfSplinesCreated = 0;

#define McheckErr(stat,msg)			\
	if ( MS::kSuccess != stat ) {	\
		cerr << msg;				\
		return MS::kFailure;		\
	}   

void* OverCoatNode::creator()
{
	return new OverCoatNode;
}

MStatus OverCoatNode::initialize()
{
	MFnNumericAttribute numAttr;
	MFnUnitAttribute unitAttr;
	MFnTypedAttribute typedAttr;

	MStatus returnStatus;    

	//Create Attributes      
	OverCoatNode::spline = typedAttr.create( "spline", "spline", MFnNurbsCurveData::kNurbsCurve, &returnStatus );
	typedAttr.setWritable(true);
	typedAttr.setReadable(true);
	McheckErr(returnStatus, "ERROR creating OverCoatNode spline attribute\n");

	OverCoatNode::color = numAttr.createColor("color", "color",  &returnStatus );
	McheckErr(returnStatus, "ERROR creating OverCoatNode color attribute\n");
	//added     
	OverCoatNode::spacing = numAttr.create("spacing", "spacing", MFnNumericData::kDouble, 0.0, &returnStatus );
	McheckErr(returnStatus, "ERROR creating OverCoatNode spacing attribute\n");

	//added 
	OverCoatNode::thick = numAttr.create( "thick", "thick", MFnNumericData::kDouble, 0.0, &returnStatus ); 
	McheckErr(returnStatus, "ERROR creating OverCoatNode thick attribute\n");
	OverCoatNode::brush = numAttr.create( "brush", "brush", MFnNumericData::kInt, 0, &returnStatus ); 
	McheckErr(returnStatus, "ERROR creating OverCoatNode thick attribute\n");
	OverCoatNode::level = numAttr.create( "level", "level", MFnNumericData::kDouble, 0.0, &returnStatus );  
	McheckErr(returnStatus, "ERROR creating OverCoatNode level attribute\n");
	OverCoatNode::transparency = numAttr.create( "transparency", "transparency", MFnNumericData::kDouble, 0.0, &returnStatus );
	McheckErr(returnStatus, "ERROR creating OverCoatNode transparency attribute\n");
	typedAttr.setStorable(false);

	//Add Attributes     
	returnStatus = addAttribute(OverCoatNode::spline);
	McheckErr(returnStatus, "ERROR adding spline attribute\n");
	returnStatus = addAttribute(OverCoatNode::color);
	McheckErr(returnStatus, "ERROR adding color attribute\n");
	//added
	returnStatus = addAttribute(OverCoatNode::spacing);
	McheckErr(returnStatus, "ERROR adding spacing attribute\n");
	//added 
	returnStatus = addAttribute(OverCoatNode::thick);
	McheckErr(returnStatus, "ERROR adding thick attribute\n");
	returnStatus = addAttribute(OverCoatNode::brush);
	McheckErr(returnStatus, "ERROR adding brush attribute\n");
	returnStatus = addAttribute(OverCoatNode::level);
	McheckErr(returnStatus, "ERROR adding level attribute\n");
	returnStatus = addAttribute(OverCoatNode::transparency);
	McheckErr(returnStatus, "ERROR adding transparency attribute\n");

	//returnStatus = attributeAffects(OverCoatNode::color, OverCoatNode::spline);      
	//McheckErr(returnStatus, "ERROR in attributeAffects\n");          
	
	//connect different attributes with each spline here   
/*	returnStatus = attributeAffects(OverCoatNode::color,
								    OverCoatNode::spline);
	McheckErr(returnStatus, "ERROR in attributeAffects\n");   
	returnStatus = attributeAffects(OverCoatNode::thick,
								    OverCoatNode::spline);
	McheckErr(returnStatus, "ERROR in attributeAffects\n");
	returnStatus = attributeAffects(OverCoatNode::brush,
								    OverCoatNode::spline);
	McheckErr(returnStatus, "ERROR in attributeAffects\n");
	returnStatus = attributeAffects(OverCoatNode::level,
								    OverCoatNode::spline);
	McheckErr(returnStatus, "ERROR in attributeAffects\n");
	returnStatus = attributeAffects(OverCoatNode::transparency,
								    OverCoatNode::spline);
	McheckErr(returnStatus, "ERROR in attributeAffects\n"); */

	return MS::kSuccess;    
}

MStatus OverCoatNode::compute(const MPlug& plug, MDataBlock& data){
	MStatus returnStatus;


	//should I do projection here or write it in another node?   

	//cout<<"test computing."<<endl; 
	MGlobal::displayInfo(MString("Compute called"));

	return MS::kSuccess;
}