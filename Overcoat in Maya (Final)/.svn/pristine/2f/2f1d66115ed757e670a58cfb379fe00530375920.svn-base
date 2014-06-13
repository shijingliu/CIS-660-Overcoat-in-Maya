#include "HairToolContext.h"
#include <maya/MPxContext.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MGlobal.h>
#include <maya/MFnCamera.h>
#include <maya/MDagPath.h>
#include <maya/MFnMesh.h>
#include <maya/MPointArray.h>
#include <maya/MFloatPoint.h>
#include <maya/MItDag.h>
#include <stdio.h>
#include <maya/MToolsInfo.h>
#include "GL\glut.h"
#include "OverCoatNode.h"

bool HairToolContext::initalized = false;

HairToolContext::HairToolContext(){
	setTitleString("Hair Tool");
}

HairToolContext::~HairToolContext(){

}

void HairToolContext::toolOnSetup(MEvent& event){
	// set the help text in the maya help boxs
	setHelpString("Overcoat Hair Tool" );
	intersectionFound = false;
	if(HairToolContext::initalized == false){
		red = 0.0f;
		green = 0.0f;
		blue = 0.0f;
		thick = 1.0f;
		brush = 1;
		transparency = 0.0f;
		spacing = 1.0f;
		HairToolContext::initalized = true;
	}


}

void HairToolContext::getClassName(MString& name) const{
	name.set("overcoatHairTool");
}

MStatus HairToolContext::doPress( MEvent& event )
 {
	// if we have a left mouse click
	if(event.mouseButton() == MEvent::kLeftMouse)
	{
		//Our Viewer
		m_View = M3dView::active3dView();

		//Get Screen click position
		event.getPosition( m_storage[0], m_storage[1] );
		screenPoints = vector<vec2>();
		screenPoints.push_back(vec2(m_storage[0], m_storage[1]));
		//char buffer[200];
		//sprintf(buffer, "print \"%i, %i\\n\"", m_storage[0], m_storage[1]);
		//MGlobal::executeCommand(buffer);

		//Camera stuff
		MPoint origin = MPoint();
		MVector direction = MVector();
		m_View.viewToWorld(m_storage[0], m_storage[1], origin, direction);

		//Iterate through meshes in scene
		bool intersection = false;
		MPointArray points =  MPointArray();
		MIntArray polygonIds =  MIntArray();
		MItDag dagIter = MItDag(MItDag::kBreadthFirst, MFn::kInvalid);
		for( ; !dagIter.isDone(); dagIter.next() ){
			MDagPath dagPath;
			dagIter.getPath(dagPath);
			MFnDagNode dagNode( dagPath);

			//Object cannot be intermediate, it must be a mesh
			if( dagNode.isIntermediateObject() ) continue;
			if( !dagPath.hasFn(MFn::kMesh) ) continue;
			if( dagPath.hasFn(MFn::kTransform) ) continue;

			MGlobal::executeCommand(MString("print \"node is a mesh \\n\""));

			MFnMesh mesh = MFnMesh(dagPath);
			points =  MPointArray();
			polygonIds =  MIntArray();
			intersection = mesh.intersect(origin, direction, points, 1e-010, MSpace::kWorld, &polygonIds);
			
			if(intersection){
				break;
			}
		}
		
		if(intersection){
			intersectionFound = true;

			MDagPath dagPath;
			dagIter.getPath(dagPath);
			MFnMesh mesh = MFnMesh(dagPath);

			//Polygon Normal
			MVector polygonNormal;
			mesh.getPolygonNormal(polygonIds[0], polygonNormal, MSpace::kWorld);
			if(polygonNormal.normal().angle(direction.normal()) < 20.0f){
				//polygonNormal = mesh.get
			}

			//Camera Right
			m_View.getCamera(dagPath);
			MFnCamera camera(dagPath);
			MVector cameraRight = camera.rightDirection(MSpace::kWorld);
			
			//Resulting Plane
			//Point
			point = points[0];
			//Normal
			normal = cameraRight^polygonNormal;

			//pushback point
			splinePoints = vector<MPoint>();
			splinePoints.push_back(MPoint(points[0].x, points[0].y, points[0].z, points[0].w));

			/*//Calculate Tvalue
			tValue = (points[0].x - origin.x)/direction.x;*/
			
		}
		else{
			intersectionFound = false;
			MGlobal::executeCommand("print \" No Intersection \\n\"");
		}

		// yay!
		return MS::kSuccess;
	}

	// just let the base class handle the event*/
	return MPxContext::doPress(event);
 }

MStatus HairToolContext::doDrag( MEvent& event )
 {
	 if(intersectionFound){
		 //Our Viewer
		m_View = M3dView::active3dView();

		//Get Screen click position
		event.getPosition( m_storage[0], m_storage[1] );
		screenPoints.push_back(vec2(m_storage[0], m_storage[1]));

		//Camera stuff
		MPoint origin = MPoint();
		MVector direction = MVector();
		m_View.viewToWorld(m_storage[0], m_storage[1], origin, direction);

		float tValue = ((point - origin)*normal) / (direction * normal);
		MPoint newPoint = tValue*direction + origin;
		splinePoints.push_back(newPoint);

		//Draw GL curve
		m_View.beginXorDrawing(true, true, 1.0f, M3dView::kStippleDashed);
			glBegin(GL_LINE_STRIP );
				for ( unsigned i = 0; i < screenPoints.size() ; i++ ){
					glVertex2i( screenPoints[i][0], screenPoints[i][1] );
				 }
			glEnd();
		m_View.endXorDrawing();
	 }
	 return MPxContext::doDrag(event);
 }

MStatus HairToolContext::doRelease( MEvent& event )
 {
	// only bother handling the release of a left mouse button.
	if(event.mouseButton() != MEvent::kLeftMouse) {
		return MPxContext::doRelease(event);
	}
	else if(!intersectionFound){
		return MPxContext::doRelease(event);
	}
	else if(splinePoints.size() < 2){
		return MPxContext::doRelease(event);
	}

	OverCoatNode::numOfSplinesCreated++;
	MGlobal::executeCommand("print \" Intersection Detected \\n\"");

	char curveCreation[8000];
	sprintf(curveCreation, "curve", OverCoatNode::numOfSplinesCreated);
	for(int i = 0; i < splinePoints.size(); i++){
		char buffer[200];
		sprintf(buffer, " -p %f %f %f", splinePoints[i].x, splinePoints[i].y, splinePoints[i].z);
		strcat(curveCreation, buffer);

	}
	MString result;
	MGlobal::executeCommand(curveCreation, result);
	const char* curveName = result.asChar();
	int splineCount;
	sscanf(curveName, "curve%d", &splineCount);

	char overCoatNodeCreation[200];
	sprintf(overCoatNodeCreation, "createNode OverCoatNode -name OverCoatNode%d", splineCount);
	MGlobal::executeCommand(overCoatNodeCreation);

	char overCoatNodeSetAttrColor[200];
	sprintf(overCoatNodeSetAttrColor, "setAttr OverCoatNode%d.color -type \"double3\" %f %f %f", splineCount, red, green, blue);
	MGlobal::executeCommand(overCoatNodeSetAttrColor);

	char overCoatNodeSetAttrThick[200];
	sprintf(overCoatNodeSetAttrThick, "setAttr OverCoatNode%d.thick %f", splineCount, thick);
	MGlobal::executeCommand(overCoatNodeSetAttrThick);

	char overCoatNodeSetAttrBrush[200];
	sprintf(overCoatNodeSetAttrBrush, "setAttr OverCoatNode%d.brush %d", splineCount, brush);
	MGlobal::executeCommand(overCoatNodeSetAttrBrush);

	char overCoatNodeSetAttrTrans[200];
	sprintf(overCoatNodeSetAttrTrans, "setAttr OverCoatNode%d.transparency %f", splineCount, transparency);
	MGlobal::executeCommand(overCoatNodeSetAttrTrans);

	char overCoatNodeConnectAttr[200];
	sprintf(overCoatNodeConnectAttr, "connectAttr curve%d.worldSpace OverCoatNode%d.spline", splineCount, splineCount);
	MGlobal::executeCommand(overCoatNodeConnectAttr);

	char overCoatNodeSetAttrSpacing[200];
	sprintf(overCoatNodeSetAttrSpacing, "setAttr OverCoatNode%d.spacing %f", splineCount, spacing);
	MGlobal::executeCommand(overCoatNodeSetAttrSpacing);

	return MS::kSuccess;
}

void HairToolContext::setRedColor(float input){
	red = input;
	MToolsInfo::setDirtyFlag(*this);
}
void HairToolContext::setGreenColor(float input){
	green = input;
	MToolsInfo::setDirtyFlag(*this);
}
void HairToolContext::setBlueColor(float input){
	blue = input;
	MToolsInfo::setDirtyFlag(*this);
}
void HairToolContext::setThick(float input){
	thick = input;
	MToolsInfo::setDirtyFlag(*this);
}
void HairToolContext::setBrush(int input){
	brush = input;
	MToolsInfo::setDirtyFlag(*this);
}
void HairToolContext::setTransparency(float input){
	transparency = input;
	MToolsInfo::setDirtyFlag(*this);
}
void HairToolContext::setSpacing(float input){
	spacing = input;
	MToolsInfo::setDirtyFlag(*this);
}

float HairToolContext::getRedColor(){
	return red;
}
float HairToolContext::getGreenColor(){
	return green;
}
float HairToolContext::getBlueColor(){
	return blue;
}
float HairToolContext::getThick(){
	return thick;
}
int HairToolContext::getBrush(){
	return brush;
}
float HairToolContext::getTransparency(){
	return transparency;
}
float HairToolContext::getSpacing(){
	return spacing;
}
