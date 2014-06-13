#include "LevelContextCommand.h"
#include "LevelToolContext.h"
#include <maya/MSyntax.h>
#include <maya/MArgParser.h>
#include <maya/MDoubleArray.h>
#include <maya/MPxCommand.h>

 MPxContext* LevelContextCommand::makeObj()
 {
	levelContext = new LevelToolContext();
	return levelContext;
 }

 void* LevelContextCommand::creator()
 {
 	return new LevelContextCommand;
 }

 MStatus LevelContextCommand::doEditFlags(){
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

		levelContext->setRedColor(inputR);
		levelContext->setGreenColor(inputG);
		levelContext->setBlueColor(inputB);
	}

	if (argData.isFlagSet(kThickFlag)) {
		double input;
		status = argData.getFlagArgument(kThickFlag, 0, input);
		if (!status) {
			status.perror("thick flag parsing failed");
			return status;
		} 

		levelContext->setThick(input);
	}

	if (argData.isFlagSet(kBrushFlag)) {
		int input;
		status = argData.getFlagArgument(kBrushFlag, 0, input);
		if (!status) {
			status.perror("brush flag parsing failed");
			return status;
		} 

		levelContext->setBrush(input);
	}

	if (argData.isFlagSet(kTransparencyFlag)) {
		double input;
		status = argData.getFlagArgument(kTransparencyFlag, 0, input);
		if (!status) {
			status.perror("transparency flag parsing failed");
			return status;
		} 

		levelContext->setTransparency(input);
	}

	if (argData.isFlagSet(kLevelFlag)) {
		double input;
		status = argData.getFlagArgument(kLevelFlag, 0, input);
		if (!status) {
			status.perror("level flag parsing failed");
			return status;
		} 

		levelContext->setLevel(input);
	}

	if (argData.isFlagSet(kSpacingFlag)) {
		double input;
		status = argData.getFlagArgument(kSpacingFlag, 0, input);
		if (!status) {
			status.perror("spacing flag parsing failed");
			return status;
		} 

		levelContext->setSpacing(input);
	}

	 return status;
 }

  MStatus LevelContextCommand::doQueryFlags(){
	 MStatus status = MS::kSuccess;

	 MArgParser argData = parser();

	if (argData.isFlagSet(kColorFlag)) {
		MDoubleArray results = MDoubleArray();
		results.append((double) levelContext->getRedColor());
		results.append((double) levelContext->getGreenColor());
		results.append((double) levelContext->getBlueColor());

		MPxCommand::setResult(results);    
	}

	if (argData.isFlagSet(kThickFlag)) {
		setResult((double) levelContext->getThick());
	}

	if (argData.isFlagSet(kBrushFlag)) {
		setResult((int) levelContext->getBrush());
	}

	if (argData.isFlagSet(kTransparencyFlag)) {
		setResult((double) levelContext->getTransparency());
	}

	if (argData.isFlagSet(kLevelFlag)) {
		setResult((double) levelContext->getLevel());
	}

	if (argData.isFlagSet(kSpacingFlag)) {
		setResult((double) levelContext->getSpacing());
	}

	 return status;
 }

  MStatus LevelContextCommand::appendSyntax(){
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
		if (MS::kSuccess != mySyntax.addFlag(kLevelFlag, kLevelFlagLong, MSyntax::kDouble)) {
                return MS::kFailure;
        }
		if (MS::kSuccess != mySyntax.addFlag(kSpacingFlag, kSpacingFlagLong, MSyntax::kDouble)) {
                return MS::kFailure;
        }

        return MS::kSuccess;
  }