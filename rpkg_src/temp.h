#pragma once

#include "hash.h"
#include <string>
#include <vector>
#include <map>
#include <stdint.h>
#include "thirdparty/rapidjson/document.h"
#include "thirdparty/rapidjson/writer.h"
#include "thirdparty/rapidjson/stringbuffer.h"
#include "thirdparty/zhmtools/ResourceLib_HM2.h"
#include "thirdparty/zhmtools/ResourceLib_HM3.h"

class temp
{
public:
	temp();
	temp(uint64_t rpkgs_index, uint64_t hash_index);
	temp(uint64_t rpkgs_index, uint64_t hash_index, uint32_t temp_version);

	void load_data();
	void load_hash_depends();
	void load_temp_data();
	void load_tblu_data();
	void get_prim_from_temp(uint32_t entry_index);
	void temp_version_check();
	void set_temp_version(uint32_t temp_version);
#ifdef RPKG_DLL
	void get_top_level_logical_parents();
	void get_entries_with_logical_parent(uint32_t logical_parent);
	void get_entries_hash_references(uint32_t entry_index);
	void get_entries_hash_reference_data(uint32_t entry_index);
	void get_temp_entries_data(std::string value_type, std::string type_string);
	void json_temp_node_scan(const rapidjson::Value& node, std::string& propertyValues_string, std::string& nPropertyID_string, std::string& type_string, std::string json_pointer, std::string json_type);
	void get_entries_data(uint32_t entry_index, std::string value_type);
	void json_node_scan(const rapidjson::Value& node, std::string& propertyValues_string, std::string& nPropertyID_string, std::string& type_string, std::string json_pointer, std::string json_type);
	void get_enum_values(std::string& property_type_string);
	void update_temp_file(uint32_t entry_index, char* update_data, uint32_t update_data_size);
	void export_json_files(std::string& json_file_path);
	int generate_temp_file_from_data(std::string temp_path);
	void get_all_bricks();
	void get_entry_name_string(int entry_index);
#endif

	std::string temp_file_name = "";
	uint32_t temp_temps_index = 0;
	uint32_t temp_rpkg_index = 0;
	uint32_t temp_hash_index = 0;
	uint32_t tblu_rpkg_index = 0;
	uint32_t tblu_hash_index = 0;
	uint32_t temp_file_version = 3;
	uint32_t temp_subentity_count = 0;
	uint32_t tblu_subentity_count = 0;
	uint32_t parent = 0;
	std::vector<uint32_t> parents;
	std::vector<uint32_t> children;
	bool file_has_been_changed = false;
	std::vector<char> temp_input_data;
	std::vector<char> temp_output_data;
	std::vector<char> temp_data;
	std::vector<char> tblu_input_data;
	std::vector<char> tblu_output_data;
	std::vector<char> tblu_data;
	std::vector<uint32_t> temp_logicalParent;
	std::vector<std::string> tblu_entityName;
	std::vector<uint32_t> temp_entityTypeResourceIndex;
	std::vector<uint32_t> tblu_entityTypeResourceIndex;
	JsonString* temp_json_input = nullptr;
	JsonString* tblu_json_input = nullptr;
	rapidjson::Document temp_json_document;
	rapidjson::Document tblu_json_document;
	std::vector<std::string> temp_depends_file_name;
	std::vector<std::string> temp_depends_hash_string;
	std::vector<uint64_t> temp_depends_hash_value;
	std::vector<uint32_t> temp_depends_index;
	std::vector<std::vector<std::string>> temp_depends_in_rpkgs;
	std::vector<uint32_t> temp_depends_in_rpkgs_index;
	std::vector<std::vector<uint32_t>> temp_depends_rpkg_index;
	std::vector<uint32_t> temp_depends_rpkg_index_index;
	std::vector<std::vector<uint32_t>> temp_depends_hash_index;
	std::vector<uint32_t> temp_depends_hash_index_index;
	int tblu_return_value = 0;
	std::vector<std::string> tblu_depends_file_name;
	std::vector<std::string> tblu_depends_hash_string;
	std::vector<uint64_t> tblu_depends_hash_value;
	std::vector<uint32_t> tblu_depends_index;
	std::vector<std::vector<std::string>> tblu_depends_in_rpkgs;
	std::vector<uint32_t> tblu_depends_in_rpkgs_index;
	std::vector<std::vector<uint32_t>> tblu_depends_rpkg_index;
	std::vector<uint32_t> tblu_depends_rpkg_index_index;
	std::vector<std::vector<uint32_t>> tblu_depends_hash_index;
	std::vector<uint32_t> tblu_depends_hash_index_index;
	std::vector<std::string> prim_depends_file_name;
	std::vector<std::string> prim_depends_hash_string;
	std::vector<uint64_t> prim_depends_hash_value;
	std::vector<uint32_t> prim_depends_index;
	std::vector<std::vector<std::string>> prim_depends_in_rpkgs;
	std::vector<uint32_t> prim_depends_in_rpkgs_index;
	std::vector<std::vector<uint32_t>> prim_depends_rpkg_index;
	std::vector<uint32_t> prim_depends_rpkg_index_index;
	std::vector<std::vector<uint32_t>> prim_depends_hash_index;
	std::vector<uint32_t> prim_depends_hash_index_index;
	ResourceConverter* resource_tool_converter;
	ResourceGenerator* resource_tool_generator;
};