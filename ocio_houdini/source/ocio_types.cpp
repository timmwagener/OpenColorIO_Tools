

//Ocio_types Definition
//-----------------------------------------------

/*
Ocio_types
Definitions for OCIO_Houdini wide constants and types.
*/






//include
//-----------------------------------------------

//Own
#include "ocio_types.h"





//Constants
//-----------------------------------------------
//-----------------------------------------------


namespace ocio_houdini_constants
{
	//LOG_MESSAGES
	bool LOG_MESSAGES = true;
	//INTERNAL_PARMS_VISIBLE
	bool INTERNAL_PARMS_VISIBLE = false;
	//ENABLE_SCOPERGBA
	bool ENABLE_SCOPERGBA = false;

	//OCIOCOLORSPACE_OPERATION_INFO
	const char* OCIOCOLORSPACE_OPERATION_INFO = "This operator loads an ocio config file either from\
												\nan $OCIO env. variable or a custom path.\
												\nYou can then apply all LUTs and transformations\
												\nthat are described in that config. If no config is\
												\ngiven, the operation will result in a noOp.";
	//OCIOLOGCONVERT_OPERATION_INFO
	const char* OCIOLOGCONVERT_OPERATION_INFO = "This operator converts from linear to log or log to linear.";
	//OCIOFILETRANSFORM_OPERATION_INFO
	const char* OCIOFILETRANSFORM_OPERATION_INFO = "This operator loads a LUT file and applies its transformation.";
	//OCIOCDLTRANSFORM_OPERATION_INFO
	const char* OCIOCDLTRANSFORM_OPERATION_INFO = "This operator grades the incoming pixels according\
												  \nto the ACES standard. Alternatively you can load\
												  \na .cc grade file or export such a file from your\
												  \ncurrent SOP (slope, offset, power) settings.";
};







//Types
//-----------------------------------------------
//-----------------------------------------------



//OCIO_houdini_functionality
//-----------------------------------------------



//Constructor / Destructor
//-----------------------------------------------

//OCIO_houdini_functionality
OCIO_houdini_functionality::OCIO_houdini_functionality(){};

//~OCIO_houdini_functionality
OCIO_houdini_functionality::~OCIO_houdini_functionality(){};




//Color transformation
//-----------------------------------------------


//filter
void OCIO_houdini_functionality::filter(COP2_Context& context,
	const TIL_Region* input,
	TIL_Region* output,
	OCIO::ConstProcessorRcPtr& processor)
{

	//log
	log("filter from ocio_types");

	//Pixel operation to perform is implemented here

	//context_data
	//Custom built in newContextData()
	//Here live custom attributes stashed within single threaded method newContextData()

	//Convert context like so if needed
	//Ocio_cdl_transform_context_data* context_data = (Ocio_cdl_transform_context_data *)context.data();


	//is_rgb_available
	bool is_rgb_available = rgb_available(context, input, output);


	//rgb available
	if (is_rgb_available)
		filter_ocio(context, input, output, processor);
	//rgb not available
	else
		filter_no_ocio(context, input, output);

};

//rgb_available
bool OCIO_houdini_functionality::rgb_available(COP2_Context& context,
	const TIL_Region* input,
	TIL_Region* output)
{
	//is_rgb_available
	bool is_rgb_available = false;

	//ptr_input_data_red
	float* ptr_input_data_red = (float*)input->getImageData("r");
	//ptr_input_data_green
	float* ptr_input_data_green = (float*)input->getImageData("g");
	//ptr_input_data_blue
	float* ptr_input_data_blue = (float*)input->getImageData("b");

	//ptr_output_data_red
	float* ptr_output_data_red = (float*)output->getImageData("r");
	//ptr_output_data_green
	float* ptr_output_data_green = (float*)output->getImageData("g");
	//ptr_output_data_blue
	float* ptr_output_data_blue = (float*)output->getImageData("b");

	//check
	if (ptr_input_data_red && ptr_input_data_green && ptr_input_data_blue && ptr_output_data_red && ptr_output_data_green && ptr_output_data_blue)
		is_rgb_available = true;

	//log
	if (is_rgb_available)
		log("RGB is available");
	else
		log("RGB is not available");

	//return
	return is_rgb_available;
};

//filter_ocio
void OCIO_houdini_functionality::filter_ocio(COP2_Context& context,
	const TIL_Region* input,
	TIL_Region* output,
	OCIO::ConstProcessorRcPtr& processor)
{

	//ptr_input_data_red
	float* ptr_input_data_red = (float*)input->getImageData("r");
	//ptr_input_data_green
	float* ptr_input_data_green = (float*)input->getImageData("g");
	//ptr_input_data_blue
	float* ptr_input_data_blue = (float*)input->getImageData("b");

	//ptr_output_data_red
	float* ptr_output_data_red = (float*)output->getImageData("r");
	//ptr_output_data_green
	float* ptr_output_data_green = (float*)output->getImageData("g");
	//ptr_output_data_blue
	float* ptr_output_data_blue = (float*)output->getImageData("b");


	//copy input to output for rgb
	std::memcpy(ptr_output_data_red, ptr_input_data_red, context.myXsize*context.myYsize * sizeof(float));
	std::memcpy(ptr_output_data_green, ptr_input_data_green, context.myXsize*context.myYsize * sizeof(float));
	std::memcpy(ptr_output_data_blue, ptr_input_data_blue, context.myXsize*context.myYsize * sizeof(float));


	//if processor exists do colorspace conversion
	if (processor)
		//color transform
		OCIO_functionality::color_transform_rgb_array(ptr_output_data_red,
			ptr_output_data_green,
			ptr_output_data_blue,
			processor,
			context.myXsize,
			context.myYsize);
	//temp
	else
		log("From filter: Processor does not exist");
};

//filter_no_ocio
void OCIO_houdini_functionality::filter_no_ocio(COP2_Context& context,
	const TIL_Region* input,
	TIL_Region* output)
{

	//iterate components (rgba) and copy if existent
	for (int component_index = 0; component_index < PLANE_MAX_VECTOR_SIZE; component_index++)
	{
		//Get image data for component of input region and output region
		//(think of planes)
		float* input_data_ptr = (float*)input->getImageData(component_index);
		float* output_data_ptr = (float*)output->getImageData(component_index);

		//if ptrs are not null
		if (input_data_ptr && output_data_ptr)
		{
			//copy input to output
			std::memcpy(output_data_ptr, input_data_ptr, context.myXsize*context.myYsize * sizeof(float));
		};

	};

};




//Misc
//-----------------------------------------------

//log
void OCIO_houdini_functionality::log(const char* msg)
{
	//log message if log_messages == true
	if (ocio_houdini_constants::LOG_MESSAGES)
		std::cout << msg << std::endl;
}

//get_time
float OCIO_houdini_functionality::get_time()
{
	//ch_manager
	CH_Manager* ch_manager = OPgetDirector()->getChannelManager();
	//fpreal_current_time
	fpreal fpreal_current_time = ch_manager->getEvaluateTime();
	//float_current_Time
	float float_current_time = (float)fpreal_current_time;

	return float_current_time;
}

//colors_equal
bool OCIO_houdini_functionality::colors_equal(float*& color_a, float*& color_b, int length)
{
	//colors_equal
	bool colors_equal = true;

	//iterate and compare
	for (int index = 0; index < length; index++)
	{
		if (color_a[index] != color_b[index])
			colors_equal = false;
	}

	return colors_equal;

};