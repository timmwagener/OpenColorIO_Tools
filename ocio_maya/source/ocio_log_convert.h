

#ifndef OCIO_LOG_CONVERT_H
#define OCIO_LOG_CONVERT_H

//Ocio_log_convert Declaration
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





//Ocio_log_convert
//-----------------------------------------------
class Ocio_log_convert : public MPxNode
{
public:
	//Attributes
	static MTypeId id;
	static MString type_name;

	static MObject a_verbose;

	static MObject a_input_color;
	static MObject a_output_color;

	static MObject a_env_or_file;
	static MObject a_last_env_or_file;
	static MObject a_config_file_path;
	static MObject a_last_config_file_path;
	static MObject a_operation;
	static MObject a_last_operation;


	
	//Methods
	//Maya
	Ocio_log_convert();
	virtual ~Ocio_log_convert();
	static void* create();
	static MStatus initialize();
	virtual void postConstructor();
	virtual MStatus compute(const MPlug &plug, MDataBlock &data);
	//OCIO
	void set_processor(int, std::string, int);
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