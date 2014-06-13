#include "FeatherContextCommand.h"
#include "FeatherToolContext.h"
#include <maya/MSyntax.h>
#include <maya/MArgParser.h>
#include <maya/MDoubleArray.h>
#include <maya/MPxCommand.h>

 MPxContext* FeatherContextCommand::makeObj()
 {
	featherContext = new FeatherToolContext();
	return featherContext;
 }

 void* FeatherContextCommand::creator()
 {
 	return new FeatherContextCommand;
 }

 MStatus FeatherContextCommand::doEditFlags(){
	 MStatus status = MS::kSuccess;

	 MArgParser argData = parser();

	if (argData.isFlagSet(kColorFlag)) {
		double inputR, inputG, inputB;
		status = argData.getFlagArgument(kColorFlag, 0, inputR);
		status = argData.getFlagArgument(kColorFlag, 1, inputG);
		status = argData.getFlagArgument(kColorFlag, 2, inputB);
		if (!status) {
			status.perror("color flag parsing failed"); 
			return status;
		} 

		featherContext->setRedColor(inputR);
		featherContext->setGreenColor(inputG);
		featherContext->setBlueColor(inputB);
	}

	if (argData.isFlagSet(kThickFlag)) {
		double input;
		status = argData.getFlagArgument(kThickFlag, 0, input);
		if (!status) {
			status.perror("thick flag parsing failed");
			return status;
		} 

		featherContext->setThick(input);
	}

	if (argData.isFlagSet(kBrushFlag)) {
		int input;
		status = argData.getFlagArgument(kBrushFlag, 0, input);
		if (!status) {
			status.perror("brush flag parsing failed");
			return status;
		} 

		featherContext->setBrush(input);
	}

	if (argData.isFlagSet(kTransparencyFlag)) {
		double input;
		status = argData.getFlagArgument(kTransparencyFlag, 0, input);
		if (!status) {
			status.perror("transparency flag parsing failed");
			return status;
		} 

		featherContext->setTransparency(input);
	}

	if (argData.isFlagSet(kSpacingFlag)) {
		double input;
		status = argData.getFlagArgument(kSpacingFlag, 0, input);
		if (!status) {
			status.perror("spacing flag parsing failed");
			return status;
		} 

		featherContext->setSpacing(input);
	}

	return status;   
 }

  MStatus FeatherContextCommand::doQueryFlags(){  
	 MStatus status = MS::kSuccess;

	 MArgParser argData = parser();

	if (argData.isFlagSet(kColorFlag)) {
		MDoubleArray results = MDoubleArray();
		results.append((double) featherContext->getRedColor());
		results.append((double) featherContext->getGreenColor());
		results.append((double) featherContext->getBlueColor());

		MPxCommand::setResult(results);
	}

	if (argData.isFlagSet(kThickFlag)) {
		setResult((double) featherContext->getThick());
	}

	if (argData.isFlagSet(kBrushFlag)) {
		setResult((int) featherContext->getBrush());
	}

	if (argData.isFlagSet(kTransparencyFlag)) {
		setResult((double) featherContext->getTransparency());
	}

	if (argData.isFlagSet(kSpacingFlag)) {
		setResult((double) featherContext->getSpacing());
	}

	 return status; 
 }

  MStatus FeatherContextCommand::appendSyntax(){
	  MSyntax mySyntax = syntax();
        
        if (MS::kSuccess != mySyntax.addFlag(kColorFlag, kColorFlagLong, MSyntax::kDouble, MSyntax::kDouble, MSyntax::kDouble)) {
                return MS::kFailure;
        }
		if (MS::kSuccess != mySyntax.addFlag(kThickFlag, kThickFlagLong, MSyntax::kDouble)) {
                return MS::kFailure;
        }
		if (MS::kSuccess != mySyntax.addFlag(kBrushFlag, kBrushFlagLong, MSyntax::kUnsigned)) {
                return MS::kFailure;
        }
		if (MS::kSuccess != mySyntax.addFlag(kTransparencyFlag, kTransparencyFlagLong, MSyntax::kDouble)) {
                return MS::kFailure;
        }
		if (MS::kSuccess != mySyntax.addFlag(kSpacingFlag, kSpacingFlagLong, MSyntax::kDouble)) {
                return MS::kFailure;
        }

        return MS::kSuccess;
  }