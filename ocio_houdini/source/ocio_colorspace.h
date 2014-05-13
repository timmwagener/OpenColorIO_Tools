

#ifndef OCIO_COLORSPACE_H
#define OCIO_COLORSPACE_H

//Ocio_colorspace Declaration
//-----------------------------------------------

/*
Ocio_colorspace
A COP2_MaskOp node that grades the incoming pixel array with OCIO.
It is not multithreaded as i expect OCIO doing smart threading on the whole array.
*/

//Constants
//-----------------------------------------------



//include
//-----------------------------------------------
//Std
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
//Boost
#include "boost/format.hpp"
#include "boost/lexical_cast.hpp"
//Houdini
#include <OP/OP_Director.h>
#include <CH/CH_Manager.h>
#include <PRM/PRM_Include.h>
#include <PRM/PRM_Parm.h>
#include <PRM/PRM_SpareData.h>
#include <TIL/TIL_Region.h>
#include <TIL/TIL_Plane.h>
#include <TIL/TIL_Sequence.h>
#include <TIL/TIL_Tile.h>
#include <TIL/TIL_Raster.h>
#include <COP2/COP2_CookAreaInfo.h>
#include <UT/UT_Lock.h>
#include <COP2/COP2_MaskOp.h>
//Own
#include "ocio_functionality.h"
#include "ocio_types.h"





//OCIO Namespace
namespace OCIO = OCIO_NAMESPACE;












//Ocio_colorspace
//-----------------------------------------------
class Ocio_colorspace : public COP2_MaskOp
{
public:
	
	//Attributes
	//-----------------------------------------------
	//-----------------------------------------------
	
	//Operator Creation
	//-----------------------------------------------
	static OP_TemplatePair template_pair;
	static OP_VariablePair variable_pair;
	static PRM_Template template_list[];
	static CH_LocalVariable local_variable_list[];
	static const char* input_labels_list[];
	//OCIO
	//-----------------------------------------------
	OCIO::ConstConfigRcPtr config;
	OCIO::ConstProcessorRcPtr processor;
	std::vector<std::string> vec_colorspace_names;
	//Misc
	//-----------------------------------------------
	bool first_execution;
	bool log_messages;
	bool internal_parms_visible;

	
	
	
	
	
	
	//Methods
	//-----------------------------------------------
	//-----------------------------------------------
	
	//COP2 Static Defaults
	//-----------------------------------------------
	//Factory method
	static OP_Node* ocio_colorspace_factory(OP_Network*,
												const char *,
												OP_Operator *);

	//static filter called in cookFullImage
	static OP_ERROR filter_static(COP2_Context& context,
									const TIL_Region* input,
									TIL_Region* output,
									COP2_Node* node);
		
	//non-static filter called in static filter
	OP_ERROR filter(COP2_Context& context,
					const TIL_Region* input,
					TIL_Region* output);
	
	//Set information on how to thread
	virtual void getMaxNumThreadsInCook(COP2_Context &,
										int &maxp, 
										int &maxn, 
										int &op) 
	const
	{
		maxp = 1; maxn = op = TIL_MAX_THREADS;
	};
		
	//Build dependencies tree for cooking operation (COP2 node specialty)
	virtual void getInputDependenciesForOutputArea(COP2_CookAreaInfo& output_area,
													const COP2_CookAreaList& input_areas,
													COP2_CookAreaList& needed_areas);

	//update_colorspace_menu
	static void	update_colorspace_menu(void* data,
										PRM_Name *,
										int,
										const PRM_SpareData *,
										const PRM_Parm *);
	//Force refresh of colorspace menu
	void force_refresh_colorspace_menu();
	
	//OCIO
	//-----------------------------------------------
	void set_config(int env_or_file = 0);
	bool config_exists();
	void set_processor();
	bool processor_exists();
	void set_colorspace_names();
	void print_config_info();
	//States
	//-----------------------------------------------
	void node_created();
	void env_or_file_changed();
	void colorspace_changed();
	void config_file_path_changed();
	//Misc
	//-----------------------------------------------
	void log(const char*);
	float get_time();
	//Getter & Setter
	//-----------------------------------------------
	bool get_env_or_file(float time);
	void set_last_env_or_file(int new_value, float time);
	bool get_last_env_or_file(float time);
	std::string get_config_file_path(float time);
	void set_last_config_file_path(std::string new_value, float time);
	std::string get_last_config_file_path(float time);
	std::string get_colorspace(Colorspace_in_or_out colorspace_in_or_out = INPUT_COLORSPACE);
	int get_colorspace_index(Colorspace_in_or_out colorspace_in_or_out = INPUT_COLORSPACE);
	void set_colorspace_by_index(int new_colorspace_index,
									Colorspace_in_or_out colorspace_in_or_out = INPUT_COLORSPACE);
	int get_internal_colorspace_index(Colorspace_in_or_out colorspace_in_or_out = INPUT_COLORSPACE);
	void set_internal_colorspace_index(int new_internal_colorspace_index, 
										Colorspace_in_or_out colorspace_in_or_out = INPUT_COLORSPACE);
	void set_config_info();
	//Temp
	//-----------------------------------------------
	
	
protected:
	
	
	//Methods
	//-----------------------------------------------
	//-----------------------------------------------
	
	//COP2 Defaults
	//-----------------------------------------------
	//Constructor
	Ocio_colorspace(OP_Network*, const char *, OP_Operator *);
	//Destructor
	virtual ~Ocio_colorspace();

	//Aquire and stash context data (PRM information etc.)
	virtual COP2_ContextData* newContextData(const TIL_Plane* plane,
												int array_index,
												float time,
												int xres, 
												int yres,
												int thread,
												int max_threads);

	//doCookMyTile - Classic cook method for COP2_Node. Must be reimplemented
	//to start cookFullImage, which is a convenience function to combine
	//all tiles and cook them
	virtual OP_ERROR doCookMyTile(COP2_Context&, TIL_TileList* );

	//updateParmsFlags
	bool updateParmsFlags();

	//Set image bounds for operation
	/*virtual void computeImageBounds(COP2_Context&);*/


};






//Ocio_colorspace_context_data
//-----------------------------------------------
class Ocio_colorspace_context_data : public COP2_ContextData
{
public:
	//Attributes
	UT_Lock	 image_lock;
	std::string plane_name;
	int component_count;
	

	//Methods
	Ocio_colorspace_context_data() {}
	virtual	~Ocio_colorspace_context_data() {}

	
};


#endif