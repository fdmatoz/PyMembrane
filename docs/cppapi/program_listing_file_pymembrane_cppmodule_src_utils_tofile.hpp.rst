
.. _program_listing_file_pymembrane_cppmodule_src_utils_tofile.hpp:

Program Listing for File tofile.hpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_utils_tofile.hpp>` (``pymembrane/cppmodule/src/utils/tofile.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef __tofile_hpp__
   #define __tofile_hpp__
   
   #include <string>
   #include <fstream>
   #include <iostream>
   
   namespace util
   {
       // Base case: Writes a single value to the file.
       template <typename T>
       void writeToFile(std::ofstream &file, const T &value)
       {
           file << value;
       }
   
       // Recursive case: Writes the first value followed by the remaining values, separated by tabs.
       template <typename T, typename... Args>
       void writeToFile(std::ofstream &file, const T &value, const Args &...args)
       {
           file << value << '\t';
           writeToFile(file, args...);
       }
   
       // Open a file with the given filename and write the provided arguments to it.
       template <typename... Args>
       void saveToFile(const std::string &filename, const Args &...args)
       {
           std::ofstream file(filename);
   
           if (!file)
           {
               std::cerr << "Error: Cannot open file for writing: " << filename << std::endl;
               return;
           }
   
           writeToFile(file, args...);
           file << std::endl;
           file.close();
       }
   
   } // namespace util
   
   #endif // __tofile_hpp__
