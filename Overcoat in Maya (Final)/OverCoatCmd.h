#ifndef OverCoatCmd_H_
#define OverCoatCmd_H_

#include <maya/MPxCommand.h>
#include <string>
#include "vec.h"
//added 
#include "EasyBMP.h"
//added 

class OverCoatCmd : public MPxCommand 
{                                                       
public:   
	    //added
	    BMP AnImage1;
		BMP AnImage2;
		BMP AnImage3;
		BMP AnImage4;
		BMP AnImage5;
		BMP AnImage6;
		BMP AnImage7;
		BMP AnImage8;
		BMP AnImage9;
		BMP AnImage10;
		BMP AnImage11;
		BMP AnImage12;
		BMP AnImage13;
		BMP AnImage14;
		BMP AnImage15;
		BMP AnImage16;
		BMP AnImage17;
		//added 
        OverCoatCmd() {};
        ~OverCoatCmd() {}; 
        virtual MStatus doIt ( const MArgList& );      
        static void*    creator() {return new OverCoatCmd();};
		//added 
		void Rendering (int Image_Height, int Image_Width,  string name, int frameNumber, int flag); 
		//added 
};


#endif