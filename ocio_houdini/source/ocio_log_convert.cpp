



//Ocio_log_convert Definition
//-----------------------------------------------



//Constants
//-----------------------------------------------



//include
//-----------------------------------------------
//Own
#include "ocio_log_convert.h"








//Parameter
//-----------------------------------------------
//-----------------------------------------------

namespace ocio_log_convert_parameters
{


	//prm_cop_default
	//Macro expanding to default COP2 tabs for Mask and Frame Scope
	COP_MASK_SWITCHER(7, "OCIOLogConvert");
	PRM_Template prm_cop_default = PRM_Template(PRM_SWITCHER, 3, &PRMswitcherName, switcher);

	//prm_env_or_file
	static PRM_Name prm_env_or_file_name("env_or_file", "Load OCIO from file");
	static PRM_Default prm_env_or_file_default(0);
	const char* prm_env_or_file_help_text = "When unchecked the OCIO file will be aquired from the environment variable.\
											\nIf checked it will be aquired from the file path given below.";
	PRM_Template prm_env_or_file = PRM_Template(PRM_TOGGLE, 1, &prm_env_or_file_name,
												&prm_env_or_file_default, 0, 0, 0, 0, 0, 
												prm_env_or_file_help_text, 0);

	//prm_last_env_or_file
	static PRM_Name prm_last_env_or_file_name("last_env_or_file", "last_env_or_file");
	static PRM_Default prm_last_env_or_file_default(0);
	PRM_Template prm_last_env_or_file = PRM_Template(PRM_TOGGLE, 1, &prm_last_env_or_file_name,
		&prm_last_env_or_file_default, 0, 0, 0, 0, 0);

	//prm_config_file_path
	static PRM_Name prm_config_file_path_name("config_file_path", "config_file_path");
	PRM_Template prm_config_file_path = PRM_Template(PRM_FILE, 1, &prm_config_file_path_name);

	//prm_last_config_file_path
	static PRM_Name prm_last_config_file_path_name("last_config_file_path", "last_config_file_path");
	PRM_Template prm_last_config_file_path = PRM_Template(PRM_STRING, 1, &prm_last_config_file_path_name);

	//prm_operation
	static PRM_Name prm_operation_name("operation", "operation");
	static PRM_Default prm_operation_default(0);
	static PRM_Name prm_operation_choices[] =
	{
		PRM_Name("log_to_lin", "log_to_lin"),
		PRM_Name("lin_to_log", "lin_to_log"),
		PRM_Name(0)
	};
	static PRM_ChoiceList prm_operation_choice_list(PRM_CHOICELIST_SINGLE, prm_operation_choices);
	PRM_Template prm_operation = PRM_Template(PRM_ORD, TOOL_PARM, 1, &prm_operation_name, &prm_operation_default, &prm_operation_choice_list);

	//prm_internal_operation_index
	static PRM_Name prm_internal_operation_index_name("internal_operation_index", "internal_operation_index");
	static PRM_Default prm_internal_operation_index_default(0);
	PRM_Template prm_internal_operation_index = PRM_Template(PRM_ORD, 1,
		&prm_internal_operation_index_name,
		&prm_internal_operation_index_default);

	//prm_config_info
	static PRM_Name prm_config_info_name("config_info", "config_info");
	PRM_Template prm_config_info = PRM_Template(PRM_STRING, 
												1, &prm_config_info_name, 
												0, 0, 0, 0, &PRM_SpareData::stringEditor);

}



//Parameter template array for UI creation
//-----------------------------------------------
PRM_Template Ocio_log_convert::template_list[] =
{
	ocio_log_convert_parameters::prm_cop_default,
	ocio_log_convert_parameters::prm_env_or_file,
	ocio_log_convert_parameters::prm_last_env_or_file,
	ocio_log_convert_parameters::prm_config_file_path,
	ocio_log_convert_parameters::prm_last_config_file_path,
	ocio_log_convert_parameters::prm_operation,
	ocio_log_convert_parameters::prm_internal_operation_index,
	ocio_log_convert_parameters::prm_config_info,
	//Sentinel (needed)
	PRM_Template()
};


//template_pair <-- ? 
OP_TemplatePair Ocio_log_convert::template_pair(Ocio_log_convert::template_list,
													&COP2_MaskOp::myTemplatePair);

//variable_pair <-- ?
OP_VariablePair Ocio_log_convert::variable_pair(0, &COP2_MaskOp::myVariablePair);

//input_labels_list
const char* Ocio_log_convert::input_labels_list[] =
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
OP_Node* Ocio_log_convert::ocio_log_convert_factory(OP_Network* parent,
														const char* name,
														OP_Operator* op)
{
	//return ptr to new instance of Ocio_image_filter
	return new Ocio_log_convert(parent, name, op);
};

//Ocio_log_convert
Ocio_log_convert::Ocio_log_convert(OP_Network* parent,
									const char* name,
									OP_Operator* op)
									: COP2_MaskOp(parent, name, op), 
									first_execution(true)
{
	//set default scope (which planes are affected by default)
	setDefaultScope(true, false, 0);
};

//~Ocio_log_convert
Ocio_log_convert::~Ocio_log_convert()
{};


//Cooking
//-----------------------------------------------

//doCookMyTile
OP_ERROR Ocio_log_convert::doCookMyTile(COP2_Context& context, TIL_TileList* tile_list)
{
	//context_data
	Ocio_log_convert_context_data* context_data = static_cast<Ocio_log_convert_context_data *>(context.data());

	//cookFullImage convenience function that assembles all tiles from tile_List
	//to one huge tile for cooking
	return cookFullImage(context, 
						tile_list, 
						&Ocio_log_convert::filter_static,
						context_data->image_lock,
						true);
};

//updateParmsFlags
bool Ocio_log_convert::updateParmsFlags()
{
	//time
	fpreal time = CHgetEvalTime();
	
	//attribute_change_occured (assign value from superclass function call)
	bool attribute_change_occured = COP2_MaskOp::updateParmsFlags();
	
	//Set attrs. invisible
	attribute_change_occured |= setVisibleState(ocio_log_convert_parameters::prm_last_env_or_file.getToken(), ocio_houdini_constants::INTERNAL_PARMS_VISIBLE);
	attribute_change_occured |= setVisibleState(ocio_log_convert_parameters::prm_last_config_file_path.getToken(), ocio_houdini_constants::INTERNAL_PARMS_VISIBLE);
	attribute_change_occured |= setVisibleState(ocio_log_convert_parameters::prm_internal_operation_index.getToken(), ocio_houdini_constants::INTERNAL_PARMS_VISIBLE);
	

	return attribute_change_occured;
}

//disableParms
unsigned Ocio_log_convert::disableParms()
{

	//parameter_changed
	unsigned parameter_changed = COP2_MaskOp::disableParms();

	//time
	float time = OCIO_houdini_functionality.get_time();
	//env_or_file
	int env_or_file = get_env_or_file(time);

	parameter_changed += enableParm(ocio_log_convert_parameters::prm_config_file_path.getToken(), env_or_file);
	
	//disable scopergba always because correct transformation needs rgb color triplet
	parameter_changed += enableParm("scopergba", ocio_houdini_constants::ENABLE_SCOPERGBA);

	//again for other attributes ... 

	return parameter_changed;
}

//getOperationInfo
const char* Ocio_log_convert::getOperationInfo()
{
	return ocio_houdini_constants::OCIOLOGCONVERT_OPERATION_INFO;
}

//filter_static
OP_ERROR Ocio_log_convert::filter_static(COP2_Context& context,
											const TIL_Region* input,
											TIL_Region* output,
											COP2_Node* node)
{
	//call filter (pure convenience to avoid working in static function)
	((Ocio_log_convert*)node)->OCIO_houdini_functionality.filter(context,
																	input,
																	output,
																	((Ocio_log_convert*)node)->processor);

	//Return error
	return ((Ocio_log_convert*)node)->error();
};




//OCIO
//-----------------------------------------------
//-----------------------------------------------

//set_config
void Ocio_log_convert::set_config(int env_or_file)
{
	//log
	OCIO_houdini_functionality.log("set_config");

	//env
	if (env_or_file == 0)
		config = OCIO_functionality::get_config_from_env();
	//file
	else
	{
		//time
		float time = OCIO_houdini_functionality.get_time();
		//config_file_path
		std::string config_file_path(get_config_file_path(time));
		//config
		config = OCIO_functionality::get_config_from_file(config_file_path); //insert filepath here
	};

};

//config_exists
bool Ocio_log_convert::config_exists()
{
	//return wether or not config exists
	if (!config)
		return false;
	return true;
};

//set_processor
void Ocio_log_convert::set_processor(int env_or_file, std::string config_file_path, int operation)
{


	//set processor
	processor = OCIO_functionality::get_processor(env_or_file, config_file_path, operation);

	//log
	if ((!config_file_path.size()) && (env_or_file == 1))
	{
		//log
		OCIO_houdini_functionality.log("Empty config path. Please set it to point to an OCIO file.");
	}
	else
	{
		//log
		OCIO_houdini_functionality.log("Set processor.");
	};

};

//processor_exists
bool Ocio_log_convert::processor_exists()
{
	//return wether or not processor exists
	if (!processor)
		return false;
	return true;
};








//Misc
//-----------------------------------------------

//getInputDependenciesForOutputArea
void Ocio_log_convert::getInputDependenciesForOutputArea(COP2_CookAreaInfo& output_area,
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
COP2_ContextData* Ocio_log_convert::newContextData(const TIL_Plane* plane,
													int array_index,
													float time,
													int xres,
													int yres,
													int thread,
													int max_threads)
{
	

	//Get Attributes
	//-----------------------------------------------

	//env_or_file
	int env_or_file = get_env_or_file(time);
	//last_env_or_file
	int last_env_or_file = get_last_env_or_file(time);


	//config_file_path
	std::string config_file_path(get_config_file_path(time));
	//last_config_file_path
	std::string last_config_file_path(get_last_config_file_path(time));

	
	//operation
	int operation = get_operation(time);
	//internal_operation_index
	int internal_operation_index = get_internal_operation_index(time);
	
	
	
	//Update config and set processor
	//-----------------------------------------------
	

	//first_execution
	if (first_execution)
	{
		//set_processor
		set_processor(env_or_file, config_file_path, operation);
		//first_execution false
		first_execution = false;
		//set_config_info
		set_config_info();
	}
	//env_or_file
	else if (env_or_file != last_env_or_file)
	{
		//set_processor
		set_processor(env_or_file, config_file_path, operation);
		//set_config_info
		set_config_info();
	}
	//config_file_path
	else if (config_file_path.compare(last_config_file_path) != 0)
	{
		//set_processor
		set_processor(env_or_file, config_file_path, operation);
		//set_config_info
		set_config_info();
	}
	//operation
	else if (operation != internal_operation_index)
	{
		//set_processor
		set_processor(env_or_file, config_file_path, operation);
	};
	
	
	
	

	
	
	
	

	//Set Last Attributes
	//-----------------------------------------------
	
	//last_env_or_file
	set_last_env_or_file(env_or_file, time);
	//last_config_file_path
	set_last_config_file_path(config_file_path, time);
	//internal_operation_index
	set_internal_operation_index(operation, time);
	
	
	
	
	//Custom param values for context data
	//-----------------------------------------------

	//context_data (store custom parameter values here)
	Ocio_log_convert_context_data* context_data = new Ocio_log_convert_context_data();

	//plane_name
	std::string plane_name(plane->getName());
	context_data->plane_name = plane_name;

	//component_count
	context_data->component_count = plane->getVectorSize();




	
	//return
	return context_data;
};




//Getter & Setter
//-----------------------------------------------
//-----------------------------------------------

//get_env_or_file
bool Ocio_log_convert::get_env_or_file(float time)
{
	//env_or_file
	bool env_or_file = evalInt(ocio_log_convert_parameters::prm_env_or_file.getToken(), 0, time);

	return env_or_file;
};

//set_last_env_or_file
void Ocio_log_convert::set_last_env_or_file(int new_value, float time)
{
	//setInt
	setInt(ocio_log_convert_parameters::prm_last_env_or_file.getToken(), 0, time, new_value);
};

//get_last_env_or_file
bool Ocio_log_convert::get_last_env_or_file(float time)
{
	//last_env_or_file
	bool last_env_or_file = evalInt(ocio_log_convert_parameters::prm_last_env_or_file.getToken(), 0, time);

	return last_env_or_file;
};

//get_config_file_path
std::string Ocio_log_convert::get_config_file_path(float time)
{
	//ut_config_file_path
	UT_String ut_config_file_path;
	
	//aquire ut_config_file_path
	evalString(ut_config_file_path, ocio_log_convert_parameters::prm_config_file_path.getToken(), 0, time);

	//config_file_path
	std::string config_file_path = ut_config_file_path.toStdString();

	return config_file_path;
};

//set_last_config_file_path
void Ocio_log_convert::set_last_config_file_path(std::string new_value, float time)
{
	//ut_new_value
	UT_String ut_new_value(new_value);
	
	//aquire ut_last_config_file_path
	setString(ut_new_value, CH_STRING_LITERAL, ocio_log_convert_parameters::prm_last_config_file_path.getToken(), 0, time);
};

//get_last_config_file_path
std::string Ocio_log_convert::get_last_config_file_path(float time)
{
	//ut_last_config_file_path
	UT_String ut_last_config_file_path;

	//aquire ut_last_config_file_path
	evalString(ut_last_config_file_path, ocio_log_convert_parameters::prm_last_config_file_path.getToken(), 0, time);

	//last_config_file_path
	std::string last_config_file_path = ut_last_config_file_path.toStdString();

	return last_config_file_path;
};

//get_operation
int Ocio_log_convert::get_operation(float time)
{
	

	//operation_index
	int operation_index = evalInt(ocio_log_convert_parameters::prm_operation.getToken(), 0, time);

	//return
	return operation_index;
};

//get_internal_operation_index
int Ocio_log_convert::get_internal_operation_index(float time)
{


	//internal_operation_index
	int internal_operation_index = evalInt(ocio_log_convert_parameters::prm_internal_operation_index.getToken(), 0, time);

	//return
	return internal_operation_index;
};

//set_internal_operation_index
void Ocio_log_convert::set_internal_operation_index(int new_internal_operation_index, float time)
{
	//set value
	setInt(ocio_log_convert_parameters::prm_internal_operation_index.getToken(), 0, time, new_internal_operation_index);
};

//set_config_info
void Ocio_log_convert::set_config_info()
{
	//current_time
	fpreal time = OCIO_houdini_functionality.get_time();
	
	//set_config
	set_config(get_env_or_file(time));
	
	//std_config_info
	std::string std_config_info;

	//assign config info
	std_config_info = OCIO_functionality::get_config_info(config);

	//ut_config_info
	UT_String ut_config_info(std_config_info);

	//set config info
	setString(ut_config_info, CH_STRING_LITERAL, ocio_log_convert_parameters::prm_config_info.getToken(), 0, time);
};