/*
 * Copyright (c) 2004-2021 The University of Notre Dame. All Rights Reserved.
 *
 * The University of Notre Dame grants you ("Licensee") a
 * non-exclusive, royalty free, license to use, modify and
 * redistribute this software in source and binary code form, provided
 * that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
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

#include <config.h>

#include "brains/SimCreator.hpp"
#include "brains/SimInfo.hpp"
#include "clusters/Cuboctahedron.hpp"
#include "clusters/Decahedron.hpp"
#include "clusters/Icosahedron.hpp"
#include "icosahedralBuilderCmd.hpp"
#include "io/DumpWriter.hpp"
#include "utils/MoLocator.hpp"
#include "utils/Tuple.hpp"

using namespace OpenMD;
using namespace std;

void createMdFile(const std::string& oldMdFileName,
                  const std::string& newMdFileName, int nMol) {
  ifstream oldMdFile;
  ofstream newMdFile;
  const int MAXLEN = 65535;
  char buffer[MAXLEN];

  // create new .omd file based on old .omd file
  oldMdFile.open(oldMdFileName.c_str());
  newMdFile.open(newMdFileName.c_str());
  oldMdFile.getline(buffer, MAXLEN);

  while (!oldMdFile.eof()) {
    // correct molecule number
    if (strstr(buffer, "nMol") != NULL) {
      snprintf(buffer, MAXLEN, "\tnMol = %i;", nMol);
      newMdFile << buffer << std::endl;
    } else {
      newMdFile << buffer << std::endl;
    }

    oldMdFile.getline(buffer, MAXLEN);
  }

  oldMdFile.close();
  newMdFile.close();
}

int main(int argc, char* argv[]) {
  gengetopt_args_info args_info;
  std::string inputFileName;
  std::string outputFileName;

  MoLocator* locator;
  RealType latticeConstant(0.0);
  int nShells(-1);

  DumpWriter* writer;

  // Parse Command Line Arguments
  if (cmdline_parser(argc, argv, &args_info) != 0) exit(1);

  /* get input file name */
  if (args_info.inputs_num)
    inputFileName = args_info.inputs[0];
  else {
    snprintf(painCave.errMsg, MAX_SIM_ERROR_MSG_LENGTH, "No input .omd file name was specified "
                             "on the command line");
    painCave.isFatal = 1;
    cmdline_parser_print_help();
    simError();
  }

  if (args_info.shells_given ||
      (args_info.cuboctahedron_given || args_info.truncatedCube_given)) {
    nShells = args_info.shells_arg;
    if (nShells < 0) {
      snprintf(painCave.errMsg, MAX_SIM_ERROR_MSG_LENGTH, "icosahedralBuilder:  The number of shells\n"
                               "\tmust be greater than or equal to zero.");
      painCave.isFatal = 1;
      cmdline_parser_print_help();
      simError();
    }
  } else {
    snprintf(painCave.errMsg, MAX_SIM_ERROR_MSG_LENGTH, "icosahedralBuilder:  The number of shells\n"
                             "\tis required to build a Mackay Icosahedron.");
    painCave.isFatal = 1;
    cmdline_parser_print_help();
    simError();
  }

  if (args_info.latticeConstant_given) {
    latticeConstant = args_info.latticeConstant_arg;
  } else {
    snprintf(painCave.errMsg, MAX_SIM_ERROR_MSG_LENGTH, "icosahedralBuilder:  No lattice constant\n"
                             "\tgiven.");
    painCave.isFatal = 1;
    cmdline_parser_print_help();
    simError();
  }

  /* parse md file and set up the system */
  SimCreator oldCreator;
  SimInfo* oldInfo = oldCreator.createSim(inputFileName, false);

  vector<Vector3d> Points;
  if (args_info.ico_given) {
    Icosahedron* ico = new Icosahedron();
    Points           = ico->getPoints(nShells);
  } else if (args_info.deca_given) {
    RegularDecahedron* deca = new RegularDecahedron(nShells);
    Points                  = deca->getPoints();
  } else if (args_info.ino_given) {
    int columnAtoms    = args_info.columnAtoms_arg;
    InoDecahedron* ino = new InoDecahedron(columnAtoms, nShells);
    Points             = ino->getPoints();
  } else if (args_info.marks_given) {
    int columnAtoms   = args_info.columnAtoms_arg;
    int twinAtoms     = args_info.twinAtoms_arg;
    Decahedron* marks = new Decahedron(columnAtoms, nShells, twinAtoms);
    Points            = marks->getPoints();
  } else if (args_info.stone_given) {
    int columnAtoms             = args_info.columnAtoms_arg;
    int twinAtoms               = args_info.twinAtoms_arg;
    int truncatedPlanes         = args_info.truncatedPlanes_arg;
    CurlingStoneDecahedron* csd = new CurlingStoneDecahedron(
        columnAtoms, nShells, twinAtoms, truncatedPlanes);
    Points = csd->getPoints();
  } else if (args_info.cuboctahedron_given || args_info.truncatedCube_given) {
    std::string lattice;
    int unitCells = 0;
    if (args_info.lattice_given) {
      lattice = args_info.lattice_arg;
    } else {
      snprintf(painCave.errMsg, MAX_SIM_ERROR_MSG_LENGTH, "icosahedralBuilder: No lattice type given.");
      painCave.isFatal = 1;
      cmdline_parser_print_help();
      simError();
    }
    if (args_info.unitCells_given) {
      unitCells = args_info.unitCells_arg;
    } else {
      snprintf(painCave.errMsg, MAX_SIM_ERROR_MSG_LENGTH, "icosahedralBuilder: Must specify unit cells.");
      painCave.isFatal = 1;
      cmdline_parser_print_help();
      simError();
    }
    if (args_info.truncatedCube_given) {
      int truncatedPlanes = args_info.truncatedPlanes_arg;
      TruncatedCube* tc =
          new TruncatedCube(lattice, unitCells, truncatedPlanes);
      Points = tc->getPoints();
    } else {
      RegularCuboctahedron* rc = new RegularCuboctahedron(lattice, unitCells);
      Points                   = rc->getPoints();
    }
  }

  outputFileName = args_info.output_arg;

  // create a new .omd file on fly which corrects the number of
  // molecules

  createMdFile(inputFileName, outputFileName, Points.size());

  delete oldInfo;

  SimCreator newCreator;
  SimInfo* NewInfo = newCreator.createSim(outputFileName, false);

  // Place molecules
  Molecule* mol;
  SimInfo::MoleculeIterator mi;
  mol = NewInfo->beginMolecule(mi);

  int l = 0;

  locator =
      new MoLocator(NewInfo->getMoleculeStamp(0), NewInfo->getForceField());

  Vector3d boxMax;
  Vector3d boxMin;

  for (unsigned int n = 0; n < Points.size(); n++) {
    mol = NewInfo->getMoleculeByGlobalIndex(l);

    Vector3d location;
    if (args_info.cuboctahedron_given || args_info.truncatedCube_given) {
      // The cubic structures are built with a unit spacing between cells
      location = Points[n] * latticeConstant;
    } else {
      // The polyhedra are built with a unit spacing between atoms,
      // which in an FCC lattice should be multiplied by a / sqrt(2).
      location = Points[n] * latticeConstant / sqrt(2.0);
    }
    Vector3d orientation = Vector3d(0, 0, 1.0);

    if (n == 0) {
      boxMax = location;
      boxMin = location;
    } else {
      for (int i = 0; i < 3; i++) {
        boxMax[i] = max(boxMax[i], location[i]);
        boxMin[i] = min(boxMin[i], location[i]);
      }
    }

    locator->placeMol(location, orientation, mol);
    l++;
  }

  Mat3x3d boundingBox;
  boundingBox(0, 0) = 10.0 * (boxMax[0] - boxMin[0]);
  boundingBox(1, 1) = 10.0 * (boxMax[1] - boxMin[1]);
  boundingBox(2, 2) = 10.0 * (boxMax[2] - boxMin[2]);

  // set Hmat
  NewInfo->getSnapshotManager()->getCurrentSnapshot()->setHmat(boundingBox);

  // create dumpwriter and write out the coordinates
  writer = new DumpWriter(NewInfo, outputFileName);

  if (writer == NULL) {
    snprintf(painCave.errMsg, MAX_SIM_ERROR_MSG_LENGTH, "Error in creating dumpwriter object ");
    painCave.isFatal = 1;
    simError();
  }

  writer->writeDump();

  // deleting the writer will put the closing at the end of the dump file

  delete writer;

  // clean up by calling simError.....
  snprintf(painCave.errMsg, MAX_SIM_ERROR_MSG_LENGTH,
          "A new OpenMD file called \"%s\" has been "
          "generated.\n",
          outputFileName.c_str());
  painCave.isFatal  = 0;
  painCave.severity = OPENMD_INFO;
  simError();
  return 0;
}
