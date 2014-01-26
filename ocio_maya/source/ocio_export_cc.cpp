


//Ocio_export_cc Implementation
//-----------------------------------------------








//include
//-----------------------------------------------
#include "ocio_export_cc.h"







//Globals
//-----------------------------------------------

//flag_names
const char* Ocio_export_cc::flag_source_node = "-sn";
const char* Ocio_export_cc::flag_long_source_node = "-source_node";

const char* Ocio_export_cc::flag_cc_path = "-cc";
const char* Ocio_export_cc::flag_long_cc_path = "-cc_path";





//Ocio_export_cc
//-----------------------------------------------

//Constructor
Ocio_export_cc::Ocio_export_cc() {};

//creator
void* Ocio_export_cc::creator()
{ 
	return new Ocio_export_cc;
}





//doIt
MStatus Ocio_export_cc::doIt(const MArgList& argument_list) 
{
	//Default values for command arguments
	//-----------------------------------------------

	//source_node
	MString ms_source_node("Default source node");
	
	//cc_path
	MString ms_cc_path(MFileIO::currentFile());
	std::string cc_dir(split_filename(ms_cc_path.asChar()).c_str());
	MString cc_file_path(cc_dir.append("/grade.cc").c_str());

	//status
	MStatus status;

	
	
	
	//arg_db
	//-----------------------------------------------
	MArgDatabase arg_db(syntax(), argument_list, &status);

	//return if wrong
	if (status != MStatus::kSuccess)
		return status;


	//get arguments if set

	//source_node
	if (arg_db.isFlagSet(flag_source_node))
		arg_db.getFlagArgument(flag_source_node, 0, ms_source_node);
	//cc_path
	if (arg_db.isFlagSet(flag_cc_path))
		arg_db.getFlagArgument(flag_cc_path, 0, cc_file_path);

	
	//str_cc_file_path
	std::string str_cc_file_path(cc_file_path.asChar());
	
	
	
	//Export
	//-----------------------------------------------
	
	//source_node
	std::string source_node(ms_source_node.asChar());
	
	//cc parameter vars
	MFloatVector vec_slope, vec_offset, vec_power;
	float saturation;
	int direction;

	//get cc parameters
	get_cc_parameters(source_node,
						vec_slope,
						vec_offset,
						vec_power,
						saturation,
						direction);
	
	//sop_ptr
	float* sop_ptr = new float[9];
	get_sop_as_float_ptr(sop_ptr, vec_slope, vec_offset, vec_power);
	
	//str_xml
	std::string str_xml = get_cc_xml(sop_ptr, saturation, direction);

	//write to file
	write_to_file(str_xml, str_cc_file_path);
	
	
	//display str_xml
	MGlobal::displayInfo(MString("XML: ") + str_xml.c_str());
	
	//display cc_path
	MGlobal::displayInfo(MString("CC File saved to: ") + str_cc_file_path.c_str());
	
	//delete sop_ptr
	delete[] sop_ptr;
	
	return MS::kSuccess;
}




//new_syntax
MSyntax Ocio_export_cc::new_syntax()
{
	
	
	//syntax
	MSyntax syntax;

	//add flags
	syntax.addFlag(flag_source_node, flag_long_source_node, MSyntax::kString);
	syntax.addFlag(flag_cc_path, flag_long_cc_path, MSyntax::kString);


	return syntax;
};







//Own
//-----------------------------------------------
//-----------------------------------------------


//get_cc_parameters
void Ocio_export_cc::get_cc_parameters(std::string node_name, 
										MFloatVector& vec_slope, 
										MFloatVector& vec_offset, 
										MFloatVector& vec_power, 
										float& saturation, 
										int& direction)
{
	//p_slope
	MPlug p_slope = get_plug(node_name, std::string("slope"));
	vec_slope.x = p_slope.child(0).asFloat();
	vec_slope.y = p_slope.child(1).asFloat();
	vec_slope.z = p_slope.child(2).asFloat();

	//p_offset
	MPlug p_offset = get_plug(node_name, std::string("offset"));
	vec_offset.x = p_offset.child(0).asFloat();
	vec_offset.y = p_offset.child(1).asFloat();
	vec_offset.z = p_offset.child(2).asFloat();

	//p_power
	MPlug p_power = get_plug(node_name, std::string("power"));
	vec_power.x = p_power.child(0).asFloat();
	vec_power.y = p_power.child(1).asFloat();
	vec_power.z = p_power.child(2).asFloat();

	//p_saturation
	MPlug p_saturation = get_plug(node_name, std::string("saturation"));
	saturation = p_saturation.asFloat();

	//p_direction
	MPlug p_direction = get_plug(node_name, std::string("direction"));
	direction = p_direction.asInt();


};

//get_sop_as_float_ptr
void Ocio_export_cc::get_sop_as_float_ptr(float*& sop_ptr,
											MFloatVector& vec_slope,
											MFloatVector& vec_offset,
											MFloatVector& vec_power)
{
	//set sop_ptr
	sop_ptr[0] = vec_slope.x;
	sop_ptr[1] = vec_slope.y;
	sop_ptr[2] = vec_slope.z;

	sop_ptr[3] = vec_offset.x;
	sop_ptr[4] = vec_offset.y;
	sop_ptr[5] = vec_offset.z;

	sop_ptr[6] = vec_power.x;
	sop_ptr[7] = vec_power.y;
	sop_ptr[8] = vec_power.z;
};

//get_cc_xml
std::string Ocio_export_cc::get_cc_xml(float*& sop_ptr,
								float& saturation, 
								int& direction)
{
	//str_xml
	std::string str_xml = OCIO_functionality::get_xml_from_cdl_transform(sop_ptr, 
																			saturation, 
																			direction);
	//return str_xml
	return str_xml;
};

//write_to_file
void Ocio_export_cc::write_to_file(std::string& str_xml, std::string& file_path)
{
	//cc_file
	std::ofstream cc_file;
	
	//open
	cc_file.open(file_path, std::ios::out | std::ios::trunc);

	//if open append
	if (cc_file.is_open())
		cc_file << str_xml.c_str();
	else
		MGlobal::displayWarning(MString("Could not open and write to file: ") + file_path.c_str());

	//close
	cc_file.close();

}

//get_plug
MPlug Ocio_export_cc::get_plug(std::string node_name, std::string attr_name)
{
	
	//sel_list
	MSelectionList sel_list;
	sel_list.add(MString(node_name.c_str()));

	//o_node
	MObject o_node;
	sel_list.getDependNode(0, o_node);
	
	//fn_dep_node
	MFnDependencyNode fn_dep_node(o_node);
	
	//p_plug
	MPlug p_plug = fn_dep_node.findPlug(attr_name.c_str(), true);

	return p_plug;
};

//split_filename
std::string Ocio_export_cc::split_filename(const std::string& str)
{
	//index
	unsigned index = str.find_last_of("/");
	
	//str_plit
	std::string str_split(str.substr(0, index));

	return str_split;
}