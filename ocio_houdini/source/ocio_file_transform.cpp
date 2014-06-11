



//Ocio_file_transform Definition
//-----------------------------------------------



//Constants
//-----------------------------------------------



//include
//-----------------------------------------------
//Own
#include "ocio_file_transform.h"








//Parameter
//-----------------------------------------------
//-----------------------------------------------

namespace ocio_file_transform_parameters
{


	//prm_cop_default
	//Macro expanding to default COP2 tabs for Mask and Frame Scope
	COP_MASK_SWITCHER(8, "OCIOFileTransform");
	PRM_Template prm_cop_default = PRM_Template(PRM_SWITCHER, 3, &PRMswitcherName, switcher);

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


	//prm_interpolation
	static PRM_Name prm_interpolation_name("interpolation", "interpolation");
	static PRM_Default prm_interpolation_default(0);
	static PRM_Name prm_interpolation_choices[] =
	{
		PRM_Name("Nearest", "Nearest"),
		PRM_Name("Linear", "Linear"),
		PRM_Name("Tetrahedral", "Tetrahedral"),
		PRM_Name("Best", "Best"),
		PRM_Name(0)
	};
	static PRM_ChoiceList prm_interpolation_choice_list(PRM_CHOICELIST_SINGLE, prm_interpolation_choices);
	PRM_Template prm_interpolation = PRM_Template(PRM_ORD, TOOL_PARM, 1, &prm_interpolation_name, &prm_interpolation_default, &prm_interpolation_choice_list);

	//prm_last_interpolation
	static PRM_Name prm_last_interpolation_name("last_interpolation", "last_interpolation");
	static PRM_Default prm_last_interpolation_default(0);
	PRM_Template prm_last_interpolation = PRM_Template(PRM_ORD, 1,
		&prm_last_interpolation_name,
		&prm_last_interpolation_default);

}



//Parameter template array for UI creation
//-----------------------------------------------
PRM_Template Ocio_file_transform::template_list[] =
{
	ocio_file_transform_parameters::prm_cop_default,
	ocio_file_transform_parameters::prm_lut_file_path,
	ocio_file_transform_parameters::prm_last_lut_file_path,
	ocio_file_transform_parameters::prm_cccid,
	ocio_file_transform_parameters::prm_last_cccid,
	ocio_file_transform_parameters::prm_direction,
	ocio_file_transform_parameters::prm_last_direction,
	ocio_file_transform_parameters::prm_interpolation,
	ocio_file_transform_parameters::prm_last_interpolation,
	//Sentinel (needed)
	PRM_Template()
};


//template_pair <-- ? 
OP_TemplatePair Ocio_file_transform::template_pair(Ocio_file_transform::template_list,
													&COP2_MaskOp::myTemplatePair);

//variable_pair <-- ?
OP_VariablePair Ocio_file_transform::variable_pair(0, &COP2_MaskOp::myVariablePair);

//input_labels_list
const char* Ocio_file_transform::input_labels_list[] =
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
OP_Node* Ocio_file_transform::ocio_file_transform_factory(OP_Network* parent,
														const char* name,
														OP_Operator* op)
{
	//return ptr to new instance of Ocio_image_filter
	return new Ocio_file_transform(parent, name, op);
};

//Ocio_file_transform
Ocio_file_transform::Ocio_file_transform(OP_Network* parent,
									const char* name,
									OP_Operator* op)
									: COP2_MaskOp(parent, name, op), 
									first_execution(true),
									log_messages(true),
									internal_parms_visible(false)
{
	//set default scope (which planes are affected by default)
	setDefaultScope(true, false, 0);
};

//~Ocio_file_transform
Ocio_file_transform::~Ocio_file_transform()
{};


//Cooking
//-----------------------------------------------

//doCookMyTile
OP_ERROR Ocio_file_transform::doCookMyTile(COP2_Context& context, TIL_TileList* tile_list)
{
	//context_data
	Ocio_file_transform_context_data* context_data = static_cast<Ocio_file_transform_context_data *>(context.data());

	//cookFullImage convenience function that assembles all tiles from tile_List
	//to one huge tile for cooking
	return cookFullImage(context, 
						tile_list, 
						&Ocio_file_transform::filter_static,
						context_data->image_lock,
						true);
};

//updateParmsFlags
bool Ocio_file_transform::updateParmsFlags()
{
	//time
	float time = get_time();
	
	//attribute_change_occured (assign value from superclass function call)
	bool attribute_change_occured = COP2_MaskOp::updateParmsFlags();
	
	//Set attrs. invisible
	attribute_change_occured |= setVisibleState(ocio_file_transform_parameters::prm_last_lut_file_path.getToken(), internal_parms_visible);
	attribute_change_occured |= setVisibleState(ocio_file_transform_parameters::prm_last_cccid.getToken(), internal_parms_visible);
	attribute_change_occured |= setVisibleState(ocio_file_transform_parameters::prm_last_direction.getToken(), internal_parms_visible);
	attribute_change_occured |= setVisibleState(ocio_file_transform_parameters::prm_last_interpolation.getToken(), internal_parms_visible);
	
	return attribute_change_occured;
}

//filter_static
OP_ERROR Ocio_file_transform::filter_static(COP2_Context& context,
											const TIL_Region* input,
											TIL_Region* output,
											COP2_Node* node)
{
	//call filter (pure convenience to avoid working in static function)
	return ((Ocio_file_transform*)node)->filter(context, input, output);
};

//filter
OP_ERROR Ocio_file_transform::filter(COP2_Context& context,
	const TIL_Region* input,
	TIL_Region* output)
{

	//log
	log("execute filter");


	//Pixel operation to perform is implemented here

	//context_data
	//Custom built in newContextData()
	//Here live custom attributes stashed within single threaded method newContextData()
	Ocio_file_transform_context_data* context_data = (Ocio_file_transform_context_data *)context.data();

		
	//Variables
	int component_index;
	

	//iterate components (rgba)
	//Do colorspace transformation on single component basis (the only thing that works so far)
	for (component_index = 0; component_index < PLANE_MAX_VECTOR_SIZE; component_index++)
	{
		//Get image data for component of input region and output region
		//(think of planes)
		float* input_data_ptr = (float*)input->getImageData(component_index);
		float* output_data_ptr = (float*)output->getImageData(component_index);
		
		//if ptrs are not null
		if (input_data_ptr && output_data_ptr)
		{
			//copy input to output
			memcpy(output_data_ptr, input_data_ptr, context.myXsize*context.myYsize * sizeof(float));

			//if processor exists do colorspace conversion
			if (processor_exists())
			{
				//fill_data_ptr_1
				float* fill_data_ptr_1 = new float[context.myXsize*context.myYsize * sizeof(float)];
				memcpy(fill_data_ptr_1, input_data_ptr, context.myXsize*context.myYsize * sizeof(float));
				//fill_data_ptr_2
				float* fill_data_ptr_2 = new float[context.myXsize*context.myYsize * sizeof(float)];
				memcpy(fill_data_ptr_2, input_data_ptr, context.myXsize*context.myYsize * sizeof(float));

				//color transform
				OCIO_functionality::color_transform_rgb_array(output_data_ptr,
					fill_data_ptr_1,
					fill_data_ptr_2,
					processor,
					context.myXsize,
					context.myYsize);

				//delete ptr
				delete[] fill_data_ptr_1;
				delete[] fill_data_ptr_2;
			};
		};

		
		
		

	
	};

	
	//Return error
	return error();
};


//OCIO
//-----------------------------------------------
//-----------------------------------------------



//set_processor
void Ocio_file_transform::set_processor(std::string lut_file_path_value,
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
bool Ocio_file_transform::processor_exists()
{
	//return wether or not processor exists
	if (!processor)
		return false;
	return true;
};








//Misc
//-----------------------------------------------

//getInputDependenciesForOutputArea
void Ocio_file_transform::getInputDependenciesForOutputArea(COP2_CookAreaInfo& output_area,
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
COP2_ContextData* Ocio_file_transform::newContextData(const TIL_Plane* plane,
													int array_index,
													float time,
													int xres,
													int yres,
													int thread,
													int max_threads)
{
	

	//Get Attributes
	//-----------------------------------------------

	//lut_file_path
	std::string lut_file_path = get_string_parameter(ocio_file_transform_parameters::prm_lut_file_path_name.getToken());
	//last_lut_file_path
	std::string last_lut_file_path = get_string_parameter(ocio_file_transform_parameters::prm_last_lut_file_path_name.getToken());
	
	//cccid
	std::string cccid = get_string_parameter(ocio_file_transform_parameters::prm_cccid.getToken());
	//last_cccid
	std::string last_cccid = get_string_parameter(ocio_file_transform_parameters::prm_last_cccid.getToken());

	//direction
	int direction = get_int_parameter(ocio_file_transform_parameters::prm_direction.getToken());
	//last_direction
	int last_direction = get_int_parameter(ocio_file_transform_parameters::prm_last_direction.getToken());

	//interpolation
	int interpolation = get_int_parameter(ocio_file_transform_parameters::prm_interpolation.getToken());
	//last_interpolation
	int last_interpolation = get_int_parameter(ocio_file_transform_parameters::prm_last_interpolation.getToken());
	
	
	
	
	//Set processor
	//-----------------------------------------------
	
	//first_execution
	if (first_execution)
	{
		set_processor(lut_file_path, cccid, direction, interpolation);
		//first_execution false
		first_execution = false;
	}
	//lut_file_path
	else if (lut_file_path.compare(last_lut_file_path) != 0)
		set_processor(lut_file_path, cccid, direction, interpolation);
	//cccid
	else if (cccid.compare(last_cccid) != 0)
		set_processor(lut_file_path, cccid, direction, interpolation);
	//direction
	else if (direction != last_direction)
		set_processor(lut_file_path, cccid, direction, interpolation);
	//interpolation
	else if (interpolation != last_interpolation)
		set_processor(lut_file_path, cccid, direction, interpolation);
	
	
	
	
	
	//Set Attributes
	//-----------------------------------------------
	
	//last_lut_file_path
	set_parameter(ocio_file_transform_parameters::prm_last_lut_file_path_name.getToken(), lut_file_path);
	//last_cccid
	set_parameter(ocio_file_transform_parameters::prm_last_cccid.getToken(), cccid);
	//last_direction
	set_parameter(ocio_file_transform_parameters::prm_last_direction.getToken(), direction);
	//last_interpolation
	set_parameter(ocio_file_transform_parameters::prm_last_interpolation.getToken(), interpolation);
	
	
	
	
	
	
	
	//Package context data
	//-----------------------------------------------

	//context_data (store custom parameter values here)
	Ocio_file_transform_context_data* context_data = new Ocio_file_transform_context_data();

	//plane_name
	std::string plane_name(plane->getName());
	context_data->plane_name = plane_name;

	//component_count
	context_data->component_count = plane->getVectorSize();




	
	//return
	return context_data;
};

//log
void Ocio_file_transform::log(const char* msg)
{
	//log message if log_messages == true
	if (log_messages)
		std::cout << msg << std::endl;
}

//get_time
float Ocio_file_transform::get_time()
{
	//ch_manager
	CH_Manager* ch_manager = OPgetDirector()->getChannelManager();
	//fpreal_current_time
	fpreal fpreal_current_time = ch_manager->getEvaluateTime();
	//float_current_Time
	float float_current_time = (float)fpreal_current_time;

	return float_current_time;
}




//Getter & Setter
//-----------------------------------------------
//-----------------------------------------------

//get_int_parameter
int Ocio_file_transform::get_int_parameter(const char* parameter_name)
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
bool Ocio_file_transform::get_bool_parameter(const char* parameter_name)
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
float Ocio_file_transform::get_float_parameter(const char* parameter_name)
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
std::string Ocio_file_transform::get_string_parameter(const char* parameter_name)
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

//set_parameter
void Ocio_file_transform::set_parameter(const char* parameter_name, std::string parameter_value)
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
void Ocio_file_transform::set_parameter(const char* parameter_name, int parameter_value)
{
	//ut_parameter_name
	UT_String ut_parameter_name(parameter_name);

	//time
	float time = get_time();

	//setInt
	setInt(ut_parameter_name, 0, time, parameter_value);
};

//set_parameter
void Ocio_file_transform::set_parameter(const char* parameter_name, float parameter_value)
{
	//ut_parameter_name
	UT_String ut_parameter_name(parameter_name);

	//time
	float time = get_time();

	//setFloat
	setFloat(ut_parameter_name, 0, time, parameter_value);
};