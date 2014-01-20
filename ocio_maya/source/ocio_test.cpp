

//Ocio_test Implementation
//-----------------------------------------------


//include
//-----------------------------------------------
#include "ocio_test.h"


//Ocio_test
//-----------------------------------------------

//Attributes
MTypeId Ocio_test::id(0x00000);
MString Ocio_test::type_name("ocio_test");

MObject Ocio_test::a_verbose;

MObject Ocio_test::a_change_list;
MObject Ocio_test::a_last_change_index;

MObject Ocio_test::a_input_color;
MObject Ocio_test::a_width;
MObject Ocio_test::a_height;

MObject Ocio_test::a_config_path;

MObject Ocio_test::a_output_color;


//Methods
Ocio_test::Ocio_test():
	boost_signal_helloworld(new Boost_signal_helloworld())
{
};
Ocio_test::~Ocio_test(){}


//create
//-----------------------------------------------
void* Ocio_test::create()
{
	return new Ocio_test();
}


//initialize
//-----------------------------------------------
MStatus Ocio_test::initialize()
{
	
	//MFnSets
	MFnEnumAttribute eAttr;
	MFnNumericAttribute nAttr;
	MFnCompoundAttribute cAttr;
	MFnTypedAttribute tAttr;

	
	//a_verbose
	a_verbose = eAttr.create("verbose", "verbose", 0);
	eAttr.addField("no verbose", 0);
	eAttr.addField("verbose", 1);
	addAttribute(a_verbose);

	//a_change_list
	a_change_list = eAttr.create("change_list", "change_list", 0);
	eAttr.addField("type 1", 0);
	eAttr.addField("type 2", 1);
	addAttribute(a_change_list);

	//a_last_change_index
	a_last_change_index = nAttr.create("last_change_index", "last_change_index", MFnNumericData::kInt, 9999);
	nAttr.setStorable(false);
	addAttribute(a_last_change_index);

	//a_input_color
	a_input_color = nAttr.createColor("input_color", "input_color");
	nAttr.setUsedAsColor(true);
	nAttr.setStorable(false);
	addAttribute(a_input_color);

	//a_width
	a_width = nAttr.create("width", "width", MFnNumericData::kInt);
	nAttr.setStorable(false);
	addAttribute(a_width);

	//a_height
	a_height = nAttr.create("height", "height", MFnNumericData::kInt);
	nAttr.setStorable(false);
	addAttribute(a_height);

	//a_config_path
	a_config_path = tAttr.create("config_path", "config_path", MFnData::kString);
	tAttr.setUsedAsFilename(true);
	tAttr.setStorable(true);
	addAttribute(a_config_path);
	
	//a_output_color
	a_output_color = nAttr.createColor("output_color", "output_color");
	nAttr.setStorable(false);
	addAttribute(a_output_color);
	


	//Attribute affects
	attributeAffects(a_change_list, a_output_color);
	attributeAffects(a_input_color, a_output_color);
	attributeAffects(a_width, a_output_color);
	attributeAffects(a_height, a_output_color);

	return MStatus::kSuccess;
}

//postConstructor
//-----------------------------------------------
void Ocio_test::postConstructor()
{
	//node
	MObject node = thisMObject();

	//value_list
	std::string attribute_name("input_colorspace");

	//value_list
	std::vector<std::string> value_list;
	value_list.push_back("HansPeter");
	value_list.push_back("Simon");
	value_list.push_back("Siggi");

	//create_enum_attribute
	create_enum_attribute(attribute_name, 
							value_list, 
							node);

	//emit test signal
	sig_postconstructor.connect(*boost_signal_helloworld);
	sig_postconstructor();

};

//setDependentsDirty
//-----------------------------------------------
MStatus Ocio_test::setDependentsDirty(const MPlug& plugBeingDirtied, MPlugArray& affectedPlugs)
{
	MStatus status;
	MObject node = thisMObject();
	MFnDependencyNode fn_dep_node(node);

        if(plugBeingDirtied.partialName() == "input_colorspace") 
		{
			MPlug p_output = fn_dep_node.findPlug( "output_color", &status );
			if(MStatus::kSuccess == status) 
			{
				CHECK_MSTATUS(affectedPlugs.append(p_output));
				MGlobal::displayInfo("Plug output appended");
			}
		}



	return MStatus::kSuccess;
};

//compute
//-----------------------------------------------
MStatus Ocio_test::compute(const MPlug &plug, MDataBlock &data)
{
	//wrong plug requested
	//-----------------------------------------------
	if ((plug != a_output_color) && (plug.parent() != a_output_color))
	{
		return MStatus::kUnknownParameter;
	};
	
	
	//right plug requested
	//-----------------------------------------------

	//get attributes from datablock
	//-----------------------------------------------
	int verbose = static_cast<int>(data.inputValue(a_verbose).asShort());
	int change_list_index = static_cast<int>(data.inputValue(a_change_list).asShort());
	int last_change_index = data.inputValue(a_last_change_index).asInt();

	int width = data.inputValue(a_width).asInt();
	int height = data.inputValue(a_height).asInt();

	//vec_input_color
	MDataHandle h_input_color = data.inputValue(a_input_color);
	MFloatVector& vec_input_color = h_input_color.asFloatVector();
	
	
	//compute
	//-----------------------------------------------
	
	//check if attrs are connected
	if (!all_attrs_connected())
	{
		if (verbose)
			MGlobal::displayInfo("Some attrs are not connected. Returning....");
		return MStatus::kSuccess;
	};
	//check if width or height == 0
	if (width == 0 || height == 0)
	{
		if (verbose)
			MGlobal::displayInfo("Either width or height is zero. Returning....");
		return MStatus::kSuccess;
	};

	//rebuild_input_colorspace
	if (!last_change_index == change_list_index)
	{
		//log
		if (verbose)
			MGlobal::displayInfo("Rebuild input colorspace");
		//rebuild input colorspace
		rebuild_input_colorspace(change_list_index, data);
	}

	//print_config_info
	//print_config_info();

	//set output color
	set_output_color(vec_input_color, data);

	//log
	//if (verbose)
		//MGlobal::displayInfo("Compute executed");

	//return success
	return MStatus::kSuccess;
	
}










//Own
//-----------------------------------------------

//rebuild_enum
void Ocio_test::rebuild_input_colorspace(int& change_list_index, MDataBlock& data)
{
	//config
	OCIO::ConstConfigRcPtr config;
	config = OCIO_functionality::get_config_from_env();


	//config_colorspaces
	std::vector<std::string> config_colorspaces = OCIO_functionality::get_colorspace_names(config);
	if (!config_colorspaces.size())
	{
		//tmp
		MGlobal::displayInfo("Config colorspaces empty. Returning None");
		return;
	};
	
	//node
	MObject node = thisMObject();
	
	//value_list
	std::string attribute_name("input_colorspace");


	//update_enum_attribute
	//remove attribute
	remove_attribute(node, attribute_name);

	//create_enum_attribute
	create_enum_attribute(attribute_name, 
							config_colorspaces,
							node);
	


	//Set last change index
	data.outputValue(a_last_change_index).setInt(change_list_index);
	//format
	boost::format change_index_msg("Last change index set to: %s");
	change_index_msg % change_list_index;
	MGlobal::displayInfo(change_index_msg.str().c_str());
	
};

//create_enum_attribute
void Ocio_test::create_enum_attribute(std::string attribute_name, 
										std::vector<std::string>& value_list, 
										MObject& node)
{
	//eAttr
	MFnEnumAttribute eAttr;

	//a_attribute
	MObject a_attribute = eAttr.create(attribute_name.c_str(), attribute_name.c_str(), 0);
	eAttr.setStorable(true);
	//iterate and add values
	for(int index = 0; index < value_list.size(); index++)
		eAttr.addField(value_list[index].c_str(), index);

	//dg_modifier
	MDGModifier dg_modifier;
	dg_modifier.addAttribute(node, a_attribute);
	dg_modifier.doIt();

	//reload AE
	MGlobal::executeCommandOnIdle(MString("openAEWindow;"));

	//tmp
	MGlobal::displayInfo(MString("Reopened AE"));
};

//remove_attribute
void Ocio_test::remove_attribute(MObject& node, std::string attribute_name)
{
	//status
	MStatus status;
	
	//fn_dep_node
	MFnDependencyNode fn_dep_node(node);
	
	//p_attribute
	MPlug p_attribute = fn_dep_node.findPlug(attribute_name.c_str(), &status);
	//o_attribute
	MObject o_attribute = p_attribute.attribute();

	//dg_modifier
	MDGModifier dg_modifier;
	dg_modifier.removeAttribute(node, o_attribute);
	dg_modifier.doIt();

	//tmp
	MGlobal::displayInfo(MString("Deleted Attr.:")+attribute_name.c_str());
};

//update_enum_attribute
void Ocio_test::update_enum_attribute(std::string attribute_name, 
										std::vector<std::string>& value_list, 
										MObject& node)
{
	//remove attribute
	remove_attribute(node, attribute_name);

	//create_enum_attribute
	create_enum_attribute(attribute_name, 
							value_list, 
							node);

};

//attr_connected
bool Ocio_test::attr_connected(std::string attr_name)
{
	//node
	MObject node = thisMObject();
	//fn_dep_node
	MFnDependencyNode fn_dep_node(node);
	//p_plug
	MPlug p_plug = fn_dep_node.findPlug(attr_name.c_str(), true);

	//return
	return p_plug.isConnected();
};

//all_attrs_connected
bool Ocio_test::all_attrs_connected()
{
	//input_color
	if (!attr_connected(std::string("input_color")))
	{
		//tmp
		MGlobal::displayInfo("input color attr. not connected");
		return false;
	};
	//width
	if (!attr_connected(std::string("width")))
	{
		//tmp
		MGlobal::displayInfo("width attr. not connected");
		return false;
	};
	//height
	if (!attr_connected(std::string("height")))
	{
		//tmp
		MGlobal::displayInfo("height attr. not connected");
		return false;
	};
		
	
	return true;
};

//set_output_color
void Ocio_test::set_output_color(MFloatVector& vec_color, MDataBlock& data)
{
	//h_output_color
	MDataHandle h_output_color = data.outputValue(a_output_color);
	MFloatVector& vec_output_color = h_output_color.asFloatVector();
	vec_output_color = vec_color;

	//clean handle
	h_output_color.setClean();
	
	//tmp
	//MGlobal::displayInfo("Set output color attribute");
};

//Temp
//-----------------------------------------------
//print_config_info
void Ocio_test::print_config_info()
{
	//config
	OCIO::ConstConfigRcPtr config;
	config = OCIO_functionality::get_config_from_env();

	//print info
	OCIO_functionality::print_config_info(config);
};