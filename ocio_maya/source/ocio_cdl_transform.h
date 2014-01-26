

#ifndef OCIO_CDL_TRANSFORM_H
#define OCIO_CDL_TRANSFORM_H

//Ocio_cdl_transform Declaration
//-----------------------------------------------


//Constants
//-----------------------------------------------



//include
//-----------------------------------------------
//Std
#include <string>
#include <iostream>
#include <vector>
//Boost
#include "boost/format.hpp"
#include "boost/lexical_cast.hpp"
//Own
#include "ocio_functionality.h"
//Maya
#include <maya/MPxNode.h>
#include <maya/MObject.h>
#include <maya/MGlobal.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnStringData.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MArrayDataBuilder.h>
#include <maya/MDataBlock.h>
#include <maya/MString.h>
#include <maya/MDGModifier.h>
#include <maya/MStatus.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MFloatVector.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MDataHandle.h>





//Ocio_cdl_transform
//-----------------------------------------------
class Ocio_cdl_transform : public MPxNode
{
public:
	//Attributes
	static MTypeId id;
	static MString type_name;

	static MObject a_verbose;

	static MObject a_input_color;
	static MObject a_output_color;

	static MObject a_slope;
	static MObject a_last_slope;
	static MObject a_offset;
	static MObject a_last_offset;
	static MObject a_power;
	static MObject a_last_power;
	static MObject a_saturation;
	static MObject a_last_saturation;
	static MObject a_direction;
	static MObject a_last_direction;


	
	//Methods
	//Maya
	Ocio_cdl_transform();
	virtual ~Ocio_cdl_transform();
	static void* create();
	static MStatus initialize();
	virtual void postConstructor();
	virtual MStatus compute(const MPlug &plug, MDataBlock &data);
	//OCIO
	void set_processor(float*&, float, int);
	void color_transform(MFloatVector&);
	//States
	//Own
	void get_sop_as_mfloatvectors(MFloatVector&,
									MFloatVector&,
									MFloatVector&, 
									MFloatVector&, 
									MFloatVector&, 
									MFloatVector&,
									MDataBlock&);
	void get_sop_as_float_ptr(float*&,
								MFloatVector&,
								MFloatVector&,
								MFloatVector&);
	void set_color_attribute(std::string, MFloatVector&);
	void set_int_attribute(std::string, int);
	void set_float_attribute(std::string, float);
	//Helper
	MPlug get_plug(std::string);
	bool attribute_exists(std::string);
	bool attr_connected(std::string);
	void set_output_color(MFloatVector&, MDataBlock&);
	std::string get_instance_name();
	//Temp
	


private:
	//Attributes
	bool first_execution;
	OCIO::ConstProcessorRcPtr processor;

	//Methods


};
#endif