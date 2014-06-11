


//ocio_houdini plugin registration
//-----------------------------------------------



//Constants
//-----------------------------------------------



//include
//-----------------------------------------------
//Houdini
#include <UT/UT_DSOVersion.h>
#include <OP/OP_Operator.h>
#include <OP/OP_OperatorTable.h>
//Own
#include "ocio_colorspace.h"
#include "ocio_log_convert.h"
#include "ocio_file_transform.h"
#include "ocio_cdl_transform.h"





//Registration
//-----------------------------------------------
//-----------------------------------------------

void newCop2Operator(OP_OperatorTable *table)
{
	
	//OCIOColorspace
	table->addOperator(new OP_Operator("OCIOColorspace",
		"OCIOColorspace",
		Ocio_colorspace::ocio_colorspace_factory,
		&Ocio_colorspace::template_pair,
		1,
		2, // optional mask input.
		&Ocio_colorspace::variable_pair,
		0, // not generator
		Ocio_colorspace::input_labels_list));

	//OCIOLogConvert
	table->addOperator(new OP_Operator("OCIOLogConvert",
		"OCIOLogConvert",
		Ocio_log_convert::ocio_log_convert_factory,
		&Ocio_log_convert::template_pair,
		1,
		2, // optional mask input.
		&Ocio_log_convert::variable_pair,
		0, // not generator
		Ocio_log_convert::input_labels_list));

	//OCIOFileTransform
	table->addOperator(new OP_Operator("OCIOFileTransform",
		"OCIOFileTransform",
		Ocio_file_transform::ocio_file_transform_factory,
		&Ocio_file_transform::template_pair,
		1,
		2, // optional mask input.
		&Ocio_file_transform::variable_pair,
		0, // not generator
		Ocio_file_transform::input_labels_list));

	//OCIOCDLTransform
	table->addOperator(new OP_Operator("OCIOCDLTransform",
		"OCIOCDLTransform",
		Ocio_cdl_transform::ocio_cdl_transform_factory,
		&Ocio_cdl_transform::template_pair,
		1,
		2, // optional mask input.
		&Ocio_cdl_transform::variable_pair,
		0, // not generator
		Ocio_cdl_transform::input_labels_list));
			
};