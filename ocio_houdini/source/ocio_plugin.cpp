


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






//Registration
//-----------------------------------------------
//-----------------------------------------------

void newCop2Operator(OP_OperatorTable *table)
{
	
	//Ocio_image_filter
	table->addOperator(new OP_Operator("OCIOColorspace",
		"OCIOColorspace",
		Ocio_colorspace::ocio_colorspace_factory,
		&Ocio_colorspace::template_pair,
		1,
		2, // optional mask input.
		&Ocio_colorspace::variable_pair,
		0, // not generator
		Ocio_colorspace::input_labels_list));
			
};