

#ifndef OCIO_LOG_CONVERT_H
#define OCIO_LOG_CONVERT_H

//Ocio_log_convert Declaration
//-----------------------------------------------

/*
Ocio_log_convert
A COP2_MaskOp node that converts from lin to log or the other way round.
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











//Ocio_log_convert
//-----------------------------------------------
class Ocio_log_convert : public COP2_MaskOp
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
	//OCIO_houdini_functionality
	//-----------------------------------------------
	OCIO_houdini_functionality OCIO_houdini_functionality;
	//OCIO
	//-----------------------------------------------
	OCIO::ConstConfigRcPtr config;
	OCIO::ConstProcessorRcPtr processor;
	//Misc
	//-----------------------------------------------
	bool first_execution;

	
	
	
	
	
	
	//Methods
	//-----------------------------------------------
	//-----------------------------------------------
	
	//COP2 Static Defaults
	//-----------------------------------------------
	//Factory method
	static OP_Node* ocio_log_convert_factory(OP_Network*,
												const char *,
												OP_Operator *);

	//static filter called in cookFullImage
	static OP_ERROR filter_static(COP2_Context& context,
									const TIL_Region* input,
									TIL_Region* output,
									COP2_Node* node);
	
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
	
	//OCIO
	//-----------------------------------------------
	void set_config(int env_or_file = 0);
	bool config_exists();
	void set_processor(int, std::string, int);
	bool processor_exists();
	
	//Getter & Setter
	//-----------------------------------------------
	bool get_env_or_file(float time);
	
	void set_last_env_or_file(int new_value, float time);
	bool get_last_env_or_file(float time);
	
	std::string get_config_file_path(float time);
	
	void set_last_config_file_path(std::string new_value, float time);
	std::string get_last_config_file_path(float time);

	int get_operation(float time);

	int get_internal_operation_index(float time);
	void set_internal_operation_index(int new_internal_operation_index, float time);
	
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
	Ocio_log_convert(OP_Network*, const char *, OP_Operator *);
	//Destructor
	virtual ~Ocio_log_convert();

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

	//disableParms
	unsigned disableParms();

	//getOperationInfo
	virtual const char* getOperationInfo();

};






//Ocio_log_convert_context_data
//-----------------------------------------------
class Ocio_log_convert_context_data : public COP2_ContextData
{
public:
	//Attributes
	UT_Lock	 image_lock;
	std::string plane_name;
	int component_count;
	

	//Methods
	Ocio_log_convert_context_data() {}
	virtual	~Ocio_log_convert_context_data() {}

	
};


#endif