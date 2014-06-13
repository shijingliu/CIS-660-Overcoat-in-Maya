#ifndef CreateHairContextCommand_H_
#define CreateHairContextCommand_H_

#include <maya/MStatus.h>
#include <maya/MPxContextCommand.h>
#include <maya/MEvent.h>
#include <maya/M3dView.h>
#include "vec.h"
#include "HairToolContext.h"

#define kColorFlag								"-c"
#define kColorFlagLong						"-color"
#define kThickFlag								"-th"
#define kThickFlagLong						"-thickness"
#define kBrushFlag								"-b"
#define kBrushFlagLong						"-brush"
#define kTransparencyFlag				"-tr"
#define kTransparencyFlagLong			"-transparency"
#define kSpacingFlag							"-s"
#define kSpacingFlagLong					"-spacing"

//added 
#define kBlendColorFlag                       "-bc"
#define kBlendColorFlagLong                   "-blendColor"
//added 

class HairContextCommand
	: public MPxContextCommand
 {
 public:

	/// \brief	This function needs to return a context for maya to use          
	/// 		when our tool requires input from the viewport.
	/// \return	a new instance of RectangleSelectTool     
	///
	virtual MPxContext* makeObj();

	//This method parses the command and allows editing of values in the context     
	virtual MStatus doEditFlags();
	//This method parses the command and allows querying of values in the context
	virtual MStatus doQueryFlags();
	//This method adds the syntax parsing
	virtual MStatus appendSyntax();

	/// \brief	This returns a new instance of the tool command for maya to use.
	/// \return	an instance of RectangleSelectContextCmd
	///
	static void*		creator();

 protected:
	HairToolContext* hairContext;
 };

#endif