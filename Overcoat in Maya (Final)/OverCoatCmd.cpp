//-
// ==========================================================================
// Copyright 1995,2006,2008 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic 
// or hard copy form.
// ==========================================================================
//+

#include <maya/MIOStream.h>
#include <maya/MRenderView.h>
#include <maya/M3dView.h>
#include "OverCoatCmd.h"

#include "vec.h"
#include <vector>

#include <maya/MArgList.h>   
#include <maya/MTime.h>
#include <maya/MMatrix.h>
#include <maya/MFnMesh.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFnMeshData.h>
#include <maya/MPointArray.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MItDag.h>
#include <maya/MFnCamera.h>
#include <maya/MFnTransform.h>
#include <maya/MUintArray.h>
#include <maya/MFloatPoint.h>
#include <maya/MDagPath.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MItCurveCV.h>
#include <maya/MGlobal.h>

#include <maya/MItSelectionList.h>
#include <maya/MSelectionList.h>

#include <maya/MRenderUtil.h>
#include <maya/MCommonRenderSettingsData.h>

#include "OverCoatNode.h" 
#include <sstream>

#include <math.h>
#include <iostream>
#include <iomanip>

#define PI 3.14159265
#define MaximumNoOfSpline 2000
#define Sample_Points 100

//I know how to change rendering; will change it later on.      
MStatus OverCoatCmd::doIt( const MArgList& args )
{
		MStatus stat = MS::kSuccess;

		MGlobal::displayInfo(MString("Creating Overcoat Node"));  

		int flag = 0; 
		for(int i = 0; i < args.length(); ++i)
		{
			//check for a valid arguments 
			if(args.asString(i, &stat) == "-flag")
			{
				MString para1 = args.asString(++i); 
				flag = para1.asInt(); 
			}
		}


		MCommonRenderSettingsData data;
        MRenderUtil::getCommonRenderSettings(data);

		int Image_Height = data.height;
		int Image_Width= data.width; 
		string n = data.name.asChar();
		if(flag == 1)
		{
		  Rendering(Image_Height, Image_Width, n, 0, flag); 
		  MString cmd = MString ("RenderViewWindow;");
		  MGlobal::executeCommand(cmd); 
		}

		if(flag == 2)
		{
			for(int i = 0; i < 360; i = i + 3)
			{
			   Rendering(Image_Height, Image_Width, n, i, flag);   
			}
		}
		   
        return stat;
}



void OverCoatCmd::Rendering(int Image_Height, int Image_Width, string name, int frameNum, int flag)
{
		//define the background color   
			double backgroundColorR = 255; 
			double backgroundColorG = 255; 
			double backgroundColorB = 255; 

			//define several pictures 
			AnImage1.ReadFromFile("C:/CIS 660/BrushType/type7.bmp");
			AnImage2.ReadFromFile("C:/CIS 660/BrushType/type8.bmp");
			AnImage3.ReadFromFile("C:/CIS 660/BrushType/type9.bmp");
			AnImage4.ReadFromFile("C:/CIS 660/BrushType/type10.bmp");

			AnImage5.ReadFromFile("C:/CIS 660/BrushType/type16.bmp");
			AnImage6.ReadFromFile("C:/CIS 660/BrushType/type17.bmp");
			AnImage7.ReadFromFile("C:/CIS 660/BrushType/type18.bmp");
			AnImage8.ReadFromFile("C:/CIS 660/BrushType/type19.bmp");

			AnImage9.ReadFromFile("C:/CIS 660/BrushType/type20.bmp");
			AnImage10.ReadFromFile("C:/CIS 660/BrushType/type21.bmp");
			AnImage11.ReadFromFile("C:/CIS 660/BrushType/type22.bmp");
			AnImage12.ReadFromFile("C:/CIS 660/BrushType/type23.bmp");

			AnImage13.ReadFromFile("C:/CIS 660/BrushType/type24.bmp");
			AnImage14.ReadFromFile("C:/CIS 660/BrushType/type25.bmp");
			AnImage15.ReadFromFile("C:/CIS 660/BrushType/type26.bmp");
			AnImage16.ReadFromFile("C:/CIS 660/BrushType/type27.bmp");

			AnImage17.ReadFromFile("C:/CIS 660/BrushType/type28.bmp");

		   //get the view direction and eye position         
			MFloatVector view_dir; 
			MFloatPoint eye_pos;
			MItDag it1(MItDag::kDepthFirst, MFn::kCamera); 
			MVector upDirection; 
			MVector rightDirection; 
			double horizontalFOV; 
			double verticalFOV;     

			MFloatMatrix projectionMat;   
			//go through all the camera in the maya and only execute the perspective camera   
			if(flag == 1)
			{
				for ( ; !it1.isDone(); it1.next() )
				{   
					MDagPath campath;   
					it1.getPath(campath); 
					MFnCamera fn(campath);
	
					MString persp = "perspShape"; 
					if(fn.name()==persp)
					{
						view_dir=MFloatVector(fn.viewDirection(MSpace::kWorld));
						eye_pos=MFloatPoint(fn.eyePoint(MSpace::kWorld));       

						upDirection = fn.upDirection(MSpace::kWorld, 0); 
			    		rightDirection = fn.rightDirection(MSpace::kWorld, 0); 

						fn.getPortFieldOfView(Image_Width, Image_Height, horizontalFOV, verticalFOV); 

						projectionMat = fn.projectionMatrix(0); 
					}
				}
			}

			if(flag == 2)
			{
				eye_pos = MFloatPoint (40*sin(frameNum*PI/180.0), 0, 40*cos(frameNum*PI/180.0)); 
				vec3 nor = vec3 (-eye_pos[0], -eye_pos[1], -eye_pos[2]).Normalize(); 
				view_dir = MFloatVector(nor[0], nor[1], nor[2]); 
				upDirection = MVector (0, 1, 0);
			}

			//cout<<"horizontalFOV:"<<horizontalFOV<<endl;     
			//cout<<"verticalFOV"<<verticalFOV<<endl; 

			/*
			cout<<"camera:"<<eye_pos.x<<","<<eye_pos.y<<","<<eye_pos.z<<endl;              
			cout<<"view dir:"<<view_dir.x<<","<<view_dir.y<<","<<view_dir.z<<endl;       
			cout<<"upDirection:"<<upDirection.x<<","<<upDirection.y<<","<<upDirection.z<<endl;       
			cout<<"rightDirection:"<<rightDirection.x<<","<<rightDirection.y<<","<<rightDirection.z<<endl; 
			*/

			//go through all the spline (CVcurves) in the maya view and record all of them    
			//define the color and thickness here  

			//vector<MFnNurbsCurve> splinesNode (MaximumNoOfSpline); 

			vector<double> spacing (MaximumNoOfSpline); 
			vector<double> transparency (MaximumNoOfSpline); 
			vector<int> brush (MaximumNoOfSpline); 
			vector<double> thickness(MaximumNoOfSpline); 
			vector<MDoubleArray> color (MaximumNoOfSpline); 

			//add depth attribute here 
			vector<double> depth (MaximumNoOfSpline); 
			vector<int> depthIndex (MaximumNoOfSpline); 

			for(int i = 0; i < MaximumNoOfSpline; i++)
			{
				color[i].setLength(4); 
			}
			//define the points and parameter array 
			int index = 0; 
			vector <int> numCVs(1000); 
			vector <MPointArray> splines (MaximumNoOfSpline); 
			for (int i = 0; i < MaximumNoOfSpline; i++)
			{
				splines[i].setLength(30000); 
			}

			vector <MPointArray> splinesPara (MaximumNoOfSpline); 
			for (int i = 0; i < MaximumNoOfSpline; i++)
			{
				splinesPara[i].setLength(30000); 
			}

			//define the starting points, end points, start parameter, and end parameter of a spline.    
			//And initialize them all to 0
			MPointArray StartPoint (MaximumNoOfSpline, MPoint::origin); 
			MPointArray EndPoint (MaximumNoOfSpline, MPoint::origin); 
			MPointArray MidPoint (MaximumNoOfSpline, MPoint::origin); 

			double StartPara[MaximumNoOfSpline]; 
			double EndPara[MaximumNoOfSpline]; 
			for (int i = 0; i < MaximumNoOfSpline; i++)
			{
				StartPara[i] = 0.0; 
				EndPara[i] = 0.0; 
			}

			//definition of splines after depth sorts    
		    vector<MPointArray> depthSplines (MaximumNoOfSpline); 
			for (int i = 0; i < MaximumNoOfSpline; i++)
			{
				depthSplines[i].setLength(40000); 
			}

			vector <MPointArray> depthSplinesPara (MaximumNoOfSpline); 
			for (int i = 0; i < MaximumNoOfSpline; i++)
			{
				depthSplinesPara[i].setLength(40000); 
			}

			double DepthStartPara[MaximumNoOfSpline]; 
			double DepthEndPara[MaximumNoOfSpline]; 
			for (int i = 0; i < MaximumNoOfSpline; i++)
			{
				DepthStartPara[i] = 0.0; 
				DepthEndPara[i] = 0.0; 
			}

			
			//correct the name for each OverCoatNode   
			MItDag itCo(MItDag::kDepthFirst, MFn::kNurbsCurve); 
			int nodeIndex = 0; 
			for ( ; !itCo.isDone(); itCo.next() )   
			{
				MDagPath curvepath;
				itCo.getPath(curvepath);               
				MFnNurbsCurve curveFn (curvepath); 
				int NodeNum = nodeIndex +1; 
				for(int w = NodeNum; w <= OverCoatNode::numOfSplinesCreated; w++)
				{
					int result; 
					MString firstPart("objExists OverCoatNode"); 
					MString cmd = firstPart + w; 
					MGlobal::executeCommand(cmd, result, true, true); 
					if(result == 1)
					{
						MString RNfirst ("rename ");
						MString rn = RNfirst + "OverCoatNode" + w + " OverCoatNode" + NodeNum; 
						MGlobal::executeCommand(rn); 
						break; 
					}
				}
				nodeIndex ++; 
			}

			MItDag it(MItDag::kDepthFirst, MFn::kNurbsCurve);
			//go through all the spline (CVcurves) in the maya view and record all of them     
			for ( ; !it.isDone(); it.next() )   
			{
				MDagPath curvepath;
				it.getPath(curvepath);        
				//definition for curves            
				MFnNurbsCurve curveFn (curvepath); 
				//numCVs[index] = curveFn.numCVs();    
				
				//cout<<"name:"<<curveFn.name().asChar()<<endl;  

				curveFn.getCVs (splines[index], MSpace::kWorld);
				int LEN = splines[index].length();
		
				StartPoint[index] = splines[index][0];
				EndPoint[index] = splines[index][LEN-1]; 

				//get the middle points here 
				MidPoint[index] = (StartPoint[index] + EndPoint[index])/2.0; 

				//grab each parameters according to start and end points here 
				curveFn.getParamAtPoint(StartPoint[index], StartPara[index], MSpace::kObject); 
				curveFn.getParamAtPoint(EndPoint[index], EndPara[index], MSpace::kObject); 

				int arrIndex = 0;       
				//for (double u = StartPara[index]; u <=EndPara[index]; u = u+0.001)       
				double step = 0.01; 
				for (double u = StartPara[index]; u <=EndPara[index]; u = u+step)
				{
					MPoint CurrentPoint;    

					curveFn.getPointAtParam(u, CurrentPoint, MSpace::kObject); 
					splinesPara[index][arrIndex].x= CurrentPoint.x; 
					splinesPara[index][arrIndex].y= CurrentPoint.y;     
					splinesPara[index][arrIndex].z= CurrentPoint.z; 
					arrIndex++;   
				}   
			    index++; 
		   }  

			//figure out the depth for each splines right now    
		    for (int i = 0; i < index; i++ )
			{
				vec3 MidVector = vec3 (MidPoint[i][0] - eye_pos[0], MidPoint[i][1] - eye_pos[1], MidPoint[i][2] - eye_pos[2]); 
				vec3 V = vec3 (view_dir.x, view_dir.y, view_dir.z);  
				double midDotv = Dot (MidVector, V); 
				vec3 MidProjection = midDotv*V; 

				depth[i] = MidProjection.Length(); 
				
			}

			//sort index again according to depth 
			for(int i = 0; i < index; i++)
			{
				 depthIndex[i] = 0; 
			     for (int j = 0; j < index; j++)
				 {
					 if(depth[i] < depth [j])
					 {
						 depthIndex[i]++; 
					 }
				 }
			}

			for(int i = 0; i < index; i++)
			{   
			    //find out the correspoinding num ith in the depth index   
				int j = 0; 
				for(j = 0; j < index; j++)
				{
					if(i == depthIndex[j])
					{
						break; 
					}
				}

				DepthStartPara[i] = StartPara[j]; 
				DepthEndPara[i] = EndPara[j]; 
				int arrIndex = 0; 
				double step = 0.01; 
				for (double u = DepthStartPara[i]; u <=DepthEndPara[i]; u = u+step)
				{ 
				   depthSplinesPara[i][arrIndex].x = splinesPara[j][arrIndex].x; 
				   depthSplinesPara[i][arrIndex].y = splinesPara[j][arrIndex].y;     
				   depthSplinesPara[i][arrIndex].z = splinesPara[j][arrIndex].z; 
				   arrIndex++;   
				}


				MString firstPart ("getAttr OverCoatNode"); 
				MString thic = firstPart + (j+1) + ".thick";    

				//obtain thickness here 
				MGlobal::executeCommand(thic, thickness[i], true, true);     

				//obtain color here    
				MString colo = firstPart + (j+1) + ".color"; 
				MGlobal::executeCommand(colo, color[i], true, true);  

				//obtain brush here 
				MString brus = firstPart + (j+1) + ".brush"; 
				MGlobal::executeCommand(brus, brush[i], true, true); 

				//obtain transparency here 
				MString trans = firstPart + (j+1) + ".transparency"; 
				MGlobal::executeCommand(trans, transparency[i], true, true); 

				//obtain spacing here 
				MString spac = firstPart + (j+1) + ".spacing"; 
				MGlobal::executeCommand(spac, spacing[i], true, true);
			}

	
		// cout<<"num of cv curves:"<< index <<endl;      

		//start to render in the maya render view port.     
        if (!MRenderView::doesRenderEditorExist())
        {
                displayError( 
                        "Cannot renderViewInteractiveRender in batch render mode.\n"
                        "Run in interactive mode, so that the render editor exists." );
              // return MS::kFailure;
        }

        if (!MRenderView::doesRenderEditorExist())
        {
                displayError( 
                        "Cannot renderViewInteractiveRender in batch render mode.\n"
                        "Run in interactive mode, so that the render editor exists." );   
               // return MS::kFailure;
        }

		MRenderView::startRender(Image_Width, Image_Height, false, false);
		RV_PIXEL* pixels = new RV_PIXEL[Image_Height*Image_Width];
		int indexPixel = 0;   

		//initialize the background color with white             
		for (int i = 0;  i < Image_Width; i++)
		{
			for (int j = 0; j< Image_Height; j++)
			{
				pixels[j*Image_Width+i].r = backgroundColorR;
				pixels[j*Image_Width+i].g = backgroundColorG;
				pixels[j*Image_Width+i].b = backgroundColorB; 
			    
			}
		}
	
		//some difinition for setting up the coordinate systems 
		double totalLength = 630 * (double) (Image_Width/640.0); 
		double tanTheta = (Image_Height/2.0)/totalLength; 
		double tanPhi = (Image_Width/2.0)/totalLength; 
		vec3 C (view_dir.x*totalLength, view_dir.y*totalLength, view_dir.z*totalLength); 
		vec3 M (eye_pos.x + C[0], eye_pos.y + C[1], eye_pos.z + C[2]); 
		vec3 U (upDirection[0], upDirection[1], upDirection[2]); 
		vec3 A = C.Cross(U); 
		vec3 B = A.Cross(C); 
		double magA = A.Length(); 
		double magB = B.Length(); 
		double magC = C.Length(); 
		double scalerH = magC*tanPhi/magA; 
		double scalerV = magC*tanTheta/magB; 
		vec3 H = scalerH*A; 
		vec3 V = scalerV*B;

		//project each points to the plane               
		for(int i = 0; i<index; i++)
		{
			//update pixel in the maya viewport  
			if(brush[i] == 1) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j++)   
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j++)  
				{
					//calculating the projection points   
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z);  
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen   
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 

						if(((xAxis - thickness[i]) > 0)&&((xAxis + thickness[i]) < Image_Width) && ((yAxis - thickness[i]) > 0)&&((yAxis + thickness[i]) < Image_Height))
						{
							//if thickness is less than 3 do nothing    
							if(thickness[i]<3)
							{
								for(int k = 0; k < thickness[i]; k++)   
								{
									int getIndex = (yAxis + k)*Image_Width + (xAxis); 

									pixels[getIndex].r = color[i][0]*255;
									pixels[getIndex].g = color[i][1]*255;
									pixels[getIndex].b = color[i][2]*255;       	
								}
							}
							//if thickness is greater than 3 start brushes    
							else
							{
								int middle = thickness[i]/2;
								for(int k = middle; k >= 0; k--)
								{
										int getIndex = (yAxis + k)*Image_Width + (xAxis); 
										double blendVal = 0.99 -(middle-k)*0.05;  

										pixels[getIndex].r = color[i][0]*255 * blendVal + (1-blendVal) * backgroundColorR;  
										pixels[getIndex].g = color[i][1]*255 * blendVal + (1-blendVal) * backgroundColorG;  
										pixels[getIndex].b = color[i][2]*255 * blendVal + (1-blendVal) * backgroundColorB; 
								
								}
								for(int k = middle+1; k < thickness[i]; k++)
								{
									   int getIndex = (yAxis + k)*Image_Width + (xAxis); 
									   double blendVal = 0.99 -(k-middle-1)*0.05; 

									   pixels[getIndex].r = color[i][0]*255 * blendVal + (1-blendVal) * backgroundColorR;  
									   pixels[getIndex].g = color[i][1]*255 * blendVal + (1-blendVal) * backgroundColorG;  
									   pixels[getIndex].b = color[i][2]*255 * blendVal + (1-blendVal) * backgroundColorB; 
								}
							}
					
						}
				}
			}

			//brush type 2 
			if(brush[i] == 2) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j++)  
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j++)  
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					if(((xAxis - thickness[i]) > 0)&&((xAxis + thickness[i]) < Image_Width) && ((yAxis - thickness[i]) > 0)&&((yAxis + thickness[i]) < Image_Height))
					{

						for(int k = 0; k < thickness[i]; k++)   
						{
							int getIndex = (yAxis + k)*Image_Width + (xAxis); 
                            
						    double tempColorR = 255 - (255 - color[i][0]*255)*(1 + (double) (254.0*(255 - backgroundColorR)/(255.0*255.0))); 
							double tempColorG = 255 - (255 - color[i][1]*255)*(1 + (double) (254.0*(255 - backgroundColorG)/(255.0*255.0))); 
							double tempColorB = 255 - (255 - color[i][2]*255)*(1 + (double) (254.0*(255 - backgroundColorB)/(255.0*255.0))); 

							if(tempColorR >= 255) 
							{
								pixels[getIndex].r = 255; 
							}
							if(tempColorG >= 255) 
							{
								pixels[getIndex].g = 255; 
							}
							if(tempColorB >= 255) 
							{
								pixels[getIndex].b = 255; 
							}
							if(tempColorR < 0) 
							{
								pixels[getIndex].r = 0; 
							}
							if(tempColorG < 0) 
							{
								pixels[getIndex].g = 0; 
							}
							if(tempColorB < 0) 
							{
								pixels[getIndex].b = 0; 
							}
							else
							{
								pixels[getIndex].r = tempColorR; 
								pixels[getIndex].g = tempColorG; 
								pixels[getIndex].b = tempColorB; 
							}
						}
					}
				}
			}

		    //brush type 3   
			if(brush[i] == 3) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j++)   
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j++)  
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					if(((xAxis - thickness[i]) > 0)&&((xAxis + thickness[i]) < Image_Width) && ((yAxis - thickness[i]) > 0)&&((yAxis + thickness[i]) < Image_Height))
					{
						int middle = thickness[i]/2;
						for(int k = middle; k >= 0; k= k - 2)
						{
								int getIndex = (yAxis + k)*Image_Width + (xAxis); 
								pixels[getIndex].r = color[i][0]*255;
						      	pixels[getIndex].g = color[i][1]*255;
							    pixels[getIndex].b = color[i][2]*255; 	
						}
						for(int k = middle+1; k < thickness[i]; k= k+2)
						{
								int getIndex = (yAxis + k)*Image_Width + (xAxis); 
								pixels[getIndex].r = color[i][0]*255;
						      	pixels[getIndex].g = color[i][1]*255;
							    pixels[getIndex].b = color[i][2]*255; 
						}
						for(int k = middle-3; k <middle+3 ; k++)
						{
								int getIndex = (yAxis + k)*Image_Width + (xAxis); 
								pixels[getIndex].r = color[i][0]*255;
						      	pixels[getIndex].g = color[i][1]*255;
							    pixels[getIndex].b = color[i][2]*255; 
						}
					}
				}
			}

			//brush type 4     
			if(brush[i] == 4) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j++)  
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j++) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					if(((xAxis - thickness[i]) > 0)&&((xAxis + thickness[i]) < Image_Width) && ((yAxis - thickness[i]) > 0)&&((yAxis + thickness[i]) < Image_Height))
					{
						for(int k = 0; k < thickness[i]; k=k+3)   
						{
							int getIndex = (yAxis + k)*Image_Width + (xAxis); 
                        
						    pixels[getIndex].r = color[i][0]*255;
							pixels[getIndex].g = color[i][1]*255;
							pixels[getIndex].b = color[i][2]*255; 
						}
					}
				}
			}

			//brush type 5          
			if(brush[i] == 5) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j++) 
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j++) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 

					if(((xAxis - thickness[i]) > 0)&&((xAxis + thickness[i]) < Image_Width) && ((yAxis - thickness[i]) > 0)&&((yAxis + thickness[i]) < Image_Height))
					{
						for(int k = 0; k < thickness[i]; k=k+4)   
						{
							int getIndex = (yAxis + k)*Image_Width + (xAxis); 
                        
						    pixels[getIndex].r = color[i][0]*255;
							pixels[getIndex].g = color[i][1]*255;
							pixels[getIndex].b = color[i][2]*255; 

							int getIndexUp1 = (yAxis + k + 1)*Image_Width + (xAxis); 
						    pixels[getIndexUp1].r = color[i][0]*255;
							pixels[getIndexUp1].g = color[i][1]*255;
							pixels[getIndexUp1].b = color[i][2]*255; 

							int getIndexUp2 = (yAxis + k + 2)*Image_Width + (xAxis); 
						    pixels[getIndexUp2].r = color[i][0]*255;
							pixels[getIndexUp2].g = color[i][1]*255;
							pixels[getIndexUp2].b = color[i][2]*255; 
						}
					}
				}
			}

			//brush type 6      
			if(brush[i] == 6) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j++)   
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j++) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					int ranNum = rand()%4;
					if(((xAxis - thickness[i]) > 0)&&((xAxis + thickness[i]) < Image_Width) && ((yAxis - thickness[i]) > 0)&&((yAxis + thickness[i]) < Image_Height))
					{
						for(int k = 0; k < thickness[i]; k=k+ 5)   
						{
							for(int upNum = 0; upNum < ranNum; upNum++)
							{
								int getIndex = (yAxis + k + upNum)*Image_Width + (xAxis); 
								int randThou = rand()%1000;
								double randomNum = (double) (randThou/1000.00); 
						
								if(randomNum >= 0.45)
								{
									pixels[getIndex].r = color[i][0]*255;
									pixels[getIndex].g = color[i][1]*255;
									pixels[getIndex].b = color[i][2]*255; 
								}else
								{
									pixels[getIndex].r = backgroundColorR; 
									pixels[getIndex].g = backgroundColorG; 
									pixels[getIndex].b = backgroundColorB; 
								} 
							}
						}
					}
				} 
			}

			//brush type 7      
			if(brush[i] == 7) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j= j+ Sample_Points*spacing[i])   
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j = j+ Sample_Points*spacing[i]) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					if(((xAxis - 28) > 0)&&((xAxis + 28) < Image_Width) && ((yAxis - 0) > 0)&&((yAxis + 60) < Image_Height))
					{
							for(int smallX = -27; smallX < 27; smallX ++)
							{
								for(int smallY = -29; smallY < 29; smallY ++)
								{
									RGBApixel Temp = AnImage1.GetPixel(smallX+27, smallY+29);
									int tempR = Temp.Red; 
									int tempG = Temp.Green; 
									int tempB = Temp.Blue; 
									if((tempR < 245) && (tempG < 245) && (tempB < 245))
									{
										int getIndex = (yAxis + 29 - smallY)*Image_Width + (xAxis+ smallX); 
								   		pixels[getIndex].r = color[i][0]*255;  
										pixels[getIndex].g = color[i][1]*255; 
										pixels[getIndex].b = color[i][2]*255; 
									}
								}
							}
					}
				}
			}

			//brush type 8 
			if(brush[i] == 8) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j = j+Sample_Points*spacing[i])     
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j = j+ Sample_Points*spacing[i]) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 

					if(((xAxis - 28) > 0)&&((xAxis + 28) < Image_Width) && ((yAxis - 0) > 0)&&((yAxis + 60) < Image_Height))
					{
							for(int smallX = -27; smallX < 27; smallX++)
							{
								for(int smallY = -29; smallY < 29; smallY++)
								{
									RGBApixel Temp = AnImage2.GetPixel(smallX+27, smallY+29);
									int tempR = Temp.Red; 
									int tempG = Temp.Green; 
									int tempB = Temp.Blue; 
									if((tempR < 245) && (tempG < 245) && (tempB < 245))
									{
										int getIndex = (yAxis + 29 - smallY)*Image_Width + (xAxis+ smallX); 
								   		pixels[getIndex].r = color[i][0]*255;  
										pixels[getIndex].g = color[i][1]*255; 
										pixels[getIndex].b = color[i][2]*255; 
									}
								}
							}
					}
				}
			}

			//brush type 9 
			if(brush[i] == 9) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j = j+ Sample_Points*spacing[i])   
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j = j+ Sample_Points*spacing[i]) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z);   
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					
					if(((xAxis - 28) > 0)&&((xAxis + 28) < Image_Width) && ((yAxis - 0) > 0)&&((yAxis + 60) < Image_Height))
					{
							for(int smallX = -27; smallX < 27; smallX++)
							{
								for(int smallY = -29; smallY < 29; smallY++)
								{
									RGBApixel Temp = AnImage3.GetPixel(smallX+27, smallY+29);
									int tempR = Temp.Red; 
									int tempG = Temp.Green;   
									int tempB = Temp.Blue; 
									if((tempR < 245) && (tempG < 245) && (tempB < 245))
									{
										int getIndex = (yAxis + 29 - smallY)*Image_Width + (xAxis+ smallX); 
								   		pixels[getIndex].r = color[i][0]*255;  
										pixels[getIndex].g = color[i][1]*255; 
										pixels[getIndex].b = color[i][2]*255; 
									}
								}
							}
					}
				}
			}

			//brush type 10 
			if(brush[i] == 10) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j = j+Sample_Points*spacing[i]) 
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j = j+ Sample_Points*spacing[i]) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					if(((xAxis - 28) > 0)&&((xAxis + 28) < Image_Width) && ((yAxis - 0) > 0)&&((yAxis + 60) < Image_Height))
					{
							for(int smallX = -27; smallX < 27; smallX++)
							{
								for(int smallY = -29; smallY < 29; smallY++)
								{
									RGBApixel Temp = AnImage4.GetPixel(smallX+27, smallY+29);
									int tempR = Temp.Red; 
									int tempG = Temp.Green; 
									int tempB = Temp.Blue; 
									if((tempR < 245) && (tempG < 245) && (tempB < 245))
									{
										int getIndex = (yAxis + 29 - smallY)*Image_Width + (xAxis+ smallX); 
								   		pixels[getIndex].r = color[i][0]*255;  
										pixels[getIndex].g = color[i][1]*255; 
										pixels[getIndex].b = color[i][2]*255; 
									}
								}
							}
					}
				}
			}

			//brush type 11  
			if(brush[i] == 11) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j++)      
					for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j++) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen     
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					if(((xAxis - thickness[i]) > 0)&&((xAxis + thickness[i]) < Image_Width) && ((yAxis - thickness[i]) > 0)&&((yAxis + thickness[i]) < Image_Height))
					{
						int middle = thickness[i]/2;
						for(int k = middle; k >= 0; k= k - 2)
						{
								int getIndex = (yAxis + k)*Image_Width + (xAxis); 
								pixels[getIndex].r = color[i][0]*255;
						      	pixels[getIndex].g = color[i][1]*255;
							    pixels[getIndex].b = color[i][2]*255; 	
						}
						for(int k = middle+1; k < thickness[i]; k= k+2)
						{
								int getIndex = (yAxis + k)*Image_Width + (xAxis); 
								pixels[getIndex].r = color[i][0]*255;
						      	pixels[getIndex].g = color[i][1]*255;
							    pixels[getIndex].b = color[i][2]*255; 
						}
						for(int k = middle-2; k <middle+2 ; k++)
						{
								int getIndex = (yAxis + k)*Image_Width + (xAxis); 
								pixels[getIndex].r = color[i][0]*255;
						      	pixels[getIndex].g = color[i][1]*255;
							    pixels[getIndex].b = color[i][2]*255; 
						}
					}
				}
			}

			//brush type 12     
			if(brush[i] == 12) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j++)  
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j++) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					if(((xAxis - thickness[i]) > 0)&&((xAxis + thickness[i]) < Image_Width) && ((yAxis - thickness[i]) > 0)&&((yAxis + thickness[i]) < Image_Height))
					{
						for(int k = 0; k < thickness[i]; k++)   
						{
							int getIndex = (yAxis + k)*Image_Width + (xAxis); 

					     	int randThou = rand()%1000;
							double randomNum = (double) (randThou/1000.00); 
						
							if(randomNum >= 0.75)
							{
								pixels[getIndex].r = color[i][0]*255;
								pixels[getIndex].g = color[i][1]*255;
								pixels[getIndex].b = color[i][2]*255; 
							}else
							{
								pixels[getIndex].r = backgroundColorR; 
								pixels[getIndex].g = backgroundColorG; 
								pixels[getIndex].b = backgroundColorB;   
							} 
						}
					}
				}
			}

			//brush type 13
			if(brush[i] == 13) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j++)    
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j++) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					if(((xAxis - thickness[i]) > 0)&&((xAxis + thickness[i]) < Image_Width) && ((yAxis - thickness[i]) > 0)&&((yAxis + thickness[i]) < Image_Height))
					{
						//if thickness is less than 3 do nothing 
		                if(thickness[i]<3)
						{
							for(int k = 0; k < thickness[i]; k++)   
							{
								int getIndex = (yAxis + k)*Image_Width + (xAxis); 

								pixels[getIndex].r = color[i][0]*255;
								pixels[getIndex].g = color[i][1]*255;
								pixels[getIndex].b = color[i][2]*255;       	
							}
						}
						//if thickness is greater than 3 start brushes     
						else
						{
						    int middle = thickness[i]/2;
							for(int k = middle; k >= 0; k--)
							{
								    int getIndex = (yAxis + k)*Image_Width + (xAxis); 
									int randThou = rand()%1000;
									double randomNum = (double) (randThou/1000.00); 
									if(randomNum >= (0.99 -(middle-k)*0.13))
									{
										pixels[getIndex].r = color[i][0]*255;
										pixels[getIndex].g = color[i][1]*255;
										pixels[getIndex].b = color[i][2]*255; 
									}else
									{
										pixels[getIndex].r = backgroundColorR;     
										pixels[getIndex].g = backgroundColorG; 
										pixels[getIndex].b = backgroundColorB; 
									} 
							}
							for(int k = middle+1; k < thickness[i]; k++)
							{
								   int getIndex = (yAxis + k)*Image_Width + (xAxis); 
								   int randThou = rand()%1000;
								   double randomNum = (double) (randThou/1000.00); 
								   if(randomNum >= (0.99 -(k-middle-1)*0.13))
								   {
										pixels[getIndex].r = color[i][0]*255;
										pixels[getIndex].g = color[i][1]*255;
										pixels[getIndex].b = color[i][2]*255; 
								   }else
								   {
										pixels[getIndex].r = backgroundColorR;     
										pixels[getIndex].g = backgroundColorG; 
										pixels[getIndex].b = backgroundColorB; 
								   } 
							}
						}
					}
				}
			}

			//brush type 14 
			if(brush[i] == 14) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j++)     
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j++) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					if(((xAxis - thickness[i]) > 0)&&((xAxis + thickness[i]) < Image_Width) && ((yAxis - thickness[i]) > 0)&&((yAxis + thickness[i]) < Image_Height))
					{
						//if thickness is less than 3 do nothing   
		                if(thickness[i]<3)
						{
							for(int k = 0; k < thickness[i]; k++)   
							{
								int getIndex = (yAxis + k)*Image_Width + (xAxis); 

								pixels[getIndex].r = color[i][0]*255;
								pixels[getIndex].g = color[i][1]*255;
								pixels[getIndex].b = color[i][2]*255;       	
							}
						}
						//if thickness is greater than 3 start brushes   
						else
						{
						    int middle = thickness[i]/2;
							for(int k = middle; k >= 0; k--)
							{
								    int getIndex = (yAxis + k)*Image_Width + (xAxis); 
									int randThou = rand()%1000;
									double randomNum = (double) (randThou/1000.00); 
									if(randomNum <= (0.99 -(middle-k)*0.13))
									{
										pixels[getIndex].r = color[i][0]*255;
										pixels[getIndex].g = color[i][1]*255;
										pixels[getIndex].b = color[i][2]*255; 
									}else
									{
										pixels[getIndex].r = backgroundColorR;     
										pixels[getIndex].g = backgroundColorG; 
										pixels[getIndex].b = backgroundColorB; 
									} 
							}
							for(int k = middle+1; k < thickness[i]; k++)
							{
								   int getIndex = (yAxis + k)*Image_Width + (xAxis); 
								   int randThou = rand()%1000;
								   double randomNum = (double) (randThou/1000.00); 
								   if(randomNum <= (0.99 -(k-middle-1)*0.13))
								   {
										pixels[getIndex].r = color[i][0]*255;
										pixels[getIndex].g = color[i][1]*255;
										pixels[getIndex].b = color[i][2]*255; 
								   }else
								   {
										pixels[getIndex].r = backgroundColorR;     
										pixels[getIndex].g = backgroundColorG; 
										pixels[getIndex].b = backgroundColorB;   
								   } 
							}
						}
					}
				}
			}

			//brush type 15
			if(brush[i] == 15) 
			{
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j++) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z);  
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					if(((xAxis - thickness[i]) > 0)&&((xAxis + thickness[i]) < Image_Width) && ((yAxis - thickness[i]) > 0)&&((yAxis + thickness[i]) < Image_Height))
					{
						for(int k = 0; k < thickness[i]; k++)   
						{
							int getIndex = (yAxis + k)*Image_Width + (xAxis); 

					     	int randThou = rand()%1000;
							double randomNum = (double) (randThou/1000.00); 
						
							if(randomNum >= 0.15)
							{
								pixels[getIndex].r = color[i][0]*255;
								pixels[getIndex].g = color[i][1]*255;
								pixels[getIndex].b = color[i][2]*255; 
							}else
							{
								pixels[getIndex].r = backgroundColorR; 
								pixels[getIndex].g = backgroundColorG; 
								pixels[getIndex].b = backgroundColorB; 
							} 
						}
					}
				}
			}   

		    //brush type 16 
			if(brush[i] == 16) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j = j+Sample_Points*spacing[i]) 
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j = j+ Sample_Points*spacing[i]) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					if(((xAxis - 28) > 0)&&((xAxis + 28) < Image_Width) && ((yAxis - 0) > 0)&&((yAxis + 60) < Image_Height))
					{
							for(int smallX = -27; smallX < 27; smallX++)
							{
								for(int smallY = -29; smallY < 29; smallY++)
								{
									RGBApixel Temp = AnImage5.GetPixel(smallX+27, smallY+29);
									int tempR = Temp.Red; 
									int tempG = Temp.Green; 
									int tempB = Temp.Blue; 
									if((tempR < 245) && (tempG < 245) && (tempB < 245))
									{
										int getIndex = (yAxis + 29 - smallY)*Image_Width + (xAxis+ smallX); 
								   		pixels[getIndex].r = color[i][0]*255;  
										pixels[getIndex].g = color[i][1]*255; 
										pixels[getIndex].b = color[i][2]*255; 
									}
								}
							}
					}
				}
			}

			//brush type 17
			if(brush[i] == 17) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j = j+Sample_Points*spacing[i]) 
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j = j+ Sample_Points*spacing[i]) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					if(((xAxis - 28) > 0)&&((xAxis + 28) < Image_Width) && ((yAxis - 0) > 0)&&((yAxis + 60) < Image_Height))
					{
							for(int smallX = -27; smallX < 27; smallX++)
							{
								for(int smallY = -29; smallY < 29; smallY++)
								{
									RGBApixel Temp = AnImage6.GetPixel(smallX+27, smallY+29);
									int tempR = Temp.Red; 
									int tempG = Temp.Green; 
									int tempB = Temp.Blue; 
									if((tempR < 245) && (tempG < 245) && (tempB < 245))
									{
										int getIndex = (yAxis + 29 - smallY)*Image_Width + (xAxis+ smallX); 
								   		pixels[getIndex].r = color[i][0]*255;  
										pixels[getIndex].g = color[i][1]*255; 
										pixels[getIndex].b = color[i][2]*255; 
									}
								}
							}
					}
				}
			}

			//brush type 18 
			if(brush[i] == 18) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j = j+Sample_Points*spacing[i]) 
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j = j+ Sample_Points*spacing[i]) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					if(((xAxis - 28) > 0)&&((xAxis + 28) < Image_Width) && ((yAxis - 0) > 0)&&((yAxis + 60) < Image_Height))
					{
							for(int smallX = -27; smallX < 27; smallX++)
							{
								for(int smallY = -29; smallY < 29; smallY++)
								{
									RGBApixel Temp = AnImage7.GetPixel(smallX+27, smallY+29);
									int tempR = Temp.Red; 
									int tempG = Temp.Green; 
									int tempB = Temp.Blue; 
									if((tempR < 245) && (tempG < 245) && (tempB < 245))
									{
										int getIndex = (yAxis + 29 - smallY)*Image_Width + (xAxis+ smallX); 
								   		pixels[getIndex].r = color[i][0]*255;  
										pixels[getIndex].g = color[i][1]*255; 
										pixels[getIndex].b = color[i][2]*255; 
									}
								}
							}
					}
				}
			}

			//brush type 19 
			if(brush[i] == 19) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j = j+Sample_Points*spacing[i]) 
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j = j+ Sample_Points*spacing[i]) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					if(((xAxis - 28) > 0)&&((xAxis + 28) < Image_Width) && ((yAxis - 0) > 0)&&((yAxis + 60) < Image_Height))
					{
							for(int smallX = -27; smallX < 27; smallX++)
							{
								for(int smallY = -29; smallY < 29; smallY++)
								{
									RGBApixel Temp = AnImage8.GetPixel(smallX+27, smallY+29);
									int tempR = Temp.Red; 
									int tempG = Temp.Green; 
									int tempB = Temp.Blue; 
									if((tempR < 245) && (tempG < 245) && (tempB < 245))
									{
										int getIndex = (yAxis + 29 - smallY)*Image_Width + (xAxis+ smallX); 
								   		pixels[getIndex].r = color[i][0]*255;  
										pixels[getIndex].g = color[i][1]*255; 
										pixels[getIndex].b = color[i][2]*255; 
									}
								}
							}
					}
				}
			}

			//brush type 20 
			if(brush[i] == 20) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j = j+Sample_Points*spacing[i]) 
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j = j+ Sample_Points*spacing[i]) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					if(((xAxis - 28) > 0)&&((xAxis + 28) < Image_Width) && ((yAxis - 0) > 0)&&((yAxis + 60) < Image_Height))
					{
							for(int smallX = -27; smallX < 27; smallX++)
							{
								for(int smallY = -29; smallY < 29; smallY++)
								{
									RGBApixel Temp = AnImage9.GetPixel(smallX+27, smallY+29);
									int tempR = Temp.Red; 
									int tempG = Temp.Green; 
									int tempB = Temp.Blue; 
									if((tempR < 245) && (tempG < 245) && (tempB < 245))
									{
										int getIndex = (yAxis + 29 - smallY)*Image_Width + (xAxis+ smallX); 
								   		pixels[getIndex].r = color[i][0]*255;  
										pixels[getIndex].g = color[i][1]*255; 
										pixels[getIndex].b = color[i][2]*255; 
									}
								}
							}
					}
				}
			}

			//brush type 21 
			if(brush[i] == 21) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j = j+Sample_Points*spacing[i]) 
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j = j+ Sample_Points*spacing[i]) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					if(((xAxis - 28) > 0)&&((xAxis + 28) < Image_Width) && ((yAxis - 0) > 0)&&((yAxis + 60) < Image_Height))
					{
							for(int smallX = -27; smallX < 27; smallX++)
							{
								for(int smallY = -29; smallY < 29; smallY++)
								{
									RGBApixel Temp = AnImage10.GetPixel(smallX+27, smallY+29);
									int tempR = Temp.Red; 
									int tempG = Temp.Green; 
									int tempB = Temp.Blue; 
									if((tempR < 245) && (tempG < 245) && (tempB < 245))
									{
										int getIndex = (yAxis + 29 - smallY)*Image_Width + (xAxis+ smallX); 
								   		pixels[getIndex].r = color[i][0]*255;  
										pixels[getIndex].g = color[i][1]*255; 
										pixels[getIndex].b = color[i][2]*255; 
									}
								}
							}
					}
				}
			}

			//brush type 22 
			if(brush[i] == 22) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j = j+Sample_Points*spacing[i]) 
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j = j+ Sample_Points*spacing[i]) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					if(((xAxis - 28) > 0)&&((xAxis + 28) < Image_Width) && ((yAxis - 0) > 0)&&((yAxis + 60) < Image_Height))
					{
							for(int smallX = -27; smallX < 27; smallX++)
							{
								for(int smallY = -29; smallY < 29; smallY++)
								{
									RGBApixel Temp = AnImage11.GetPixel(smallX+27, smallY+29);
									int tempR = Temp.Red; 
									int tempG = Temp.Green; 
									int tempB = Temp.Blue; 
									if((tempR < 245) && (tempG < 245) && (tempB < 245))
									{
										int getIndex = (yAxis + 29 - smallY)*Image_Width + (xAxis+ smallX); 
								   		pixels[getIndex].r = color[i][0]*255;  
										pixels[getIndex].g = color[i][1]*255; 
										pixels[getIndex].b = color[i][2]*255; 
									}
								}
							}
					}
				}
			}

			//brush type 23 
			if(brush[i] == 23) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j = j+Sample_Points*spacing[i]) 
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j = j+ Sample_Points*spacing[i]) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					if(((xAxis - 28) > 0)&&((xAxis + 28) < Image_Width) && ((yAxis - 0) > 0)&&((yAxis + 60) < Image_Height))
					{
							for(int smallX = -27; smallX < 27; smallX++)
							{
								for(int smallY = -29; smallY < 29; smallY++)
								{
									RGBApixel Temp = AnImage12.GetPixel(smallX+27, smallY+29);
									int tempR = Temp.Red; 
									int tempG = Temp.Green; 
									int tempB = Temp.Blue; 
									if((tempR < 245) && (tempG < 245) && (tempB < 245))
									{
										int getIndex = (yAxis + 29 - smallY)*Image_Width + (xAxis+ smallX); 
								   		pixels[getIndex].r = color[i][0]*255;  
										pixels[getIndex].g = color[i][1]*255; 
										pixels[getIndex].b = color[i][2]*255; 
									}
								}
							}
					}
				}
			}

			//brush type 24
			if(brush[i] == 24) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j = j+Sample_Points*spacing[i]) 
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j = j+ Sample_Points*spacing[i]) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					if(((xAxis - 28) > 0)&&((xAxis + 28) < Image_Width) && ((yAxis - 0) > 0)&&((yAxis + 60) < Image_Height))
					{
							for(int smallX = -27; smallX < 27; smallX++)
							{
								for(int smallY = -29; smallY < 29; smallY++)
								{
									RGBApixel Temp = AnImage13.GetPixel(smallX+27, smallY+29);
									int tempR = Temp.Red; 
									int tempG = Temp.Green; 
									int tempB = Temp.Blue; 
									if((tempR < 245) && (tempG < 245) && (tempB < 245))
									{
										int getIndex = (yAxis + 29 - smallY)*Image_Width + (xAxis+ smallX); 
								   		pixels[getIndex].r = color[i][0]*255;  
										pixels[getIndex].g = color[i][1]*255; 
										pixels[getIndex].b = color[i][2]*255; 
									}
								}
							}
					}
				}
			}

		  //brush type 25 
			if(brush[i] == 25) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j = j+Sample_Points*spacing[i]) 
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j = j+ Sample_Points*spacing[i]) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					if(((xAxis - 28) > 0)&&((xAxis + 28) < Image_Width) && ((yAxis - 0) > 0)&&((yAxis + 60) < Image_Height))
					{
							for(int smallX = -27; smallX < 27; smallX++)
							{
								for(int smallY = -29; smallY < 29; smallY++)
								{
									RGBApixel Temp = AnImage14.GetPixel(smallX+27, smallY+29);
									int tempR = Temp.Red; 
									int tempG = Temp.Green; 
									int tempB = Temp.Blue; 
									if((tempR < 245) && (tempG < 245) && (tempB < 245))
									{
										int getIndex = (yAxis + 29 - smallY)*Image_Width + (xAxis+ smallX); 
								   		pixels[getIndex].r = color[i][0]*255;  
										pixels[getIndex].g = color[i][1]*255; 
										pixels[getIndex].b = color[i][2]*255; 
									}
								}
							}
					}
				}
			}

			//brush type 26 
			if(brush[i] == 26) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j = j+Sample_Points*spacing[i]) 
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j = j+ Sample_Points*spacing[i]) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					if(((xAxis - 28) > 0)&&((xAxis + 28) < Image_Width) && ((yAxis - 0) > 0)&&((yAxis + 60) < Image_Height))
					{
							for(int smallX = -27; smallX < 27; smallX++)
							{
								for(int smallY = -29; smallY < 29; smallY++)
								{
									RGBApixel Temp = AnImage15.GetPixel(smallX+27, smallY+29);
									int tempR = Temp.Red; 
									int tempG = Temp.Green; 
									int tempB = Temp.Blue; 
									if((tempR < 245) && (tempG < 245) && (tempB < 245))
									{
										int getIndex = (yAxis + 29 - smallY)*Image_Width + (xAxis+ smallX); 
								   		pixels[getIndex].r = color[i][0]*255;  
										pixels[getIndex].g = color[i][1]*255; 
										pixels[getIndex].b = color[i][2]*255; 
									}
								}
							}
					}
				}
			}

			//brush type 27 
			if(brush[i] == 27) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j = j+Sample_Points*spacing[i]) 
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j = j+ Sample_Points*spacing[i]) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					if(((xAxis - 28) > 0)&&((xAxis + 28) < Image_Width) && ((yAxis - 0) > 0)&&((yAxis + 60) < Image_Height))
					{
							for(int smallX = -27; smallX < 27; smallX++)
							{
								for(int smallY = -29; smallY < 29; smallY++)
								{
									RGBApixel Temp = AnImage16.GetPixel(smallX+27, smallY+29);
									int tempR = Temp.Red; 
									int tempG = Temp.Green; 
									int tempB = Temp.Blue; 
									if((tempR < 245) && (tempG < 245) && (tempB < 245))
									{
										int getIndex = (yAxis + 29 - smallY)*Image_Width + (xAxis+ smallX); 
								   		pixels[getIndex].r = color[i][0]*255;  
										pixels[getIndex].g = color[i][1]*255; 
										pixels[getIndex].b = color[i][2]*255; 
									}
								}
							}
					}
				}
			}

			//brush type 28 
			if(brush[i] == 28) 
			{
				//for(int j = 0; j<(Sample_Points*EndPara[i]+1); j = j+Sample_Points*spacing[i]) 
				for(int j = 0; j<(Sample_Points*DepthEndPara[i]+1); j = j+ Sample_Points*spacing[i]) 
				{
					//calculating the projection points  
					//vec3 a (splinesPara[i][j].x-eye_pos.x, splinesPara[i][j].y-eye_pos.y, splinesPara[i][j].z-eye_pos.z); 
					vec3 a (depthSplinesPara[i][j].x-eye_pos.x, depthSplinesPara[i][j].y-eye_pos.y, depthSplinesPara[i][j].z-eye_pos.z);
					vec3 b (view_dir.x, view_dir.y, view_dir.z);   
					double aDotb = Dot (a, b);
					vec3 projection = aDotb*b;    

					double targetLen = totalLength*a.Length()/projection.Length(); 
					vec3 finalVec = targetLen*(a.Normalize());
					vec3 finalPos = vec3 (eye_pos.x + finalVec[0], eye_pos.y + finalVec[1], eye_pos.z + finalVec[2]);  
					vec3 P = finalPos; 

					//find out x, y in the NDC coordinate 
					double Sx = ((P[0] - M[0] + H[0] + V[0]) * V[1] - (P[1] - M[1] + H[1] + V[1])* V[0])/(2*H[0]*V[1] - 2*H[1]*V[0]); 
					double Sy = ((P[0] - M[0] + H[0] + V[0]) * H[1] - (P[1] - M[1] + H[1] + V[1])* H[0])/(2*V[0]*H[1] - 2*V[1]*H[0]); 

					//convert from NDC coordinate to 2D screen    
					int xAxis = (int) (Sx*Image_Width + 0.5);
					int yAxis = (int) (Sy*Image_Height + 0.5); 
					if(((xAxis - 28) > 0)&&((xAxis + 28) < Image_Width) && ((yAxis - 0) > 0)&&((yAxis + 60) < Image_Height))
					{
							for(int smallX = -27; smallX < 27; smallX++)
							{
								for(int smallY = -29; smallY < 29; smallY++)
								{
									RGBApixel Temp = AnImage17.GetPixel(smallX+27, smallY+29);
									int tempR = Temp.Red; 
									int tempG = Temp.Green; 
									int tempB = Temp.Blue; 
									if((tempR < 245) && (tempG < 245) && (tempB < 245))
									{
										int getIndex = (yAxis + 29 - smallY)*Image_Width + (xAxis+ smallX); 
								   		pixels[getIndex].r = color[i][0]*255;  
										pixels[getIndex].g = color[i][1]*255; 
										pixels[getIndex].b = color[i][2]*255; 
									}
								}
							}
					}
				}
			}

			 

		} 

		if(flag == 2)
		{
			//print out the image through easyBMP 
			BMP Output; 
			Output.SetSize(Image_Width, Image_Height);
			Output.SetBitDepth(24);
			for(unsigned int x = 0; x < Image_Width; x++) {
				for(unsigned int y = 0; y < Image_Height; y++) {
					   int getIndex = y*Image_Width + x; 
					   Output(x,Image_Height-1-y)->Red = pixels[getIndex].r; 
					   Output(x,Image_Height-1-y)->Green = pixels[getIndex].g; 
					   Output(x,Image_Height-1-y)->Blue = pixels[getIndex].b; 
				}
			}


			//Nathan, you can change the absolute path here for your computer     
			string path = "C:\\CIS 660\\OverCoatImage\\";

			//get complete path 
			stringstream out;
			string frameNo; 
			out<<frameNum; 
			frameNo = out.str(); 
			string completeP = path + name + frameNo + ".bmp"; 
			char *completePath=new char[completeP.size()+1];
			completePath[completeP.size()]=0;
			memcpy(completePath,completeP.c_str(),completeP.size());


			//output the image 
			Output.WriteToFile(completePath); 
		}

		//update all the points in the maya render view port             
		//MRenderView::updatePixels(0, Image_Height-1, 0, Image_Width-1, pixels);                    
		if(flag == 1)
		{
		  MRenderView::updatePixels(0, Image_Width-1, 0, Image_Height-1, pixels);
		}
        delete [] pixels;
		//
        // Inform the Render View that we have completed rendering the entire image.      
        //
        if (MRenderView::endRender() != MS::kSuccess)
        {
                displayError( "OverCoat: error occured in endRender." );
              //  return MS::kFailure;
        }  
}