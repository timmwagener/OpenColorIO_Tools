



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
	COP_MASK_SWITCHER(1, "OCIOCDLTransform");
	PRM_Template prm_cop_default = PRM_Template(PRM_SWITCHER, 3, &PRMswitcherName, switcher);

	//prm_temp
	static PRM_Name prm_temp_name("temp", "temp");
	static PRM_Default prm_temp_default(0);
	const char* prm_temp_help_text = "Temp parameter";
	PRM_Template prm_temp = PRM_Template(PRM_TOGGLE, 1, &prm_temp_name,
		&prm_temp_default, 0, 0, 0, 0, 0, prm_temp_help_text, 0);

}



//Parameter template array for UI creation
//-----------------------------------------------
PRM_Template Ocio_cdl_transform::template_list[] =
{
	ocio_cdl_transform_parameters::prm_cop_default,
	ocio_cdl_transform_parameters::prm_temp,
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
									log_messages(true),
									internal_parms_visible(true)
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
	
	
	return attribute_change_occured;
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
	log("execute filter");


	//Pixel operation to perform is implemented here

	//context_data
	//Custom built in newContextData()
	//Here live custom attributes stashed within single threaded method newContextData()
	Ocio_cdl_transform_context_data* context_data = (Ocio_cdl_transform_context_data *)context.data();

		
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
		};
	
	};

	
	//Return error
	return error();
};


//OCIO
//-----------------------------------------------
//-----------------------------------------------



//set_processor
void Ocio_cdl_transform::set_processor(int env_or_file, std::string config_file_path, int operation)
{


	//log
	log("Set processor.");

};

//processor_exists
bool Ocio_cdl_transform::processor_exists()
{
	//return wether or not processor exists
	if (!processor)
		return false;
	return true;
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

	
	
	
	
	//Set processor
	//-----------------------------------------------
	
	
	
	
	
	
	
	
	
	
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




//Getter & Setter
//-----------------------------------------------
//-----------------------------------------------

