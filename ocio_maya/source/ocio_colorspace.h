

#ifndef OCIO_COLORSPACE_H
#define OCIO_COLORSPACE_H

//Ocio_colorspace Declaration
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





//Ocio_colorspace
//-----------------------------------------------
class Ocio_colorspace : public MPxNode
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
	static MObject a_internal_input_colorspace_index;
	static MObject a_internal_output_colorspace_index;
	
	//Methods
	//Maya
	Ocio_colorspace();
	virtual ~Ocio_colorspace();
	static void* create();
	static MStatus initialize();
	virtual void postConstructor();
	virtual MStatus setDependentsDirty(const MPlug&, MPlugArray&);
	virtual MStatus compute(const MPlug &plug, MDataBlock &data);
	//OCIO
	void set_config(int);
	bool config_exists();
	void set_colorspace_names();
	void set_processor();
	void color_transform(MFloatVector&);
	//States
	void node_created();
	void env_or_file_changed();
	void colorspace_changed();
	void config_file_path_changed();
	//Own
	MPlug get_plug(std::string);
	bool attribute_exists(std::string);
	int get_env_or_file();
	int get_last_env_or_file();
	void set_last_env_or_file(int);
	std::string get_config_file_path();
	std::string get_last_config_file_path();
	void set_last_config_file_path(std::string);
	bool attr_connected(std::string);
	void set_output_color(MFloatVector&, MDataBlock&);
	//Colorspace indices
	int get_input_colorspace();
	void set_input_colorspace(int);
	int get_output_colorspace();
	void set_output_colorspace(int);
	int get_internal_input_colorspace_index();
	void set_internal_input_colorspace_index(int);
	int get_internal_output_colorspace_index();
	void set_internal_output_colorspace_index(int);
	//Enum
	void create_dynamic_enum_attribute(std::string, std::vector<std::string>&);
	void remove_dynamic_attribute(std::string);
	void update_dynamic_enum_attribute(std::string, std::vector<std::string>&);
	//Temp
	void print_config_info();


private:
	//Attributes
	bool first_execution;
	OCIO::ConstConfigRcPtr config;
	OCIO::ConstProcessorRcPtr processor;
	std::vector<std::string> vec_colorspace_names;

	//Methods


};
#endif