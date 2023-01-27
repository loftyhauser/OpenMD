/*
 * Copyright (c) 2004-present, The University of Notre Dame. All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * SUPPORT OPEN SCIENCE!  If you use OpenMD or its source code in your
 * research, please cite the appropriate papers when you publish your
 * work.  Good starting points are:
 *
 * [1] Meineke, et al., J. Comp. Chem. 26, 252-271 (2005).
 * [2] Fennell & Gezelter, J. Chem. Phys. 124, 234104 (2006).
 * [3] Sun, Lin & Gezelter, J. Chem. Phys. 128, 234107 (2008).
 * [4] Vardeman, Stocker & Gezelter, J. Chem. Theory Comput. 7, 834 (2011).
 * [5] Kuang & Gezelter, Mol. Phys., 110, 691-701 (2012).
 * [6] Lamichhane, Gezelter & Newman, J. Chem. Phys. 141, 134109 (2014).
 * [7] Lamichhane, Newman & Gezelter, J. Chem. Phys. 141, 134110 (2014).
 * [8] Bhattarai, Newman & Gezelter, Phys. Rev. B 99, 094106 (2019).
 */

#ifndef BRAINS_PAIRLIST_HPP

#define BRAINS_PAIRLIST_HPP

#include <iostream>
#include <set>
#include <utility>
#include <vector>

namespace OpenMD {

  /**
   * @class PairList PairList.hpp "brains/PairList.hpp"
   * @brief PairList class maintains a general purpose list of atom
   * pairs using the global indices of the atoms.  This structure is
   * the general form for exclude lists as well as 1-4, 1-3, and 1-2
   * lists.
   */
  class PairList {
  public:
    PairList() : modified_(false) {}

    /** Adds a pair into this PairList class */
    void addPair(int i, int j);

    void addPairs(std::set<int>& set1, std::set<int>& set2);
    template<typename IterType1, typename IterType2>
    void addPairs(IterType1 iter1_first, IterType1 iter1_last,
                  IterType2 iter2_first, IterType2 iter2_last);

    /** Remove a pair from PairList class */
    void removePair(int i, int j);

    void removePairs(std::set<int>& set1, std::set<int>& set2);
    template<typename IterType1, typename IterType2>
    void removePairs(IterType1 iter1_first, IterType1 iter1_last,
                     IterType2 iter2_first, IterType2 iter2_last);

    /** Checks whether pair (i, j) is in this PairList class */
    bool hasPair(int i, int j);

    /** Returns the number of pairs in the list */
    int getSize();

    /** Returns the size of PairList */
    int getSizeOfPairList();

    /** Returns the pairs in a plain array*/
    int* getPairList();

    /** write out the exclusion list to an ostream */
    friend std::ostream& operator<<(std::ostream& o, PairList& e);

  private:
    std::set<std::pair<int, int>> pairSet_;
    std::vector<int> pairList_;
    bool modified_;
  };
}  // namespace OpenMD

#endif  // __PAIRLIST_H__
