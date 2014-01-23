

#ifndef OCIO_FILE_TRANSFORM_H
#define OCIO_FILE_TRANSFORM_H

//Ocio_file_transform Declaration
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





//Ocio_file_transform
//-----------------------------------------------
class Ocio_file_transform : public MPxNode
{
public:
	//Attributes
	static MTypeId id;
	static MString type_name;

	static MObject a_verbose;

	static MObject a_input_color;
	static MObject a_output_color;

	static MObject a_lut_file_path;
	static MObject a_last_lut_file_path;
	static MObject a_cccid;
	static MObject a_last_cccid;
	static MObject a_direction;
	static MObject a_last_direction;
	static MObject a_interpolation;
	static MObject a_last_interpolation;

	
	//Methods
	//Maya
	Ocio_file_transform();
	virtual ~Ocio_file_transform();
	static void* create();
	static MStatus initialize();
	virtual void postConstructor();
	virtual MStatus compute(const MPlug &plug, MDataBlock &data);
	//OCIO
	void set_processor(std::string, std::string, int, int);
	void color_transform(MFloatVector&);
	//States
	
	//Own
	std::string get_string_attribute(std::string);
	void set_string_attribute(std::string, std::string);
	void set_int_attribute(std::string, int);
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