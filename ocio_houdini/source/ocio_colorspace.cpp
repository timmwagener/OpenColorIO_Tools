



//Ocio_colorspace Definition
//-----------------------------------------------



//Constants
//-----------------------------------------------



//include
//-----------------------------------------------
//Own
#include "ocio_colorspace.h"








//Parameter
//-----------------------------------------------
//-----------------------------------------------

namespace ocio_colorspace_parameters
{

	//prm_cop_default
	//Macro expanding to default COP2 tabs for Mask and Frame Scope
	COP_MASK_SWITCHER(9, "OCIOColorspace");
	PRM_Template prm_cop_default = PRM_Template(PRM_SWITCHER, 3, &PRMswitcherName, switcher);

	//prm_env_or_file
	static PRM_Name prm_env_or_file_name("env_or_file", "Load OCIO from file");
	static PRM_Default prm_env_or_file_default(0);
	const char* prm_env_or_file_help_text = "When unchecked the OCIO file will be aquired from the environment variable.\
																					\nIf checked it will be aquired from the file path given below.";
	PRM_Template prm_env_or_file = PRM_Template(PRM_TOGGLE, TOOL_PARM, 1, &prm_env_or_file_name,
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

	//prm_input_colorspace
	static PRM_Name prm_input_colorspace_name("input_colorspace", "input_colorspace");
	static PRM_Default prm_input_colorspace_default(0);
	static PRM_ChoiceList prm_input_colorspace_choice_list(PRM_CHOICELIST_SINGLE,
		&Ocio_colorspace::update_colorspace_menu);
	PRM_Template prm_input_colorspace = PRM_Template(PRM_ORD, TOOL_PARM, 1, &prm_input_colorspace_name, &prm_input_colorspace_default,
		&prm_input_colorspace_choice_list);



	//prm_internal_input_colorspace_index
	static PRM_Name prm_internal_input_colorspace_index_name("internal_input_colorspace_index", "internal_input_colorspace_index");
	static PRM_Default prm_internal_input_colorspace_index_default(0);
	PRM_Template prm_internal_input_colorspace_index = PRM_Template(PRM_ORD, 1,
		&prm_internal_input_colorspace_index_name,
		&prm_internal_input_colorspace_index_default);

	//prm_output_colorspace
	static PRM_Name prm_output_colorspace_name("output_colorspace", "output_colorspace");
	static PRM_Default prm_output_colorspace_default(0);
	static PRM_ChoiceList prm_output_colorspace_choice_list(PRM_CHOICELIST_SINGLE,
		&Ocio_colorspace::update_colorspace_menu);
	PRM_Template prm_output_colorspace = PRM_Template(PRM_ORD, TOOL_PARM, 1, &prm_output_colorspace_name, &prm_output_colorspace_default,
		&prm_output_colorspace_choice_list);

	//prm_internal_output_colorspace_index
	static PRM_Name prm_internal_output_colorspace_index_name("internal_output_colorspace_index", "internal_output_colorspace_index");
	static PRM_Default prm_internal_output_colorspace_index_default(0);
	PRM_Template prm_internal_output_colorspace_index = PRM_Template(PRM_ORD, 1,
		&prm_internal_output_colorspace_index_name,
		&prm_internal_output_colorspace_index_default);

	//prm_config_info
	static PRM_Name prm_config_info_name("config_info", "config_info");
	PRM_Template prm_config_info = PRM_Template(PRM_STRING,
		1, &prm_config_info_name,
		0, 0, 0, 0, &PRM_SpareData::stringEditor);

};



//Parameter template array for UI creation
//-----------------------------------------------
PRM_Template Ocio_colorspace::template_list[] =
{
	ocio_colorspace_parameters::prm_cop_default,
	ocio_colorspace_parameters::prm_env_or_file,
	ocio_colorspace_parameters::prm_last_env_or_file,
	ocio_colorspace_parameters::prm_config_file_path,
	ocio_colorspace_parameters::prm_last_config_file_path,
	ocio_colorspace_parameters::prm_input_colorspace,
	ocio_colorspace_parameters::prm_internal_input_colorspace_index,
	ocio_colorspace_parameters::prm_output_colorspace,
	ocio_colorspace_parameters::prm_internal_output_colorspace_index,
	ocio_colorspace_parameters::prm_config_info,
	//Sentinel (needed)
	PRM_Template()
};


//template_pair <-- ? 
OP_TemplatePair Ocio_colorspace::template_pair(Ocio_colorspace::template_list,
													&COP2_MaskOp::myTemplatePair);

//variable_pair <-- ?
OP_VariablePair Ocio_colorspace::variable_pair(0, &COP2_MaskOp::myVariablePair);

//input_labels_list
const char* Ocio_colorspace::input_labels_list[] =
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
OP_Node* Ocio_colorspace::ocio_colorspace_factory(OP_Network* parent,
														const char* name,
														OP_Operator* op)
{
	//return ptr to new instance of Ocio_image_filter
	return new Ocio_colorspace(parent, name, op);
};

//Ocio_colorspace
Ocio_colorspace::Ocio_colorspace(OP_Network* parent,
									const char* name,
									OP_Operator* op)
									: COP2_MaskOp(parent, name, op), 
									first_execution(true),
									log_messages(false),
									internal_parms_visible(false)
{
	//set default scope (which planes are affected by default)
	setDefaultScope(true, false, 0);
};

//~Ocio_colorspace
Ocio_colorspace::~Ocio_colorspace()
{};


//Cooking
//-----------------------------------------------

//doCookMyTile
OP_ERROR Ocio_colorspace::doCookMyTile(COP2_Context& context, TIL_TileList* tile_list)
{
	//context_data
	Ocio_colorspace_context_data* context_data = static_cast<Ocio_colorspace_context_data *>(context.data());

	//cookFullImage convenience function that assembles all tiles from tile_List
	//to one huge tile for cooking
	return cookFullImage(context, 
						tile_list, 
						&Ocio_colorspace::filter_static,
						context_data->image_lock,
						true);
};

//updateParmsFlags
bool Ocio_colorspace::updateParmsFlags()
{
	//time
	fpreal time = CHgetEvalTime();
	
	//attribute_change_occured (assign value from superclass function call)
	bool attribute_change_occured = COP2_MaskOp::updateParmsFlags();
	
	//Set attrs. invisible
	attribute_change_occured |= setVisibleState(ocio_colorspace_parameters::prm_last_env_or_file.getToken(), internal_parms_visible);
	attribute_change_occured |= setVisibleState(ocio_colorspace_parameters::prm_last_config_file_path.getToken(), internal_parms_visible);
	attribute_change_occured |= setVisibleState(ocio_colorspace_parameters::prm_internal_input_colorspace_index.getToken(), internal_parms_visible);
	attribute_change_occured |= setVisibleState(ocio_colorspace_parameters::prm_internal_output_colorspace_index.getToken(), internal_parms_visible);

	return attribute_change_occured;
}

//filter_static
OP_ERROR Ocio_colorspace::filter_static(COP2_Context& context,
											const TIL_Region* input,
											TIL_Region* output,
											COP2_Node* node)
{
	//call filter (pure convenience to avoid working in static function)
	return ((Ocio_colorspace*)node)->filter(context, input, output);
};

//filter
OP_ERROR Ocio_colorspace::filter(COP2_Context& context,
	const TIL_Region* input,
	TIL_Region* output)
{

	//Pixel operation to perform is implemented here

	//context_data
	//Custom built in newContextData()
	//Here live custom attributes stashed within single threaded method newContextData()
	Ocio_colorspace_context_data* context_data = (Ocio_colorspace_context_data *)context.data();

		
	//Variables
	int component_count = context_data->component_count;
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

//set_config
void Ocio_colorspace::set_config(int env_or_file)
{
	//log
	log("set_config");

	//env
	if (env_or_file == 0)
		config = OCIO_functionality::get_config_from_env();
	//file
	else
	{	
		//time
		float time = get_time();
		//config_file_path
		std::string config_file_path(get_config_file_path(time));
		//config
		config = OCIO_functionality::get_config_from_file(config_file_path); //insert filepath here
	};

};

//config_exists
bool Ocio_colorspace::config_exists()
{
	//return wether or not config exists
	if (!config)
		return false;
	return true;
};

//set_processor
void Ocio_colorspace::set_processor()
{
	//config exists
	if (!config_exists())
	{
		//set processor to 0
		processor = 0;
		//log
		log("Config does not exist. Processor not created.");
		return;
	};

	//log
	log("Set Processor.");


	//input_colorspace_name
	std::string input_colorspace_name = get_colorspace();
	//output_colorspace_name
	std::string output_colorspace_name = get_colorspace(OUTPUT_COLORSPACE);

	
	//set processor
	processor = OCIO_functionality::get_processor(config, input_colorspace_name, output_colorspace_name);

	
};

//processor_exists
bool Ocio_colorspace::processor_exists()
{
	//return wether or not processor exists
	if (!processor)
		return false;
	return true;
};

//set_colorspace_names
void Ocio_colorspace::set_colorspace_names()
{
	//log
	log("Set Colorspace names.");

	//vec_colorspace_names
	vec_colorspace_names = OCIO_functionality::get_colorspace_names(config);
};

//print_config_info
void Ocio_colorspace::print_config_info()
{
	OCIO_functionality::print_config_info(config);
};


//States
//-----------------------------------------------

//node_created
void Ocio_colorspace::node_created()
{
	//time
	float time = get_time();
	
	//set config
	set_config(get_env_or_file(time));
	//set_config_info
	set_config_info();
	
	//set_colorspace_names
	set_colorspace_names();
	//force_refresh_colorspace_menu
	force_refresh_colorspace_menu();
	

	//colorspace_count
	int colorspace_count = OCIO_functionality::get_colorspace_count(config);

	//internal_input_colorspace
	int internal_input_colorspace = get_internal_colorspace_index();
	//internal_output_colorspace
	int internal_output_colorspace = get_internal_colorspace_index(OUTPUT_COLORSPACE);

	//if internal_colorspace_indices < colorspace_count
	if (internal_input_colorspace < colorspace_count &&
		internal_output_colorspace < colorspace_count)
	{
		//Set colorspaces to internal values
		set_colorspace_by_index(internal_input_colorspace);
		set_colorspace_by_index(internal_output_colorspace, OUTPUT_COLORSPACE);
	}
	//else
	else
	{
		//Set internal colorspaces to zero
		//where the newly created parms are by default
		set_internal_colorspace_index(0);
		set_internal_colorspace_index(0, OUTPUT_COLORSPACE);
	}


	//set first execution false
	first_execution = false;

	//set_processor
	set_processor();

};

//env_or_file_changed
void Ocio_colorspace::env_or_file_changed()
{
	
	//time
	float time = get_time();

	//set config
	set_config(get_env_or_file(time));
	//set_config_info
	set_config_info();

	//set_colorspace_names
	set_colorspace_names();
	//force_refresh_colorspace_menu
	force_refresh_colorspace_menu();

	//Set colorspaces to zero
	set_colorspace_by_index(0, INPUT_COLORSPACE);
	set_colorspace_by_index(0, OUTPUT_COLORSPACE);

	//Set internal colorspaces to zero
	set_internal_colorspace_index(0, INPUT_COLORSPACE);
	set_internal_colorspace_index(0, OUTPUT_COLORSPACE);
	
	//set_last_env_or_file
	set_last_env_or_file(get_env_or_file(time), time);
	
	//set last config
	if (get_env_or_file(time))
		set_last_config_file_path(get_config_file_path(time), time);


	//set_processor
	set_processor();

	
};

//colorspace_changed
void Ocio_colorspace::colorspace_changed()
{
	//set internal colorspace indices
	set_internal_colorspace_index(get_colorspace_index(), INPUT_COLORSPACE);
	set_internal_colorspace_index(get_colorspace_index(OUTPUT_COLORSPACE), OUTPUT_COLORSPACE);

	//set_processor
	set_processor();
};

//config_file_path_changed
void Ocio_colorspace::config_file_path_changed()
{
	//time
	float time = get_time();

	//config from env, then return
	if (!get_env_or_file(time))
		return;

	//set config
	set_config(get_env_or_file(time));
	//set_config_info
	set_config_info();

	//set_colorspace_names
	set_colorspace_names();
	//force_refresh_colorspace_menu
	force_refresh_colorspace_menu();

	//Set colorspaces to zero
	set_colorspace_by_index(0, INPUT_COLORSPACE);
	set_colorspace_by_index(0, OUTPUT_COLORSPACE);

	//Set internal colorspaces to zero
	set_internal_colorspace_index(0, INPUT_COLORSPACE);
	set_internal_colorspace_index(0, OUTPUT_COLORSPACE);

	//set last config path
	set_last_config_file_path(get_config_file_path(time), time);

	//set_processor
	set_processor();
};



//Misc
//-----------------------------------------------

//getInputDependenciesForOutputArea
void Ocio_colorspace::getInputDependenciesForOutputArea(COP2_CookAreaInfo& output_area,
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

//update_colorspace_menu
void Ocio_colorspace::update_colorspace_menu(void* data,
												PRM_Name* prm_name,
												int max_size,
												const PRM_SpareData *,
												const PRM_Parm *)
{
	//Cast ptr to yourself
	Ocio_colorspace* node = (Ocio_colorspace*)data;
	
	//index
	int index;
	
	//iterate vec_values and set in menu
	for (index = 0; index < node->vec_colorspace_names.size() && index < max_size; index++)
	{
		prm_name[index].setToken(node->vec_colorspace_names[index].c_str());
		prm_name[index].setLabel(node->vec_colorspace_names[index].c_str());
	}
	
	//NULL Terminator neccessary
	prm_name[index].setToken(0); 
};

//force_refresh_colorspace_menu
void Ocio_colorspace::force_refresh_colorspace_menu()
{
	//update_ui
	getParm(ocio_colorspace_parameters::prm_input_colorspace_name.getToken()).valueChanged(0, true);
	getParm(ocio_colorspace_parameters::prm_output_colorspace_name.getToken()).valueChanged(0, true);
};

//newContextData
COP2_ContextData* Ocio_colorspace::newContextData(const TIL_Plane* plane,
													int array_index,
													float time,
													int xres,
													int yres,
													int thread,
													int max_threads)
{
	
	//Update config and processor based on states
	//-----------------------------------------------
	//node_created
	if (first_execution)
		node_created();
	//env_or_file_changed
	else if (get_env_or_file(time) != get_last_env_or_file(time))
		env_or_file_changed();
	////colorspace_changed
	else if ((get_colorspace_index() != get_internal_colorspace_index()) || (get_colorspace_index(OUTPUT_COLORSPACE) != get_internal_colorspace_index(OUTPUT_COLORSPACE)))
		colorspace_changed();
	////config file path changed
	else if (get_config_file_path(time).compare(get_last_config_file_path(time)) != 0)
		config_file_path_changed();




	//context_data (store custom parameter values here)
	Ocio_colorspace_context_data* context_data = new Ocio_colorspace_context_data();
	
	//Set custom param values here
	//to have them available in filter

	//plane_name
	std::string plane_name(plane->getName());
	context_data->plane_name = plane_name;

	//component_count
	context_data->component_count = plane->getVectorSize();
	
	//return
	return context_data;
};

//computeImageBounds
//void Ocio_image_filter::computeImageBounds(COP2_Context&)
//{};

//log
void Ocio_colorspace::log(const char* msg)
{
	//log message if log_messages == true
	if (log_messages)
		std::cout << msg << std::endl;
}

//get_time
float Ocio_colorspace::get_time()
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

//get_env_or_file
bool Ocio_colorspace::get_env_or_file(float time)
{
	//env_or_file
	bool env_or_file = evalInt(ocio_colorspace_parameters::prm_env_or_file.getToken(), 0, time);

	return env_or_file;
};

//set_last_env_or_file
void Ocio_colorspace::set_last_env_or_file(int new_value, float time)
{
	//setInt
	setInt(ocio_colorspace_parameters::prm_last_env_or_file.getToken(), 0, time, new_value);
};

//get_last_env_or_file
bool Ocio_colorspace::get_last_env_or_file(float time)
{
	//last_env_or_file
	bool last_env_or_file = evalInt(ocio_colorspace_parameters::prm_last_env_or_file.getToken(), 0, time);

	return last_env_or_file;
};

//get_config_file_path
std::string Ocio_colorspace::get_config_file_path(float time)
{
	//ut_config_file_path
	UT_String ut_config_file_path;
	
	//aquire ut_config_file_path
	evalString(ut_config_file_path, ocio_colorspace_parameters::prm_config_file_path.getToken(), 0, time);

	//config_file_path
	std::string config_file_path = ut_config_file_path.toStdString();

	return config_file_path;
};

//set_last_config_file_path
void Ocio_colorspace::set_last_config_file_path(std::string new_value, float time)
{
	//ut_new_value
	UT_String ut_new_value(new_value);
	
	//aquire ut_last_config_file_path
	setString(ut_new_value, CH_STRING_LITERAL, ocio_colorspace_parameters::prm_last_config_file_path.getToken(), 0, time);
};

//get_last_config_file_path
std::string Ocio_colorspace::get_last_config_file_path(float time)
{
	//ut_last_config_file_path
	UT_String ut_last_config_file_path;

	//aquire ut_last_config_file_path
	evalString(ut_last_config_file_path, ocio_colorspace_parameters::prm_last_config_file_path.getToken(), 0, time);

	//last_config_file_path
	std::string last_config_file_path = ut_last_config_file_path.toStdString();

	return last_config_file_path;
};

//get_input_colorspace
std::string Ocio_colorspace::get_colorspace(Colorspace_in_or_out colorspace_in_or_out)
{

	//current_time
	fpreal current_time = get_time();

	//ut_input_colorspace_name
	UT_String ut_colorspace_name;
	//aquire value

	//OUTPUT_COLORSPACE
	if (colorspace_in_or_out)
		evalString(ut_colorspace_name, ocio_colorspace_parameters::prm_output_colorspace_name.getToken(), 0, current_time);
	//INPUT_COLORSPACE
	else
		evalString(ut_colorspace_name, ocio_colorspace_parameters::prm_input_colorspace_name.getToken(), 0, current_time);

	//colorspace_name
	std::string colorspace_name = ut_colorspace_name.toStdString();
	
	
	//return
	return colorspace_name;
}

//get_colorspace_index
int Ocio_colorspace::get_colorspace_index(Colorspace_in_or_out colorspace_in_or_out)
{
	//current_time
	fpreal current_time = get_time();

	//colorspace_index
	int colorspace_index;

	//aquire value

	//OUTPUT_COLORSPACE
	if (colorspace_in_or_out)
		colorspace_index = evalInt(ocio_colorspace_parameters::prm_output_colorspace_name.getToken(), 0, current_time);
	//INPUT_COLORSPACE
	else
		colorspace_index = evalInt(ocio_colorspace_parameters::prm_input_colorspace_name.getToken(), 0, current_time);

	//return
	return colorspace_index;
};

//set_colorspace_by_index
void Ocio_colorspace::set_colorspace_by_index(int new_colorspace_index,
	Colorspace_in_or_out colorspace_in_or_out)
{
	//current_time
	fpreal current_time = get_time();

	//OUTPUT_COLORSPACE
	if (colorspace_in_or_out)
		setInt(ocio_colorspace_parameters::prm_output_colorspace.getToken(), 0, current_time, new_colorspace_index);
	//INPUT_COLORSPACE
	else
		setInt(ocio_colorspace_parameters::prm_input_colorspace.getToken(), 0, current_time, new_colorspace_index);

	


};

//get_internal_colorspace_index
int Ocio_colorspace::get_internal_colorspace_index(Colorspace_in_or_out colorspace_in_or_out)
{
	//current_time
	fpreal current_time = get_time();

	//internal_colorspace_index
	int internal_colorspace_index;

	//aquire value

	//OUTPUT_COLORSPACE
	if (colorspace_in_or_out)
		internal_colorspace_index = evalInt(ocio_colorspace_parameters::prm_internal_output_colorspace_index.getToken(), 0, current_time);
	//INPUT_COLORSPACE
	else
		internal_colorspace_index = evalInt(ocio_colorspace_parameters::prm_internal_input_colorspace_index.getToken(), 0, current_time);

	

	//return
	return internal_colorspace_index;
};

//set_internal_colorspace_index
void Ocio_colorspace::set_internal_colorspace_index(int new_internal_colorspace_index,
														Colorspace_in_or_out colorspace_in_or_out)
{
	//current_time
	fpreal current_time = get_time();

	//OUTPUT_COLORSPACE
	if (colorspace_in_or_out)
		setInt(ocio_colorspace_parameters::prm_internal_output_colorspace_index.getToken(), 0, current_time, new_internal_colorspace_index);
	//INPUT_COLORSPACE
	else
		setInt(ocio_colorspace_parameters::prm_internal_input_colorspace_index.getToken(), 0, current_time, new_internal_colorspace_index);
};

//set_config_info
void Ocio_colorspace::set_config_info()
{
	//std_config_info
	std::string std_config_info;

	//assign config info
	std_config_info = OCIO_functionality::get_config_info(config);

	//current_time
	fpreal current_time = get_time();

	//ut_config_info
	UT_String ut_config_info(std_config_info);

	//set config info
	setString(ut_config_info, CH_STRING_LITERAL, ocio_colorspace_parameters::prm_config_info.getToken(), 0, current_time);
};