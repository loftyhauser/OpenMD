 /*
 * Copyright (c) 2005 The University of Notre Dame. All Rights Reserved.
 *
 * The University of Notre Dame grants you ("Licensee") a
 * non-exclusive, royalty free, license to use, modify and
 * redistribute this software in source and binary code form, provided
 * that the following conditions are met:
 *
 * 1. Acknowledgement of the program authors must be made in any
 *    publication of scientific results based in part on use of the
 *    program.  An acceptable form of acknowledgement is citation of
 *    the article in which the program was described (Matthew
 *    A. Meineke, Charles F. Vardeman II, Teng Lin, Christopher
 *    J. Fennell and J. Daniel Gezelter, "OOPSE: An Object-Oriented
 *    Parallel Simulation Engine for Molecular Dynamics,"
 *    J. Comput. Chem. 26, pp. 252-271 (2005))
 *
 * 2. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 3. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 * This software is provided "AS IS," without a warranty of any
 * kind. All express or implied conditions, representations and
 * warranties, including any implied warranty of merchantability,
 * fitness for a particular purpose or non-infringement, are hereby
 * excluded.  The University of Notre Dame and its licensors shall not
 * be liable for any damages suffered by licensee as a result of
 * using, modifying or distributing the software or its
 * derivatives. In no event will the University of Notre Dame or its
 * licensors be liable for any lost revenue, profit or data, or for
 * direct, indirect, special, consequential, incidental or punitive
 * damages, however caused and regardless of the theory of liability,
 * arising out of the use of or inability to use software, even if the
 * University of Notre Dame has been advised of the possibility of
 * such damages.
 */
 
/**
 * @file StringUtils.hpp
 * @author Dan Gezelter
 * @date 10/18/2004
 * @version 1.0
 */

#ifndef UTILS_STRINGUTILS_HPP
#define UTILS_STRINGUTILS_HPP
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace oopse {
 
  /**
   * Converts a string to UPPER CASE
   * @param S
   */
  std::string UpperCase(const std::string& S);

  /**
   * Converts a string to lower case
   * @param S
   */
  std::string LowerCase(const std::string& S);

  /**
   * Removes left and right spaces from a string
   *
   * @param str  String to trim
   *
   * @return  char* to the trimed string
   */
  char* trimSpaces(char *str);

  /**
   * Finds the location of the string "begin <startText>" in an input stream.
   * @param theStream
   * @param startText
   *
   * @return the line number of the block within the theStream
   */
  int findBegin(std::istream &theStream, char* startText );

  /**
   * Counts the number of tokens on line which are delimited by the characters 
   * listed in delimiters
   * @param line
   * @param delimiters
   */
  int countTokens(char *line, char *delimiters);

  /**
   * discovers whether or not the line contains the "end" token
   *
   * @param line  The line to test
   * 
   * @return int  (==1 if the line has "end", ==0 if not).
   */
  int isEndLine(char *line);


  /**
   * Convert a variable to a string
   * @param T data type
   * @param v data to be converted
   * @return a string
   */
  template<typename T>
  std::string toString(const T& v) {
    std::ostringstream oss;        
    if (!oss << v) {
      std::cerr << "toString Error" << std::endl;
    }
    return oss.str();
  }
  
  template<typename T>
  T lexi_cast(const std::string& str) {
    T result;
    std::istringstream iss(str);        
    if (!(iss >> result)) {
      std::cerr << "lexi_cast Error" << std::endl;
    }
    return result;
  }
  
  std::string OOPSE_itoa(int value, unsigned int base);
  
  /**@todo need implementation */
  std::string getPrefix(const std::string& str);
  
  /**@todo need implementation */
  std::string getSuffix(const std::string& str);
  
  

  
}  
#endif
