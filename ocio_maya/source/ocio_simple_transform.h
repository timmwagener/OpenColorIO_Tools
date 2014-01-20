

#ifndef OCIO_SIMPLE_TRANSFORM_H
#define OCIO_SIMPLE_TRANSFORM_H

//Ocio_simple_transform Declaration
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





//Ocio_simple_transform
//-----------------------------------------------
class Ocio_simple_transform : public MPxNode
{
public:
	//Attributes
	static MTypeId id;
	static MString type_name;

	static MObject a_verbose;
	static MObject a_input_color;
	static MObject a_output_color;
	
	//Methods
	//Maya
	Ocio_simple_transform();
	virtual ~Ocio_simple_transform();
	static void* create();
	static MStatus initialize();
	virtual void postConstructor();
	virtual MStatus compute(const MPlug &plug, MDataBlock &data);
	//Own
	bool attr_connected(std::string);
	void set_output_color(MFloatVector&, MDataBlock&);
	//Temp
	void print_config_info();


private:
	//Attributes
	OCIO::ConstConfigRcPtr config;
	OCIO::ConstProcessorRcPtr processor;

	//Methods


};
#endif