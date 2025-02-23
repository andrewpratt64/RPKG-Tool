#pragma once
#include "rpkg.h"
#include "hash.h"
#include <string>
#include <vector>
#include <stdint.h>

struct rpkg_extraction_vars
{
	std::string input_path = "";
	std::string filter = "";
	std::string search = "";
	std::string search_type = "";
	std::string output_path = "";
	std::string ores_path = "";

	bool search_mode = false;
	bool ores_mode = false;
};

class rpkg_function
{
public:
	static void json_to_hash_meta(std::string& input_path, std::string& filter, std::string& output_path);
	static void hash_meta_to_json(std::string& input_path, std::string& filter, std::string& output_path);
	static void edit_hash_depends(std::string& input_path, std::string& filter, std::string& search, std::string& search_type, std::string& output_path);
	static void edit_patch_deletion_list(std::string& input_path, std::string& filter, std::string& search, std::string& search_type, std::string& output_path);
	static void extract_all_hash_depends_from(std::string& input_path, std::string& filter, std::string& output_path, bool extract_prim_models);
	static void extract_all_prim_model_of_temp_from(std::string& input_path, std::string& filter, std::string& output_path);
	static void extract_all_prim_model_from(std::string& input_path, std::string& filter, std::string& output_path);
	static void rebuild_prim_model_in(std::string& input_path, std::string& filter, std::string& output_path);
	static void extract_prim_model_from(std::string& input_path, std::string& filter, std::string& output_path);
	static void rebuild_prim_in(std::string& input_path, std::string& filter, std::string& output_path, bool generate_rpkgs);
	static bool rebuild_text(std::string& text_folder, std::string& tga_file_path, std::string& text_file_name, std::string& meta_file_path, std::string& rpkg_output_file, std::string& rpkgs_path, bool generate_rpkgs);
	static void rebuild_text_in(std::string& input_path, std::string& filter, std::string& output_path, bool generate_rpkgs);
	static void extract_all_text_from(std::string& input_path, std::string& filter, std::string& output_path);
	static void extract_text_from(std::string& input_path, std::string& filter, std::string& output_path);
	static void extract_all_prim_of_temp_from(std::string& input_path, std::string& filter, std::string& output_path, int type);
	static void recursive_temp_loader(uint32_t rpkgs_index, uint32_t hash_index, uint32_t temp_version, std::map<uint32_t, uint32_t>& parents_map, std::vector<uint32_t> temps_indexes, uint32_t level, uint32_t level_max, uint32_t temps_max);
	static void extract_all_prim_from(std::string& input_path, std::string& filter, std::string& output_path, int type);
	static void extract_prim_from(std::string& input_path, std::string& filter, std::string& output_path, int type);
	static int load_temp_tblu_hash_depends(uint64_t rpkg_index, uint64_t hash_index);
	static void dev_extract_temp_from(std::string& input_path, std::string& filter, std::string& output_path);
	static void extract_temp_from(std::string& input_path, std::string& filter, std::string& output_path);
	static void extract_prel_refs(std::string& input_path, std::string& filter, std::string& output_path);
	static void get_unique_name(std::map<std::string, uint32_t>& wwem_name_map, std::string& wem_base_name);
	static void hash_probe_from_file(std::string& input_path, std::string& filter, std::string& output_path);
	static void hash_probe(std::string& input_path, std::string& filter, std::string& output_path);
	static void extract_direct_hash_depends_from(std::string& input_path, std::string& filter, std::string& output_path, bool extract_prim_models);
	static void hash_depends(std::string& input_path, std::string& filter, std::string& output_path);
	static void rebuild_rtlv_from_json_from(std::string& input_path, std::string& filter, std::string& output_path);
	static void rebuild_locr_from_json_from(std::string& input_path, std::string& filter, std::string& output_path);
	static void rebuild_dlge_from_json_from(std::string& input_path, std::string& filter, std::string& output_path);
	static void extract_rtlv_to_json_from(std::string& input_path, std::string& filter, std::string& output_path, bool output_to_string);
	static void extract_locr_to_json_from(std::string& input_path, std::string& filter, std::string& output_path, bool output_to_string);
	static void extract_dlge_to_json_from(std::string& input_path, std::string& filter, std::string& output_path, bool output_to_string);
	static void generate_rpkg_from(std::string& input_path, std::string& filter, std::string& output_path);
	static void rebuild_gfxf_in(std::string& input_path, std::string& filter, std::string& output_path);
	static void rebuild_wwev_in(std::string& input_path, std::string& filter, std::string& output_path);
	static void extract_wwem_to_ogg_from(std::string& input_path, std::string& filter, std::string& output_path);
	static void extract_wwev_to_ogg_from(std::string& input_path, std::string& filter, std::string& output_path);
	static void extract_wwes_to_ogg_from(std::string& input_path, std::string& filter, std::string& output_path);
	static void extract_from_rpkg_with_map(rpkg_extraction_vars& rpkg_vars);
	static void import_rpkg_files_in_folder(std::string& input_rpkg_folder_path);
	static void extract_gfxf_from(std::string& input_path, std::string& filter, std::string& output_path);
	static void extract_ores_from(std::string& input_path, std::string& filter, std::string& output_path);
	static void search_hash_data(std::string& search_type, std::string& search, std::vector<char>& search_data, std::string& hash_file_name);
	static void import_hash_meta(hash& hash_meta_data, std::string& rpkg_meta_file_path);
	static void extract_hash_meta(uint64_t i, uint64_t j, std::string& final_path);
	static void extract_rpkg_meta(uint64_t i, std::string& hash_file_path);
	static bool import_rpkg_meta(rpkg& rpkg_meta_data, std::string& rpkg_meta_file_path, std::string& input_rpkg_folder_path);
	static void import_rpkg(std::string& rpkg_file_name, bool with_timing);
	static void extract_from_rpkg(rpkg_extraction_vars& rpkg_vars);
};