



//Ocio_cdl_transform Definition
//-----------------------------------------------



//Constants
//-----------------------------------------------



//include
//-----------------------------------------------
//Own
#include "ocio_cdl_transform.h"








//Parameter
//-----------------------------------------------
//-----------------------------------------------

namespace ocio_cdl_transform_parameters
{


	//prm_cop_default
	//Macro expanding to default COP2 tabs for Mask and Frame Scope
	COP_MASK_SWITCHER(17, "OCIOCDLTransform");
	PRM_Template prm_cop_default = PRM_Template(PRM_SWITCHER, 3, &PRMswitcherName, switcher);

	//prm_slope
	static PRM_Name prm_slope_name("slope", "slope");
	const char* prm_slope_help_text = "Slope";
	PRM_Template prm_slope = PRM_Template(PRM_RGB, TOOL_PARM, 4, &prm_slope_name, PRMoneDefaults);

	//prm_last_slope
	static PRM_Name prm_last_slope_name("last_slope", "last_slope");
	PRM_Template prm_last_slope = PRM_Template(PRM_RGB, 4, &prm_last_slope_name, PRMoneDefaults);


	//prm_offset
	static PRM_Name prm_offset_name("offset", "offset");
	const char* prm_offset_help_text = "Offset";
	PRM_Template prm_offset = PRM_Template(PRM_RGB, TOOL_PARM, 4, &prm_offset_name);

	//prm_last_offset
	static PRM_Name prm_last_offset_name("last_offset", "last_offset");
	PRM_Template prm_last_offset = PRM_Template(PRM_RGB, 4, &prm_last_offset_name);
	

	//prm_power
	static PRM_Name prm_power_name("power", "power");
	const char* prm_power_help_text = "Power";
	PRM_Template prm_power = PRM_Template(PRM_RGB, TOOL_PARM, 4, &prm_power_name, PRMoneDefaults);

	//prm_last_power
	static PRM_Name prm_last_power_name("last_power", "last_power");
	PRM_Template prm_last_power = PRM_Template(PRM_RGB, 4, &prm_last_power_name, PRMoneDefaults);


	//prm_saturation
	static PRM_Name prm_saturation_name("saturation", "saturation");
	static PRM_Default prm_saturation_default(1.0);
	PRM_Template prm_saturation = PRM_Template(PRM_FLT, TOOL_PARM, 1, &prm_saturation_name, &prm_saturation_default);

	//prm_last_saturation
	static PRM_Name prm_last_saturation_name("last_saturation", "last_saturation");
	static PRM_Default prm_last_saturation_default(1.0);
	PRM_Template prm_last_saturation = PRM_Template(PRM_FLT, 1, &prm_last_saturation_name, &prm_last_saturation_default);


	//prm_direction
	static PRM_Name prm_direction_name("direction", "direction");
	static PRM_Default prm_direction_default(0);
	static PRM_Name prm_direction_choices[] =
	{
		PRM_Name("Forward", "Forward"),
		PRM_Name("Inverse", "Inverse"),
		PRM_Name(0)
	};
	static PRM_ChoiceList prm_direction_choice_list(PRM_CHOICELIST_SINGLE, prm_direction_choices);
	PRM_Template prm_direction = PRM_Template(PRM_ORD, TOOL_PARM, 1, &prm_direction_name, &prm_direction_default, &prm_direction_choice_list);

	//prm_last_direction
	static PRM_Name prm_last_direction_name("last_direction", "last_direction");
	static PRM_Default prm_last_direction_default(0);
	PRM_Template prm_last_direction = PRM_Template(PRM_ORD, 1,
		&prm_last_direction_name,
		&prm_last_direction_default);

	
	//prm_node_or_file
	static PRM_Name prm_node_or_file_name("node_or_file", "node_or_file");
	static PRM_Default prm_node_or_file_default(0);
	const char* prm_node_or_file_help_text = "Use the node settings like slope, power and saturation for grading,\
																						\n or load an cdl file.";
	PRM_Template prm_node_or_file = PRM_Template(PRM_TOGGLE, TOOL_PARM, 1, &prm_node_or_file_name,
		&prm_node_or_file_default);

	//prm_last_node_or_file
	static PRM_Name prm_last_node_or_file_name("last_node_or_file", "last_node_or_file");
	static PRM_Default prm_last_node_or_file_default(0);
	PRM_Template prm_last_node_or_file = PRM_Template(PRM_TOGGLE, 1, &prm_last_node_or_file_name,
		&prm_last_node_or_file_default);
	
	
	//prm_lut_file_path
	static PRM_Name prm_lut_file_path_name("lut_file_path", "lut_file_path");
	PRM_Template prm_lut_file_path = PRM_Template(PRM_FILE, 1, &prm_lut_file_path_name);

	//prm_last_lut_file_path
	static PRM_Name prm_last_lut_file_path_name("last_lut_file_path", "last_lut_file_path");
	PRM_Template prm_last_lut_file_path = PRM_Template(PRM_FILE, 1, &prm_last_lut_file_path_name);


	//prm_cccid
	static PRM_Name prm_cccid_name("cccid", "cccid");
	PRM_Template prm_cccid = PRM_Template(PRM_FILE, 1, &prm_cccid_name);

	//prm_last_cccid
	static PRM_Name prm_last_cccid_name("last_cccid", "last_cccid");
	PRM_Template prm_last_cccid = PRM_Template(PRM_FILE, 1, &prm_last_cccid_name);


	//prm_export_switcher
	static PRM_Name prm_export_switcher_name("export_switcher");
	static PRM_Default prm_export_switcher_list[] = 
	{
		PRM_Default(2, "Export Grade")
	};
	PRM_Template prm_export_switcher = PRM_Template(PRM_SWITCHER, 1, &prm_export_switcher_name, prm_export_switcher_list);

	//prm_export_grade_file_path
	static PRM_Name prm_export_grade_file_path_name("export_grade_file_path", "export_grade_file_path");
	PRM_Template prm_export_grade_file_path = PRM_Template(PRM_FILE, 1, &prm_export_grade_file_path_name);

	//prm_export_grade
	static PRM_Name prm_export_grade_name("export_grade", "export_grade");
	PRM_Template prm_export_grade = PRM_Template(PRM_CALLBACK, 1, &prm_export_grade_name, 0, 0, 0,
		Ocio_cdl_transform::export_grade);

}



//Parameter template array for UI creation
//-----------------------------------------------
PRM_Template Ocio_cdl_transform::template_list[] =
{
	ocio_cdl_transform_parameters::prm_cop_default,
	ocio_cdl_transform_parameters::prm_slope,
	ocio_cdl_transform_parameters::prm_last_slope,
	ocio_cdl_transform_parameters::prm_offset,
	ocio_cdl_transform_parameters::prm_last_offset,
	ocio_cdl_transform_parameters::prm_power,
	ocio_cdl_transform_parameters::prm_last_power,
	ocio_cdl_transform_parameters::prm_saturation,
	ocio_cdl_transform_parameters::prm_last_saturation,
	ocio_cdl_transform_parameters::prm_direction,
	ocio_cdl_transform_parameters::prm_last_direction,
	ocio_cdl_transform_parameters::prm_node_or_file,
	ocio_cdl_transform_parameters::prm_last_node_or_file,
	ocio_cdl_transform_parameters::prm_lut_file_path,
	ocio_cdl_transform_parameters::prm_last_lut_file_path,
	ocio_cdl_transform_parameters::prm_cccid,
	ocio_cdl_transform_parameters::prm_last_cccid,
	ocio_cdl_transform_parameters::prm_export_switcher,
	ocio_cdl_transform_parameters::prm_export_grade_file_path,
	ocio_cdl_transform_parameters::prm_export_grade,
	//Sentinel (needed)
	PRM_Template()
};


//template_pair <-- ? 
OP_TemplatePair Ocio_cdl_transform::template_pair(Ocio_cdl_transform::template_list,
													&COP2_MaskOp::myTemplatePair);

//variable_pair <-- ?
OP_VariablePair Ocio_cdl_transform::variable_pair(0, &COP2_MaskOp::myVariablePair);

//input_labels_list
const char* Ocio_cdl_transform::input_labels_list[] =
{
	"Input Image",
	"Input Mask",
	0
};




//Local Variables
//-----------------------------------------------
//-----------------------------------------------






//Ocio_image_filter
//-----------------------------------------------
//-----------------------------------------------


//Constructors and factory
//-----------------------------------------------

//ocio_colorspace_factory
OP_Node* Ocio_cdl_transform::ocio_cdl_transform_factory(OP_Network* parent,
														const char* name,
														OP_Operator* op)
{
	//return ptr to new instance of Ocio_image_filter
	return new Ocio_cdl_transform(parent, name, op);
};

//Ocio_cdl_transform
Ocio_cdl_transform::Ocio_cdl_transform(OP_Network* parent,
									const char* name,
									OP_Operator* op)
									: COP2_MaskOp(parent, name, op), 
									first_execution(true),
									log_messages(ocio_houdini_constants::LOG_MESSAGES),
									internal_parms_visible(ocio_houdini_constants::INTERNAL_PARMS_VISIBLE)
{
	//set default scope (which planes are affected by default)
	setDefaultScope(true, false, 0);
};

//~Ocio_cdl_transform
Ocio_cdl_transform::~Ocio_cdl_transform()
{};


//Cooking
//-----------------------------------------------

//doCookMyTile
OP_ERROR Ocio_cdl_transform::doCookMyTile(COP2_Context& context, TIL_TileList* tile_list)
{
	//context_data
	Ocio_cdl_transform_context_data* context_data = static_cast<Ocio_cdl_transform_context_data *>(context.data());

	//cookFullImage convenience function that assembles all tiles from tile_List
	//to one huge tile for cooking
	return cookFullImage(context, 
						tile_list, 
						&Ocio_cdl_transform::filter_static,
						context_data->image_lock,
						true);
};

//updateParmsFlags
bool Ocio_cdl_transform::updateParmsFlags()
{
	//time
	fpreal time = CHgetEvalTime();
	
	//attribute_change_occured (assign value from superclass function call)
	bool attribute_change_occured = COP2_MaskOp::updateParmsFlags();
	
	//Set attrs. invisible

	//last attrs
	attribute_change_occured |= setVisibleState(ocio_cdl_transform_parameters::prm_last_slope.getToken(), internal_parms_visible);
	attribute_change_occured |= setVisibleState(ocio_cdl_transform_parameters::prm_last_offset.getToken(), internal_parms_visible);
	attribute_change_occured |= setVisibleState(ocio_cdl_transform_parameters::prm_last_power.getToken(), internal_parms_visible);
	attribute_change_occured |= setVisibleState(ocio_cdl_transform_parameters::prm_last_saturation.getToken(), internal_parms_visible);
	attribute_change_occured |= setVisibleState(ocio_cdl_transform_parameters::prm_last_direction.getToken(), internal_parms_visible);
	attribute_change_occured |= setVisibleState(ocio_cdl_transform_parameters::prm_last_node_or_file.getToken(), internal_parms_visible);
	attribute_change_occured |= setVisibleState(ocio_cdl_transform_parameters::prm_last_lut_file_path.getToken(), internal_parms_visible);
	attribute_change_occured |= setVisibleState(ocio_cdl_transform_parameters::prm_last_cccid.getToken(), internal_parms_visible);

	
	return attribute_change_occured;
}

//disableParms
unsigned Ocio_cdl_transform::disableParms()
{
	
	//parameter_changed
	unsigned parameter_changed = COP2_MaskOp::disableParms();
	
	//node_or_file
	bool node_or_file = get_bool_parameter(ocio_cdl_transform_parameters::prm_node_or_file.getToken());


	parameter_changed += enableParm(ocio_cdl_transform_parameters::prm_lut_file_path.getToken(), node_or_file);
	parameter_changed += enableParm(ocio_cdl_transform_parameters::prm_cccid.getToken(), node_or_file);

	//hide scopergba always because correct transformation needs rgb color triplet
	parameter_changed += enableParm("scopergba", false);

	//again for other attributes ... 

	return parameter_changed;
}

//getOperationInfo
const char* Ocio_cdl_transform::getOperationInfo()
{
	return ocio_houdini_constants::OCIOCDLTRANSFORM_OPERATION_INFO;
}

//filter_static
OP_ERROR Ocio_cdl_transform::filter_static(COP2_Context& context,
											const TIL_Region* input,
											TIL_Region* output,
											COP2_Node* node)
{
	//call filter (pure convenience to avoid working in static function)
	return ((Ocio_cdl_transform*)node)->filter(context, input, output);
};

//filter
OP_ERROR Ocio_cdl_transform::filter(COP2_Context& context,
	const TIL_Region* input,
	TIL_Region* output)
{

	//log
	log("filter");

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
		filter_ocio(context, input, output);
	//rgb not available
	else
		filter_no_ocio(context, input, output);
	

	
	//Return error
	return error();
};

//rgb_available
bool Ocio_cdl_transform::rgb_available(COP2_Context& context,
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
void Ocio_cdl_transform::filter_ocio(COP2_Context& context,
										const TIL_Region* input,
										TIL_Region* output)
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
	if (processor_exists())
	{
		//color transform
		OCIO_functionality::color_transform_rgb_array(ptr_output_data_red,
			ptr_output_data_green,
			ptr_output_data_blue,
			processor,
			context.myXsize,
			context.myYsize);
	}

	//temp
	else
		log("From filter: Processor does not exist");
};

//filter_no_ocio
void Ocio_cdl_transform::filter_no_ocio(COP2_Context& context,
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


//OCIO
//-----------------------------------------------
//-----------------------------------------------



//set_processor
void Ocio_cdl_transform::set_processor(float*& ptr_slope,
	float*& ptr_offset,
	float*& ptr_power,
	float saturation,
	int direction)
{
	//ptr_sop
	float* ptr_sop = new float[9];

	//set sop_ptr
	ptr_sop[0] = ptr_slope[0];
	ptr_sop[1] = ptr_slope[1];
	ptr_sop[2] = ptr_slope[2];

	ptr_sop[3] = ptr_offset[0];
	ptr_sop[4] = ptr_offset[1];
	ptr_sop[5] = ptr_offset[2];

	ptr_sop[6] = ptr_power[0];
	ptr_sop[7] = ptr_power[1];
	ptr_sop[8] = ptr_power[2];


	//set processor
	processor = OCIO_functionality::get_processor_from_cdl_transform(ptr_sop, saturation, direction);

	//display msg
	if (!processor)
		log("Processor empty");
	else
		log("Processor set");

	delete[] ptr_sop;

};

//set_processor
void Ocio_cdl_transform::set_processor(std::string lut_file_path_value,
	std::string cccid_value,
	int direction_value,
	int interpolation_value)
{


	//set processor
	processor = OCIO_functionality::get_processor_from_file_transform(lut_file_path_value,
		cccid_value,
		direction_value,
		interpolation_value);

	//msg_set_processor_success
	std::string msg_set_processor_success("Processor set");
	//msg_set_processor_failure
	std::string msg_set_processor_failure("No Lut file path set. No transformation will be applied. ");

	//display msg
	if (!lut_file_path_value.size())
		log(msg_set_processor_failure.c_str());
	else
		log(msg_set_processor_success.c_str());

};

//processor_exists
bool Ocio_cdl_transform::processor_exists()
{
	//return wether or not processor exists
	if (!processor)
		return false;
	return true;
};

//get_cc_xml
std::string Ocio_cdl_transform::get_cc_xml(float*& ptr_slope,
										float*& ptr_offset,
										float*& ptr_power,
										float saturation,
										int direction)
{
	//ptr_sop
	float* ptr_sop = new float[9];

	//set sop_ptr
	ptr_sop[0] = ptr_slope[0];
	ptr_sop[1] = ptr_slope[1];
	ptr_sop[2] = ptr_slope[2];

	ptr_sop[3] = ptr_offset[0];
	ptr_sop[4] = ptr_offset[1];
	ptr_sop[5] = ptr_offset[2];

	ptr_sop[6] = ptr_power[0];
	ptr_sop[7] = ptr_power[1];
	ptr_sop[8] = ptr_power[2];
	
	//str_xml
	std::string str_xml = OCIO_functionality::get_xml_from_cdl_transform(ptr_sop, saturation, direction);

	//delete ptr
	delete[] ptr_sop;
	
	//return str_xml
	return str_xml;
};

//write_to_file
void Ocio_cdl_transform::write_to_file(std::string& str_xml, std::string& file_path)
{
	//cc_file
	std::ofstream cc_file;

	//open
	cc_file.open(file_path, std::ios::out | std::ios::trunc);

	//if open append
	if (cc_file.is_open())
	{
		//write to file
		cc_file << str_xml.c_str();
		//print
		std::cout << "Exported grade to: " << file_path.c_str() << std::endl;
	}
	else
		std::cout << "Could not open and write to file: " << file_path.c_str() << std::endl;
		

	//close
	cc_file.close();

}

//export_grade
int Ocio_cdl_transform::export_grade(void *data,
										int index,
										float time,
										const PRM_Template* prm_template)
{
	//Cast ptr to yourself
	Ocio_cdl_transform* node = (Ocio_cdl_transform*)data;
	
	
	
	//Get Attributes
	//-----------------------------------------------

	//ptr_slope
	float* ptr_slope = new float[3];
	node->get_color_parameter(ocio_cdl_transform_parameters::prm_slope.getToken(), ptr_slope);

	//ptr_offset
	float* ptr_offset = new float[3];
	node->get_color_parameter(ocio_cdl_transform_parameters::prm_offset.getToken(), ptr_offset);
	
	//ptr_power
	float* ptr_power = new float[3];
	node->get_color_parameter(ocio_cdl_transform_parameters::prm_power.getToken(), ptr_power);
	
	//saturation
	float saturation = node->get_float_parameter(ocio_cdl_transform_parameters::prm_saturation.getToken());
	
	//direction
	int direction = node->get_int_parameter(ocio_cdl_transform_parameters::prm_direction.getToken());
	

	//export_grade_file_path
	std::string export_grade_file_path(node->get_string_parameter(ocio_cdl_transform_parameters::prm_export_grade_file_path.getToken()));


	
	
	
	//Export
	//-----------------------------------------------
	
	//str_xml
	std::string str_xml = node->get_cc_xml(ptr_slope, ptr_offset, ptr_power, saturation, direction);

	//write_to_file
	node->write_to_file(str_xml, export_grade_file_path);




	//Delete ptr
	//-----------------------------------------------

	delete[] ptr_slope;
	delete[] ptr_offset;
	delete[] ptr_power;

	
	
	return 1;
};





//Misc
//-----------------------------------------------

//getInputDependenciesForOutputArea
void Ocio_cdl_transform::getInputDependenciesForOutputArea(COP2_CookAreaInfo& output_area,
															const COP2_CookAreaList& input_areas,
															COP2_CookAreaList& needed_areas)
{
	//area
	COP2_CookAreaInfo* area;

	//make dependency from output plane to the same plane on the input
	area = makeOutputAreaDependOnInputPlane(0,
											output_area.getPlane().getName(),
											output_area.getArrayIndex(),
											output_area.getTime(),
											input_areas, 
											needed_areas);

	//Use all the planes area
	if (area)
		area->enlargeNeededAreaToBounds();

	//make dependency for mask
	getMaskDependency(output_area, input_areas, needed_areas);
};

//newContextData
COP2_ContextData* Ocio_cdl_transform::newContextData(const TIL_Plane* plane,
													int array_index,
													float time,
													int xres,
													int yres,
													int thread,
													int max_threads)
{
	

	//Get Attributes
	//-----------------------------------------------
	
	//ptr_slope
	float* ptr_slope = new float[3];
	get_color_parameter(ocio_cdl_transform_parameters::prm_slope.getToken(), ptr_slope);
	//ptr_last_slope
	float* ptr_last_slope = new float[3];
	get_color_parameter(ocio_cdl_transform_parameters::prm_last_slope.getToken(), ptr_last_slope);

	//ptr_offset
	float* ptr_offset = new float[3];
	get_color_parameter(ocio_cdl_transform_parameters::prm_offset.getToken(), ptr_offset);
	//ptr_last_offset
	float* ptr_last_offset = new float[3];
	get_color_parameter(ocio_cdl_transform_parameters::prm_last_offset.getToken(), ptr_last_offset);

	//ptr_power
	float* ptr_power = new float[3];
	get_color_parameter(ocio_cdl_transform_parameters::prm_power.getToken(), ptr_power);
	//ptr_last_power
	float* ptr_last_power = new float[3];
	get_color_parameter(ocio_cdl_transform_parameters::prm_last_power.getToken(), ptr_last_power);

	//saturation
	float saturation = get_float_parameter(ocio_cdl_transform_parameters::prm_saturation.getToken());
	//last_saturation
	float last_saturation = get_float_parameter(ocio_cdl_transform_parameters::prm_last_saturation.getToken());
	
	//direction
	int direction = get_int_parameter(ocio_cdl_transform_parameters::prm_direction.getToken());
	//last_direction
	int last_direction = get_int_parameter(ocio_cdl_transform_parameters::prm_last_direction.getToken());


	//node_or_file
	bool node_or_file = get_bool_parameter(ocio_cdl_transform_parameters::prm_node_or_file.getToken());
	//last_node_or_file
	bool last_node_or_file = get_bool_parameter(ocio_cdl_transform_parameters::prm_last_node_or_file.getToken());

	//lut_file_path
	std::string lut_file_path = get_string_parameter(ocio_cdl_transform_parameters::prm_lut_file_path_name.getToken());
	//last_lut_file_path
	std::string last_lut_file_path = get_string_parameter(ocio_cdl_transform_parameters::prm_last_lut_file_path_name.getToken());

	//cccid
	std::string cccid = get_string_parameter(ocio_cdl_transform_parameters::prm_cccid.getToken());
	//last_cccid
	std::string last_cccid = get_string_parameter(ocio_cdl_transform_parameters::prm_last_cccid.getToken());


	
	
	
	
	//Set processor
	//-----------------------------------------------
	
	
	//node_or_file == true
	if (node_or_file)
	{
		//interpolation
		int interpolation = 1;
		
		//first_execution
		if (first_execution)
		{
			//set_processor
			set_processor(lut_file_path, cccid, direction, interpolation);
			//first_execution false
			first_execution = false;
		}
		//node_or_file changed
		else if (node_or_file != last_node_or_file)
			set_processor(lut_file_path, cccid, direction, interpolation);
		//direction changed
		else if (direction != last_direction)
			//set_processor
			set_processor(lut_file_path, cccid, direction, interpolation);
		//lut_file_path
		else if (lut_file_path.compare(last_lut_file_path) != 0)
			set_processor(lut_file_path, cccid, direction, interpolation);
		//cccid
		else if (cccid.compare(last_cccid) != 0)
			set_processor(lut_file_path, cccid, direction, interpolation);
		
	}

	//node_or_file == false
	else
	{
		//first_execution
		if (first_execution)
		{
			//set_processor
			set_processor(ptr_slope, ptr_offset, ptr_power, saturation, direction);
			//first_execution false
			first_execution = false;
		}
		//node_or_file changed
		else if (node_or_file != last_node_or_file)
			//set_processor
			set_processor(ptr_slope, ptr_offset, ptr_power, saturation, direction);
		//slope changed
		else if (!colors_equal(ptr_slope, ptr_last_slope))
			//set_processor
			set_processor(ptr_slope, ptr_offset, ptr_power, saturation, direction);
		//offset changed
		else if (!colors_equal(ptr_offset, ptr_last_offset))
			//set_processor
			set_processor(ptr_slope, ptr_offset, ptr_power, saturation, direction);
		//power changed
		else if (!colors_equal(ptr_power, ptr_last_power))
			//set_processor
			set_processor(ptr_slope, ptr_offset, ptr_power, saturation, direction);
		//saturation changed
		else if (saturation != last_saturation)
			//set_processor
			set_processor(ptr_slope, ptr_offset, ptr_power, saturation, direction);
		//direction changed
		else if (direction != last_direction)
			//set_processor
			set_processor(ptr_slope, ptr_offset, ptr_power, saturation, direction);
	};



	
		
	
	
	
	
	



	//Set Attributes
	//-----------------------------------------------
	
	//set last_slope
	set_parameter(ocio_cdl_transform_parameters::prm_last_slope.getToken(), ptr_slope);
	//set last_offset
	set_parameter(ocio_cdl_transform_parameters::prm_last_offset.getToken(), ptr_offset);
	//set last_power
	set_parameter(ocio_cdl_transform_parameters::prm_last_power.getToken(), ptr_power);
	//set last_saturation
	set_parameter(ocio_cdl_transform_parameters::prm_last_saturation.getToken(), saturation);
	//last_direction
	set_parameter(ocio_cdl_transform_parameters::prm_last_direction.getToken(), direction);

	//set last_node_or_file
	set_parameter(ocio_cdl_transform_parameters::prm_last_node_or_file.getToken(), node_or_file);

	//last_lut_file_path
	set_parameter(ocio_cdl_transform_parameters::prm_last_lut_file_path_name.getToken(), lut_file_path);
	//last_cccid
	set_parameter(ocio_cdl_transform_parameters::prm_last_cccid.getToken(), cccid);

	
	//Delete ptr
	//-----------------------------------------------
	
	delete[] ptr_slope;
	delete[] ptr_last_slope;
	delete[] ptr_offset;
	delete[] ptr_last_offset;
	delete[] ptr_power;
	delete[] ptr_last_power;
	
	
	
	
	
	
	
	//Custom param values for context data
	//-----------------------------------------------

	//context_data (store custom parameter values here)
	Ocio_cdl_transform_context_data* context_data = new Ocio_cdl_transform_context_data();

	//plane_name
	std::string plane_name(plane->getName());
	context_data->plane_name = plane_name;

	//component_count
	context_data->component_count = plane->getVectorSize();

	
	
	
	
	
	//return
	return context_data;
};

//log
void Ocio_cdl_transform::log(const char* msg)
{
	//log message if log_messages == true
	if (log_messages)
		std::cout << msg << std::endl;
}

//get_time
float Ocio_cdl_transform::get_time()
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
bool Ocio_cdl_transform::colors_equal(float*& color_a, float*& color_b, int length)
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




//Getter & Setter
//-----------------------------------------------
//-----------------------------------------------

//get_int_parameter
int Ocio_cdl_transform::get_int_parameter(const char* parameter_name)
{
	//ut_parameter_name
	UT_String ut_parameter_name(parameter_name);

	//time
	float time = get_time();

	//result
	int result = evalInt(ut_parameter_name, 0, time);

	return result;
};

//get_bool_parameter
bool Ocio_cdl_transform::get_bool_parameter(const char* parameter_name)
{
	//ut_parameter_name
	UT_String ut_parameter_name(parameter_name);

	//time
	float time = get_time();

	//result
	bool result = evalInt(ut_parameter_name, 0, time);

	return result;
};

//get_float_parameter
float Ocio_cdl_transform::get_float_parameter(const char* parameter_name)
{
	//ut_parameter_name
	UT_String ut_parameter_name(parameter_name);

	//time
	float time = get_time();

	//result
	float result = evalFloat(ut_parameter_name, 0, time);

	return result;
};

//get_string_parameter
std::string Ocio_cdl_transform::get_string_parameter(const char* parameter_name)
{
	//ut_parameter_name
	UT_String ut_parameter_name(parameter_name);

	//time
	float time = get_time();

	//ut_result
	UT_String ut_result;

	//aquire ut_result
	evalString(ut_result, ut_parameter_name, 0, time);

	//result
	std::string result = ut_result.toStdString();

	return result;
};

//get_color_parameter
void Ocio_cdl_transform::get_color_parameter(const char* parameter_name, float*& ptr_color, int length)
{
	//ut_parameter_name
	UT_String ut_parameter_name(parameter_name);

	//time
	float time = get_time();

	//set result
	for (int index = 0; index < length; index++)
	{
		ptr_color[index] = evalFloat(ut_parameter_name, index, time);
	}
};

//set_parameter
void Ocio_cdl_transform::set_parameter(const char* parameter_name, std::string parameter_value)
{
	//ut_parameter_name
	UT_String ut_parameter_name(parameter_name);

	//ut_parameter_value
	UT_String ut_parameter_value(parameter_value);

	//time
	float time = get_time();

	//set parameter_value
	setString(ut_parameter_value, CH_STRING_LITERAL, ut_parameter_name, 0, time);
};

//set_parameter
void Ocio_cdl_transform::set_parameter(const char* parameter_name, int parameter_value)
{
	//ut_parameter_name
	UT_String ut_parameter_name(parameter_name);

	//time
	float time = get_time();

	//setInt
	setInt(ut_parameter_name, 0, time, parameter_value);
};

//set_parameter
void Ocio_cdl_transform::set_parameter(const char* parameter_name, float parameter_value)
{
	//ut_parameter_name
	UT_String ut_parameter_name(parameter_name);

	//time
	float time = get_time();

	//setFloat
	setFloat(ut_parameter_name, 0, time, parameter_value);
};

//set_parameter
void Ocio_cdl_transform::set_parameter(const char* parameter_name, float* parameter_value, int length)
{
	//ut_parameter_name
	UT_String ut_parameter_name(parameter_name);

	//time
	float time = get_time();

	//set value
	for (int index = 0; index < length; index++)
	{
		//setFloat
		setFloat(ut_parameter_name, index, time, parameter_value[index]);
	};
};