#ifndef CreateHairTool_H_
#define CreateHairTool_H_

#include <maya/MStatus.h>
#include <maya/MPxContext.h>
#include <maya/MEvent.h>
#include <maya/M3dView.h>
#include <maya/MVector.h>
#include <maya/MPoint.h>
#include "vec.h"
#include <vector>

class HairToolContext : public MPxContext{
 public:
	 //Constructor / Destructor
	HairToolContext();
	virtual ~HairToolContext();

	//Setup
	virtual void toolOnSetup( MEvent& event );
	virtual void getClassName(MString& name) const;

	//Mouse Events
	virtual MStatus doPress( MEvent& event );
	virtual MStatus doDrag( MEvent & event );
	virtual MStatus doRelease( MEvent & event );

	//Functions to implement later
	//virutal MStatus setCursor(const MCursor &cursor)   

	//Setters and Getters
	void setRedColor(float input);
	void setGreenColor(float input);
	void setBlueColor(float input);

	//added 
	void setBlendRedColor(float input); 
	void setBlendGreenColor(float input); 
	void setBlendBlueColor(float input);    
	//added 

	void setThick(float input);
	void setBrush(int input);
	void setTransparency(float input);
	void setSpacing(float input);

	float getRedColor();
	float getGreenColor();
	float getBlueColor();

	//added 
	float getBlendRedColor(); 
	float getBlendGreenColor(); 
	float getBlendBlueColor(); 
	//added 

	float getThick();
	int getBrush();
	float getTransparency();
	float getSpacing();
		
 private:
	 //House Keeping Variables
	 bool intersectionFound;
	 float tValue;
	 MVector normal;
	 MPoint point;
	static bool initalized;

	 //Tool Attributes
	 float red;
	 float green;
	 float blue;

	 //added 
	 float blendRed; 
	 float blendGreen; 
	 float blendBlue; 
	 //added 

	 float thick;
	 int brush;
	 float transparency;
	 float spacing;

	//List of Places clicked
	 short m_storage[2];
	 vector<vec2> screenPoints;
	 vector<MPoint> splinePoints;

	// The View we are doing the selection in.
	M3dView m_View;
};
#endif