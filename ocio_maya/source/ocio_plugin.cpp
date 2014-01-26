

//ocio_plugin Implementation
//-----------------------------------------------
/*
Exported functions that maya uses to register the plugin 
*/

//Preprocessor
//-----------------------------------------------
#define EXPORT __declspec(dllexport)


//include
//-----------------------------------------------
//ocio_maya
#include "ocio_test.h"
#include "ocio_simple_transform.h"
#include "ocio_colorspace.h"
#include "ocio_file_transform.h"
#include "ocio_log_convert.h"
#include "ocio_cdl_transform.h"
#include "ocio_export_cc.h"
//Maya
#include <maya/MFnPlugin.h>





//initialize
//-----------------------------------------------
EXPORT MStatus initializePlugin(MObject obj)
{
	MStatus status;

	//initialize plugin functionset
	MFnPlugin fnPlugin(obj, "Timm Wagener", "0.1", "2012");

	//ocio_test
	status = fnPlugin.registerNode(Ocio_test::type_name, Ocio_test::id, Ocio_test::create, Ocio_test::initialize, MPxNode::kDependNode);
	//ocio_simple_transform
	status = fnPlugin.registerNode(Ocio_simple_transform::type_name, Ocio_simple_transform::id, Ocio_simple_transform::create, Ocio_simple_transform::initialize, MPxNode::kDependNode);
	//OCIOColorspace
	status = fnPlugin.registerNode(Ocio_colorspace::type_name, Ocio_colorspace::id, Ocio_colorspace::create, Ocio_colorspace::initialize, MPxNode::kDependNode);
	//OCIOFileTransform
	status = fnPlugin.registerNode(Ocio_file_transform::type_name, Ocio_file_transform::id, Ocio_file_transform::create, Ocio_file_transform::initialize, MPxNode::kDependNode);
	//OCIOLogConvert
	status = fnPlugin.registerNode(Ocio_log_convert::type_name, Ocio_log_convert::id, Ocio_log_convert::create, Ocio_log_convert::initialize, MPxNode::kDependNode);
	//OCIOCDLTransform
	status = fnPlugin.registerNode(Ocio_cdl_transform::type_name, Ocio_cdl_transform::id, Ocio_cdl_transform::create, Ocio_cdl_transform::initialize, MPxNode::kDependNode);
	
	//ocio_export_cc
	status = fnPlugin.registerCommand("ocio_export_cc", Ocio_export_cc::creator, Ocio_export_cc::new_syntax);


	return MStatus::kSuccess;
}


//uninitialize
//-----------------------------------------------
EXPORT MStatus uninitializePlugin(MObject obj)
{
	MStatus status;

	//initialize plugin functionset
	MFnPlugin fnPlugin(obj);

	//ocio_test
	status = fnPlugin.deregisterNode(Ocio_test::id);
	//ocio_simple_transform
	status = fnPlugin.deregisterNode(Ocio_simple_transform::id);
	//OCIOColorspace
	status = fnPlugin.deregisterNode(Ocio_colorspace::id);
	//OCIOFileTransform
	status = fnPlugin.deregisterNode(Ocio_file_transform::id);
	//OCIOLogConvert
	status = fnPlugin.deregisterNode(Ocio_log_convert::id);
	//OCIOCDLTransform
	status = fnPlugin.deregisterNode(Ocio_cdl_transform::id);
	
	//ocio_export_cc
	status = fnPlugin.deregisterCommand("ocio_export_cc");
	

	return MStatus::kSuccess;
}
