
.. _program_listing_file_pymembrane_cppmodule_src_utils_fromdicttovec.hpp:

Program Listing for File fromdicttovec.hpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_utils_fromdicttovec.hpp>` (``pymembrane/cppmodule/src/utils/fromdicttovec.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef __fromdicttovec_hpp__
   #define __fromdicttovec_hpp__
   
   #pragma once
   
   #include <string>
   #include <vector>
   #include <type_traits>
   #include "fromstring.hpp"
   #include <tuple>
   
   namespace util
   {
       inline std::vector<int> from_dict_to_vector_types(std::vector<int> prop_vec, std::map<int, std::string> dict_map)
       {
           int vec_size = prop_vec.size();
           for (auto items : dict_map)
           {
               int region_type = items.first;
               auto value = from_string_int(items.second);
               if (region_type < vec_size)
                   prop_vec[region_type] = value;
           }
           return prop_vec;
       }
       inline std::vector<double> from_dict_to_vector_types(std::vector<double> prop_vec, std::map<int, std::string> dict_map)
       {
           int vec_size = prop_vec.size();
           for (auto items : dict_map)
           {
               int region_type = items.first;
               auto value = from_string_double(items.second);
               if (region_type < vec_size)
                   prop_vec[region_type] = value;
           }
           return prop_vec;
       }
       inline std::vector<bool> from_dict_to_vector_types(std::vector<bool> prop_vec, std::map<int, std::string> dict_map)
       {
           int vec_size = prop_vec.size();
           for (auto items : dict_map)
           {
               int region_type = items.first;
               auto value = from_string_bool(items.second);
               if (region_type < vec_size)
                   prop_vec[region_type] = value;
           }
           return prop_vec;
       }
   
       inline std::vector<int> from_dict_to_vector_types(std::vector<int> prop_vec, std::map<std::string, std::string> dict_map)
       {
           int vec_size = prop_vec.size();
           for (auto items : dict_map)
           {
               unsigned int region_type = from_string_int(items.first);
               auto value = from_string_int(items.second);
               if (region_type < vec_size)
                   prop_vec[region_type] = value;
           }
           return prop_vec;
       }
       inline std::vector<double> from_dict_to_vector_types(std::vector<double> prop_vec, std::map<std::string, std::string> dict_map)
       {
           int vec_size = prop_vec.size();
           for (auto items : dict_map)
           {
               unsigned int region_type = from_string_int(items.first);
               auto value = from_string_double(items.second);
               if (region_type < vec_size)
                   prop_vec[region_type] = value;
           }
           return prop_vec;
       }
       inline std::vector<bool> from_dict_to_vector_types(std::vector<bool> prop_vec, std::map<std::string, std::string> dict_map)
       {
           int vec_size = prop_vec.size();
           for (auto items : dict_map)
           {
               unsigned int region_type = from_string_int(items.first);
               auto value = from_string_bool(items.second);
               if (region_type < vec_size)
                   prop_vec[region_type] = value;
           }
           return prop_vec;
       }
   
       inline std::vector<int> from_dict_to_vector_types_pair(std::vector<int> prop_vec, uint MAX_ITEM, std::map<std::pair<std::string, std::string>, std::string>  dict_map)
       {
           int vec_size = prop_vec.size();
           for (auto item : dict_map)
           {
               uint pair_index = from_string_int(std::get<0>(item.first)) + MAX_ITEM * from_string_int(std::get<1>(item.first));
               if (pair_index < vec_size)
                   prop_vec[pair_index] = from_string_int(item.second);
           }
           return prop_vec;
       }
       inline std::vector<double> from_dict_to_vector_types_pair(std::vector<double> prop_vec, uint MAX_ITEM, std::map<std::pair<std::string, std::string>, std::string>  dict_map)
       {
           int vec_size = prop_vec.size();
           for (auto item : dict_map)
           {
               uint pair_index = from_string_int(std::get<0>(item.first)) + MAX_ITEM * from_string_int(std::get<1>(item.first));
               if (pair_index < vec_size)
                   prop_vec[pair_index] = from_string_double(item.second);
           }
           return prop_vec;
       }
       inline std::vector<bool> from_dict_to_vector_types_pair(std::vector<bool> prop_vec, uint MAX_ITEM, std::map<std::pair<std::string, std::string>, std::string>  dict_map)
       {
           int vec_size = prop_vec.size();
           for (auto item : dict_map)
           {
               uint pair_index = from_string_int(std::get<0>(item.first)) + MAX_ITEM * from_string_int(std::get<1>(item.first));
               if (pair_index < vec_size)
                   prop_vec[pair_index] = from_string_bool(item.second);
           }
           return prop_vec;
       }
   
   } // namespace util
   
   #endif
